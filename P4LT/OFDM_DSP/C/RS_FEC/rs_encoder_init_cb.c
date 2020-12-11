/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: rs_encoder_init_cb.c $
 * COPYRIGHT   : 2004 by TL Consulting GmbH
 * DESCRIPTION : Reed-Solomon encoder initialization for optimization of SCK
 *					source: numb. of information bits; result: numb. of code bits
 * OPERATION   : 
 * PROTOTYPE   : int RS_Encoder_Init_cb(int RS_inf_bit_Tx);    
 *                                                          
 * ASSUMPTIONS :
 *
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/RS_FEC/rs_encoder_init_cb.c $
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

int segment ( "seg_ext_pmco" ) RS_Encoder_Init_cb(int RS_inf_bit_Tx)
//int RS_Encoder_Init_cb(int RS_inf_bit_Tx)
{
	int i, n, kl, k, RS_Symb_rem, RS_K_Symb;
    
	RS_K_Symb = RS_inf_bit_Tx/LOG_GF64;
	RS_dummy_bit_Tx = RS_inf_bit_Tx - LOG_GF64*RS_K_Symb;
	if (RS_dummy_bit_Tx>0) {
		RS_K_Symb++;
		RS_dummy_bit_Tx = RS_K_Symb*LOG_GF64 - RS_inf_bit_Tx;}
	// check result
	//if (RS_inf_bit_Tx + RS_dummy_bit_Tx != RS_K_Symb*LOG_GF64) return 1;
	
	RS_Codes_Tx = RS_K_Symb/RS6355_K;
	RS_Symb_rem = RS_K_Symb - RS6355_K*RS_Codes_Tx;
	if ( RS_Symb_rem != 0 ) RS_Codes_Tx++;
    if ( RS_Codes_Tx <=0 ) return 1;
	if ( RS_Codes_Tx > RS_CODES_MAX ) return 1;
	
	// Clear RS_Tx field
    vset_f_E((float *)&RS_Tx[0][0],0.0f,sizeof(RS_Tx));
	// Get initial code parameters
    if ( RS_Codes_Tx > 1 ) {
		for (i=0; i<RS_Codes_Tx-1; i++) RS_Tx[i][1] = RS6355_K;
	} 
	if ( RS_Symb_rem != 0 ) RS_Tx[RS_Codes_Tx-1][1] = RS_Symb_rem;
	else RS_Tx[RS_Codes_Tx-1][1] = RS6355_K;		

	// Optimize RS code parameters
	if ( RS_Codes_Tx > 1) {
		k  = RS_Tx[0][1];
		kl = RS_Tx[RS_Codes_Tx-1][1];
		while ( kl+RS_Codes_Tx <= k ) {
			kl += RS_Codes_Tx-1;
			k--;
		}
		n = k + RS6355_R;
		for (i=0;  i<RS_Codes_Tx-1; i++) {
			RS_Tx[i][0] = n;
			RS_Tx[i][1] = k;
		}
		RS_Tx[RS_Codes_Tx-1][0] = kl + RS6355_R;
		RS_Tx[RS_Codes_Tx-1][1] = kl;
	} else {
		RS_Tx[0][0] = RS_Tx[0][1]  + RS6355_R;
		if ( RS_Tx[0][0] - RS6355_R <= 0 ) return 1;
		
	}

	//  PCM / TCM parameters
	RS_PCM_Nbit_Tx = 0;
	RS_TCM_Nbit_Tx = 0;
	for (i=0;  i<RS_Codes_Tx; i++)  {
		RS_PCM_Nbit_Tx +=  RS_Tx[i][1];
		RS_TCM_Nbit_Tx +=  RS_Tx[i][0];
	}
   
	RS_PCM_Nbit_Tx *= LOG_GF64;
    RS_TCM_Nbit_Tx *= LOG_GF64;
    
    // check result
    if (RS_inf_bit_Tx != RS_PCM_Nbit_Tx -  RS_dummy_bit_Tx) return 1;
    
    

    
	return 0;

}

