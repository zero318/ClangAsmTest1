/**
  * Touhou Community Reliant Automatic Patcher
  * Main DLL
  *
  * ----
  *
  * SHA-256 hash implementation
  * From https://web.archive.org/web/20120415202539/http://bradconte.com:80/sha256_c.html
  */

#include <stdlib.h>
#include <stdint.h>
#include "zero/util.h"
#include "zero/custom_intrin.h"

typedef union {
	uint32_t dwords[8];
	uint64_t qwords[4];
	uint8_t bytes[32];
	struct {
		__m128i lower_simd;
		__m128i upper_simd;
	};
} SHA256_HASH;

dllexport SHA256_HASH sha256_calc(const uint8_t data[], size_t length);

typedef struct {
    uint8_t data[64];
    uint32_t datalen;
    union {
        uint32_t dwords[2];
        uint64_t qword;
    } bitlen;
    SHA256_HASH state;
} SHA256_CTX;

static constexpr SHA256_HASH initial_state = {
    0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A,
    0x510E527f, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19
};

#if defined(__INTRIN_H_)
# define bswap_32 _byteswap_ulong
#else
uint32_t bswap_32(uint32_t x);
# if defined(__WATCOMC__)
#  pragma aux bswap_32 = " bswap eax " parm [ax] modify [ax];
# else
uint32_t bswap_32(uint32_t x)
{
	return
		((x & 0xff000000) >> 24) | ((x & 0x00ff0000) >> 8) |
		((x & 0x0000ff00) << 8) | ((x & 0x000000ff) << 24)
	;
}
# endif
#endif

#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

uint32_t k[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

dllexport void sha256_transform(SHA256_CTX *ctx)
{
	uint32_t a,b,c,d,e,f,g,h;
    uint32_t t1,t2,m[64];
	uint32_t *data_uint = (uint32_t*)ctx->data;

	for(size_t i = 0; i < 16; ++i) {
		m[i] = bswap_32(data_uint[i]);
	}
//#pragma clang loop distribute(enable)
	for(size_t i = 16; i < 64; ++i) {
		m[i] = SIG1(m[i-2]) + m[i-7] + SIG0(m[i-15]) + m[i-16];
	}

	a = ctx->state.dwords[0];
	b = ctx->state.dwords[1];
	c = ctx->state.dwords[2];
	d = ctx->state.dwords[3];
	e = ctx->state.dwords[4];
	f = ctx->state.dwords[5];
	g = ctx->state.dwords[6];
	h = ctx->state.dwords[7];

//#pragma clang loop distribute(enable)
	for(size_t i = 0; i < 64; ++i) {
		t1 = h + EP1(e) + CH(e,f,g) + k[i] + m[i];
		t2 = EP0(a) + MAJ(a,b,c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	ctx->state.dwords[0] += a;
	ctx->state.dwords[1] += b;
	ctx->state.dwords[2] += c;
	ctx->state.dwords[3] += d;
	ctx->state.dwords[4] += e;
	ctx->state.dwords[5] += f;
	ctx->state.dwords[6] += g;
	ctx->state.dwords[7] += h;
}

dllexport inline void sha256_init(SHA256_CTX *ctx)
{
	ctx->datalen = 0;
	ctx->bitlen.qword = 0;
    ctx->state = initial_state;
}

dllexport inline void sha256_update(SHA256_CTX *ctx, const uint8_t data[], uint32_t len)
{
    size_t blocks = len / 64;
    ctx->datalen = len % 64;
    ctx->bitlen.qword = blocks * 512;
    for (size_t i = 0; i < blocks; ++i) {
        memcpy(ctx->data, data, 64);
        data += 64;
        sha256_transform(ctx);
    }
    if (ctx->datalen) {
        memcpy(ctx->data, data, ctx->datalen);
    }
}

dllexport inline void sha256_final(SHA256_CTX *ctx)
{
	// Pad whatever data is left in the buffer.
    ctx->data[ctx->datalen] = 0x80;
    bool short_padding = ctx->datalen < 56;
    size_t i = ctx->datalen;
    while (++i < 64) {
        ctx->data[i] = 0x00;
    }
    if (!short_padding) {
        sha256_transform(ctx);
        memset(ctx->data, 0, 64);
    }

	// Append to the padding the total message's length in bits and transform.
    ctx->bitlen.qword += ctx->datalen * 8;
    uint32_t* data_uint = (uint32_t*)ctx->data;
	data_uint[15] = bswap_32(ctx->bitlen.dwords[0]);
	data_uint[14] = bswap_32(ctx->bitlen.dwords[1]);
	sha256_transform(ctx);

	// Since this implementation uses little endian byte ordering and SHA uses big endian,
	// reverse all the bytes when copying the final state to the output hash.
	for(size_t i = 0; i < 8; ++i) {
        ctx->state.dwords[i] = bswap_32(ctx->state.dwords[i]);
	}
}

dllexport bool CPU_Supports_SHA = false;

dllexport SHA256_HASH sha256_calc(const uint8_t data[], size_t length) {
    SHA256_CTX sha256_ctx;
    sha256_ctx.state = initial_state;
    size_t blocks = length / 64;
    sha256_ctx.datalen = length % 64;
    sha256_ctx.bitlen.qword = blocks * 512;
    for (size_t i = 0; i < blocks; ++i) {
        memcpy(sha256_ctx.data, data, 64);
        data += 64;
        sha256_transform(&sha256_ctx);
    }
    if (sha256_ctx.datalen) {
        memcpy(sha256_ctx.data, data, sha256_ctx.datalen);
    }
    sha256_final(&sha256_ctx);
    return sha256_ctx.state;
}