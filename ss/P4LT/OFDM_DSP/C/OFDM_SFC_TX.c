/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_SFC_TX.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : TX Sampling frequency conversion and Spectral Shift
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/OFDM_SFC_TX.c $
 * 
 * 9     24.07.08 15:33 Chhamaa
 * 
 * 4     5.04.06 15:28 Chhamaa
 * 
 * 4     17.03.06 14:41 Chhamaa
 * 
 * 2     13.09.05 13:23 Ats
 * Exchange from TLC
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 4     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       15.02.2003 DLK
 *		 18.05.2004 PL,DLK perpared for multiple bandwidths
 * implementation for FN 4000,8000,12000,16000,24000
 *	   10.06.2004 PL
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"
#include "../h/int_dec_filters/INTDEC_externs.h"
#include "../h/int_dec_filters/INTDEC_filt_c.h"

//========================================
void segment ( "seg_ext_pmco" ) OFDM_SFC_TX()
	{
			float Exp_Gen(float, complex_float, complex_float *, float, int);
	
	int Step_o; // outer step
	int Step_i; // inner step
	int NL_SFC; // for 4/3 and 2*4/3 only, to define blocklength

	// **************************************************************************
	// * 		Sampling Frequency conversion from Fn to 64kHz (complex)		*
	// *	No:		Bandwidth		Fn		x-FFT		x-SFC      
	// *    2008/1		 2000		 2000	x8			x4 (for N=64,128 only)
	// *	1			 4000 HD	 4000	x2			x2x4
	// *	2			 4000 HD     4000   x8          x2 (for N=64,128 only)
	// *	3			 4000 LD	 6000	x4			x4/3x2
	// *	4			 4000 LD     6000   x8          x4/3 (for N=64,128 only)
	// *				 4000 LD	 4800	x8			x5/3 (for N=64,128 only)
	
	// *	5			 8000 HD	 8000	x2			x4	
	// *				 8000 LD(a)	12000	x2			x4/3x2
	// *				 8000 LD(b)	 9600	x4			x5/3 (for N<=256 only)
		
	// *	6		   	12000		12000	x2			x4/3x2
	// * 	7		   	16000		16000	x2			x2
	// *	8			20000		24000	x2			x4/3
	// *	9			24000		24000	x2			x4/3
	// * 	10			28000		32000	x2			x1
	// *	11			32000		32000	x2			x1
	// *					*
	// * 	LD:Low Delay 		HD:High Delay
	// **************************************************************************
	
	// **************************************************************************
	// * 		Spectral Shift by Rot_Fr_TX										*
	// *			Note:  Rot_Fr_TX.re = cosf(Freq_Offset_TX);					*
	// *				   Rot_Fr_TX.im = sinf(Freq_Offset_TX);					*
	// *						Freq_Offset_TX = Df_Hz_TX*OFDM_TWO_PI/FS_TX_RX;	*
	// *						with Df_Hz_TX in Hz and FS_TX_RX Sampling Freq.	*
	// *				   Delta_Phi_Symbol_TX is phase increment				*
	// *									after block of length 2*NL			*
	// **************************************************************************
	
	/*if(Num_Symbol_TX==10)
   		            	{
   		            		asm("#include <def21161.h>");
			    	asm("dm(CP1A)=m5;");
    				asm("dm(CP3A)=m5;");
	    			asm("dm(SPCTL0) = m5;");
    				asm("dm(SPCTL2) = m5;");
		        	asm("nop;");
                    asm("nop;");
                    asm("nop;");
   		            	}*/
   		            	
	switch (FN)
		{
		case 2000:
		//Variant 1:  x-FFT:8  x-SFC: x4
		//	!!! N<=128) (32*NL=32*(128+32)=5120cmplx=10240words)
		
		// x4 ******************************************************************
		// Input: 	Cmplx_Array[0...8*NL-1] 	-> 8*NL (complex)
		//			 -->[Interpolation by 4]<--
		// Output: 	Cmplx_Array[8*NL...40*NL-1] 	-> 32*NL (complex)
		// 			    Note: 40*NL=40*(128+32)=6400 cmplx
		//				40*NL > 4224 cmplx samples !! more steps necessary
		//				In Memory Cmplx is followed by Fl_Array_R
		//				The tail of Fl_Array_R holds Filter Delay Line [4302..4351]
		//				Cmplx[]+Fl_Array_R[] comprise 4096+4302=8398  32bit words
		//				= 4199 cmplx samples

		if (Filter_TX_Interpolation == 8)
		{		
		// Amplifying Signal by Interpolation Factor
		vsmul_f_DD((float *)&Cmplx_Array[0],SFC_Interpolation,(float *)&Cmplx_Array[0],8*NL2);  
   
		// Save Input into buffer
		vcopy_ED((float *)&Interp_Buf[0],(float *)&Cmplx_Array[0],2*8*NL);
		 	
		// Blocks of size NL (complex) are interpolated by 4
		// this is repeated eight times (Step_o=0..7)
		for (Step_o=0; Step_o < 8; Step_o++)
			{ 
			// Restore block No "Step_o" of size NL (complex) into Cmplx_Array[0]
			vcopy_DE((float *)&Cmplx_Array[0],(float *)&Interp_Buf[0+Step_o*NL],2*NL);
		
			// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int4x24_Filt_Coeff_V4[0],2*INTFILT_4_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int4x24_filt_c_Delay[0],2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));

			// Interpolation by 4 - Output: Cmplx_Array[2*NL...6*NL-1] -> 4*NL (complex)
			Int4x24_filt_c(&Cmplx_Array[2*NL],Cmplx_Array, NL, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Int4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
	
			// next line instead of inner loop "Step_i" only
			//vcopy_ED((float *)&OFDM_Symbol_TX[0+Step_o*4*NL],(float *)&Cmplx_Array[2*NL],2*4*NL);
			
			for (Step_i=0; Step_i < 2; Step_i++) //2*2NL(complex)
	    		{
	     		// Note: Delta_Phi_Symbol_TX is valid for blocksize 2*NL only !
				Start_Phi_TX = fmodf(Start_Phi_New_TX,OFDM_TWO_PI);
	    		Start_Phi_New_TX = Exp_Gen(Start_Phi_TX,Rot_Fr_TX,Cmplx_Array,Delta_Phi_Symbol_TX,2*NL);
	    		vmul_c_DDD(&Cmplx_Array[2*NL+Step_i*2*NL],&Cmplx_Array[2*NL+Step_i*2*NL],Cmplx_Array,2*NL);
				vcopy_ED((float *)&OFDM_Symbol_TX[0+Step_o*4*NL+Step_i*2*NL],(float *)&Cmplx_Array[2*NL+Step_i*2*NL],2*2*NL);
	    		}
			
			}
		//Output: 	OFDM_Symbol_TX[0...32*NL-1] 	-> 32*NL (complex)
		}
		break;
			
		case 4000:
		//==>Variant 1
		if (Filter_TX_Interpolation == 2)
		{
		//Variant 1:  x-FFT:2  x-SFC: x2x4
		
		// x2 ******************************************************************
		// Input: 	Cmplx_Array[0...2*NL-1] 	-> 2*NL (complex)
		//			 -->[Interpolation by 2]<--
		// Output: 	Cmplx_Array[2*NL...6*NL-1] 	-> 4*NL (complex)
		//			Interp_Buf[0...4*NL-1]
		//				Note: 6*NL <= 6912 32bit words (= for N=512,L=64)
		//				In Memory Cmplx is followed by Fl_Array_R
		//				The tail of Fl_Array_R holds Filter Delay Line [4302..4351]
		//				Cmplx[]+Fl_Array_R[] comprise 4096+4302=8398  32bit words
		//				= 4199 cmplx samples
		//				!! Fl_Array_R will be overwritten by this procedure !!
		
		// Restore Filter Coefficients from external Memory to PM 
		vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int2x12_Filt_Coeff_V1[0],2*INTFILT_2_LENGTH);
		// Restore Delay Array from external Memory to DM
		vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int2x12_filt_c_Delay[0],2*(INTFILT_2_LENGTH/INTFILT_2_POLYNUMBER+1));
	
		// Amplifying Signal by Interpolation Factor
//    	vsmul_f_DD((float *)&Cmplx_Array[0],sqrtf(SFC_Interpolation),(float *)&Cmplx_Array[0],2*NL2);  
    	vsmul_f_DD((float *)&Cmplx_Array[0],SFC_Interpolation,(float *)&Cmplx_Array[0],2*NL2);  
    
		
		// Interpolation by 2
		Int2x12_filt_c(&Cmplx_Array[2*NL],Cmplx_Array, 2*NL, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

		// Save Delay Array into external memory buffer
		vcopy_ED((float *)&Int2x12_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_2_LENGTH/INTFILT_2_POLYNUMBER+1));
	
		// Save result into buffer
		vcopy_ED((float *)&Interp_Buf[0],(float *)&Cmplx_Array[2*NL],2*4*NL); // check float adressing!
		
		
		// x4 ******************************************************************
		// Input: 	Interp_Buf[0...4*NL-1] 	-> 4*NL (complex)
		//			 -->[Interpolation by 4]<--
		// Output: 	OFDM_Symbol_TX[0...16*NL-1] 	-> 16*NL (complex)
		//				Note: 16*NL (complex) <= 18432 32bit words (= for N=512,L=64)
		//				OFDM_Symbol_TX -> Tx_Array comprise 36864  32bit words
		//				!! OFDM_symbol_TX is ping-pong buffer !!
		
		// Blocks of size NL (complex) are interpolated by 4
		// this is repeated four times (Step_o=0..3)
		for (Step_o=0; Step_o < 4; Step_o++)
			{ 
			// Restore block No "Step_o" of size NL (complex) into Cmplx_Array[0]
			vcopy_DE((float *)&Cmplx_Array[0],(float *)&Interp_Buf[0+Step_o*NL],2*NL);
		
			// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int4x24_Filt_Coeff_V1[0],2*INTFILT_4_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int4x24_filt_c_Delay[0],2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
	
			// Interpolation by 4 - Output: Cmplx_Array[2*NL...6*NL-1] -> 4*NL (complex)
			Int4x24_filt_c(&Cmplx_Array[2*NL],Cmplx_Array, NL, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Int4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
	
			// next line instead of inner loop "Step_i" only
			//vcopy_ED((float *)&OFDM_Symbol_TX[0+Step_o*4*NL],(float *)&Cmplx_Array[2*NL],2*4*NL);
			
			for (Step_i=0; Step_i < 2; Step_i++) //2*2NL(complex)
	    		{
	     		// Note: Delta_Phi_Symbol_TX is valid for blocksize 2*NL only !
				Start_Phi_TX = fmodf(Start_Phi_New_TX,OFDM_TWO_PI);
	    		Start_Phi_New_TX = Exp_Gen(Start_Phi_TX,Rot_Fr_TX,Cmplx_Array,Delta_Phi_Symbol_TX,2*NL);
	    		vmul_c_DDD(&Cmplx_Array[2*NL+Step_i*2*NL],&Cmplx_Array[2*NL+Step_i*2*NL],Cmplx_Array,2*NL);
				vcopy_ED((float *)&OFDM_Symbol_TX[0+Step_o*4*NL+Step_i*2*NL],(float *)&Cmplx_Array[2*NL+Step_i*2*NL],2*2*NL);
	    		}
	    	
			}
		}// end of variant 1
		
		//==>Variant 2
		if (Filter_TX_Interpolation == 8)
		{		
		//Variant 2:  x-FFT:8  x-SFC: x2
		//	!!! N<=128) (16*NL=16*(128+32)=2560cmplx=5120words)
		
		// x2 ******************************************************************
		// Input: 	Cmplx_Array[0...8*NL-1] 	-> 8*NL (complex)
		//			 -->[Interpolation by 2]<--
		// Output: 	Cmplx_Array[8*NL...24*NL-1] 	-> 16*NL (complex)
		// 			    Note: 24*NL=24*(128+32)=3840 cmplx
		//				24*NL <= 4224 cmplx samples
		//				In Memory Cmplx is followed by Fl_Array_R
		//				The tail of Fl_Array_R holds Filter Delay Line [4302..4351]
		//				Cmplx[]+Fl_Array_R[] comprise 4096+4302=8398  32bit words
		//				= 4199 cmplx samples
		
		// Restore Filter Coefficients from external Memory to PM 
		vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int2x12_Filt_Coeff_V2[0],2*INTFILT_2_LENGTH);
		// Restore Delay Array from external Memory to DM
		vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int2x12_filt_c_Delay[0],2*(INTFILT_2_LENGTH/INTFILT_2_POLYNUMBER+1));
		
		// Amplifying Signal by Interpolation Factor
    	vsmul_f_DD((float *)&Cmplx_Array[0],SFC_Interpolation,(float *)&Cmplx_Array[0],2*8*NL);  
    		
		// Interpolation by 2
		Int2x12_filt_c(&Cmplx_Array[8*NL],Cmplx_Array, 8*NL, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

		// Save Delay Array into external memory buffer
		vcopy_ED((float *)&Int2x12_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_2_LENGTH/INTFILT_2_POLYNUMBER+1));
	
		// Save result into buffer
//		vcopy_ED((float *)&Interp_Buf[0],(float *)&Cmplx_Array[8*NL],2*4*NL); // check float adressing!
// not used PL		
		// next line instead of inner loop "Step_i" only
		//vcopy_ED((float *)&OFDM_Symbol_TX[0],(float *)&Cmplx_Array[2*NL],2*4*NL);
	    	
		for (Step_i=0; Step_i < 8; Step_i++)
	    		{
	     		// Note: Delta_Phi_Symbol_TX is valid for blocksize 2*NL only !
	    		Start_Phi_TX = fmodf(Start_Phi_New_TX,OFDM_TWO_PI);
	    		Start_Phi_New_TX = Exp_Gen(Start_Phi_TX,Rot_Fr_TX,Cmplx_Array,Delta_Phi_Symbol_TX,2*NL);
	    		vmul_c_DDD(&Cmplx_Array[8*NL+Step_i*2*NL],&Cmplx_Array[8*NL+Step_i*2*NL],Cmplx_Array,2*NL);
				vcopy_ED((float *)&OFDM_Symbol_TX[0+Step_i*2*NL],(float *)&Cmplx_Array[8*NL+Step_i*2*NL],2*2*NL);
	    		}
	    		
		}// end of variant 2
		break;
		
		case 4800:
		if (Filter_TX_Interpolation == 8)
		{
		//Variant 4:  x-FFT:8  x-SFC: x5/3
		//	!!! N<=128) allowed are 64/8, 128/16, used is 64/8 for FN=4.8kHz
		//	!!! blocksize N=128: 5*8NL = 40NL <= 40(128+16) <= 5760 (complex))
		//	!!!     			5/3*8NL = 1920(complex)
		//	!!! blocksize N= 64: 5*8NL = 40NL <= 40(64+8) <= 2880 (complex))
		//	!!!     			5/3*8NL = 960 (complex)	
		//
		// x5 ******************************************************************
		// Input: 	Cmplx_Array[0...8*NL-1] 	-> 8*NL (complex)
		//			 -->[Interpolation by 5]<--
		// Output: 	5*8NL = 40NL <= 40(64+8) <= 2880 (complex)
		//				In Memory Cmplx is followed by Fl_Array_R
		//				The tail of Fl_Array_R holds Filter Delay Line [4300..4351]
		//				Cmplx[]+Fl_Array_R[] comprise 4096+4300=8396  32bit words
		//				= 4198 cmplx samples
		//				!! Fl_Array_R will be overwritten by this procedure !!
		// Processing in several steps is mandatory for N=128
		
		// Blocksize for processing of exp_gen
		NL_SFC = 5* ((int)((float)NL/3.0f)); //  NL_SFC=5/3*NL this is what compiler likes to see	
		
		// Amplifying Signal by Interpolation Factor
		vsmul_f_DD((float *)&Cmplx_Array[0],5.0f,(float *)&Cmplx_Array[0],2*8*NL);  
   
		// Save Input into buffer
		vcopy_ED((float *)&Interp_Buf[0],(float *)&Cmplx_Array[0],2*8*NL);
		 	
		// Blocks of size NL (complex) are interpolated by 5 /3
		// this is repeated eight times (Step_o=0..7)
		for (Step_o=0; Step_o < 8; Step_o++)
			{ 
			// Restore block No "Step_o" of size NL (complex) into Cmplx_Array[0]
			vcopy_DE((float *)&Cmplx_Array[0],(float *)&Interp_Buf[0+Step_o*NL],2*NL);
		
			// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int5x25_Filt_Coeff_V1[0],2*INTFILT_5_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int5x25_filt_c_Delay[0],2*(INTFILT_5_LENGTH/INTFILT_5_POLYNUMBER+1));
	
			// Interpolation by 5 - Output: Cmplx_Array[2*NL...7*NL-1] -> 5*NL (complex)
			Int5x25_filt_c(&Cmplx_Array[2*NL],Cmplx_Array, NL, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Int5x25_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_5_LENGTH/INTFILT_5_POLYNUMBER+1));
	
			// 5*NL (complex) to process in next operation	
			
	     // Decimation by 3: input: 5NL(complex) output 5/3NL=NL_SFC(complex), which is integer for
				//					allowed NL combinations. after 8 calls: 8*5/3NL=40/3NL=8*NL_SFC
			// real part
	    	vcopy_s_EE( (float *)&Cmplx_Array[0], 2*1, (float *)&Cmplx_Array[2*NL], 2*3, NL_SFC );
			// imag part
	    	vcopy_s_EE( (((float *)&Cmplx_Array[0])+1), 2*1, (((float *)&Cmplx_Array[2*NL])+1), 2*3, NL_SFC );
	    	// Output: 	Cmplx_Array[0...5/3*NL-1] = [0...NL_SFC-1]
			
			Start_Phi_TX = fmodf(Start_Phi_New_TX,OFDM_TWO_PI);
	    	Start_Phi_New_TX = Exp_Gen(Start_Phi_TX,Rot_Fr_TX,&Cmplx_Array[2*NL_SFC],Delta_Phi_Symbol_TX,NL_SFC);
	    	vmul_c_DDD(&Cmplx_Array[0],&Cmplx_Array[0],&Cmplx_Array[2*NL_SFC],NL_SFC);
	    
	    	// Save result into OFDM_Symbol_TX [0..NL_SFC-1] [NL_SFC..2*NL_SFC-1] [2*NL_SFC..3*NL_SFC-1].. [7*NL_SFC..8*NL_SFC-1]
			vcopy_ED((float *)&OFDM_Symbol_TX[0+Step_o*NL_SFC],(float *)&Cmplx_Array[0],2*NL_SFC); 
			}
		//Output: 	OFDM_Symbol_TX[0...8*NL_SFC-1] 	-> 8*NL_SFC = 40/3*NL (complex)
		}
		break;
		
		case 6000:
		//==>Variant 2
		if (Filter_TX_Interpolation == 2)
		{
		// Variant 2: FFTx2 SFC: x4/3x4
				// Note: N=512
		
		NL_SFC = 4* ((int)((float)NL/3.0f)); //  NL_SFC=4/3*NL this is what compiler likes to see	
			//Rot_Fr_TX.re = cosf(Freq_Offset_TX);
			//Rot_Fr_TX.im = sinf(Freq_Offset_TX);
			//Delta_Phi_Symbol_TX = Freq_Offset_TX*(float)(NL_SFC);
			
		// x4 /3 ******************************************************************
		// Input: 	OFDM_Symbol_TX[0...2*NL-1] 	-> 2*NL (complex)
		//			 -->[Interpolation by 4]<--
		// Output: 	Interp_Buf[0...8*NL-1] 	-> 8*NL (complex)
		//				Note: 8*NL (complex) <= 9216 32bit words (= for N=512,L=64)
		//				In Memory Cmplx is followed by Fl_Array_R
		//				The tail of Fl_Array_R holds Filter Delay Line [4300..4351]
		//				Cmplx[]+Fl_Array_R[] comprise 4096+4300=8396  32bit words
		//				= 4198 cmplx samples
		//				!! Operation is split into two parts !!
		
		// Amplifying Signal by Interpolation Factor
		vsmul_f_DD((float *)&Cmplx_Array[0],16.0f,(float *)&Cmplx_Array[0],2*2*NL);  
   
		// Save Input into buffer
		vcopy_ED((float *)&OFDM_Symbol_TX[0],(float *)&Cmplx_Array[0],2*2*NL);
		 	
		// Blocks of size NL (complex) are interpolated by 4 /3
		// this is repeated two times (Step_o=0..1)
		for (Step_o=0; Step_o < 2; Step_o++)
			{ 
			// Restore block No "Step_o" of size NL (complex) into Cmplx_Array[0]
			vcopy_DE((float *)&Cmplx_Array[0],(float *)&OFDM_Symbol_TX[0+Step_o*NL],2*NL);
		
			// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int4x24_Filt_Coeff_V3[0],2*INTFILT_4_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int4x24_filt_c_Delay[0],2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
	
			// Interpolation by 4 - Output: Cmplx_Array[2*NL...6*NL-1] -> 4*NL (complex)
			Int4x24_filt_c(&Cmplx_Array[2*NL],Cmplx_Array, NL, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Int4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
	
			// 4*NL (complex) to process in next operation	
			
	     	// Decimation by 3: input: 4NL(complex) output 4/3NL=NL_SFC(complex), which is integer for
				//					allowed NL combinations. after 4 calls: 4*4/3NL=16/3NL=4*NL_SFC
			// real part
	    	vcopy_s_ED( (float *)&Interp_Buf[0+Step_o*NL_SFC], 2*1, (float *)&Cmplx_Array[2*NL], 2*3, NL_SFC );
			// imag part
	    	vcopy_s_ED( (((float *)&Interp_Buf[0+Step_o*NL_SFC])+1), 2*1, (((float *)&Cmplx_Array[2*NL])+1), 2*3, NL_SFC );
	    	
			}
			// Output: 	Interp_Buf[0...8/3*NL-1] = [0...2*NL_SFC-1]
			
		// x4 ******************************************************************
		// Input: 	Interp_Buf[0...2*NL_SFC-1] 	-> 2*NL_SFC (complex)
		//			 -->[Interpolation by 4]<--
		// Output: 	OFDM_Symbol_TX[0...8*NL_SFC-1] 	-> 8*NL_SFC = 32/3*NL (complex)
		//			
		//				Note: 5*NL_SFC=5*768 <= 7680 32bit words (= for N=512,L=64)
		//				In Memory Cmplx is followed by Fl_Array_R
		//				The tail of Fl_Array_R holds Filter Delay Line [4300..4351]
		//				Cmplx[]+Fl_Array_R[] comprise 4096+4300=8396  32bit words
		//				= 4198 cmplx samples
		//				!! Fl_Array_R will be overwritten by this procedure !!
		
		// split into two times interpolation from NL_SFC -> 4*NL_SFC
		
		for (Step_o=0; Step_o < 2; Step_o++)
			{ 
			// Restore block No "Step_o" of size NL_SFC (complex) into Cmplx_Array[4*NL_SFC...5*NL_SFC-1]
			vcopy_DE((float *)&Cmplx_Array[4*NL_SFC],(float *)&Interp_Buf[0+Step_o*NL_SFC],2*NL_SFC);
		
			// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int4x24_Filt_Coeff_V2[0],2*INTFILT_4_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int4x24_filt_c_Delay1[0],2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
	 		
			// Interpolation by 4
			Int4x24_filt_c(&Cmplx_Array[0],&Cmplx_Array[4*NL_SFC], NL_SFC, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);
	
			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Int4x24_filt_c_Delay1[0],(float *)IntDec_Filt_Delay,2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
		
			for (Step_i=0; Step_i < 4; Step_i++) //split 4*NL_SFC into four times NL_SFC(complex)
	    		{
				Start_Phi_TX = fmodf(Start_Phi_New_TX,OFDM_TWO_PI);
	    		Start_Phi_New_TX = Exp_Gen(Start_Phi_TX,Rot_Fr_TX,&Cmplx_Array[4*NL_SFC],Delta_Phi_Symbol_TX,NL_SFC);
	    		vmul_c_DDD(&Cmplx_Array[0+Step_i*NL_SFC],&Cmplx_Array[0+Step_i*NL_SFC],&Cmplx_Array[4*NL_SFC],NL_SFC);
	    		}
	    		
			// Save result into OFDM_Symbol_TX [0..4*NL_SFC-1] [4*NL_SFC..8*NL_SFC-1]
			vcopy_ED((float *)&OFDM_Symbol_TX[0+Step_o*4*NL_SFC],(float *)&Cmplx_Array[0],2*4*NL_SFC); 
			}
		//Output: 	OFDM_Symbol_TX[0...8*NL_SFC-1] 	-> 8*NL_SFC = 32/3*NL (complex)
		}
		//==>Variant 3
		if (Filter_TX_Interpolation == 4)
		{
		// Variant 3: FFTx4 SFC: x4/3x2
				// Note: Nmax =256 !!! for N=512 allocated memory isn't sufficient
		
		NL_SFC = 4* ((int)((float)NL/3.0f)); //  NL_SFC=4/3*NL this is what compiler likes to see	
			//Rot_Fr_TX.re = cosf(Freq_Offset_TX);
			//Rot_Fr_TX.im = sinf(Freq_Offset_TX);
			//Delta_Phi_Symbol_TX = Freq_Offset_TX*(float)(NL_SFC);
			
		// x4 /3 ******************************************************************
		// Input: 	OFDM_Symbol_TX[0...4*NL-1] 	-> 4*NL (complex)
		//			 -->[Interpolation by 4]<--
		// Output: 	Interp_Buf[0...16*NL-1] 	-> 16*NL (complex)
		//				Note: 16*NL (complex) <= 9216 32bit words (= for N=256,L=32)
		//				In Memory Cmplx is followed by Fl_Array_R
		//				The tail of Fl_Array_R holds Filter Delay Line [4300..4351]
		//				Cmplx[]+Fl_Array_R[] comprise 4096+4300=8396  32bit words
		//				= 4198 cmplx samples
		//				!! Operation is split into two parts !!
		
		// Amplifying Signal by Interpolation Factor
		vsmul_f_DD((float *)&Cmplx_Array[0],8.0f,(float *)&Cmplx_Array[0],2*4*NL);  
   
		// Save Input into buffer
		vcopy_ED((float *)&OFDM_Symbol_TX[0],(float *)&Cmplx_Array[0],2*4*NL);
		 	
		// Blocks of size NL (complex) are interpolated by 4 /3
		// this is repeated four times (Step_o=0..3)
		for (Step_o=0; Step_o < 4; Step_o++)
			{ 
			// Restore block No "Step_o" of size NL (complex) into Cmplx_Array[0]
			vcopy_DE((float *)&Cmplx_Array[0],(float *)&OFDM_Symbol_TX[0+Step_o*NL],2*NL);
		
			// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int4x24_Filt_Coeff_V2[0],2*INTFILT_4_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int4x24_filt_c_Delay[0],2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
	
			// Interpolation by 4 - Output: Cmplx_Array[2*NL...6*NL-1] -> 4*NL (complex)
			Int4x24_filt_c(&Cmplx_Array[2*NL],Cmplx_Array, NL, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Int4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
	
			// 4*NL (complex) to process in next operation	
			
	     	// Decimation by 3: input: 4NL(complex) output 4/3NL=NL_SFC(complex), which is integer for
				//					allowed NL combinations. after 4 calls: 4*4/3NL=16/3NL=4*NL_SFC
			// real part
	    	vcopy_s_ED( (float *)&Interp_Buf[0+Step_o*NL_SFC], 2*1, (float *)&Cmplx_Array[2*NL], 2*3, NL_SFC );
			// imag part
	    	vcopy_s_ED( (((float *)&Interp_Buf[0+Step_o*NL_SFC])+1), 2*1, (((float *)&Cmplx_Array[2*NL])+1), 2*3, NL_SFC );
	    	
			}
			// Output: 	Interp_Buf[0...16/3*NL-1] = [0...4*NL_SFC-1]
			
		// x2 ******************************************************************
		// Input: 	Interp_Buf[0...4*NL_SFC-1] 	-> 4*NL_SFC (complex)
		//			 -->[Interpolation by 2]<--
		// Output: 	OFDM_Symbol_TX[0...8*NL_SFC-1] 	-> 8*NL_SFC = 32/3*NL (complex)
		//			
		//				Note: 3*NL_SFC <= 4608 32bit words (= for N=512,L=64)
		//				In Memory Cmplx is followed by Fl_Array_R
		//				The tail of Fl_Array_R holds Filter Delay Line [4300..4351]
		//				Cmplx[]+Fl_Array_R[] comprise 4096+4300=8396  32bit words
		//				= 4198 cmplx samples
		//				!! Fl_Array_R will be overwritten by this procedure !!
		
		// split into four times interpolation from NL_SFC -> 2*NL_SFC
		
		for (Step_o=0; Step_o < 4; Step_o++)
			{ 
			// Restore block No "Step_o" of size NL_SFC (complex) into Cmplx_Array[2*NL_SFC...3*NL_SFC-1]
			vcopy_DE((float *)&Cmplx_Array[2*NL_SFC],(float *)&Interp_Buf[0+Step_o*NL_SFC],2*NL_SFC);
		
			// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int2x12_Filt_Coeff_V1[0],2*INTFILT_2_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int2x12_filt_c_Delay[0],2*(INTFILT_2_LENGTH/INTFILT_2_POLYNUMBER+1));
	 		
			// Interpolation by 2
			Int2x12_filt_c(&Cmplx_Array[0],&Cmplx_Array[2*NL_SFC], NL_SFC, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);
	
			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Int2x12_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_2_LENGTH/INTFILT_2_POLYNUMBER+1));
		
			for (Step_i=0; Step_i < 2; Step_i++) //split 2*NL_SFC into 2*NL_SFC(complex)
	    		{
				Start_Phi_TX = fmodf(Start_Phi_New_TX,OFDM_TWO_PI);
	    		Start_Phi_New_TX = Exp_Gen(Start_Phi_TX,Rot_Fr_TX,&Cmplx_Array[2*NL_SFC],Delta_Phi_Symbol_TX,NL_SFC);
	    		vmul_c_DDD(&Cmplx_Array[0+Step_i*NL_SFC],&Cmplx_Array[0+Step_i*NL_SFC],&Cmplx_Array[2*NL_SFC],NL_SFC);
	    		}
	    		
			// Save result into OFDM_Symbol_TX [0..2*NL_SFC-1] [2*NL_SFC..4*NL_SFC-1] [4*NL_SFC..6*NL_SFC-1] [6*NL_SFC..8*NL_SFC-1]
			vcopy_ED((float *)&OFDM_Symbol_TX[0+Step_o*2*NL_SFC],(float *)&Cmplx_Array[0],2*2*NL_SFC); 
			}
		//Output: 	OFDM_Symbol_TX[0...8*NL_SFC-1] 	-> 8*NL_SFC = 32/3*NL (complex)
		}
		
		//==>Variant 4
		if (Filter_TX_Interpolation == 8)
		{
		//Variant 4:  x-FFT:8  x-SFC: x4/3
		//	!!! N<=128) allowed are 64/8, 128/16, used is 64/8 for FN=6kHz
		//	!!! blocksize N=128: 4*8NL = 32NL <= 32(128+16) <= 4608 (complex))
		//	!!!     			4/3*8NL = 1536(complex)
		//	!!! blocksize N= 64: 4*8NL = 32NL <= 32(64+8) <= 2304 (complex))
		//	!!!     			4/3*8NL = 768 (complex)	
		//
		// x4 ******************************************************************
		// Input: 	Cmplx_Array[0...8*NL-1] 	-> 8*NL (complex)
		//			 -->[Interpolation by 4]<--
		// Output: 	4*8NL = 32NL <= 32(64+8) <= 2304 (complex)
		//				In Memory Cmplx is followed by Fl_Array_R
		//				The tail of Fl_Array_R holds Filter Delay Line [4300..4351]
		//				Cmplx[]+Fl_Array_R[] comprise 4096+4300=8396  32bit words
		//				= 4198 cmplx samples
		//				!! Fl_Array_R will be overwritten by this procedure !!
		// Processing in several steps is mandatory for N=128
		
		// Blocksize for processing of exp_gen
		NL_SFC = 4* ((int)((float)NL/3.0f)); //  NL_SFC=4/3*NL this is what compiler likes to see	
		
		// Amplifying Signal by Interpolation Factor
		vsmul_f_DD((float *)&Cmplx_Array[0],4.0f,(float *)&Cmplx_Array[0],2*8*NL);  
   
		// Save Input into buffer
		vcopy_ED((float *)&Interp_Buf[0],(float *)&Cmplx_Array[0],2*8*NL);
		 	
		// Blocks of size NL (complex) are interpolated by 4 /3
		// this is repeated eight times (Step_o=0..7)
		for (Step_o=0; Step_o < 8; Step_o++)
			{ 
			// Restore block No "Step_o" of size NL (complex) into Cmplx_Array[0]
			vcopy_DE((float *)&Cmplx_Array[0],(float *)&Interp_Buf[0+Step_o*NL],2*NL);
		
			// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int4x24_Filt_Coeff_V2[0],2*INTFILT_4_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int4x24_filt_c_Delay[0],2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
	
			// Interpolation by 4 - Output: Cmplx_Array[2*NL...6*NL-1] -> 4*NL (complex)
			Int4x24_filt_c(&Cmplx_Array[2*NL],Cmplx_Array, NL, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Int4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
	
			// 4*NL (complex) to process in next operation	
			
	     // Decimation by 3: input: 4NL(complex) output 4/3NL=NL_SFC(complex), which is integer for
				//					allowed NL combinations. after 8 calls: 8*4/3NL=32/3NL=8*NL_SFC
			// real part
	    	vcopy_s_EE( (float *)&Cmplx_Array[0], 2*1, (float *)&Cmplx_Array[2*NL], 2*3, NL_SFC );
			// imag part
	    	vcopy_s_EE( (((float *)&Cmplx_Array[0])+1), 2*1, (((float *)&Cmplx_Array[2*NL])+1), 2*3, NL_SFC );
	    	// Output: 	Cmplx_Array[0...4/3*NL-1] = [0...NL_SFC-1]
			
			Start_Phi_TX = fmodf(Start_Phi_New_TX,OFDM_TWO_PI);
	    	Start_Phi_New_TX = Exp_Gen(Start_Phi_TX,Rot_Fr_TX,&Cmplx_Array[2*NL_SFC],Delta_Phi_Symbol_TX,NL_SFC);
	    	vmul_c_DDD(&Cmplx_Array[0],&Cmplx_Array[0],&Cmplx_Array[2*NL_SFC],NL_SFC);
	    
	    	// Save result into OFDM_Symbol_TX [0..NL_SFC-1] [NL_SFC..2*NL_SFC-1] [2*NL_SFC..3*NL_SFC-1].. [7*NL_SFC..8*NL_SFC-1]
			vcopy_ED((float *)&OFDM_Symbol_TX[0+Step_o*NL_SFC],(float *)&Cmplx_Array[0],2*NL_SFC); 
			}
		//Output: 	OFDM_Symbol_TX[0...8*NL_SFC-1] 	-> 8*NL_SFC = 32/3*NL (complex)
		}
		
		
		break;
		
		case 8000:
		// SFC: x4
		// x4 ******************************************************************
		// Input: 	Cmplx_Array[0...2*NL-1] 	-> 2*NL (complex)
		//			 -->[Interpolation by 4]<--
		// Output: 	Output: 	OFDM_Symbol_TX[0...8*NL-1] 	-> 8*NL (complex)
		//				Note: 8*NL (complex) <= 9216 32bit words (= for N=512,L=64)
	//				In Memory Cmplx is followed by Fl_Array_R
		//				The tail of Fl_Array_R holds Filter Delay Line [4300..4351]
		//				Cmplx[]+Fl_Array_R[] comprise 4096+4300=8396  32bit words
		//				= 4198 cmplx samples
		//				!! Operation is split into two parts !!
		
		// Amplifying Signal by Interpolation Factor
//    	vsmul_f_DD((float *)&Cmplx_Array[0],sqrtf(SFC_Interpolation),(float *)&Cmplx_Array[0],2*NL2);  
    	vsmul_f_DD((float *)&Cmplx_Array[0],SFC_Interpolation,(float *)&Cmplx_Array[0],2*NL2);  
    
		// Save Input into buffer
		vcopy_ED((float *)&Interp_Buf[0],(float *)&Cmplx_Array[0],2*2*NL); // check float adressing!
		
		// Blocks of size NL (complex) are interpolated by 4
		// this is repeated two times (Step_o=0..1)
		for (Step_o=0; Step_o < 2; Step_o++)
			{ 
			// Restore block No "Step_o" of size NL (complex) into Cmplx_Array[0]
			vcopy_DE((float *)&Cmplx_Array[0],(float *)&Interp_Buf[0+Step_o*NL],2*NL);
		
			// Restore Filter Coefficients from external Memory to PM 
			//vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int4x24_Filt_Coeff_V1[0],2*INTFILT_4_LENGTH);
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int4x24_Filt_Coeff_V3[0],2*INTFILT_4_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int4x24_filt_c_Delay[0],2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
	
			// Interpolation by 4 - Output: Cmplx_Array[2*NL...6*NL-1] -> 4*NL (complex)
			Int4x24_filt_c(&Cmplx_Array[2*NL],Cmplx_Array, NL, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Int4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
	
			// next line instead of inner loop "Step_i" only
			//vcopy_ED((float *)&OFDM_Symbol_TX[0+Step_o*4*NL],(float *)&Cmplx_Array[2*NL],2*4*NL);
	    		
			for (Step_i=0; Step_i < 2; Step_i++) //2*2NL(complex)
	    		{
	     		// Note: Delta_Phi_Symbol_TX is valid for blocksize 2*NL only !
				Start_Phi_TX = fmodf(Start_Phi_New_TX,OFDM_TWO_PI);
	    		Start_Phi_New_TX = Exp_Gen(Start_Phi_TX,Rot_Fr_TX,Cmplx_Array,Delta_Phi_Symbol_TX,2*NL);
	    		vmul_c_DDD(&Cmplx_Array[2*NL+Step_i*2*NL],&Cmplx_Array[2*NL+Step_i*2*NL],Cmplx_Array,2*NL);
				vcopy_ED((float *)&OFDM_Symbol_TX[0+Step_o*4*NL+Step_i*2*NL],(float *)&Cmplx_Array[2*NL+Step_i*2*NL],2*2*NL);
	    		}
	    	
			}
		break;
		
		case 9600:
		if (Filter_TX_Interpolation == 4)
		{
		//Variant 5:  x-FFT:4  x-SFC: x5/3
		//	!!! N<=256) allowed are 64/8, 128/16#7, 256/8#32 used is 128/16#7 for FN=9.6kHz
		//	!!! blocksize N=128: 5*4NL = 20NL <= 20(128+16) <= 2880 (complex))
		//	!!!     			5/3*4NL = 960(complex)
		//	!!! blocksize N=256: 5*4NL = 20NL <= 20(256+32) <= 5760 (complex))
		//	!!!     			5/3*4NL = 1920 (complex)	
		//
		// x5 ******************************************************************
		// Input: 	Cmplx_Array[0...4*NL-1] 	-> 4*NL (complex)
		//			 -->[Interpolation by 5]<--
		// Output: 	5*4NL = 20NL <= 20(128+16) <= 2880 (complex)
		//				In Memory Cmplx is followed by Fl_Array_R
		//				The tail of Fl_Array_R holds Filter Delay Line [4300..4351]
		//				Cmplx[]+Fl_Array_R[] comprise 4096+4300=8396  32bit words
		//				= 4198 cmplx samples
		//				!! Fl_Array_R will be overwritten by this procedure !!
		// Processing in several steps is mandatory for N=128
		
		// Blocksize for processing of exp_gen
		NL_SFC = 5* ((int)((float)NL/3.0f)); //  NL_SFC=5/3*NL this is what compiler likes to see	
		
		// Amplifying Signal by Interpolation Factor
		vsmul_f_DD((float *)&Cmplx_Array[0],5.0f,(float *)&Cmplx_Array[0],2*4*NL);  
   
		// Save Input into buffer
		vcopy_ED((float *)&Interp_Buf[0],(float *)&Cmplx_Array[0],2*4*NL);
		 	
		// Blocks of size NL (complex) are interpolated by 5 /3
		// this is repeated four times (Step_o=0..3)
		for (Step_o=0; Step_o < 4; Step_o++)
			{ 
			// Restore block No "Step_o" of size NL (complex) into Cmplx_Array[0]
			vcopy_DE((float *)&Cmplx_Array[0],(float *)&Interp_Buf[0+Step_o*NL],2*NL);
		
			// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int5x25_Filt_Coeff_V2[0],2*INTFILT_5_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int5x25_filt_c_Delay[0],2*(INTFILT_5_LENGTH/INTFILT_5_POLYNUMBER+1));
	
			// Interpolation by 5 - Output: Cmplx_Array[2*NL...7*NL-1] -> 5*NL (complex)
			Int5x25_filt_c(&Cmplx_Array[2*NL],Cmplx_Array, NL, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Int5x25_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_5_LENGTH/INTFILT_5_POLYNUMBER+1));
	
			// 5*NL (complex) to process in next operation	
			
	     // Decimation by 3: input: 5NL(complex) output 5/3NL=NL_SFC(complex), which is integer for
				//					allowed NL combinations. after 4 calls: 4*5/3NL=20/3NL=4*NL_SFC
			// real part
	    	vcopy_s_EE( (float *)&Cmplx_Array[0], 2*1, (float *)&Cmplx_Array[2*NL], 2*3, NL_SFC );
			// imag part
	    	vcopy_s_EE( (((float *)&Cmplx_Array[0])+1), 2*1, (((float *)&Cmplx_Array[2*NL])+1), 2*3, NL_SFC );
	    	// Output: 	Cmplx_Array[0...5/3*NL-1] = [0...NL_SFC-1]
			
			Start_Phi_TX = fmodf(Start_Phi_New_TX,OFDM_TWO_PI);
	    	Start_Phi_New_TX = Exp_Gen(Start_Phi_TX,Rot_Fr_TX,&Cmplx_Array[2*NL_SFC],Delta_Phi_Symbol_TX,NL_SFC);
	    	vmul_c_DDD(&Cmplx_Array[0],&Cmplx_Array[0],&Cmplx_Array[2*NL_SFC],NL_SFC);
	    
	    	// Save result into OFDM_Symbol_TX [0..NL_SFC-1] [NL_SFC..2*NL_SFC-1] [2*NL_SFC..3*NL_SFC-1] [3*NL_SFC..4*NL_SFC-1]
			vcopy_ED((float *)&OFDM_Symbol_TX[0+Step_o*NL_SFC],(float *)&Cmplx_Array[0],2*NL_SFC); 
			}
		//Output: 	OFDM_Symbol_TX[0...4*NL_SFC-1] 	-> 4*NL_SFC = 20/3*NL (complex)
		}
		
		break;
		
		case 12000:
		// SFC: x4/3x2
		
		NL_SFC = 4* ((int)((float)NL/3.0f)); //  NL_SFC=4/3*NL this is what compiler likes to see	
			//Rot_Fr_TX.re = cosf(Freq_Offset_TX);
			//Rot_Fr_TX.im = sinf(Freq_Offset_TX);
			//Delta_Phi_Symbol_TX = Freq_Offset_TX*(float)(NL_SFC);
			
		// x4 /3 ******************************************************************
		// Input: 	OFDM_Symbol_TX[0...2*NL-1] 	-> 2*NL (complex)
		//			 -->[Interpolation by 4]<--
		// Output: 	Interp_Buf[0...8*NL-1] 	-> 8*NL (complex)
		//				Note: 8*NL (complex) <= 9216 32bit words (= for N=512,L=64)
	//				In Memory Cmplx is followed by Fl_Array_R
		//				The tail of Fl_Array_R holds Filter Delay Line [4300..4351]
		//				Cmplx[]+Fl_Array_R[] comprise 4096+4300=8396  32bit words
		//				= 4198 cmplx samples
		//				!! Operation is split into two parts !!
		
		// Amplifying Signal by Interpolation Factor
		vsmul_f_DD((float *)&Cmplx_Array[0],8.0f,(float *)&Cmplx_Array[0],2*NL2);  
   
		// Save Input into buffer
		vcopy_ED((float *)&OFDM_Symbol_TX[0],(float *)&Cmplx_Array[0],2*2*NL);
		 	
		// Blocks of size NL (complex) are interpolated by 4 /3
		// this is repeated two times (Step_o=0..1)
		for (Step_o=0; Step_o < 2; Step_o++)
			{ 
			// Restore block No "Step_o" of size NL (complex) into Cmplx_Array[0]
			vcopy_DE((float *)&Cmplx_Array[0],(float *)&OFDM_Symbol_TX[0+Step_o*NL],2*NL);
		
			// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int4x24_Filt_Coeff_V1[0],2*INTFILT_4_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int4x24_filt_c_Delay[0],2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
	
			// Interpolation by 4 - Output: Cmplx_Array[2*NL...6*NL-1] -> 4*NL (complex)
			Int4x24_filt_c(&Cmplx_Array[2*NL],Cmplx_Array, NL, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Int4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
	
			// 4*NL (complex) to process in next operation	
			
	     	// Decimation by 3: input: 4NL(complex) output 4/3NL=NL_SFC(complex), which is integer for
				//					allowed NL combinations. 2calls: 2*4/3NL=8/3NL=2*NL_SFC
			// real part
	    	vcopy_s_ED( (float *)&Interp_Buf[0+Step_o*NL_SFC], 2*1, (float *)&Cmplx_Array[2*NL], 2*3, NL_SFC );
			// imag part
	    	vcopy_s_ED( (((float *)&Interp_Buf[0+Step_o*NL_SFC])+1), 2*1, (((float *)&Cmplx_Array[2*NL])+1), 2*3, NL_SFC );
	    	
			}
			// Output: 	Interp_Buf[0...8/3*NL-1] = [0...2*NL_SFC-1]
			
		// x2 ******************************************************************
		// Input: 	Interp_Buf[0...2*NL_SFC-1] 	-> 2*NL_SFC (complex)
		//			 -->[Interpolation by 2]<--
		// Output: 	OFDM_Symbol_TX[0...4*NL_SFC-1] 	-> 4*NL_SFC = 16/3*NL (complex)
		//			
		//				Note: 3*NL_SFC <= 4608 32bit words (= for N=512,L=64)
		//				In Memory Cmplx is followed by Fl_Array_R
		//				The tail of Fl_Array_R holds Filter Delay Line [4300..4351]
		//				Cmplx[]+Fl_Array_R[] comprise 4096+4300=8396  32bit words
		//				= 4198 cmplx samples
		//				!! Fl_Array_R will be overwritten by this procedure !!
		
		// split into two times NL_SFC->2*NL_SFC
		
		for (Step_o=0; Step_o < 2; Step_o++)
			{ 
			// Restore block No "Step_o" of size NL_SFC (complex) into Cmplx_Array[2*NL_SFC...3*NL_SFC-1]
			vcopy_DE((float *)&Cmplx_Array[2*NL_SFC],(float *)&Interp_Buf[0+Step_o*NL_SFC],2*NL_SFC);
		
			// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int2x12_Filt_Coeff_V1[0],2*INTFILT_2_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int2x12_filt_c_Delay[0],2*(INTFILT_2_LENGTH/INTFILT_2_POLYNUMBER+1));
	 		
			// Interpolation by 2
			Int2x12_filt_c(&Cmplx_Array[0],&Cmplx_Array[2*NL_SFC], NL_SFC, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);
	
			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Int2x12_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_2_LENGTH/INTFILT_2_POLYNUMBER+1));
		
			for (Step_i=0; Step_i < 2; Step_i++) //split 2*NL_SFC into 2*NL_SFC(complex)
	    		{
				Start_Phi_TX = fmodf(Start_Phi_New_TX,OFDM_TWO_PI);
	    		Start_Phi_New_TX = Exp_Gen(Start_Phi_TX,Rot_Fr_TX,&Cmplx_Array[2*NL_SFC],Delta_Phi_Symbol_TX,NL_SFC);
	    		vmul_c_DDD(&Cmplx_Array[0+Step_i*NL_SFC],&Cmplx_Array[0+Step_i*NL_SFC],&Cmplx_Array[2*NL_SFC],NL_SFC);
	    		}
	    		
			// Save result into OFDM_Symbol_TX [0..2*NL_SFC-1] [2*NL_SFC..4*NL_SFC-1]
			vcopy_ED((float *)&OFDM_Symbol_TX[0+Step_o*2*NL_SFC],(float *)&Cmplx_Array[0],2*2*NL_SFC); 
			}
		//Output: 	OFDM_Symbol_TX[0...4*NL_SFC-1] 	-> 4*NL_SFC = 16/3*NL (complex)
		break;
		
		
		
		case 16000:
		// SFC: x2
		// x2 ******************************************************************
		// Input: 	Cmplx_Array[0...2*NL-1] 	-> 2*NL (complex)
		//			 -->[Interpolation by 2]<--
		// Output: 	Cmplx_Array[2*NL...6*NL-1] 	-> 4*NL (complex)
		//				Note: 6*NL <= 6912 32bit words (= for N=512,L=64)
	//				In Memory Cmplx is followed by Fl_Array_R
		//				The tail of Fl_Array_R holds Filter Delay Line [4300..4351]
		//				Cmplx[]+Fl_Array_R[] comprise 4096+4300=8396  32bit words
		//				= 4198 cmplx samples
		//				!! Fl_Array_R will be overwritten by this procedure !!
		
		// Restore Filter Coefficients from external Memory to PM 
		vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int2x12_Filt_Coeff_V3[0],2*INTFILT_2_LENGTH);
		// Restore Delay Array from external Memory to DM
		vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int2x12_filt_c_Delay[0],2*(INTFILT_2_LENGTH/INTFILT_2_POLYNUMBER+1));
		
		// Amplifying Signal by Interpolation Factor
    	vsmul_f_DD((float *)&Cmplx_Array[0],SFC_Interpolation,(float *)&Cmplx_Array[0],2*NL2);  
    		
		// Interpolation by 2
		Int2x12_filt_c(&Cmplx_Array[2*NL],Cmplx_Array, 2*NL, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

		// Save Delay Array into external memory buffer
		vcopy_ED((float *)&Int2x12_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_2_LENGTH/INTFILT_2_POLYNUMBER+1));
	
		// Save result into buffer
//		vcopy_ED((float *)&Interp_Buf[0],(float *)&Cmplx_Array[2*NL],2*4*NL); // check float adressing!
// not used (PL) ???		
		// next line instead of inner loop "Step_i" only
		//vcopy_ED((float *)&OFDM_Symbol_TX[0],(float *)&Cmplx_Array[2*NL],2*4*NL);
	    	
		for (Step_i=0; Step_i < 2; Step_i++)
	    		{
	     		// Note: Delta_Phi_Symbol_TX is valid for blocksize 2*NL only !
	    		Start_Phi_TX = fmodf(Start_Phi_New_TX,OFDM_TWO_PI);
	    		Start_Phi_New_TX = Exp_Gen(Start_Phi_TX,Rot_Fr_TX,Cmplx_Array,Delta_Phi_Symbol_TX,2*NL);
	    		vmul_c_DDD(&Cmplx_Array[2*NL+Step_i*2*NL],&Cmplx_Array[2*NL+Step_i*2*NL],Cmplx_Array,2*NL);
				vcopy_ED((float *)&OFDM_Symbol_TX[0+Step_i*2*NL],(float *)&Cmplx_Array[2*NL+Step_i*2*NL],2*2*NL);
	    		}
	    		
		break;
		
		
		
		case 24000:
		// SFC: x4/3
		// x4 ******************************************************************
		// Input: 	Cmplx_Array[0...2*NL-1] 	-> 2*NL (complex)
		//			 -->[Interpolation by 4]<--
		// Output: 	Output: 	OFDM_Symbol_TX[0...8*NL-1] 	-> 8*NL (complex)
		//				Note: 8*NL (complex) <= 9216 32bit words (= for N=512,L=64)
	//				In Memory Cmplx is followed by Fl_Array_R
		//				The tail of Fl_Array_R holds Filter Delay Line [4300..4351]
		//				Cmplx[]+Fl_Array_R[] comprise 4096+4300=8396  32bit words
		//				= 4198 cmplx samples
		//				!! Operation is split into two parts !!
		
		NL_SFC = 4* ((int)((float)NL/3.0f)); //  NL_SFC=4/3*NL this is what compiler likes to see
			//Rot_Fr_TX.re = cosf(Freq_Offset_TX);
			//Rot_Fr_TX.im = sinf(Freq_Offset_TX);
			//Delta_Phi_Symbol_TX = Freq_Offset_TX*(float)(NL_SFC);
		// Amplifying Signal by Interpolation Factor
//    	vsmul_f_DD((float *)&Cmplx_Array[0],sqrtf(SFC_Interpolation),(float *)&Cmplx_Array[0],2*NL2);  
    	vsmul_f_DD((float *)&Cmplx_Array[0],SFC_Interpolation*3.0f,(float *)&Cmplx_Array[0],2*NL2);  
    
		// Save Input into buffer
		vcopy_ED((float *)&Interp_Buf[0],(float *)&Cmplx_Array[0],2*2*NL); // check float adressing!
		
		
		// Blocks of size NL (complex) are interpolated by 4
		// this is repeated two times (Step_o=0..1)
		for (Step_o=0; Step_o < 2; Step_o++)
			{ 
			// Restore block No "Step_o" of size NL (complex) into Cmplx_Array[0]
			vcopy_DE((float *)&Cmplx_Array[0],(float *)&Interp_Buf[0+Step_o*NL],2*NL);
		
			// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int4x24_Filt_Coeff_V3[0],2*INTFILT_4_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int4x24_filt_c_Delay[0],2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
	
			// Interpolation by 4 - Output: Cmplx_Array[2*NL...6*NL-1] -> 4*NL (complex)
			Int4x24_filt_c(&Cmplx_Array[2*NL],Cmplx_Array, NL, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Int4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
	
			// Decimation by 3: real part 		
			vcopy_s_ED( (float *)&OFDM_Symbol_TX[0+Step_o*NL_SFC], 2*1, (float *)&Cmplx_Array[2*NL], 2*3, NL_SFC );
			// Decimation by 3: imag part 		
			vcopy_s_ED( (((float *)&OFDM_Symbol_TX[0+Step_o*NL_SFC])+1), 2*1, (((float *)&Cmplx_Array[2*NL])+1), 2*3, NL_SFC );
	    		
	    		// output: OFDM_Symbol_TX[0..NL_SFC-1](Step_o=0);[NL_SFC...2NL_SFC-1](Step_o=1)	
	    		// Restore block No "Step_o" of size NL_SFC (complex) into Cmplx_Array[0]
			vcopy_DE((float *)&Cmplx_Array[0],(float *)&OFDM_Symbol_TX[0+Step_o*NL_SFC],2*NL_SFC);
		
	    		Start_Phi_TX = fmodf(Start_Phi_New_TX,OFDM_TWO_PI);
	    		Start_Phi_New_TX = Exp_Gen(Start_Phi_TX,Rot_Fr_TX,&Cmplx_Array[NL_SFC],Delta_Phi_Symbol_TX,NL_SFC);
	    		vmul_c_DDD(Cmplx_Array,Cmplx_Array,&Cmplx_Array[NL_SFC],NL_SFC);
			
			vcopy_ED((float *)&OFDM_Symbol_TX[0+Step_o*NL_SFC],(float *)&Cmplx_Array[0],2*NL_SFC);

			}
		

		break;
		
		case 32000:
		
		// SFC: x1 (no SFC)
		// Frequency offset compensation and OFDM received symbol extraction                     
    	Start_Phi_TX = fmodf(Start_Phi_New_TX,OFDM_TWO_PI);
    	Start_Phi_New_TX = Exp_Gen(Start_Phi_TX,Rot_Fr_TX,Compl_Exp,Delta_Phi_Symbol_TX,2*NL);
    	vmul_c_DDD(Cmplx_Array,Cmplx_Array,Compl_Exp,2*NL);

		vcopy_ED((float *)&OFDM_Symbol_TX[0],(float *)&Cmplx_Array[0],2*2*NL);
		
		
		
		
		
		break;
		
		

		
		} // end of switch
	


	
	}

