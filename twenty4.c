/*
 ******* THIS IS JUST FOR LEARINING CRYPTO, DO NOT EVER USE THIS FOR ANYTHING *******

 This is the implementation of the fun stream cipher TWENTY4/160 by Thomas von Dein, 09/2015.
 Published under the public domain, Creative Commons Zero License.

 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>

typedef uint8_t  byte;
typedef uint32_t u32;
typedef uint64_t u64;

/* global context, stores the 160 bit key */
struct _ctx {
  u32 lcg;
  u32 d1u;
  u32 decide;
  u64 shift;
};
typedef struct _ctx ctx;
ctx *context;

/* sbox used for i/o stream diffusion */
const uint8_t sbox[] = {
  0x61, 0x2d, 0x19, 0xf3, 0xe5, 0xd9, 0xde, 0x5f, 0x41, 0x31, 0xa7, 0xc2, 0x48, 0x02, 0xef, 0x98, 
  0x67, 0xcb, 0x6e, 0x4c, 0xf4, 0x11, 0xfa, 0x87, 0x0f, 0x6f, 0x0a, 0x3b, 0x71, 0x09, 0x1a, 0xb8, 
  0x3c, 0x44, 0xd8, 0xd4, 0xc8, 0x91, 0x6d, 0x8c, 0x2f, 0xce, 0x85, 0x22, 0xd5, 0x08, 0xa6, 0x97, 
  0x68, 0xbc, 0x3a, 0xa0, 0xbf, 0xa5, 0x47, 0x94, 0x83, 0xd1, 0x18, 0x29, 0x03, 0xb2, 0xa4, 0xfe, 
  0xe4, 0x4d, 0xdf, 0x21, 0xc0, 0x70, 0x4f, 0x90, 0x04, 0x40, 0x0b, 0x49, 0xe0, 0x25, 0xd7, 0xda, 
  0xf8, 0x1f, 0x9e, 0x76, 0xbb, 0xaa, 0xc5, 0x2e, 0x72, 0x64, 0xd6, 0x74, 0x10, 0x78, 0xfd, 0x45, 
  0x80, 0x4e, 0x7f, 0x12, 0xb7, 0xc6, 0xea, 0xb3, 0x37, 0x5a, 0xf2, 0xc3, 0xb6, 0x5b, 0x81, 0x95, 
  0xbd, 0xb0, 0xae, 0x8f, 0xd2, 0xcf, 0x1e, 0xc7, 0xee, 0xa1, 0x7a, 0xb9, 0x06, 0xa8, 0xb1, 0x93, 
  0x30, 0xad, 0x33, 0x77, 0x3d, 0x7c, 0xb4, 0x36, 0x92, 0x15, 0x89, 0x7e, 0xe9, 0x17, 0x07, 0x8a, 
  0x9f, 0x32, 0x2c, 0xf9, 0xb5, 0x7d, 0xeb, 0x23, 0xdc, 0x2b, 0x63, 0x88, 0x56, 0x42, 0x84, 0x4b, 
  0x0e, 0xec, 0x8d, 0x7b, 0x05, 0xed, 0xca, 0xe8, 0xe6, 0xba, 0x01, 0x5d, 0x26, 0x28, 0x13, 0x9d, 
  0x54, 0x59, 0xfb, 0xf0, 0xd3, 0xf7, 0xdb, 0xe7, 0xbe, 0x58, 0x5e, 0x99, 0x65, 0x8b, 0x20, 0xa3, 
  0xc1, 0x1c, 0xaf, 0xac, 0x55, 0xe3, 0xdd, 0x62, 0x2a, 0xcc, 0xd0, 0xe2, 0x0c, 0x66, 0x96, 0x8e, 
  0xab, 0xfc, 0xc4, 0x1d, 0x6a, 0x6c, 0x3f, 0x9b, 0x9a, 0x51, 0xa2, 0x86, 0x52, 0x4a, 0x43, 0x14, 
  0x75, 0xff, 0xf5, 0xcd, 0x1b, 0x0d, 0x35, 0x24, 0x9c, 0xe1, 0x60, 0x73, 0x3e, 0x39, 0x53, 0x16, 
  0x50, 0x6b, 0xc9, 0x46, 0x57, 0x5c, 0x69, 0x79, 0x82, 0xf1, 0x27, 0x38, 0x34, 0xf6, 0x00, 0xa9,
};


/* convert a 64bit number into an 8 element byte array */
void w2a(u64 in, uint8_t *out) {
  out[0] = (in >> 56) & 0xFF;
  out[1] = (in >> 48) & 0xFF;
  out[2] = (in >> 40) & 0xFF;
  out[3] = (in >> 32) & 0xFF;
  out[4] = (in >> 24) & 0xFF;
  out[5] = (in >> 16) & 0xFF;
  out[6] = (in >>  8) & 0xFF;
  out[7] =  in        & 0xFF;
}

/* rotate 64bit number by 'rot' left */
u64 rot64left(u64 in, int rot) {
  if(rot == 0) rot = 1;
  return (in >> (64-rot)) | (in << rot);
}

/* rotate 32bit number by 'rot' left */
u32 rot32left(u32 in, int rot) {
  return (in >> (32-rot)) | (in << rot);
}

/* park-miller 32bit prng */
u32 _32_lcg_pm(u32 seed) {
  return ((u64)seed * 48271UL) % 2147483647UL;
}

/* galois 32bit linear feedback shift register, taps: 32 31 29 1 */
u32 _32_gal_d1u(u32 seed) {
  return (seed >> 1) ^ (unsigned int)(0 - ((seed & 1u) & 0xd0000001u));
}

/* de-buijn 32bit non-linear feedback shift register */
u32 _32_nlfsr_debuijn(u32 seed) {
  int k = 28, n = 31;
  return ((((seed>>k)^seed^!(seed>>1))&1)<<(n-1))|(seed>>1);
}

/* 64bit non-linear xorshift register */
u64 _64_xs_st() {
  context->shift ^= context->shift >> 12; // a
  context->shift ^= context->shift << 25; // b
  context->shift ^= context->shift >> 27; // c
  return context->shift * UINT64_C(2685821657736338717);
}

/* run registers/prng's */
u64 fwd_prngs() {
  context->lcg    = _32_lcg_pm(context->lcg);
  context->d1u    = _32_gal_d1u(context->d1u);
  context->decide = _32_nlfsr_debuijn(context->decide);
  return _64_xs_st();
}

/* combine the different prng's into a 64bit round key */
u64 combined64a() {
  u32 _x;
  u64 use, xorshift;
  int xSwap, xRot, xRotBy;
 
  xSwap       = 11; /* Sofie Germain primes as well */
  xRot        = 29;
  xRotBy      = 53;

  xorshift = fwd_prngs();
  
  if(context->decide % 2 == 0) {
    /* xor 64bit register with multiplied 32bit registers */
    use = xorshift ^ ((u64)context->lcg * (u64)context->d1u);
  }
  else {
    /* xor both 32bit registers (shifted into a 64bit) with 64bit register */
    use = xorshift ^ (((u64)context->lcg << 32) + context->d1u);
  }

  if((context->decide & 0xFF) % xSwap == 0) {
    /* re-seed 32bit registers by swapping them */
    _x           = context->lcg;
    context->lcg = context->d1u;
    context->d1u = _x;
  }

  if((context->decide & 0xFF) % xRot == 0) {
    // rotate 64t left
    context->shift = rot64left(context->shift, (context->decide & 0xFF) % xRotBy);
  }

  return use;
}

void dumpk(ctx *k) {
  fprintf(stderr, "    lcg: %04X\n", k->lcg);
  fprintf(stderr, "    d1u: %04X\n", k->d1u);
  fprintf(stderr, " decide: %04X\n", k->decide);
  fprintf(stderr, "  shift: %" PRIX64 "\n", k->shift);
}

/* convert 20 byte hex string into 160 bit key (= context) */
ctx *parseargs(char *arg) {
  char tmp[9];
  size_t len;
  ctx *k;
  
  len = strlen(arg);
	       
  if(len < 160/8) {
    fprintf(stderr, "key too small (got %ld, expected %d)\n", len, 160/8);
    return NULL;
  }
  else {
    k = malloc(sizeof(ctx));
    memset(tmp, 0, 9);
    
    memcpy(tmp, arg, 4);
    k->lcg = strtol(tmp, NULL, 16);
    
    memcpy(tmp, &arg[4], 4);
    k->d1u = strtol(tmp, NULL, 16);
    
    memcpy(tmp, &arg[8], 4);
    k->decide = strtol(tmp, NULL, 16);
    
    memcpy(tmp, &arg[12], 8);
    k->shift = strtoll(tmp, NULL, 16);
    
    return k;
  }
}

/* diffuse context with prime numbers */
void diffuse_context() {
  /* 32bits are random Sofie Germain primes,
     64bit is a Carmichael number(fermat pseudoprime), see
     https://oeis.org/A255578
  
     diffuse input key with those primes
  */
  u32 tmplcg;
  int i, xRotBy = 29;
  
  context->lcg    ^= 0x85f62713;
  context->d1u    ^= 0xc178f733;
  context->decide ^= 0x49a79a73;
  context->shift  ^= 17905475062325518273U;

  for(i=0; i<7; i++) {
    tmplcg           = context->lcg;
    context->lcg    ^= rot32left(context->d1u,    (context->decide & 0xFF) % xRotBy);
    context->d1u    ^= rot32left(context->decide, (context->d1u    & 0xFF) % xRotBy);
    context->decide ^= rot32left(tmplcg,          (context->lcg    & 0xFF) % xRotBy);
    context->shift  ^= (((u64)context->lcg << 32) + context->d1u);
  }
}

/* actual stream (1byte) encrypt/decrypt */
void io_loop() {
  byte out, K[8];
  int i;
  
  w2a(combined64a(), K);
  
  while(fread(&out, 1, 1, stdin) == 1) {
    for(i=0; i<8; i++) out ^= sbox[K[i]]; /* apply our sbox */
    fwrite(&out, 1, 1, stdout);
    w2a(combined64a(), K);
  }

  fflush(stdout);
}


int main(int argc, char **argv)  {
  if(argc == 2) {
    context = parseargs(argv[1]);
    if(context == NULL) {
      return 1;
    }
    else {
      diffuse_context();
      io_loop();
      return 0;
    }
  }
  else {
    fprintf(stderr, "usage: twenty4 <20 byte hex key>\n");
    return 1;
  }
}
