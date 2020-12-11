/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: rs_encoder_init.c $
 * COPYRIGHT   : 2004 by TL Consulting GmbH
 * DESCRIPTION : Reed-Solomon encoder
 * OPERATION   : 
 * PROTOTYPE   : int RS_Encoder_Init(int TCM_Bits_Tx);    
 *                                                          
 * ASSUMPTIONS :
 *
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/RS_FEC/rs_encoder_init.c $
 * 
 * 1     19.01.05 15:41 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * 
 * 1     09.04.04 vgp
 * 
 *****************************************************************************/
#include "../../h/util/util.h"
#include "../../h/rs_fec/RS6355_defs.h"
#include "../../h/rs_fec/RS_externs.h"

int segment ( "seg_ext_pmco" ) RS_Encoder_Init(int TCM_Bits_Tx)
//int RS_Encoder_Init(int TCM_Bits_Tx)
{
	int i, n, nl, k, RS_Symb_rem, RS_Symb;
    
	RS_Symb = TCM_Bits_Tx/LOG_GF64;
	RS_TCM_Bits_Rem_Tx = TCM_Bits_Tx - LOG_GF64*RS_Symb;
	RS_Codes_Tx = RS_Symb/RS6355_N;
	RS_Symb_rem = RS_Symb - RS6355_N*RS_Codes_Tx;
	if ( RS_Symb_rem != 0 ) RS_Codes_Tx++;
    if ( RS_Codes_Tx <=0 ) return 1;
	if ( RS_Codes_Tx > RS_CODES_MAX ) return 1;

	// Get initial code parameters
    if ( RS_Codes_Tx > 1 ) {
		for (i=0; i<RS_Codes_Tx-1; i++) RS_Tx[i][0] = RS6355_N;
	} 
	if ( RS_Symb_rem != 0 ) RS_Tx[RS_Codes_Tx-1][0] = RS_Symb_rem;
	else RS_Tx[RS_Codes_Tx-1][0] = RS6355_N;		

	// Optimize RS code parameters
	if ( RS_Codes_Tx > 1) {
		n  = RS_Tx[0][0];
		nl = RS_Tx[RS_Codes_Tx-1][0];
		while ( nl+RS_Codes_Tx <= n ) {
			nl += RS_Codes_Tx-1;
			n--;
		}
		k = n - RS6355_R;
		for (i=0;  i<RS_Codes_Tx-1; i++) {
			RS_Tx[i][0] = n;
			RS_Tx[i][1] = k;
		}
		RS_Tx[RS_Codes_Tx-1][0] = nl;
		RS_Tx[RS_Codes_Tx-1][1] = nl - RS6355_R;
	} else {
		if ( RS_Tx[0][0] - RS6355_R <= 0 ) return 1;
		RS_Tx[0][1] = RS_Tx[0][0]  - RS6355_R;
	}

	// PCM parameters
	RS_PCM_Nbit_Tx = 0;
	RS_TCM_Nbit_Tx = 0;
	for (i=0;  i<RS_Codes_Tx; i++)  {
		RS_PCM_Nbit_Tx +=  RS_Tx[i][1];
		RS_TCM_Nbit_Tx +=  RS_Tx[i][0];
	}
    RS_PCM_Nbit_Tx *= LOG_GF64; // number of information bits from V11
    RS_TCM_Nbit_Tx *= LOG_GF64; // number of information bits for TCM encoder
                                // RS_TCM_Bits_Rem_Tx - number of dummy bits to be added before TCM encoder
	
	RSn_Min_Tx = RS_Tx[RS_Codes_Tx-1][0];
	RSk_Min_Tx = RS_Tx[RS_Codes_Tx-1][1];
                                

	return 0;

}

