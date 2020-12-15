import typing
from hashlib import blake2s
from struct import Struct

from .reducers import reducerIndex


class PoorManDataClass:
	__slots__ = ()

	def __repr__(self):
		return "".join((self.__class__.__name__, "(", ", ".join(("".join((k, "=", repr(getattr(self, k))))) for k in self.__class__.__slots__), ")"))

	def __eq__(self, other):
		if not isinstance(other, __class__):
			return NotImplemented
		return self.c, self.t == other.c, other.t


u4 = Struct("<I")

class POWConfig(PoorManDataClass):
	__slots__ = ("nonce", "reducer")

	def __init__(self, nonce: bytes, reducer: typing.Callable[[int], int]) -> None:
		if isinstance(nonce, int):
			nonce = u4.pack(nonce)

		self.nonce = nonce
		self.reducer = reducer

	@classmethod
	def fromDict(cls, powDic: dict):
		return cls(nonce=powDic["nonce"], reducer=reducerIndex[powDic["reducer"]])


class HashConfig(PoorManDataClass):
	__slots__ = ("t", "offset", "r")

	def __init__(self, t: int, offset: int, r: typing.Tuple[int]) -> None:
		self.t = t
		self.offset = offset
		self.r = r


class Config(PoorManDataClass):
	__slots__ = ("h", "p")

	def __init__(self, h: "HashConfig", p: "POWConfig") -> None:
		self.h = h
		self.p = p

class GeneratedHashtable(PoorManDataClass):
	__slots__ = ("c", "t", "o2b")

	def __init__(self, c: HashConfig, t: tuple, o2b: typing.Callable=None) -> None:
		self.c = c
		self.t = t
		self.o2b = o2b


class uint16_t(int):
	__slots__ = ()


twoShorts = Struct("<HH")


def halfIntHash(full: bytes) -> uint16_t:
	a, b = twoShorts.unpack(full)
	return a ^ b


def hashtableLookup(gHT: "GeneratedHashtable", s: typing.Any) -> typing.Optional[typing.Any]:
	hC = gHT.c.h
	t = gHT.t
	reduced, half, full = singleByteHashStrings(gHT.c.p, hC.o2b(s))

	idx = perfectHashRemap(hC, reduced)
	control, res = t[idx]
	if control != half:
		raise KeyError(s)

	return res


def singleByteHashStrings(powCfg: "POWConfig", d: bytes):
	h = blake2s(key=powCfg.nonce, digest_size=4)
	h.update(d)
	full = h.digest()
	half = halfIntHash(full)
	reduced = powCfg.reducer(*full)
	return reduced, half, full


def perfectHashRemap(hashConfig: "HashConfig", i: int) -> int:
	i -= hashConfig.offset
	return i % hashConfig.t + hashConfig.r[i // hashConfig.t]


def genTable(powCfg: "POWConfig", src: typing.Mapping[typing.Any, typing.Any], obj2Bytes: typing.Callable):
	initialLen = len(src)
	src = {obj2Bytes(k): v for (k, v) in src.items()}
	assert initialLen == len(src)
	src = sorted(src.items(), key=lambda x: x[0])
	return [(*singleByteHashStrings(powCfg, kb), v) for (kb, v) in src]
