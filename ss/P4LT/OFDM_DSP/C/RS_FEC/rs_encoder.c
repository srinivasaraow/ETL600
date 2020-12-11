/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: rs_encoder.c $
 * COPYRIGHT   : 2004 by TL Consulting GmbH
 * DESCRIPTION : Reed-Solomon encoder
 * OPERATION   : 
 * PROTOTYPE   : int RS_Encoder(int *TCM_Out, int *PCM_In, int *tmp);    
 *                                                          
 * ASSUMPTIONS :
 *
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/RS_FEC/rs_encoder.c $
 * 
 * 1     19.01.05 15:41 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * 
 * 1     11.04.04 vgp
 * 2     10.09.04  Interleaver, vgp
 * 
 *****************************************************************************/
#include <stdlib.h>
#include "../../h/util/util.h"
#include "../../h/rs_fec/RS_externs.h"

int segment ( "seg_ext_pmco" ) RS_Encoder(int *BitStr_Out, int *BitStr_In, int *tmp)
//int RS_Encoder(int *BitStr_Out, int *BitStr_In, int *tmp)
{
	int           i, j, k, RSn, RSk;

	int  *LogCW64, *CW64, *CWBitVec, *RSInfVec;
	int  In_Symb_Ind, Out_Bit_Ind;

    int  Pack1BitTo6BitWord_DD( int   R[],  int X[], int NBit);
    void Unpack6BitTo1BitWord_DD( int   R[], int  X[],  int   Nsymb);  
	void RS6355_Encoder(int *, int *);


	// tmp[] vector length 2*GF64 + LOG_GF64*GF64 = 2*64 + 6*64 = 128 + 384 = 512
    // switch tmp[] vector to 16-bit addressing mode
	CW64 = (int *)(2*(unsigned int)tmp);                   // 64 word length
	LogCW64 = CW64 + GF64;         // 64 word length
	CWBitVec = LogCW64 + GF64;    // 6*64 = 384 word length
	RSInfVec = CWBitVec + LOG_GF64*GF64; // RS_PCM_Nbit_Tx/6 word length

	// initialize input index
	In_Symb_Ind = 0;
	
	// pack V11 inf. bits into RS information word
	Pack1BitTo6BitWord_DD( RSInfVec,  BitStr_In, RS_PCM_Nbit_Tx);

    // RS Encode main loop
    for (i=0; i<RS_Codes_Tx; i++) {

    	// Get RS code parameters
		RSn = RS_Tx[i][0];
		RSk = RS_Tx[i][1];
		   
		// copy inf. symb
		vcopy_EE((float*)CW64,(float *)&RSInfVec[In_Symb_Ind],RSk);

		// Set to zero rest of the information symbols
		if (RSk != RS6355_K) vset_i_E((int *)&CW64[RSk], 0, RS6355_K-RSk);
		
        // encode information word
		RS6355_Encoder(CW64, LogCW64);

		// unpack RS(RSn,RSk) codeword in temp. RS codeword to bit string
	    Unpack6BitTo1BitWord_DD( CWBitVec, CW64,  RSk);  
	    Unpack6BitTo1BitWord_DD( &CWBitVec[LOG_GF64*RSk], &CW64[RS6355_K], RS6355_R);  
        
        // interleaver
        if ( RS_Codes_Tx > 1 ) {
          Out_Bit_Ind = LOG_GF64*i;	
          for (j=0; j<RSn; j++) {
            vcopy_EE((float *)&BitStr_Out[Out_Bit_Ind],(float *)&CWBitVec[j*LOG_GF64],LOG_GF64);
          	if ( j < RSn_Min_Tx ) Out_Bit_Ind += LOG_GF64*RS_Codes_Tx;
          	else Out_Bit_Ind += LOG_GF64*(RS_Codes_Tx-1);
          }
        } else {
          vcopy_EE((float *)&BitStr_Out[0],(float *)CWBitVec,LOG_GF64*RSn);
        }
        
        // update input symbol index
		In_Symb_Ind += RSk;        

	} // next RS code

	return 0;
}

