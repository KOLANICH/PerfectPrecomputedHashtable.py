import typing
from hashlib import blake2s
from pathlib import Path
from struct import unpack

import perfection

from .hash import *


def prepareForPerfectHashtable(powCfg, src: typing.Mapping[typing.Any, typing.Any], obj2Bytes):
	fullMappingList = genTable(powCfg, src, obj2Bytes)
	assert len(fullMappingList) == len(src)
	fullMappingList = sorted(set(fullMappingList), key=lambda el: el[0])
	assert len(fullMappingList) == len(src)

	maxEl = fullMappingList[-1]
	minEl = fullMappingList[0]
	offset = minEl[0]

	reducedSpan = maxEl[0] - offset
	#print("Span", reducedSpan, "min", offset, "max", maxEl[0])

	fullMappingList = [(el[0] - offset, *el[1:]) for el in fullMappingList]

	reducedHashesList = sorted({el[0] for el in fullMappingList})
	assert len(reducedHashesList) == len(src)

	return offset, fullMappingList, reducedHashesList


def genPerfectHashtable(powCfg: POWConfig, src: typing.Mapping[typing.Any, typing.Any], obj2Bytes):
	offset, fullMappingList, reducedHashesList = prepareForPerfectHashtable(powCfg, src, obj2Bytes)
	h = perfection.hash_parameters(reducedHashesList, minimize=False)

	tbl = [(None, None)] * len(h.slots)

	for reduced, half, full, value in fullMappingList:
		idx = perfectHashRemap(h, reduced)
		tbl[idx] = (half, value)

	cfg = Config(
		p=powCfg,
		h=HashConfig(
			t=h.t,
			offset=offset - h.offset,
			r=h.r
		)
	)

	return GeneratedHashtable(t=tbl, c=cfg)
