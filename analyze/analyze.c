#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <zlib.h>

typedef uint8_t  byte;
typedef uint32_t word;
typedef uint16_t half;

struct _bytes {
  size_t len;
  byte *bin;
};
typedef struct _bytes Bytes;

#define CHUNK 16384



Bytes *fetch(char *infile) {
  size_t insize, p=0;
  FILE *in;
  Bytes *out;
  
  if((in = fopen(infile, "rb")) == NULL) {
    perror("Could not open infile");
    exit(1);
  }

  fseek(in, 0L, SEEK_END);
  insize = ftell(in);
  fseek(in, 0L, SEEK_SET);

  out = malloc(sizeof(Bytes));
  
  out->bin = malloc(insize * sizeof(byte));
  out->len = insize;
  memset(out->bin, 0, insize);

  while (!ferror(in) && !feof(in)) {
    fread(&out->bin[p++], 1, 1, in);
  }
  fclose(in);

  return out;
}


size_t defl(FILE *source, FILE *dest, int level)
{
    int ret, flush;
    unsigned have;
    z_stream strm;
    size_t zsize;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];
    zsize = 0;
    
    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK)
        return ret;

    /* compress until end of file */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)deflateEnd(&strm);
            return Z_ERRNO;
        }
        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

        /* run deflate() on input until output buffer not full, finish
           compression if all of source has been read in */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)deflateEnd(&strm);
                return Z_ERRNO;
            }
	    zsize += have;
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);     /* all input will be used */

        /* done when last data in file processed */
    } while (flush != Z_FINISH);
    assert(ret == Z_STREAM_END);        /* stream will be complete */

    /* clean up and return */
    (void)deflateEnd(&strm);
    return zsize;
}


size_t get_zsize(char *infile) {
  FILE *in;
  FILE *z;
  size_t zsize;
  
  if((in = fopen(infile, "rb")) == NULL) {
    perror("Could not open infile");
    exit(1);
  }

  if((z = fopen("/tmp/analyze.z", "wb")) == NULL) {
    perror("Could not open zfile");
    exit(1);
  }

  zsize = defl(in, z, 9);

  fclose(in);
  fclose(z);
  unlink("/tmp/analyze.z");

  return zsize;
}


double get_entropy(byte *source, size_t len) {
  int *hist;
  double H;
  int wherechar[256];
  int i,histlen;
  
  histlen = 0;
  H = 0;
  hist = (int*)calloc(len, sizeof(int));

  for(i=0; i<256; i++)
    wherechar[i] = -1;
  
  for(i=0; i<(int)len; i++){
    if(wherechar[(int)source[i]] == -1) {
      wherechar[(int)source[i]] = histlen;
      histlen++;
    }
    hist[wherechar[(int)source[i]]]++;
  }

  for(i=0; i<histlen; i++) {
    H -= (double)hist[i] / len * log2((double)hist[i] / len);
  }
  
  return H;
}


double get_distribution(Bytes *in) {
  int i;
  byte hash[256] = {0};
  double chars = 0;
  
  for (i=0; i<(int)in->len; i++) {
    hash[in->bin[i]]++;
  }

  for (i=0; i<256; i++) {
    if(hash[i]) {
      chars++;
    }
  }

  return chars;
}



void analyze(char *infile) {
  double chars = 0, dist, red, entropy, zrate;
  size_t zsize;
  Bytes *in = fetch(infile);
  
  entropy = get_entropy(in->bin, in->len);
  chars   = get_distribution(in);
  zsize   = get_zsize(infile);
  
  dist  = chars / 2.56;
  red   = 100 - dist;
  zrate = zsize > in->len ? 0 : 100 - (zsize / (in->len / 100));
  
  fprintf(stdout,
          "                          File size: %ld bytes\n"
          "     Char distribution (best: 100%%): %lf%%\n"
          "       Char redundancy (best: 0.0%%): %lf%%\n"
          "    Char entropy (best: 8.0 bits/c): %lf bits/char\n"
          "      Compression rate (best: 0.0%%): %lf%% (%ld => %ld bytes)\n",
          in->len, dist, red, entropy, zrate, in->len, zsize
          );
}

void print_chars_hex(Bytes *in) {
  int i;
  for (i=0; i<(int)in->len; i++) {
    fprintf(stdout, "%02x\n", in->bin[i]);
  }
}

int main(int argc, char **argv)  {
  if(argc != 2) {
    fprintf(stderr, "Usage: analyze <file>\n");
    return 1;
  }
  else {
    analyze(argv[1]);
  }
  return 0;
}
