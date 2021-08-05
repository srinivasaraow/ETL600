#ifndef RS6355_H

#define RS6355_H

#include "rs6355_defs.h"

// Reed-Solomon (63,55,9) encoder/decoder

void RS6355_Encoder(unsigned int *CW, int *TmpEnc);
      // CW      points int. array of length 63 32 bit words
      // TmpEnc  points integer array of lenght 63 bit words

int RS6355_Decoder(unsigned int *RCW, int *Lctr,unsigned int *DCW, int *TmpDec);
      // RCW     points integer array to decode of lenght 63 32bit words
      // Lctr    points integer array of locators of erasures terminated by -1 (0xffffffff)
      //         maximum length is equal to 9, each element memberships set {0,1..63}
      // DCW     points integer array of decoded code word
      // TmpDec  points integer array of length 728 32bit words
      // return 0 is decoding is succeed, 1 - otherwise

#endif

