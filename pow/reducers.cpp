
typedef uint8_t (*HashReducer)(uint32_t n);

constexpr uint8_t f0(uint32_t n) {
	uint8_t a = (n >> 24) & 0xFF;
	uint8_t b = (n >> 16) & 0xFF;
	uint8_t c = (n >> 8) & 0xFF;
	uint8_t d = n & 0xFF;
	return (a + b + c + d);
}

constexpr uint8_t f1(uint32_t n) {
	uint8_t a = (n >> 24) & 0xFF;
	uint8_t b = (n >> 16) & 0xFF;
	uint8_t c = (n >> 8) & 0xFF;
	uint8_t d = n & 0xFF;
	return (a ^ b ^ c ^ d);
}

constexpr uint8_t f2(uint32_t n) {
	uint8_t a = (n >> 24) & 0xFF;
	uint8_t b = (n >> 16) & 0xFF;
	uint8_t c = (n >> 8) & 0xFF;
	uint8_t d = n & 0xFF;
	return ((a ^ b) + (c ^ d));
}

constexpr uint8_t f3(uint32_t n) {
	uint8_t a = (n >> 24) & 0xFF;
	uint8_t b = (n >> 16) & 0xFF;
	uint8_t c = (n >> 8) & 0xFF;
	uint8_t d = n & 0xFF;
	return ((a ^ c) + (b ^ d));
}

constexpr uint8_t f4(uint32_t n) {
	uint8_t a = (n >> 24) & 0xFF;
	uint8_t b = (n >> 16) & 0xFF;
	uint8_t c = (n >> 8) & 0xFF;
	uint8_t d = n & 0xFF;
	return ((a ^ d) + (c ^ b));
}

constexpr uint8_t f5(uint32_t n) {
	uint8_t a = (n >> 24) & 0xFF;
	uint8_t b = (n >> 16) & 0xFF;
	uint8_t c = (n >> 8) & 0xFF;
	uint8_t d = n & 0xFF;
	return ((a + b)) ^ ((c + d));
}

constexpr uint8_t f6(uint32_t n) {
	uint8_t a = (n >> 24) & 0xFF;
	uint8_t b = (n >> 16) & 0xFF;
	uint8_t c = (n >> 8) & 0xFF;
	uint8_t d = n & 0xFF;
	return ((a + c)) ^ ((b + d));
}

constexpr uint8_t f7(uint32_t n) {
	uint8_t a = (n >> 24) & 0xFF;
	uint8_t b = (n >> 16) & 0xFF;
	uint8_t c = (n >> 8) & 0xFF;
	uint8_t d = n & 0xFF;
	return ((a + d)) ^ ((b + c));
}

constexpr uint8_t f8(uint32_t n) {
	uint8_t a = (n >> 24) & 0xFF;
	uint8_t b = (n >> 16) & 0xFF;
	uint8_t c = (n >> 8) & 0xFF;
	uint8_t d = n & 0xFF;
	return (a - b + c - d);
}

constexpr uint8_t f9(uint32_t n) {
	uint8_t a = (n >> 24) & 0xFF;
	uint8_t b = (n >> 16) & 0xFF;
	uint8_t c = (n >> 8) & 0xFF;
	uint8_t d = n & 0xFF;
	return (a + b + c - d);
}

constexpr uint8_t f10(uint32_t n) {
	uint8_t a = (n >> 24) & 0xFF;
	uint8_t b = (n >> 16) & 0xFF;
	uint8_t c = (n >> 8) & 0xFF;
	uint8_t d = n & 0xFF;
	return (a + b - c - d);
}

constexpr uint8_t f11(uint32_t n) {
	uint8_t a = (n >> 24) & 0xFF;
	uint8_t b = (n >> 16) & 0xFF;
	uint8_t c = (n >> 8) & 0xFF;
	uint8_t d = n & 0xFF;
	return ((a ^ b) - (c ^ d));
}

constexpr uint8_t f12(uint32_t n) {
	uint8_t a = (n >> 24) & 0xFF;
	uint8_t b = (n >> 16) & 0xFF;
	uint8_t c = (n >> 8) & 0xFF;
	uint8_t d = n & 0xFF;
	return ((a ^ c) - (b ^ d));
}

constexpr uint8_t f13(uint32_t n) {
	uint8_t a = (n >> 24) & 0xFF;
	uint8_t b = (n >> 16) & 0xFF;
	uint8_t c = (n >> 8) & 0xFF;
	uint8_t d = n & 0xFF;
	return ((a ^ d) - (c ^ b));
}

constexpr uint8_t f14(uint32_t n) {
	uint8_t a = (n >> 24) & 0xFF;
	uint8_t b = (n >> 16) & 0xFF;
	uint8_t c = (n >> 8) & 0xFF;
	uint8_t d = n & 0xFF;
	return ((a + b)) ^ ((c - d));
}

constexpr uint8_t f15(uint32_t n) {
	uint8_t a = (n >> 24) & 0xFF;
	uint8_t b = (n >> 16) & 0xFF;
	uint8_t c = (n >> 8) & 0xFF;
	uint8_t d = n & 0xFF;
	return ((a - b)) ^ ((c + d));
}

constexpr uint8_t f16(uint32_t n) {
	uint8_t a = (n >> 24) & 0xFF;
	uint8_t b = (n >> 16) & 0xFF;
	uint8_t c = (n >> 8) & 0xFF;
	uint8_t d = n & 0xFF;
	return ((a + c)) ^ ((b - d));
}

constexpr uint8_t f17(uint32_t n) {
	uint8_t a = (n >> 24) & 0xFF;
	uint8_t b = (n >> 16) & 0xFF;
	uint8_t c = (n >> 8) & 0xFF;
	uint8_t d = n & 0xFF;
	return ((a - c)) ^ ((b + d));
}

constexpr uint8_t f18(uint32_t n) {
	uint8_t a = (n >> 24) & 0xFF;
	uint8_t b = (n >> 16) & 0xFF;
	uint8_t c = (n >> 8) & 0xFF;
	uint8_t d = n & 0xFF;
	return ((a + d)) ^ ((b - c));
}

constexpr uint8_t f19(uint32_t n) {
	uint8_t a = (n >> 24) & 0xFF;
	uint8_t b = (n >> 16) & 0xFF;
	uint8_t c = (n >> 8) & 0xFF;
	uint8_t d = n & 0xFF;
	return ((a - d)) ^ ((b + c));
}

constexpr std::array<HashReducer, 22> reductors {f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19};
