#ifndef RS_GLOBALS_H

#define RS_GLOBALS_H

#include "rs6355_defs.h"
#include "rs_defs.h"

// Reed-Solomon globals


unsigned int segment ( "seg_extn SHT_NOBITS" ) RS_TCM_Bits_Rem_Tx;
unsigned int segment ( "seg_extn SHT_NOBITS" ) RS_TCM_Nbit_Tx;
unsigned int segment ( "seg_extn SHT_NOBITS" ) RS_Codes_Tx;
unsigned int segment ( "seg_extn SHT_NOBITS" ) RS_Tx[RS_CODES_MAX][2];
unsigned int segment ( "seg_extn SHT_NOBITS" ) RS_PCM_Nbit_Tx;
unsigned int segment ( "seg_extn SHT_NOBITS" ) RSn_Min_Tx;
unsigned int segment ( "seg_extn SHT_NOBITS" ) RSk_Min_Tx;
unsigned int segment ( "seg_extn SHT_NOBITS" ) RS_dummy_bit_Tx;
unsigned int segment ( "seg_extn SHT_NOBITS" ) RS_redund_bits_Tx;
 
unsigned int segment ( "seg_extn SHT_NOBITS" ) RS_TCM_Bits_Rem_Rx;
unsigned int segment ( "seg_extn SHT_NOBITS" ) RS_TCM_Nbit_Rx;
unsigned int segment ( "seg_extn SHT_NOBITS" ) RS_Codes_Rx;
unsigned int segment ( "seg_extn SHT_NOBITS" ) RS_Rx[RS_CODES_MAX][2];        
unsigned int segment ( "seg_extn SHT_NOBITS" ) RS_PCM_Nbit_Rx;
unsigned int segment ( "seg_extn SHT_NOBITS" ) RSn_Min_Rx;
unsigned int segment ( "seg_extn SHT_NOBITS" ) RSk_Min_Rx;
unsigned int segment ( "seg_extn SHT_NOBITS" ) RS_dummy_bit_Rx;
unsigned int segment ( "seg_extn SHT_NOBITS" ) RS_redund_bits_Rx;

int RS6355_Lctr[RS6355_R+1] = {-1, 0, 0, 0, 0, 0, 0, 0, 0};
   
 
#endif

