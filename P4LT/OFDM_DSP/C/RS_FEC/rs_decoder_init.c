/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: rs_decoder_init.c $
 * COPYRIGHT   : 2004 by TL Consulting GmbH
 * DESCRIPTION : Reed-Solomon decoder
 * OPERATION   : 
 * PROTOTYPE   : int RS_Decoder_Init(int TCM_Bits_Rx);    
 *                                                          
 * ASSUMPTIONS :
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/RS_FEC/rs_decoder_init.c $
 * 
 * 1     19.01.05 15:41 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * 
 * 1     09.04.04  vgp
 * 
 *****************************************************************************/
#include "../../h/util/util.h"
#include "../../h/rs_fec/RS6355_defs.h"
#include "../../h/rs_fec/RS_externs.h"


int  segment ( "seg_ext_pmco" ) RS_Decoder_Init(int TCM_Bits_Rx)
//int RS_Decoder_Init(int TCM_Bits_Rx)
{
	int i, n, nl, k, RS_Symb_rem, RS_Symb;
    
	RS_Symb = TCM_Bits_Rx/LOG_GF64;
	RS_TCM_Bits_Rem_Rx = TCM_Bits_Rx - LOG_GF64*RS_Symb;
	RS_Codes_Rx = RS_Symb/RS6355_N;
	RS_Symb_rem = RS_Symb - RS6355_N*RS_Codes_Rx;
	if ( RS_Symb_rem != 0 ) RS_Codes_Rx++;
    if ( RS_Codes_Rx <=0 ) return 1;
	if ( RS_Codes_Rx > RS_CODES_MAX ) return 1;

	// Get initial code parameters
    if ( RS_Codes_Rx > 1 ) {
		for (i=0; i<RS_Codes_Rx-1; i++) RS_Rx[i][0] = RS6355_N;
	} 
	if ( RS_Symb_rem != 0 ) RS_Rx[RS_Codes_Rx-1][0] = RS_Symb_rem;
	else RS_Rx[RS_Codes_Rx-1][0] = RS6355_N;		

	// Optimize RS code parameters
	if ( RS_Codes_Rx > 1) {
		n  = RS_Rx[0][0];
		nl = RS_Rx[RS_Codes_Rx-1][0];
		while ( nl+RS_Codes_Rx <= n ) {
			nl += RS_Codes_Rx-1;
			n--;
		}
		k = n - RS6355_R;
		for (i=0;  i<RS_Codes_Rx-1; i++) {
			RS_Rx[i][0] = n;
			RS_Rx[i][1] = k;
		}
		RS_Rx[RS_Codes_Rx-1][0] = nl;
		RS_Rx[RS_Codes_Rx-1][1] = nl - RS6355_R;
	} else {
		if ( RS_Rx[0][0] - RS6355_R <= 0 ) return 1;
		RS_Rx[0][1] = RS_Rx[0][0]  - RS6355_R;
	}
    
	// PCM parameters
	RS_PCM_Nbit_Rx = 0;
	for (i=0;  i<RS_Codes_Rx; i++)  RS_PCM_Nbit_Rx +=  RS_Rx[i][1];
    RS_TCM_Nbit_Rx  = (RS_PCM_Nbit_Rx + RS6355_R*RS_Codes_Rx)*LOG_GF64; // number of RS code bits in TCM inf. bits
	RS_PCM_Nbit_Rx *= LOG_GF64; // number of information V11 bits in TCM inf. bits
	                            // RS_TCM_Bits_Rem_Rx - number of dummy bits in TCM inf. bits

	RSn_Min_Rx = RS_Rx[RS_Codes_Rx-1][0];
	RSk_Min_Rx = RS_Rx[RS_Codes_Rx-1][1];

	return 0;
}

