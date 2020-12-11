#ifndef RS_EXTERNS_H

#define RS_EXTERNS_H

#include "rs_defs.h"
#include "rs6355_defs.h"

// Reed-Solomon externs

extern unsigned int RS_TCM_Bits_Rem_Tx;
extern unsigned int RS_TCM_Nbit_Tx;
extern unsigned int RS_Codes_Tx;
extern unsigned int RS_Tx[RS_CODES_MAX][2];
extern unsigned int RS_PCM_Nbit_Tx;
extern unsigned int RSn_Min_Tx;
extern unsigned int RSk_Min_Tx;
extern unsigned int RS_dummy_bit_Tx;
extern unsigned int RS_redund_bits_Tx;
 
extern unsigned int RS_TCM_Bits_Rem_Rx;
extern unsigned int RS_TCM_Nbit_Rx;
extern unsigned int RS_Codes_Rx;
extern unsigned int RS_Rx[RS_CODES_MAX][2];        
extern unsigned int RS_PCM_Nbit_Rx;
extern unsigned int RSn_Min_Rx;
extern unsigned int RSk_Min_Rx;
extern unsigned int RS_dummy_bit_Rx;
extern unsigned int RS_redund_bits_Rx;


extern int RS6355_Lctr[RS6355_R+1];

#endif

