import typing


class uint8_t(int):
	__slots__ = ()


def f0(d: int, c: int, b: int, a: int) -> uint8_t:
	return (a + b + c + d) & 0xFF


def f1(d: int, c: int, b: int, a: int) -> uint8_t:
	return a ^ b ^ c ^ d


def f2(d: int, c: int, b: int, a: int) -> uint8_t:
	return ((a ^ b) + (c ^ d)) & 0xFF


def f3(d: int, c: int, b: int, a: int) -> uint8_t:
	return ((a ^ c) + (b ^ d)) & 0xFF


def f4(d: int, c: int, b: int, a: int) -> uint8_t:
	return ((a ^ d) + (c ^ b)) & 0xFF


def f5(d: int, c: int, b: int, a: int) -> uint8_t:
	return ((a + b) & 0xFF) ^ ((c + d) & 0xFF)


def f6(d: int, c: int, b: int, a: int) -> uint8_t:
	return ((a + c) & 0xFF) ^ ((b + d) & 0xFF)


def f7(d: int, c: int, b: int, a: int) -> uint8_t:
	return ((a + d) & 0xFF) ^ ((b + c) & 0xFF)


def f8(d: int, c: int, b: int, a: int) -> uint8_t:
	return (a - b + c - d) & 0xFF


def f9(d: int, c: int, b: int, a: int) -> uint8_t:
	return (a + b + c - d) & 0xFF


def f10(d: int, c: int, b: int, a: int) -> uint8_t:
	return (a + b - c - d) & 0xFF


def f11(d: int, c: int, b: int, a: int) -> uint8_t:
	return ((a ^ b) - (c ^ d)) & 0xFF


def f12(d: int, c: int, b: int, a: int) -> uint8_t:
	return ((a ^ c) - (b ^ d)) & 0xFF


def f13(d: int, c: int, b: int, a: int) -> uint8_t:
	return ((a ^ d) - (c ^ b)) & 0xFF


def f14(d: int, c: int, b: int, a: int) -> uint8_t:
	return ((a + b) & 0xFF) ^ ((c - d) & 0xFF)


def f15(d: int, c: int, b: int, a: int) -> uint8_t:
	return ((a - b) & 0xFF) ^ ((c + d) & 0xFF)


def f16(d: int, c: int, b: int, a: int) -> uint8_t:
	return ((a + c) & 0xFF) ^ ((b - d) & 0xFF)


def f17(d: int, c: int, b: int, a: int) -> uint8_t:
	return ((a - c) & 0xFF) ^ ((b + d) & 0xFF)


def f18(d: int, c: int, b: int, a: int) -> uint8_t:
	return ((a + d) & 0xFF) ^ ((b - c) & 0xFF)


def f19(d: int, c: int, b: int, a: int) -> uint8_t:
	return ((a - d) & 0xFF) ^ ((b + c) & 0xFF)


reducerIndex = [f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19,]
