#pragma once
/*
   BLAKE2 reference source code package - optimized C implementations

   Copyright 2012, Samuel Neves <sneves@dei.uc.pt>.  You may use this under the
   terms of the CC0, the OpenSSL Licence, or the Apache Public License 2.0, at
   your option.  The terms of these licenses can be found at:

   - CC0 1.0 Universal : https://creativecommons.org/publicdomain/zero/1.0
   - OpenSSL license   : https://www.openssl.org/source/license.html
   - Apache 2.0        : http://www.apache.org/licenses/LICENSE-2.0

   More information about the BLAKE2 hash function can be found at
   https://blake2.net.
*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/*** Start of inlined file: blake2.h ***/

#include <stddef.h>
#include <stdint.h>

#if defined(_MSC_VER)
#define BLAKE2_PACKED(x) __pragma(pack(push, 1)) x __pragma(pack(pop))
#else
#define BLAKE2_PACKED(x) x __attribute__((packed))
#endif

#if defined(__cplusplus)
extern "C" {
#endif

enum blake2s_constant {
	BLAKE2S_BLOCKBYTES = 64,
	BLAKE2S_OUTBYTES = 32,
	BLAKE2S_KEYBYTES = 32,
	BLAKE2S_SALTBYTES = 8,
	BLAKE2S_PERSONALBYTES = 8
};

enum blake2b_constant {
	BLAKE2B_BLOCKBYTES = 128,
	BLAKE2B_OUTBYTES = 64,
	BLAKE2B_KEYBYTES = 64,
	BLAKE2B_SALTBYTES = 16,
	BLAKE2B_PERSONALBYTES = 16
};

typedef struct blake2s_state__ {
	uint32_t h[8];
	uint32_t t[2];
	uint32_t f[2];
	uint8_t buf[BLAKE2S_BLOCKBYTES];
	size_t buflen;
	size_t outlen;
	uint8_t last_node;
} blake2s_state;

typedef struct blake2b_state__ {
	uint64_t h[8];
	uint64_t t[2];
	uint64_t f[2];
	uint8_t buf[BLAKE2B_BLOCKBYTES];
	size_t buflen;
	size_t outlen;
	uint8_t last_node;
} blake2b_state;

typedef struct blake2sp_state__ {
	blake2s_state S[8][1];
	blake2s_state R[1];
	uint8_t buf[8 * BLAKE2S_BLOCKBYTES];
	size_t buflen;
	size_t outlen;
} blake2sp_state;

typedef struct blake2bp_state__ {
	blake2b_state S[4][1];
	blake2b_state R[1];
	uint8_t buf[4 * BLAKE2B_BLOCKBYTES];
	size_t buflen;
	size_t outlen;
} blake2bp_state;

BLAKE2_PACKED(struct blake2s_param__ {
	uint8_t digest_length; /* 1 */
	uint8_t key_length;	   /* 2 */
	uint8_t fanout;		   /* 3 */
	uint8_t depth;		   /* 4 */
	uint32_t leaf_length;  /* 8 */
	uint32_t node_offset;  /* 12 */
	uint16_t xof_length;   /* 14 */
	uint8_t node_depth;	   /* 15 */
	uint8_t inner_length;  /* 16 */
	/* uint8_t  reserved[0]; */
	uint8_t salt[BLAKE2S_SALTBYTES];		 /* 24 */
	uint8_t personal[BLAKE2S_PERSONALBYTES]; /* 32 */
});

typedef struct blake2s_param__ blake2s_param;

BLAKE2_PACKED(struct blake2b_param__ {
	uint8_t digest_length;					 /* 1 */
	uint8_t key_length;						 /* 2 */
	uint8_t fanout;							 /* 3 */
	uint8_t depth;							 /* 4 */
	uint32_t leaf_length;					 /* 8 */
	uint32_t node_offset;					 /* 12 */
	uint32_t xof_length;					 /* 16 */
	uint8_t node_depth;						 /* 17 */
	uint8_t inner_length;					 /* 18 */
	uint8_t reserved[14];					 /* 32 */
	uint8_t salt[BLAKE2B_SALTBYTES];		 /* 48 */
	uint8_t personal[BLAKE2B_PERSONALBYTES]; /* 64 */
});

typedef struct blake2b_param__ blake2b_param;

typedef struct blake2xs_state__ {
	blake2s_state S[1];
	blake2s_param P[1];
} blake2xs_state;

typedef struct blake2xb_state__ {
	blake2b_state S[1];
	blake2b_param P[1];
} blake2xb_state;

/* Padded structs result in a compile-time error */
enum {
	BLAKE2_DUMMY_1 = 1 / (int) (sizeof(blake2s_param) == BLAKE2S_OUTBYTES),
	BLAKE2_DUMMY_2 = 1 / (int) (sizeof(blake2b_param) == BLAKE2B_OUTBYTES)
};

/* Streaming API */
int blake2s_init(blake2s_state *S, size_t outlen);
int blake2s_init_key(blake2s_state *S, size_t outlen, const void *key, size_t keylen);
int blake2s_init_param(blake2s_state *S, const blake2s_param *P);
int blake2s_update(blake2s_state *S, const void *in, size_t inlen);
int blake2s_final(blake2s_state *S, void *out, size_t outlen);

int blake2b_init(blake2b_state *S, size_t outlen);
int blake2b_init_key(blake2b_state *S, size_t outlen, const void *key, size_t keylen);
int blake2b_init_param(blake2b_state *S, const blake2b_param *P);
int blake2b_update(blake2b_state *S, const void *in, size_t inlen);
int blake2b_final(blake2b_state *S, void *out, size_t outlen);

int blake2sp_init(blake2sp_state *S, size_t outlen);
int blake2sp_init_key(blake2sp_state *S, size_t outlen, const void *key, size_t keylen);
int blake2sp_update(blake2sp_state *S, const void *in, size_t inlen);
int blake2sp_final(blake2sp_state *S, void *out, size_t outlen);

int blake2bp_init(blake2bp_state *S, size_t outlen);
int blake2bp_init_key(blake2bp_state *S, size_t outlen, const void *key, size_t keylen);
int blake2bp_update(blake2bp_state *S, const void *in, size_t inlen);
int blake2bp_final(blake2bp_state *S, void *out, size_t outlen);

/* Variable output length API */
int blake2xs_init(blake2xs_state *S, const size_t outlen);
int blake2xs_init_key(blake2xs_state *S, const size_t outlen, const void *key, size_t keylen);
int blake2xs_update(blake2xs_state *S, const void *in, size_t inlen);
int blake2xs_final(blake2xs_state *S, void *out, size_t outlen);

int blake2xb_init(blake2xb_state *S, const size_t outlen);
int blake2xb_init_key(blake2xb_state *S, const size_t outlen, const void *key, size_t keylen);
int blake2xb_update(blake2xb_state *S, const void *in, size_t inlen);
int blake2xb_final(blake2xb_state *S, void *out, size_t outlen);

/* Simple API */
int blake2s(void *out, size_t outlen, const void *in, size_t inlen, const void *key, size_t keylen);
int blake2b(void *out, size_t outlen, const void *in, size_t inlen, const void *key, size_t keylen);

int blake2sp(void *out, size_t outlen, const void *in, size_t inlen, const void *key, size_t keylen);
int blake2bp(void *out, size_t outlen, const void *in, size_t inlen, const void *key, size_t keylen);

int blake2xs(void *out, size_t outlen, const void *in, size_t inlen, const void *key, size_t keylen);
int blake2xb(void *out, size_t outlen, const void *in, size_t inlen, const void *key, size_t keylen);

/* This is simply an alias for blake2b */
int blake2(void *out, size_t outlen, const void *in, size_t inlen, const void *key, size_t keylen);

#if defined(__cplusplus)
}
#endif

/*** End of inlined file: blake2.h ***/

/*** Start of inlined file: blake2-impl.h ***/
#ifndef BLAKE2_IMPL_H
#define BLAKE2_IMPL_H

#include <stdint.h>
#include <string.h>

#if !defined(__cplusplus) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
#if defined(_MSC_VER)
#define BLAKE2_INLINE __inline
#elif defined(__GNUC__)
#define BLAKE2_INLINE __inline__
#else
#define BLAKE2_INLINE
#endif
#else
#define BLAKE2_INLINE inline
#endif

static BLAKE2_INLINE uint32_t load32(const void *src) {
#if defined(NATIVE_LITTLE_ENDIAN)
	uint32_t w;
	memcpy(&w, src, sizeof w);
	return w;
#else
	const uint8_t *p = (const uint8_t *) src;
	return ((uint32_t) (p[0]) << 0) |
		   ((uint32_t) (p[1]) << 8) |
		   ((uint32_t) (p[2]) << 16) |
		   ((uint32_t) (p[3]) << 24);
#endif
}

static BLAKE2_INLINE uint64_t load64(const void *src) {
#if defined(NATIVE_LITTLE_ENDIAN)
	uint64_t w;
	memcpy(&w, src, sizeof w);
	return w;
#else
	const uint8_t *p = (const uint8_t *) src;
	return ((uint64_t) (p[0]) << 0) |
		   ((uint64_t) (p[1]) << 8) |
		   ((uint64_t) (p[2]) << 16) |
		   ((uint64_t) (p[3]) << 24) |
		   ((uint64_t) (p[4]) << 32) |
		   ((uint64_t) (p[5]) << 40) |
		   ((uint64_t) (p[6]) << 48) |
		   ((uint64_t) (p[7]) << 56);
#endif
}

static BLAKE2_INLINE uint16_t load16(const void *src) {
#if defined(NATIVE_LITTLE_ENDIAN)
	uint16_t w;
	memcpy(&w, src, sizeof w);
	return w;
#else
	const uint8_t *p = (const uint8_t *) src;
	return (uint16_t) (((uint32_t) (p[0]) << 0) |
					   ((uint32_t) (p[1]) << 8));
#endif
}

static BLAKE2_INLINE void store16(void *dst, uint16_t w) {
#if defined(NATIVE_LITTLE_ENDIAN)
	memcpy(dst, &w, sizeof w);
#else
	uint8_t *p = (uint8_t *) dst;
	*p++ = (uint8_t) w;
	w >>= 8;
	*p++ = (uint8_t) w;
#endif
}

static BLAKE2_INLINE void store32(void *dst, uint32_t w) {
#if defined(NATIVE_LITTLE_ENDIAN)
	memcpy(dst, &w, sizeof w);
#else
	uint8_t *p = (uint8_t *) dst;
	p[0] = (uint8_t) (w >> 0);
	p[1] = (uint8_t) (w >> 8);
	p[2] = (uint8_t) (w >> 16);
	p[3] = (uint8_t) (w >> 24);
#endif
}

static BLAKE2_INLINE void store64(void *dst, uint64_t w) {
#if defined(NATIVE_LITTLE_ENDIAN)
	memcpy(dst, &w, sizeof w);
#else
	uint8_t *p = (uint8_t *) dst;
	p[0] = (uint8_t) (w >> 0);
	p[1] = (uint8_t) (w >> 8);
	p[2] = (uint8_t) (w >> 16);
	p[3] = (uint8_t) (w >> 24);
	p[4] = (uint8_t) (w >> 32);
	p[5] = (uint8_t) (w >> 40);
	p[6] = (uint8_t) (w >> 48);
	p[7] = (uint8_t) (w >> 56);
#endif
}

static BLAKE2_INLINE uint64_t load48(const void *src) {
	const uint8_t *p = (const uint8_t *) src;
	return ((uint64_t) (p[0]) << 0) |
		   ((uint64_t) (p[1]) << 8) |
		   ((uint64_t) (p[2]) << 16) |
		   ((uint64_t) (p[3]) << 24) |
		   ((uint64_t) (p[4]) << 32) |
		   ((uint64_t) (p[5]) << 40);
}

static BLAKE2_INLINE void store48(void *dst, uint64_t w) {
	uint8_t *p = (uint8_t *) dst;
	p[0] = (uint8_t) (w >> 0);
	p[1] = (uint8_t) (w >> 8);
	p[2] = (uint8_t) (w >> 16);
	p[3] = (uint8_t) (w >> 24);
	p[4] = (uint8_t) (w >> 32);
	p[5] = (uint8_t) (w >> 40);
}

static BLAKE2_INLINE uint32_t rotr32(const uint32_t w, const unsigned c) {
	return (w >> c) | (w << (32 - c));
}

static BLAKE2_INLINE uint64_t rotr64(const uint64_t w, const unsigned c) {
	return (w >> c) | (w << (64 - c));
}

/* prevents compiler optimizing out memset() */
static BLAKE2_INLINE void secure_zero_memory(void *v, size_t n) {
	static void *(*const volatile memset_v)(void *, int, size_t) = &memset;
	memset_v(v, 0, n);
}

#endif

/*** End of inlined file: blake2-impl.h ***/

/*** Start of inlined file: blake2-config.h ***/
#ifndef BLAKE2_CONFIG_H
#define BLAKE2_CONFIG_H

/* These don't work everywhere */
#if defined(__SSE2__) || defined(__x86_64__) || defined(__amd64__)
#define HAVE_SSE2
#endif

#if defined(__SSSE3__)
#define HAVE_SSSE3
#endif

#if defined(__SSE4_1__)
#define HAVE_SSE41
#endif

#if defined(__AVX__)
#define HAVE_AVX
#endif

#if defined(__XOP__)
#define HAVE_XOP
#endif

#ifdef HAVE_AVX2
#ifndef HAVE_AVX
#define HAVE_AVX
#endif
#endif

#ifdef HAVE_XOP
#ifndef HAVE_AVX
#define HAVE_AVX
#endif
#endif

#ifdef HAVE_AVX
#ifndef HAVE_SSE41
#define HAVE_SSE41
#endif
#endif

#ifdef HAVE_SSE41
#ifndef HAVE_SSSE3
#define HAVE_SSSE3
#endif
#endif

#ifdef HAVE_SSSE3
#define HAVE_SSE2
#endif

#if !defined(HAVE_SSE2)
#error "This code requires at least SSE2."
#endif

#endif

/*** End of inlined file: blake2-config.h ***/

#include <emmintrin.h>
#if defined(HAVE_SSSE3)
#include <tmmintrin.h>
#endif
#if defined(HAVE_SSE41)
#include <smmintrin.h>
#endif
#if defined(HAVE_AVX)
#include <immintrin.h>
#endif
#if defined(HAVE_XOP)
#include <x86intrin.h>
#endif

/*** Start of inlined file: blake2s-round.h ***/
#ifndef BLAKE2S_ROUND_H
#define BLAKE2S_ROUND_H

#define LOADU(p) _mm_loadu_si128((const __m128i *) (p))
#define STOREU(p, r) _mm_storeu_si128((__m128i *) (p), r)

#define TOF(reg) _mm_castsi128_ps((reg))
#define TOI(reg) _mm_castps_si128((reg))

#define LIKELY(x) __builtin_expect((x), 1)

/* Microarchitecture-specific macros */
#ifndef HAVE_XOP
#ifdef HAVE_SSSE3
#define _mm_roti_epi32(r, c) (                \
	(8 == -(c))	   ? _mm_shuffle_epi8(r, r8)  \
	: (16 == -(c)) ? _mm_shuffle_epi8(r, r16) \
				   : _mm_xor_si128(_mm_srli_epi32((r), -(c)), _mm_slli_epi32((r), 32 - (-(c)))))
#else
#define _mm_roti_epi32(r, c) _mm_xor_si128(_mm_srli_epi32((r), -(c)), _mm_slli_epi32((r), 32 - (-(c))))
#endif
#else
/* ... */
#endif

#define G1(row1, row2, row3, row4, buf)                   \
	row1 = _mm_add_epi32(_mm_add_epi32(row1, buf), row2); \
	row4 = _mm_xor_si128(row4, row1);                     \
	row4 = _mm_roti_epi32(row4, -16);                     \
	row3 = _mm_add_epi32(row3, row4);                     \
	row2 = _mm_xor_si128(row2, row3);                     \
	row2 = _mm_roti_epi32(row2, -12);

#define G2(row1, row2, row3, row4, buf)                   \
	row1 = _mm_add_epi32(_mm_add_epi32(row1, buf), row2); \
	row4 = _mm_xor_si128(row4, row1);                     \
	row4 = _mm_roti_epi32(row4, -8);                      \
	row3 = _mm_add_epi32(row3, row4);                     \
	row2 = _mm_xor_si128(row2, row3);                     \
	row2 = _mm_roti_epi32(row2, -7);

#define DIAGONALIZE(row1, row2, row3, row4)                  \
	row1 = _mm_shuffle_epi32(row1, _MM_SHUFFLE(2, 1, 0, 3)); \
	row4 = _mm_shuffle_epi32(row4, _MM_SHUFFLE(1, 0, 3, 2)); \
	row3 = _mm_shuffle_epi32(row3, _MM_SHUFFLE(0, 3, 2, 1));

#define UNDIAGONALIZE(row1, row2, row3, row4)                \
	row1 = _mm_shuffle_epi32(row1, _MM_SHUFFLE(0, 3, 2, 1)); \
	row4 = _mm_shuffle_epi32(row4, _MM_SHUFFLE(1, 0, 3, 2)); \
	row3 = _mm_shuffle_epi32(row3, _MM_SHUFFLE(2, 1, 0, 3));

#if defined(HAVE_XOP)

/*** Start of inlined file: blake2s-load-xop.h ***/
#ifndef BLAKE2S_LOAD_XOP_H
#define BLAKE2S_LOAD_XOP_H

#define TOB(x) ((x) *4 * 0x01010101 + 0x03020100) /* ..or not TOB */

#if 0
/* Basic VPPERM emulation, for testing purposes */
static __m128i _mm_perm_epi8(const __m128i src1, const __m128i src2, const __m128i sel)
{
   const __m128i sixteen = _mm_set1_epi8(16);
   const __m128i t0 = _mm_shuffle_epi8(src1, sel);
   const __m128i s1 = _mm_shuffle_epi8(src2, _mm_sub_epi8(sel, sixteen));
   const __m128i mask = _mm_or_si128(_mm_cmpeq_epi8(sel, sixteen),
									 _mm_cmpgt_epi8(sel, sixteen)); /* (>=16) = 0xff : 00 */
   return _mm_blendv_epi8(t0, s1, mask);
}
#endif

#define LOAD_MSG_0_1(buf) \
	buf = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(6), TOB(4), TOB(2), TOB(0)));

#define LOAD_MSG_0_2(buf) \
	buf = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(7), TOB(5), TOB(3), TOB(1)));

#define LOAD_MSG_0_3(buf) \
	buf = _mm_perm_epi8(m2, m3, _mm_set_epi32(TOB(4), TOB(2), TOB(0), TOB(6)));

#define LOAD_MSG_0_4(buf) \
	buf = _mm_perm_epi8(m2, m3, _mm_set_epi32(TOB(5), TOB(3), TOB(1), TOB(7)));

#define LOAD_MSG_1_1(buf)                                                      \
	t0 = _mm_perm_epi8(m1, m2, _mm_set_epi32(TOB(0), TOB(5), TOB(0), TOB(0))); \
	buf = _mm_perm_epi8(t0, m3, _mm_set_epi32(TOB(5), TOB(2), TOB(1), TOB(6)));

#define LOAD_MSG_1_2(buf)                                                      \
	t1 = _mm_perm_epi8(m1, m2, _mm_set_epi32(TOB(2), TOB(0), TOB(4), TOB(6))); \
	buf = _mm_perm_epi8(t1, m3, _mm_set_epi32(TOB(3), TOB(7), TOB(1), TOB(0)));

#define LOAD_MSG_1_3(buf)                                                      \
	t0 = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(5), TOB(0), TOB(0), TOB(1))); \
	buf = _mm_perm_epi8(t0, m2, _mm_set_epi32(TOB(7), TOB(1), TOB(0), TOB(3)));

#define LOAD_MSG_1_4(buf)                                                      \
	t1 = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(3), TOB(7), TOB(2), TOB(0))); \
	buf = _mm_perm_epi8(t1, m3, _mm_set_epi32(TOB(2), TOB(1), TOB(4), TOB(3)));

#define LOAD_MSG_2_1(buf)                                                      \
	t0 = _mm_perm_epi8(m1, m2, _mm_set_epi32(TOB(0), TOB(1), TOB(0), TOB(7))); \
	buf = _mm_perm_epi8(t0, m3, _mm_set_epi32(TOB(7), TOB(2), TOB(4), TOB(0)));

#define LOAD_MSG_2_2(buf)                                                      \
	t1 = _mm_perm_epi8(m0, m2, _mm_set_epi32(TOB(0), TOB(2), TOB(0), TOB(4))); \
	buf = _mm_perm_epi8(t1, m3, _mm_set_epi32(TOB(5), TOB(2), TOB(1), TOB(0)));

#define LOAD_MSG_2_3(buf)                                                      \
	t0 = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(0), TOB(7), TOB(3), TOB(0))); \
	buf = _mm_perm_epi8(t0, m2, _mm_set_epi32(TOB(2), TOB(1), TOB(6), TOB(5)));

#define LOAD_MSG_2_4(buf)                                                      \
	t1 = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(4), TOB(1), TOB(6), TOB(0))); \
	buf = _mm_perm_epi8(t1, m3, _mm_set_epi32(TOB(2), TOB(1), TOB(6), TOB(3)));

#define LOAD_MSG_3_1(buf)                                                      \
	t0 = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(0), TOB(0), TOB(3), TOB(7))); \
	t0 = _mm_perm_epi8(t0, m2, _mm_set_epi32(TOB(7), TOB(2), TOB(1), TOB(0))); \
	buf = _mm_perm_epi8(t0, m3, _mm_set_epi32(TOB(3), TOB(5), TOB(1), TOB(0)));

#define LOAD_MSG_3_2(buf)                                                      \
	t1 = _mm_perm_epi8(m0, m2, _mm_set_epi32(TOB(0), TOB(0), TOB(1), TOB(5))); \
	buf = _mm_perm_epi8(t1, m3, _mm_set_epi32(TOB(6), TOB(4), TOB(1), TOB(0)));

#define LOAD_MSG_3_3(buf)                                                      \
	t0 = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(0), TOB(4), TOB(5), TOB(2))); \
	buf = _mm_perm_epi8(t0, m3, _mm_set_epi32(TOB(2), TOB(1), TOB(0), TOB(7)));

#define LOAD_MSG_3_4(buf)                                                      \
	t1 = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(0), TOB(0), TOB(0), TOB(6))); \
	buf = _mm_perm_epi8(t1, m2, _mm_set_epi32(TOB(2), TOB(6), TOB(0), TOB(4)));

#define LOAD_MSG_4_1(buf)                                                      \
	t0 = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(0), TOB(2), TOB(5), TOB(0))); \
	buf = _mm_perm_epi8(t0, m2, _mm_set_epi32(TOB(6), TOB(2), TOB(1), TOB(5)));

#define LOAD_MSG_4_2(buf)                                                      \
	t1 = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(0), TOB(4), TOB(7), TOB(0))); \
	buf = _mm_perm_epi8(t1, m3, _mm_set_epi32(TOB(7), TOB(2), TOB(1), TOB(0)));

#define LOAD_MSG_4_3(buf)                                                      \
	t0 = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(3), TOB(6), TOB(0), TOB(0))); \
	t0 = _mm_perm_epi8(t0, m2, _mm_set_epi32(TOB(3), TOB(2), TOB(7), TOB(0))); \
	buf = _mm_perm_epi8(t0, m3, _mm_set_epi32(TOB(2), TOB(1), TOB(6), TOB(3)));

#define LOAD_MSG_4_4(buf)                                                      \
	t1 = _mm_perm_epi8(m0, m2, _mm_set_epi32(TOB(0), TOB(4), TOB(0), TOB(1))); \
	buf = _mm_perm_epi8(t1, m3, _mm_set_epi32(TOB(2), TOB(4), TOB(0), TOB(5)));

#define LOAD_MSG_5_1(buf)                                                      \
	t0 = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(0), TOB(0), TOB(6), TOB(2))); \
	buf = _mm_perm_epi8(t0, m2, _mm_set_epi32(TOB(4), TOB(2), TOB(1), TOB(0)));

#define LOAD_MSG_5_2(buf)                                                      \
	t1 = _mm_perm_epi8(m0, m2, _mm_set_epi32(TOB(3), TOB(7), TOB(6), TOB(0))); \
	buf = _mm_perm_epi8(t1, m3, _mm_set_epi32(TOB(3), TOB(2), TOB(1), TOB(4)));

#define LOAD_MSG_5_3(buf)                                                      \
	t0 = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(1), TOB(0), TOB(7), TOB(4))); \
	buf = _mm_perm_epi8(t0, m3, _mm_set_epi32(TOB(7), TOB(1), TOB(0), TOB(3)));

#define LOAD_MSG_5_4(buf)                                                      \
	t1 = _mm_perm_epi8(m1, m2, _mm_set_epi32(TOB(5), TOB(0), TOB(1), TOB(0))); \
	buf = _mm_perm_epi8(t1, m3, _mm_set_epi32(TOB(6), TOB(1), TOB(5), TOB(3)));

#define LOAD_MSG_6_1(buf)                                                      \
	t0 = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(4), TOB(0), TOB(1), TOB(0))); \
	buf = _mm_perm_epi8(t0, m3, _mm_set_epi32(TOB(3), TOB(6), TOB(1), TOB(4)));

#define LOAD_MSG_6_2(buf)                                                      \
	t1 = _mm_perm_epi8(m1, m2, _mm_set_epi32(TOB(6), TOB(0), TOB(0), TOB(1))); \
	buf = _mm_perm_epi8(t1, m3, _mm_set_epi32(TOB(3), TOB(5), TOB(7), TOB(0)));

#define LOAD_MSG_6_3(buf)                                                      \
	t0 = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(0), TOB(0), TOB(6), TOB(0))); \
	buf = _mm_perm_epi8(t0, m2, _mm_set_epi32(TOB(5), TOB(1), TOB(0), TOB(4)));

#define LOAD_MSG_6_4(buf)                                                      \
	t1 = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(0), TOB(2), TOB(3), TOB(7))); \
	buf = _mm_perm_epi8(t1, m2, _mm_set_epi32(TOB(2), TOB(1), TOB(0), TOB(7)));

#define LOAD_MSG_7_1(buf)                                                      \
	t0 = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(3), TOB(0), TOB(7), TOB(0))); \
	buf = _mm_perm_epi8(t0, m3, _mm_set_epi32(TOB(3), TOB(4), TOB(1), TOB(5)));

#define LOAD_MSG_7_2(buf)                                                      \
	t1 = _mm_perm_epi8(m0, m2, _mm_set_epi32(TOB(5), TOB(1), TOB(0), TOB(7))); \
	buf = _mm_perm_epi8(t1, m3, _mm_set_epi32(TOB(3), TOB(2), TOB(6), TOB(0)));

#define LOAD_MSG_7_3(buf)                                                      \
	t0 = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(2), TOB(0), TOB(0), TOB(5))); \
	t0 = _mm_perm_epi8(t0, m2, _mm_set_epi32(TOB(3), TOB(4), TOB(1), TOB(0))); \
	buf = _mm_perm_epi8(t0, m3, _mm_set_epi32(TOB(2), TOB(7), TOB(0), TOB(3)));

#define LOAD_MSG_7_4(buf)                                                      \
	t1 = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(0), TOB(6), TOB(4), TOB(0))); \
	buf = _mm_perm_epi8(t1, m2, _mm_set_epi32(TOB(2), TOB(1), TOB(0), TOB(6)));

#define LOAD_MSG_8_1(buf)                                                      \
	t0 = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(0), TOB(0), TOB(0), TOB(6))); \
	t0 = _mm_perm_epi8(t0, m2, _mm_set_epi32(TOB(3), TOB(7), TOB(1), TOB(0))); \
	buf = _mm_perm_epi8(t0, m3, _mm_set_epi32(TOB(3), TOB(2), TOB(6), TOB(0)));

#define LOAD_MSG_8_2(buf)                                                      \
	t1 = _mm_perm_epi8(m0, m2, _mm_set_epi32(TOB(4), TOB(3), TOB(5), TOB(0))); \
	buf = _mm_perm_epi8(t1, m3, _mm_set_epi32(TOB(3), TOB(2), TOB(1), TOB(7)));

#define LOAD_MSG_8_3(buf)                                                      \
	t0 = _mm_perm_epi8(m0, m2, _mm_set_epi32(TOB(6), TOB(1), TOB(0), TOB(0))); \
	buf = _mm_perm_epi8(t0, m3, _mm_set_epi32(TOB(2), TOB(5), TOB(4), TOB(3)));

#define LOAD_MSG_8_4(buf) \
	buf = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(4), TOB(7), TOB(2), TOB(5)));

#define LOAD_MSG_9_1(buf)                                                      \
	t0 = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(1), TOB(7), TOB(0), TOB(0))); \
	buf = _mm_perm_epi8(t0, m2, _mm_set_epi32(TOB(3), TOB(2), TOB(4), TOB(6)));

#define LOAD_MSG_9_2(buf) \
	buf = _mm_perm_epi8(m0, m1, _mm_set_epi32(TOB(5), TOB(6), TOB(4), TOB(2)));

#define LOAD_MSG_9_3(buf)                                                      \
	t0 = _mm_perm_epi8(m0, m2, _mm_set_epi32(TOB(0), TOB(3), TOB(5), TOB(0))); \
	buf = _mm_perm_epi8(t0, m3, _mm_set_epi32(TOB(2), TOB(1), TOB(7), TOB(5)));

#define LOAD_MSG_9_4(buf)                                                      \
	t1 = _mm_perm_epi8(m0, m2, _mm_set_epi32(TOB(0), TOB(0), TOB(0), TOB(7))); \
	buf = _mm_perm_epi8(t1, m3, _mm_set_epi32(TOB(4), TOB(6), TOB(0), TOB(3)));

#endif

/*** End of inlined file: blake2s-load-xop.h ***/

#elif defined(HAVE_SSE41)

/*** Start of inlined file: blake2s-load-sse41.h ***/
#ifndef BLAKE2S_LOAD_SSE41_H
#define BLAKE2S_LOAD_SSE41_H

#define LOAD_MSG_0_1(buf) \
	buf = TOI(_mm_shuffle_ps(TOF(m0), TOF(m1), _MM_SHUFFLE(2, 0, 2, 0)));

#define LOAD_MSG_0_2(buf) \
	buf = TOI(_mm_shuffle_ps(TOF(m0), TOF(m1), _MM_SHUFFLE(3, 1, 3, 1)));

#define LOAD_MSG_0_3(buf)                                \
	t0 = _mm_shuffle_epi32(m2, _MM_SHUFFLE(3, 2, 0, 1)); \
	t1 = _mm_shuffle_epi32(m3, _MM_SHUFFLE(0, 1, 3, 2)); \
	buf = _mm_blend_epi16(t0, t1, 0xC3);

#define LOAD_MSG_0_4(buf)               \
	t0 = _mm_blend_epi16(t0, t1, 0x3C); \
	buf = _mm_shuffle_epi32(t0, _MM_SHUFFLE(2, 3, 0, 1));

#define LOAD_MSG_1_1(buf)               \
	t0 = _mm_blend_epi16(m1, m2, 0x0C); \
	t1 = _mm_slli_si128(m3, 4);         \
	t2 = _mm_blend_epi16(t0, t1, 0xF0); \
	buf = _mm_shuffle_epi32(t2, _MM_SHUFFLE(2, 1, 0, 3));

#define LOAD_MSG_1_2(buf)                                \
	t0 = _mm_shuffle_epi32(m2, _MM_SHUFFLE(0, 0, 2, 0)); \
	t1 = _mm_blend_epi16(m1, m3, 0xC0);                  \
	t2 = _mm_blend_epi16(t0, t1, 0xF0);                  \
	buf = _mm_shuffle_epi32(t2, _MM_SHUFFLE(2, 3, 0, 1));

#define LOAD_MSG_1_3(buf)               \
	t0 = _mm_slli_si128(m1, 4);         \
	t1 = _mm_blend_epi16(m2, t0, 0x30); \
	t2 = _mm_blend_epi16(m0, t1, 0xF0); \
	buf = _mm_shuffle_epi32(t2, _MM_SHUFFLE(3, 0, 1, 2));

#define LOAD_MSG_1_4(buf)               \
	t0 = _mm_unpackhi_epi32(m0, m1);    \
	t1 = _mm_slli_si128(m3, 4);         \
	t2 = _mm_blend_epi16(t0, t1, 0x0C); \
	buf = _mm_shuffle_epi32(t2, _MM_SHUFFLE(3, 0, 1, 2));

#define LOAD_MSG_2_1(buf)               \
	t0 = _mm_unpackhi_epi32(m2, m3);    \
	t1 = _mm_blend_epi16(m3, m1, 0x0C); \
	t2 = _mm_blend_epi16(t0, t1, 0x0F); \
	buf = _mm_shuffle_epi32(t2, _MM_SHUFFLE(3, 1, 0, 2));

#define LOAD_MSG_2_2(buf)               \
	t0 = _mm_unpacklo_epi32(m2, m0);    \
	t1 = _mm_blend_epi16(t0, m0, 0xF0); \
	t2 = _mm_slli_si128(m3, 8);         \
	buf = _mm_blend_epi16(t1, t2, 0xC0);

#define LOAD_MSG_2_3(buf)               \
	t0 = _mm_blend_epi16(m0, m2, 0x3C); \
	t1 = _mm_srli_si128(m1, 12);        \
	t2 = _mm_blend_epi16(t0, t1, 0x03); \
	buf = _mm_shuffle_epi32(t2, _MM_SHUFFLE(0, 3, 2, 1));

#define LOAD_MSG_2_4(buf)               \
	t0 = _mm_slli_si128(m3, 4);         \
	t1 = _mm_blend_epi16(m0, m1, 0x33); \
	t2 = _mm_blend_epi16(t1, t0, 0xC0); \
	buf = _mm_shuffle_epi32(t2, _MM_SHUFFLE(1, 2, 3, 0));

#define LOAD_MSG_3_1(buf)               \
	t0 = _mm_unpackhi_epi32(m0, m1);    \
	t1 = _mm_unpackhi_epi32(t0, m2);    \
	t2 = _mm_blend_epi16(t1, m3, 0x0C); \
	buf = _mm_shuffle_epi32(t2, _MM_SHUFFLE(3, 1, 0, 2));

#define LOAD_MSG_3_2(buf)               \
	t0 = _mm_slli_si128(m2, 8);         \
	t1 = _mm_blend_epi16(m3, m0, 0x0C); \
	t2 = _mm_blend_epi16(t1, t0, 0xC0); \
	buf = _mm_shuffle_epi32(t2, _MM_SHUFFLE(2, 0, 1, 3));

#define LOAD_MSG_3_3(buf)               \
	t0 = _mm_blend_epi16(m0, m1, 0x0F); \
	t1 = _mm_blend_epi16(t0, m3, 0xC0); \
	buf = _mm_shuffle_epi32(t1, _MM_SHUFFLE(0, 1, 2, 3));

#define LOAD_MSG_3_4(buf)            \
	t0 = _mm_alignr_epi8(m0, m1, 4); \
	buf = _mm_blend_epi16(t0, m2, 0x33);

#define LOAD_MSG_4_1(buf)               \
	t0 = _mm_unpacklo_epi64(m1, m2);    \
	t1 = _mm_unpackhi_epi64(m0, m2);    \
	t2 = _mm_blend_epi16(t0, t1, 0x33); \
	buf = _mm_shuffle_epi32(t2, _MM_SHUFFLE(2, 0, 1, 3));

#define LOAD_MSG_4_2(buf)            \
	t0 = _mm_unpackhi_epi64(m1, m3); \
	t1 = _mm_unpacklo_epi64(m0, m1); \
	buf = _mm_blend_epi16(t0, t1, 0x33);

#define LOAD_MSG_4_3(buf)               \
	t0 = _mm_unpackhi_epi64(m3, m1);    \
	t1 = _mm_unpackhi_epi64(m2, m0);    \
	t2 = _mm_blend_epi16(t1, t0, 0x33); \
	buf = _mm_shuffle_epi32(t2, _MM_SHUFFLE(2, 1, 0, 3));

#define LOAD_MSG_4_4(buf)               \
	t0 = _mm_blend_epi16(m0, m2, 0x03); \
	t1 = _mm_slli_si128(t0, 8);         \
	t2 = _mm_blend_epi16(t1, m3, 0x0F); \
	buf = _mm_shuffle_epi32(t2, _MM_SHUFFLE(2, 0, 3, 1));

#define LOAD_MSG_5_1(buf)            \
	t0 = _mm_unpackhi_epi32(m0, m1); \
	t1 = _mm_unpacklo_epi32(m0, m2); \
	buf = _mm_unpacklo_epi64(t0, t1);

#define LOAD_MSG_5_2(buf)               \
	t0 = _mm_srli_si128(m2, 4);         \
	t1 = _mm_blend_epi16(m0, m3, 0x03); \
	buf = _mm_blend_epi16(t1, t0, 0x3C);

#define LOAD_MSG_5_3(buf)               \
	t0 = _mm_blend_epi16(m1, m0, 0x0C); \
	t1 = _mm_srli_si128(m3, 4);         \
	t2 = _mm_blend_epi16(t0, t1, 0x30); \
	buf = _mm_shuffle_epi32(t2, _MM_SHUFFLE(2, 3, 0, 1));

#define LOAD_MSG_5_4(buf)                                \
	t0 = _mm_unpacklo_epi64(m2, m1);                     \
	t1 = _mm_shuffle_epi32(m3, _MM_SHUFFLE(2, 0, 1, 0)); \
	t2 = _mm_srli_si128(t0, 4);                          \
	buf = _mm_blend_epi16(t1, t2, 0x33);

#define LOAD_MSG_6_1(buf)               \
	t0 = _mm_slli_si128(m1, 12);        \
	t1 = _mm_blend_epi16(m0, m3, 0x33); \
	buf = _mm_blend_epi16(t1, t0, 0xC0);

#define LOAD_MSG_6_2(buf)               \
	t0 = _mm_blend_epi16(m3, m2, 0x30); \
	t1 = _mm_srli_si128(m1, 4);         \
	t2 = _mm_blend_epi16(t0, t1, 0x03); \
	buf = _mm_shuffle_epi32(t2, _MM_SHUFFLE(2, 1, 3, 0));

#define LOAD_MSG_6_3(buf)               \
	t0 = _mm_unpacklo_epi64(m0, m2);    \
	t1 = _mm_srli_si128(m1, 4);         \
	t2 = _mm_blend_epi16(t0, t1, 0x0C); \
	buf = _mm_shuffle_epi32(t2, _MM_SHUFFLE(3, 1, 0, 2));

#define LOAD_MSG_6_4(buf)            \
	t0 = _mm_unpackhi_epi32(m1, m2); \
	t1 = _mm_unpackhi_epi64(m0, t0); \
	buf = _mm_shuffle_epi32(t1, _MM_SHUFFLE(0, 1, 2, 3));

#define LOAD_MSG_7_1(buf)               \
	t0 = _mm_unpackhi_epi32(m0, m1);    \
	t1 = _mm_blend_epi16(t0, m3, 0x0F); \
	buf = _mm_shuffle_epi32(t1, _MM_SHUFFLE(2, 0, 3, 1));

#define LOAD_MSG_7_2(buf)               \
	t0 = _mm_blend_epi16(m2, m3, 0x30); \
	t1 = _mm_srli_si128(m0, 4);         \
	t2 = _mm_blend_epi16(t0, t1, 0x03); \
	buf = _mm_shuffle_epi32(t2, _MM_SHUFFLE(1, 0, 2, 3));

#define LOAD_MSG_7_3(buf)               \
	t0 = _mm_unpackhi_epi64(m0, m3);    \
	t1 = _mm_unpacklo_epi64(m1, m2);    \
	t2 = _mm_blend_epi16(t0, t1, 0x3C); \
	buf = _mm_shuffle_epi32(t2, _MM_SHUFFLE(2, 3, 1, 0));

#define LOAD_MSG_7_4(buf)            \
	t0 = _mm_unpacklo_epi32(m0, m1); \
	t1 = _mm_unpackhi_epi32(m1, m2); \
	t2 = _mm_unpacklo_epi64(t0, t1); \
	buf = _mm_shuffle_epi32(t2, _MM_SHUFFLE(2, 1, 0, 3));

#define LOAD_MSG_8_1(buf)               \
	t0 = _mm_unpackhi_epi32(m1, m3);    \
	t1 = _mm_unpacklo_epi64(t0, m0);    \
	t2 = _mm_blend_epi16(t1, m2, 0xC0); \
	buf = _mm_shufflehi_epi16(t2, _MM_SHUFFLE(1, 0, 3, 2));

#define LOAD_MSG_8_2(buf)               \
	t0 = _mm_unpackhi_epi32(m0, m3);    \
	t1 = _mm_blend_epi16(m2, t0, 0xF0); \
	buf = _mm_shuffle_epi32(t1, _MM_SHUFFLE(0, 2, 1, 3));

#define LOAD_MSG_8_3(buf)               \
	t0 = _mm_unpacklo_epi64(m0, m3);    \
	t1 = _mm_srli_si128(m2, 8);         \
	t2 = _mm_blend_epi16(t0, t1, 0x03); \
	buf = _mm_shuffle_epi32(t2, _MM_SHUFFLE(1, 3, 2, 0));

#define LOAD_MSG_8_4(buf)               \
	t0 = _mm_blend_epi16(m1, m0, 0x30); \
	buf = _mm_shuffle_epi32(t0, _MM_SHUFFLE(0, 3, 2, 1));

#define LOAD_MSG_9_1(buf)               \
	t0 = _mm_blend_epi16(m0, m2, 0x03); \
	t1 = _mm_blend_epi16(m1, m2, 0x30); \
	t2 = _mm_blend_epi16(t1, t0, 0x0F); \
	buf = _mm_shuffle_epi32(t2, _MM_SHUFFLE(1, 3, 0, 2));

#define LOAD_MSG_9_2(buf)               \
	t0 = _mm_slli_si128(m0, 4);         \
	t1 = _mm_blend_epi16(m1, t0, 0xC0); \
	buf = _mm_shuffle_epi32(t1, _MM_SHUFFLE(1, 2, 0, 3));

#define LOAD_MSG_9_3(buf)            \
	t0 = _mm_unpackhi_epi32(m0, m3); \
	t1 = _mm_unpacklo_epi32(m2, m3); \
	t2 = _mm_unpackhi_epi64(t0, t1); \
	buf = _mm_shuffle_epi32(t2, _MM_SHUFFLE(0, 2, 1, 3));

#define LOAD_MSG_9_4(buf)               \
	t0 = _mm_blend_epi16(m3, m2, 0xC0); \
	t1 = _mm_unpacklo_epi32(m0, m3);    \
	t2 = _mm_blend_epi16(t0, t1, 0x0F); \
	buf = _mm_shuffle_epi32(t2, _MM_SHUFFLE(1, 2, 3, 0));

#endif

/*** End of inlined file: blake2s-load-sse41.h ***/

#else

/*** Start of inlined file: blake2s-load-sse2.h ***/
#ifndef BLAKE2S_LOAD_SSE2_H
#define BLAKE2S_LOAD_SSE2_H

#define LOAD_MSG_0_1(buf) buf = _mm_set_epi32(m6, m4, m2, m0)
#define LOAD_MSG_0_2(buf) buf = _mm_set_epi32(m7, m5, m3, m1)
#define LOAD_MSG_0_3(buf) buf = _mm_set_epi32(m12, m10, m8, m14)
#define LOAD_MSG_0_4(buf) buf = _mm_set_epi32(m13, m11, m9, m15)
#define LOAD_MSG_1_1(buf) buf = _mm_set_epi32(m13, m9, m4, m14)
#define LOAD_MSG_1_2(buf) buf = _mm_set_epi32(m6, m15, m8, m10)
#define LOAD_MSG_1_3(buf) buf = _mm_set_epi32(m11, m0, m1, m5)
#define LOAD_MSG_1_4(buf) buf = _mm_set_epi32(m7, m2, m12, m3)
#define LOAD_MSG_2_1(buf) buf = _mm_set_epi32(m15, m5, m12, m11)
#define LOAD_MSG_2_2(buf) buf = _mm_set_epi32(m13, m2, m0, m8)
#define LOAD_MSG_2_3(buf) buf = _mm_set_epi32(m7, m3, m10, m9)
#define LOAD_MSG_2_4(buf) buf = _mm_set_epi32(m1, m6, m14, m4)
#define LOAD_MSG_3_1(buf) buf = _mm_set_epi32(m11, m13, m3, m7)
#define LOAD_MSG_3_2(buf) buf = _mm_set_epi32(m14, m12, m1, m9)
#define LOAD_MSG_3_3(buf) buf = _mm_set_epi32(m4, m5, m2, m15)
#define LOAD_MSG_3_4(buf) buf = _mm_set_epi32(m0, m10, m6, m8)
#define LOAD_MSG_4_1(buf) buf = _mm_set_epi32(m10, m2, m5, m9)
#define LOAD_MSG_4_2(buf) buf = _mm_set_epi32(m15, m4, m7, m0)
#define LOAD_MSG_4_3(buf) buf = _mm_set_epi32(m6, m11, m14, m3)
#define LOAD_MSG_4_4(buf) buf = _mm_set_epi32(m8, m12, m1, m13)
#define LOAD_MSG_5_1(buf) buf = _mm_set_epi32(m8, m0, m6, m2)
#define LOAD_MSG_5_2(buf) buf = _mm_set_epi32(m3, m11, m10, m12)
#define LOAD_MSG_5_3(buf) buf = _mm_set_epi32(m15, m7, m4, m1)
#define LOAD_MSG_5_4(buf) buf = _mm_set_epi32(m14, m5, m13, m9)
#define LOAD_MSG_6_1(buf) buf = _mm_set_epi32(m4, m14, m1, m12)
#define LOAD_MSG_6_2(buf) buf = _mm_set_epi32(m10, m13, m15, m5)
#define LOAD_MSG_6_3(buf) buf = _mm_set_epi32(m9, m6, m0, m8)
#define LOAD_MSG_6_4(buf) buf = _mm_set_epi32(m2, m3, m7, m11)
#define LOAD_MSG_7_1(buf) buf = _mm_set_epi32(m3, m12, m7, m13)
#define LOAD_MSG_7_2(buf) buf = _mm_set_epi32(m9, m1, m14, m11)
#define LOAD_MSG_7_3(buf) buf = _mm_set_epi32(m8, m15, m5, m2)
#define LOAD_MSG_7_4(buf) buf = _mm_set_epi32(m6, m4, m0, m10)
#define LOAD_MSG_8_1(buf) buf = _mm_set_epi32(m0, m11, m14, m6)
#define LOAD_MSG_8_2(buf) buf = _mm_set_epi32(m8, m3, m9, m15)
#define LOAD_MSG_8_3(buf) buf = _mm_set_epi32(m1, m13, m12, m10)
#define LOAD_MSG_8_4(buf) buf = _mm_set_epi32(m4, m7, m2, m5)
#define LOAD_MSG_9_1(buf) buf = _mm_set_epi32(m1, m7, m8, m10)
#define LOAD_MSG_9_2(buf) buf = _mm_set_epi32(m5, m6, m4, m2)
#define LOAD_MSG_9_3(buf) buf = _mm_set_epi32(m3, m9, m15, m13)
#define LOAD_MSG_9_4(buf) buf = _mm_set_epi32(m12, m14, m11, m0)

#endif

/*** End of inlined file: blake2s-load-sse2.h ***/

#endif

#define ROUND(r)                         \
	LOAD_MSG_##r##_1(buf1);              \
	G1(row1, row2, row3, row4, buf1);    \
	LOAD_MSG_##r##_2(buf2);              \
	G2(row1, row2, row3, row4, buf2);    \
	DIAGONALIZE(row1, row2, row3, row4); \
	LOAD_MSG_##r##_3(buf3);              \
	G1(row1, row2, row3, row4, buf3);    \
	LOAD_MSG_##r##_4(buf4);              \
	G2(row1, row2, row3, row4, buf4);    \
	UNDIAGONALIZE(row1, row2, row3, row4);

#endif

/*** End of inlined file: blake2s-round.h ***/

static const uint32_t blake2s_IV[8] =
	{
		0x6A09E667UL, 0xBB67AE85UL, 0x3C6EF372UL, 0xA54FF53AUL, 0x510E527FUL, 0x9B05688CUL, 0x1F83D9ABUL, 0x5BE0CD19UL};

/* Some helper functions */
static void blake2s_set_lastnode(blake2s_state *S) {
	S->f[1] = (uint32_t) -1;
}

static int blake2s_is_lastblock(const blake2s_state *S) {
	return S->f[0] != 0;
}

static void blake2s_set_lastblock(blake2s_state *S) {
	if(S->last_node)
		blake2s_set_lastnode(S);

	S->f[0] = (uint32_t) -1;
}

static void blake2s_increment_counter(blake2s_state *S, const uint32_t inc) {
	uint64_t t = ((uint64_t) S->t[1] << 32) | S->t[0];
	t += inc;
	S->t[0] = (uint32_t) (t >> 0);
	S->t[1] = (uint32_t) (t >> 32);
}

/* init2 xors IV with input parameter block */
int blake2s_init_param(blake2s_state *S, const blake2s_param *P) {
	size_t i;
	/*blake2s_init0( S ); */
	const uint8_t *v = (const uint8_t *) (blake2s_IV);
	const uint8_t *p = (const uint8_t *) (P);
	uint8_t *h = (uint8_t *) (S->h);
	/* IV XOR ParamBlock */
	memset(S, 0, sizeof(blake2s_state));

	for(i = 0; i < BLAKE2S_OUTBYTES; ++i)
		h[i] = v[i] ^ p[i];

	S->outlen = P->digest_length;
	return 0;
}

/* Some sort of default parameter block initialization, for sequential blake2s */
int blake2s_init(blake2s_state *S, size_t outlen) {
	blake2s_param P[1];

	/* Move interval verification here? */
	if((!outlen) || (outlen > BLAKE2S_OUTBYTES))
		return -1;

	P->digest_length = (uint8_t) outlen;
	P->key_length = 0;
	P->fanout = 1;
	P->depth = 1;
	store32(&P->leaf_length, 0);
	store32(&P->node_offset, 0);
	store16(&P->xof_length, 0);
	P->node_depth = 0;
	P->inner_length = 0;
	/* memset(P->reserved, 0, sizeof(P->reserved) ); */
	memset(P->salt, 0, sizeof(P->salt));
	memset(P->personal, 0, sizeof(P->personal));

	return blake2s_init_param(S, P);
}

int blake2s_init_key(blake2s_state *S, size_t outlen, const void *key, size_t keylen) {
	blake2s_param P[1];

	/* Move interval verification here? */
	if((!outlen) || (outlen > BLAKE2S_OUTBYTES))
		return -1;

	if((!key) || (!keylen) || keylen > BLAKE2S_KEYBYTES)
		return -1;

	P->digest_length = (uint8_t) outlen;
	P->key_length = (uint8_t) keylen;
	P->fanout = 1;
	P->depth = 1;
	store32(&P->leaf_length, 0);
	store32(&P->node_offset, 0);
	store16(&P->xof_length, 0);
	P->node_depth = 0;
	P->inner_length = 0;
	/* memset(P->reserved, 0, sizeof(P->reserved) ); */
	memset(P->salt, 0, sizeof(P->salt));
	memset(P->personal, 0, sizeof(P->personal));

	if(blake2s_init_param(S, P) < 0)
		return -1;

	{
		uint8_t block[BLAKE2S_BLOCKBYTES];
		memset(block, 0, BLAKE2S_BLOCKBYTES);
		memcpy(block, key, keylen);
		blake2s_update(S, block, BLAKE2S_BLOCKBYTES);
		secure_zero_memory(block, BLAKE2S_BLOCKBYTES); /* Burn the key from stack */
	}
	return 0;
}

static void blake2s_compress(blake2s_state *S, const uint8_t block[BLAKE2S_BLOCKBYTES]) {
	__m128i row1, row2, row3, row4;
	__m128i buf1, buf2, buf3, buf4;
#if defined(HAVE_SSE41)
	__m128i t0, t1;
#if !defined(HAVE_XOP)
	__m128i t2;
#endif
#endif
	__m128i ff0, ff1;
#if defined(HAVE_SSSE3) && !defined(HAVE_XOP)
	const __m128i r8 = _mm_set_epi8(12, 15, 14, 13, 8, 11, 10, 9, 4, 7, 6, 5, 0, 3, 2, 1);
	const __m128i r16 = _mm_set_epi8(13, 12, 15, 14, 9, 8, 11, 10, 5, 4, 7, 6, 1, 0, 3, 2);
#endif
#if defined(HAVE_SSE41)
	const __m128i m0 = LOADU(block + 00);
	const __m128i m1 = LOADU(block + 16);
	const __m128i m2 = LOADU(block + 32);
	const __m128i m3 = LOADU(block + 48);
#else
	const uint32_t m0 = load32(block + 0 * sizeof(uint32_t));
	const uint32_t m1 = load32(block + 1 * sizeof(uint32_t));
	const uint32_t m2 = load32(block + 2 * sizeof(uint32_t));
	const uint32_t m3 = load32(block + 3 * sizeof(uint32_t));
	const uint32_t m4 = load32(block + 4 * sizeof(uint32_t));
	const uint32_t m5 = load32(block + 5 * sizeof(uint32_t));
	const uint32_t m6 = load32(block + 6 * sizeof(uint32_t));
	const uint32_t m7 = load32(block + 7 * sizeof(uint32_t));
	const uint32_t m8 = load32(block + 8 * sizeof(uint32_t));
	const uint32_t m9 = load32(block + 9 * sizeof(uint32_t));
	const uint32_t m10 = load32(block + 10 * sizeof(uint32_t));
	const uint32_t m11 = load32(block + 11 * sizeof(uint32_t));
	const uint32_t m12 = load32(block + 12 * sizeof(uint32_t));
	const uint32_t m13 = load32(block + 13 * sizeof(uint32_t));
	const uint32_t m14 = load32(block + 14 * sizeof(uint32_t));
	const uint32_t m15 = load32(block + 15 * sizeof(uint32_t));
#endif
	row1 = ff0 = LOADU(&S->h[0]);
	row2 = ff1 = LOADU(&S->h[4]);
	row3 = _mm_loadu_si128((__m128i const *) &blake2s_IV[0]);
	row4 = _mm_xor_si128(_mm_loadu_si128((__m128i const *) &blake2s_IV[4]), LOADU(&S->t[0]));
	ROUND(0);
	ROUND(1);
	ROUND(2);
	ROUND(3);
	ROUND(4);
	ROUND(5);
	ROUND(6);
	ROUND(7);
	ROUND(8);
	ROUND(9);
	STOREU(&S->h[0], _mm_xor_si128(ff0, _mm_xor_si128(row1, row3)));
	STOREU(&S->h[4], _mm_xor_si128(ff1, _mm_xor_si128(row2, row4)));
}

int blake2s_update(blake2s_state *S, const void *pin, size_t inlen) {
	const unsigned char *in = (const unsigned char *) pin;
	if(inlen > 0) {
		size_t left = S->buflen;
		size_t fill = BLAKE2S_BLOCKBYTES - left;
		if(inlen > fill) {
			S->buflen = 0;
			memcpy(S->buf + left, in, fill); /* Fill buffer */
			blake2s_increment_counter(S, BLAKE2S_BLOCKBYTES);
			blake2s_compress(S, S->buf); /* Compress */
			in += fill;
			inlen -= fill;
			while(inlen > BLAKE2S_BLOCKBYTES) {
				blake2s_increment_counter(S, BLAKE2S_BLOCKBYTES);
				blake2s_compress(S, in);
				in += BLAKE2S_BLOCKBYTES;
				inlen -= BLAKE2S_BLOCKBYTES;
			}
		}
		memcpy(S->buf + S->buflen, in, inlen);
		S->buflen += inlen;
	}
	return 0;
}

int blake2s_final(blake2s_state *S, void *out, size_t outlen) {
	uint8_t buffer[BLAKE2S_OUTBYTES] = {0};
	size_t i;

	if(out == NULL || outlen < S->outlen)
		return -1;

	if(blake2s_is_lastblock(S))
		return -1;

	blake2s_increment_counter(S, (uint32_t) S->buflen);
	blake2s_set_lastblock(S);
	memset(S->buf + S->buflen, 0, BLAKE2S_BLOCKBYTES - S->buflen); /* Padding */
	blake2s_compress(S, S->buf);

	for(i = 0; i < 8; ++i) /* Output full hash to temp buffer */
		store32(buffer + sizeof(S->h[i]) * i, S->h[i]);

	memcpy(out, buffer, S->outlen);
	secure_zero_memory(buffer, sizeof(buffer));
	return 0;
}

/* inlen, at least, should be uint64_t. Others can be size_t. */
int blake2s(void *out, size_t outlen, const void *in, size_t inlen, const void *key, size_t keylen) {
	blake2s_state S[1];

	/* Verify parameters */
	if(NULL == in && inlen > 0)
		return -1;

	if(NULL == out)
		return -1;

	if(NULL == key && keylen > 0)
		return -1;

	if(!outlen || outlen > BLAKE2S_OUTBYTES)
		return -1;

	if(keylen > BLAKE2S_KEYBYTES)
		return -1;

	if(keylen > 0) {
		if(blake2s_init_key(S, outlen, key, keylen) < 0)
			return -1;
	} else {
		if(blake2s_init(S, outlen) < 0)
			return -1;
	}

	blake2s_update(S, (const uint8_t *) in, inlen);
	blake2s_final(S, out, outlen);
	return 0;
}
