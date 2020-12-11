/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: rs_decoder_init_cb.c $
 * COPYRIGHT   : 2004 by TL Consulting GmbH
 * DESCRIPTION : Reed-Solomon decoder initialization for optimization of SCK
 *					source: numb. of information bits; result: numb. of code bits
 * OPERATION   : 
 * PROTOTYPE   : int RS_Decoder_Init_cb(int RS_inf_bit_Rx);    
 *                                                          
 * ASSUMPTIONS :
 *
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/RS_FEC/rs_decoder_init_cb.c $
 * 
 * 1     19.01.05 15:41 Chchlee
 * 
 * 1     28.09.04 18:01 Maag01
 * Initial version
 * 
 * 1	29.07.04	PL
 * 
 *****************************************************************************/
#include "../../h/util/util.h"
#include "../../h/rs_fec/RS6355_defs.h"
#include "../../h/rs_fec/RS_externs.h"

int segment ( "seg_ext_pmco" ) RS_Decoder_Init_cb(int RS_inf_bit_Rx)
//int RS_Decoder_Init_cb(int RS_inf_bit_Rx)
{
	int i, n, kl, k, RS_Symb_rem, RS_K_Symb;
    
	RS_K_Symb = RS_inf_bit_Rx/LOG_GF64;
	RS_dummy_bit_Rx = RS_inf_bit_Rx - LOG_GF64*RS_K_Symb;
	if (RS_dummy_bit_Rx>0) {
		RS_K_Symb++;
		RS_dummy_bit_Rx = RS_K_Symb*LOG_GF64 - RS_inf_bit_Rx;}
	// check result
	//if (RS_inf_bit_Rx + RS_dummy_bit_Rx != RS_K_Symb*LOG_GF64) return 1;
	
	RS_Codes_Rx = RS_K_Symb/RS6355_K;
	RS_Symb_rem = RS_K_Symb - RS6355_K*RS_Codes_Rx;
	if ( RS_Symb_rem != 0 ) RS_Codes_Rx++;
    if ( RS_Codes_Rx <=0 ) return 1;
	if ( RS_Codes_Rx > RS_CODES_MAX ) return 1;
	
	// Clear RS_Tx field
    vset_f_E((float *)&RS_Rx[0][0],0.0f,sizeof(RS_Rx));
	// Get initial code parameters
    if ( RS_Codes_Rx > 1 ) {
		for (i=0; i<RS_Codes_Rx-1; i++) RS_Rx[i][1] = RS6355_K;
	} 
	if ( RS_Symb_rem != 0 ) RS_Rx[RS_Codes_Rx-1][1] = RS_Symb_rem;
	else RS_Rx[RS_Codes_Rx-1][1] = RS6355_K;		

	// Optimize RS code parameters
	if ( RS_Codes_Rx > 1) {
		k  = RS_Rx[0][1];
		kl = RS_Rx[RS_Codes_Rx-1][1];
		while ( kl+RS_Codes_Rx <= k ) {
			kl += RS_Codes_Rx-1;
			k--;
		}
		n = k + RS6355_R;
		for (i=0;  i<RS_Codes_Rx-1; i++) {
			RS_Rx[i][0] = n;
			RS_Rx[i][1] = k;
		}
		RS_Rx[RS_Codes_Rx-1][0] = kl + RS6355_R;
		RS_Rx[RS_Codes_Rx-1][1] = kl;
	} else {
		RS_Rx[0][0] = RS_Rx[0][1]  + RS6355_R;
		if ( RS_Rx[0][0] - RS6355_R <= 0 ) return 1;
		
	}

	//  PCM / TCM parameters
	RS_PCM_Nbit_Rx = 0;
	RS_TCM_Nbit_Rx = 0;
	for (i=0;  i<RS_Codes_Rx; i++)  {
		RS_PCM_Nbit_Rx +=  RS_Rx[i][1];
		RS_TCM_Nbit_Rx +=  RS_Rx[i][0];
	}
   
	RS_PCM_Nbit_Rx *= LOG_GF64;
    RS_TCM_Nbit_Rx *= LOG_GF64;
    
    // check result
    if (RS_inf_bit_Rx != RS_PCM_Nbit_Rx -  RS_dummy_bit_Rx) return 1;
    
    

    
	return 0;

}

