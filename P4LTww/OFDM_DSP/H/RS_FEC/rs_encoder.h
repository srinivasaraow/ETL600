#ifndef RS_ENCODER_H

#define RS_ENCODER_H

// Reed-Solomon encoder based on sortening of RS(63,55,9) code

int RS_Encoder_Init(int TCM_Bits_Tx);
int RS_Encoder(int *TCM_Out, int *PCM_In, int *tmp);

#endif

