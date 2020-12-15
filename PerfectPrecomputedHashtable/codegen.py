import ast
import typing
from pathlib import Path

from . import hash as hashModule
from . import reducers
from .hash import Config, HashConfig, POWConfig, GeneratedHashtable, PoorManDataClass

__all__ = ("genCode", "genAST", "GenCfg")


def primitive2AST(v: typing.Any):
	return ast.parse(repr(v), mode="eval").body

identityLambdaArgName = "x"
identityLambdaAST = ast.Lambda(args=ast.arguments(posonlyargs=[], args=[ast.arg(arg=identityLambdaArgName)], kwonlyargs=[], kw_defaults=[], defaults=[]), body=ast.Name(id=identityLambdaArgName, ctx=ast.Load()))

class GenCfg(PoorManDataClass):
	__slots__ = ("tableName", "configName", "preamble", "valueASTGen", "o2b", "nnConstName")

	def __init__(self, tableName: str = "hashedHashTable", configName: str = "gHT", nnConstName: str = "nN", preamble=None, valueASTGen: typing.Callable = primitive2AST, o2b=identityLambdaAST) -> None:
		self.tableName = tableName
		self.configName = configName
		self.valueASTGen = valueASTGen
		self.preamble = preamble
		self.o2b = o2b
		self.nnConstName = nnConstName


def makeTableAst(gCfg: GenCfg, tbl):
	return ast.Tuple(elts=[tableRowAst(gCfg, el) for el in tbl], ctx=ast.Load())


def tableRowAst(gCfg: GenCfg, r):
	checkValue, value = r
	if checkValue is not None:
		return ast.Expr(value=ast.Tuple(elts=[ast.Constant(value=checkValue), gCfg.valueASTGen(value)], ctx=ast.Load()))
	return ast.Name(id=gCfg.nnConstName, ctx=ast.Load())


class HexUnparser(ast._Unparser):
	def _write_constant(self, value):
		if isinstance(value, int):
			a = repr(value)
			b = hex(value)
			self.write(min([a, b], key=len))
		else:
			super()._write_constant(value)


def makeStorageClass(name, dic: typing.Mapping[str, typing.Any]) -> ast.ClassDef:
	body = []
	for k, v in dic.items():
		needAssignment = True
		if not isinstance(v, ast.AST):
			v = primitive2AST(v)
		else:
			if hasattr(v, "name"):
				needAssignment = False
				v.name = k

		if needAssignment:
			v = ast.Assign(
				targets=[
					ast.Name(
						id=k,
						ctx=ast.Store(),
					)
				],
				value=v,
			)

		body.append(v)

	return ast.ClassDef(
		name=name,
		bases=[],
		keywords=[],
		body=body,
		decorator_list=[],
	)


def takeFromModuleAST(moduleAST, toTake):
	taken = {}
	for node in moduleAST.body:
		if isinstance(node, (ast.FunctionDef, ast.ClassDef)):
			if node.name in toTake:
				taken[node.name] = node
		elif isinstance(node, ast.Assign):
			if len(node.targets) == 1:
				t = node.targets[0]
				if isinstance(t, ast.Name) and t.id in toTake:
					taken[t.id] = node
	return taken


def takeFromFile(path: Path, toTake):
	return takeFromModuleAST(ast.parse(path.read_text()), toTake)


def takeFromModule(module, toTake):
	return takeFromFile(Path(module.__file__), toTake)

def patchHashtableLookup(htl, gCfg, argToReplace: str = 'gHT'):
	argToReplaceIdx = None

	for i, arg in enumerate(htl.args.args):
		if arg.arg == argToReplace:
			argToReplaceIdx = i
	del htl.args.args[argToReplaceIdx]

	if argToReplace != gCfg.configName:
		htl.body.insert(0, ast.Assign(targets=[ast.Name(id=argToReplace, ctx=ast.Store())], value=ast.Name(id=gCfg.configName, ctx=ast.Load())))


def genAST(g: GeneratedHashtable, gCfg: GenCfg) -> ast.Module:
	p = makeStorageClass("p", {"nonce": g.c.p.nonce})

	fromReducers = takeFromModule(reducers, {"uint8_t", g.c.p.reducer.__name__})

	reducerF = fromReducers[g.c.p.reducer.__name__]
	reducerF.name = "reducer"
	p.body.append(reducerF)
	
	h = makeStorageClass(
		"h",
		{
			"t": g.c.h.t,
			"r": g.c.h.r,
			"offset": g.c.h.offset,
			"o2b": gCfg.o2b
		},
	)

	paramsStorageClass = ast.ClassDef(
		name="c",
		bases=[],
		keywords=[],
		body=[p, h],
		decorator_list=[],
	)

	tableBodyAst = makeTableAst(gCfg, g.t)

	nnConstAssignment = ast.Assign(targets=[ast.Name(id=gCfg.nnConstName, ctx=ast.Store())], value=ast.Tuple(elts=[ast.Constant(value=None)]*2, ctx=ast.Load()))
	tableAssignment = ast.Assign(targets=[ast.Name(id=gCfg.tableName, ctx=ast.Store())], value=tableBodyAst)
	generatedHashtableClass = makeStorageClass(gCfg.configName, {"t": ast.Name(id=gCfg.tableName, ctx=ast.Load())})
	generatedHashtableClass.body.insert(0, paramsStorageClass)

	fromHash = takeFromModule(hashModule, {"uint16_t", "twoShorts", "halfIntHash", "hashtableLookup", "singleByteHashStrings", "perfectHashRemap"})
	patchHashtableLookup(fromHash["hashtableLookup"], gCfg)

	r = ast.Module(body=[
		ast.Import(names=[ast.alias(name='typing')]),
		ast.ImportFrom(module='hashlib', names=[ast.alias(name='blake2s')], level=0),
		ast.ImportFrom(module='struct', names=[ast.alias(name='Struct')], level=0),
		
		*(gCfg.preamble if gCfg.preamble else ()),
		fromReducers["uint8_t"],
		*fromHash.values(),
		nnConstAssignment,
		tableAssignment,
		generatedHashtableClass,
	], type_ignores=[])
	ast.fix_missing_locations(r)
	return r


def genCode(g: GeneratedHashtable, gCfg: GenCfg) -> str:
	a = genAST(g=g, gCfg=gCfg)
	up = HexUnparser()
	return up.visit(a)
