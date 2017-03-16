#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/**
 * Computes the full sequence of a Galois LFSR
 * with the given number of bits and given taps.
 */
void galois(uint32_t* out, int nbits, uint32_t taps, uint32_t init, int length) {

  int i;
  
  uint32_t outmask = 0;;
  for(i=0; i < nbits; i++) {
    outmask |= 1 << i;
  }

  uint32_t lfsr = init;

  for(i=0; i < length; i++) {

    out[i] = lfsr & outmask;

    uint32_t lsb = lfsr & 1;
    lfsr >>= 1;

    if(lsb) {
      lfsr ^= taps;
    }
  }
}

/**
 * Prints the maximal length sequence of an LFSR with a user-specfied number of
 * bits and user specified polynomial taps. Can also generate gold codes from
 * such an LFSR.
 * For terms to use, see: https://users.ece.cmu.edu/~koopman/lfsr/
 */
int main(int argc, char** argv) {

  enum {
    NORM,
    GOLD
  } mode = NORM;

  if(argc < 4) {
    printf("USAGE: %s [#bits] [taps] [init]\n", argv[0]);
    printf("EXAMPLE: %s 16 0xB400 0x0001\n", argv[0]);
    printf("EXAMPLE: %s 4 0x9 0x01\n", argv[0]);
    printf("EXAMPLE: %s 5 0x14 0x01\n", argv[0]);
    printf("EXAMPLE: %s 6 0x30 0x01\n\n", argv[0]);
    printf("USAGE (gold codes): %s -g [codeindex] [#bits] [taps1] [taps2] [init]\n", argv[0]);  
    printf("EXAMPLE: %s -g 1 5 0x14 0x17 0x01\n", argv[0]);

    return 0;
  }
  
  int curarg = 1;
  int goldOffset = 0;
  
  if ('-' == argv[curarg][0] && strstr(argv[curarg], "g")) {
    mode = GOLD;
    curarg++;
    goldOffset = atoi(argv[curarg++]);
  }

  int nbits = atoi(argv[curarg++]);
  uint32_t taps = (int)strtol(argv[curarg++], NULL, 16);
  uint32_t taps2;

  if(mode==GOLD) {
    taps2  = (int)strtol(argv[curarg++], NULL, 16);
  }

  uint32_t init = (int)strtol(argv[curarg++], NULL, 16);

  uint32_t length = pow(2, nbits) - 1;

  uint32_t buf[length];
  uint32_t buf2[length];

  galois(buf, nbits, taps, init, length);  

  int i;

  switch(mode) {
  case NORM:
    for(i=0; i < length; i++) {
      printf("%d,\n", buf[i]);
    }

    break;
  case GOLD:

    galois(buf2, nbits, taps2, init, length);

    for(i=0; i<length; i++){
      // Discard all but 0th bit
      buf[i] = buf[i]&1; 
      buf2[i] = buf2[i]&1;
    }
    
    for(i=0; i<length; i++) {

      if(i+goldOffset>=length) {
	goldOffset -= length;
      }

      printf("%d,\n", buf[i] ^ buf2[i+goldOffset]);
    }


    break;
  }

  return 0;
    
}
