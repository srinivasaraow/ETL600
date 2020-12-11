/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: rs_decoder.c $
 * COPYRIGHT   : 2004 by TL Consulting GmbH
 * DESCRIPTION : Reed-Solomon decoder
 * OPERATION   : 
 * PROTOTYPE   : void RS_Decoder(int *PCM_Out, int *TCM_In, int *tmp) ;    
 *                                                          
 * ASSUMPTIONS :
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/RS_FEC/rs_decoder.c $
 * 
 * 3     7.03.05 8:50 Ats
 * Exchange with TLC
 * 
 * 1     19.01.05 15:41 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * 
 * 1     11.04.04  vgp
 * 2     10.09.04  Deinterleaver, vgp
 *****************************************************************************/
#include "../../h/util/util.h"
#include "../../h/rs_fec/RS_externs.h"

//int  segment ( "seg_ext_pmco" ) RS_Decoder(int *BitStr_Out, int *BitStr_In, int *tmp)
int  RS_Decoder(int *BitStr_Out, int *BitStr_In, int *tmp)
{
	int           i, j, k, RSn, RSk, status, Decode_status;
	
	int  *CW64, *CWBitVec, *RSCodVec, *TmpVec;
	int  In_Symb_Ind, Out_Bit_Ind;

    int  Pack1BitTo6BitWord_DD( int   R[],  int X[], int NBit);
    void Unpack6BitTo1BitWord_DD( int   R[], int  X[],  int   Nsymb);  
    int  RS6355_Decoder(int *,int *,int *,int *);


	// tmp[] vector length 2*GF64 + LOG_GF64*GF64 = 2*64 + 6*64 = 128 + 384 = 512
    // switch tmp[] vector to 16-bit addressing mode
	CW64 = (int *)(2*(unsigned int)tmp);   // 64 word length
	CWBitVec = CW64 + GF64;                // 6*64 = 384 word length
	RSCodVec = CWBitVec + LOG_GF64*GF64;   // RS_TCM_Nbit_Rx/6 word length
	TmpVec = RSCodVec + RS_TCM_Nbit_Rx/6;  // 728 words
	
	// clear output status flag
	Decode_status = 0;
		
	// out bit index
	Out_Bit_Ind = 0;
	
	// pack V11 inf. bits into RS information word
	Pack1BitTo6BitWord_DD( RSCodVec, BitStr_In, RS_TCM_Nbit_Rx);

    for (i=0; i<RS_Codes_Rx; i++) {
    	
	   RSn = RS_Rx[i][0];
	   RSk = RS_Rx[i][1];
	   
       // deinterleaver
       if ( RS_Codes_Rx > 1 ) {
       	 k = 0;
       	 In_Symb_Ind = i;
         for (j=0; j<RSn; j++) {
          	if (k == RSk) k = RS6355_K;
          	CW64[k++] =  RSCodVec[In_Symb_Ind];
         	if ( j < RSn_Min_Rx ) In_Symb_Ind += RS_Codes_Rx;
          	else In_Symb_Ind += RS_Codes_Rx-1;          	
         }
       } else {  // number of RS codes equal to 1
	     // copy inf. symb
	     vcopy_EE((float*)CW64,(float *)&RSCodVec[0],RSk);
	     // copy parity symb
	     vcopy_EE((float*)&CW64[RS6355_K],(float *)&RSCodVec[RSk],RS6355_K);
       }

	   // Set to zero rest of the information symbols
	   if (RSk != RS6355_K) vset_i_E((int *)&CW64[RSk], 0, RS6355_K-RSk);

// insert max 4 errors in order to debug
// error in the rang 0x01..0x3f, no error - 0x00	   	   
#if 0
CW64[0] ^= 0x3f;
CW64[1] ^= 0x3f;
CW64[2] ^= 0x3f;
CW64[3] ^= 0x3f;
//CW64[4] ^= 0x3f; // forse errors of RS
#endif
      	   	 	  
	   // Decode	  
       status = RS6355_Decoder(CW64,RS6355_Lctr,CW64,TmpVec);
       if (status != 0) Decode_status = 1;

	   Unpack6BitTo1BitWord_DD( &BitStr_Out[Out_Bit_Ind], CW64,  RSk);         
	   
       // update output bit index
	   Out_Bit_Ind += LOG_GF64*RSk;
	   	  
	} // next RS codeword
	
	
	return Decode_status;
}

