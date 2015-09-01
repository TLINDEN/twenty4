#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef uint8_t  byte;
typedef uint32_t word;
typedef uint16_t half;

#define MAX 4096

void dump256(byte *hash) {
  int i, b = 1;
  
  for(i=0; i<256; i++) {
    fprintf(stderr, "0x%02x, ", hash[i]);
    if(b == 16) {
      fprintf(stderr, "\n");
      b = 1;
    }
    else
      b++;
  }

  fprintf(stderr, "\n");
}

int main() {
  byte raw[MAX];
  byte hash[256] = {0}, reg[256] = {0}, out;
  int i, b=0, has=0;
  
  fread(raw, MAX, 1, stdin);

  memset(hash, 0, 256);
  
  for (i=0; i<MAX; i++) {
    out = raw[i];
    if(reg[out] == 0) {
      reg[out]++;
      hash[b++] = out;
      has++;
    }
    if(has == 256) {
      dump256(hash);
      has = b = 0;
      memset(hash, 0, 256);
      memset(reg, 0, 256);
    }
  }

  //fwrite(hash, 256, 1, stdout);
  fprintf(stderr, "done\n");
  return 0;
}
