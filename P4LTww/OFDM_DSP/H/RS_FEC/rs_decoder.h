#ifndef RS_DECODER_H

#define RS_DECODER_H

// Reed-Solomon decoder based on sortening of RS(63,55,9) code

int RS_Decoder_Init(int TCM_Bits_Rx);
int  RS_Decoder(int *PCM_Out, int *TCM_In, int *tmp);

#endif

