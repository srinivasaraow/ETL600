/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_SFC_RX.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : RX sampling frequency conversion
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/OFDM_SFC_RX.c $
 * 
 * 10    24.07.08 15:32 Chhamaa
 * 
 * 5     5.04.06 15:28 Chhamaa
 * 
 * 5     17.03.06 14:41 Chhamaa
 * 
 * 3     16.09.05 9:41 Chalgyg
 * OFDM_DSP_V0_10 (1160905)
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 6     22.12.04 9:30 Chchlee
 * 
 * 5     2.12.04 17:54 Chhamaa
 * 
 * 4     28.09.04 17:45 Maag01
 * 
 * Initial version of FN 32000
 *       15.02.2003 DLK
 * implementation for FN 4000,8000,12000,16000,24000
 *	   10.06.2004 PL
 * new implementation for FN 12000,24000
 *		07.07.2004 PL
 * BW16kHz test: decimation without filtering and spectrum analysis
 *		28.10.2004 PL
 *      
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


#define FREQ_OFFSET_KOMP
//#undef FREQ_OFFSET_KOMP
//#define FREQ_KOMP_MASTER_ONLY
#undef FREQ_KOMP_MASTER_ONLY

//========================================
void segment ( "seg_ext_pmco" ) OFDM_SFC_RX()
	{
		
		void Test_FIR(); // PL Test
		
		float Exp_Gen(float, complex_float, complex_float *, float, int);
		
		int Step_o; // outer step
		int Step_i; // inner step
		int NL_SFC; // for 3/4 and /2*3/4 only, to define blocklength
		float tmp_rot; //for rotation coefficient


	// **************************************************************************
	// * 		Sampling Frequency conversion from 64kHz to Fn (complex)		*
	// *	No:			Bandwidth	Fn		/-SFC		/-FFT      
	// *	1			 4000 HD	 4000	/4/2		/2
	// *	2			 4000 HD     4000   /2          /8 (for N=64,128 only)
	// *	3			 4000 LD	 6000	/2*3/4		/4
	// *	4			 4000 LD     6000   *3/4        /8 (for N=64,128 only)
	// *			     4000 LD	 4800   *3/5		/8 (for N=64,128 only)
	
	// *				 8000		 8000	/4			/2
	// *				 8000 LD(a)	12000	/2*3/4		/2
	// *				 8000 LD(b)	 9600	*3/5		/4 (for N<=256 only)

	// *			   	12000		12000	*3/2/4		/2
	// * 			   	16000		16000	/2			/2
	// *				20000		24000	*3/4		/2
	// *				24000		24000	*3/4		/2
	// * 				28000		32000	/1			/2
	// *				32000		32000	/1			/2
	// *					*
	// **************************************************************************
	
	// **************************************************************************
	// * 		Spectral Shift by Rot_Fr_TX										*
	// *			Note:  Rot_Fr.re = cosf(Freq_Offset/2.0f+Freq_Offset_TX);	*
	// *				   Rot_Fr.im = -sinf(Freq_Offset/2.0f+Freq_Offset_TX);	*
	// *						Freq_Offset_TX = Df_Hz_TX*OFDM_TWO_PI/FS_TX_RX;	*
	// *						Freq_Offset is measured in "Fr_Tracking.c"		*
	// *									and in "OFDM_Freq_Estim_RX.c"		*
	// *						with Df_Hz_TX in Hz and FS_TX_RX Sampling Freq.	*
	// *				   Delta_Phi_Symbol is phase increment					*
	// *									after one block 					*
	// **************************************************************************
	tmp_rot= Freq_Offset_RX;
	
	/*
	#ifdef FREQ_OFFSET_KOMP
		if (SFC_Freq_Comp == ON) {
			// Rotation coefficient for complex exponent to frequency offset compensation; 
			#ifdef FREQ_KOMP_MASTER_ONLY
			if (OFDM_Modem_Type==OFDM_MASTER)
			#endif	
				if (Freq_Offset_Control_Flag == 1) tmp_rot += Freq_Offset/SFC_Interpolation/((float)Filter_RX_Decimation);
			}
	#endif
	*/

	Rot_Fr.re = cosf(tmp_rot);
	Rot_Fr.im = -sinf(tmp_rot);
		
	switch (FN)
		{
		case 2000:
//==>Variant 1
		if (Filter_RX_Decimation == 8)
		{
		//Variant 1:  x-SFC: /4  x-FFT:/8  
		// Input OFDM_Symbol_RX[0..32NL-1]
		
		// /4:
		// Phase increment for one OFDM symbol
		Delta_Phi_Symbol = -(tmp_rot*2.0f)*(float)NL;
	
		for (Step_o=0; Step_o < 8; Step_o++)   //8*4NL
			{         
			for (Step_i=0; Step_i < 2; Step_i++)
	    		{
				// Frequency offset compensation and OFDM received symbol extraction                     
	  			// Save input block of size 2NL complex in Cmplx_Array [2NL..4NL-1] (step 0) and [4NL..6NL-1] (step1)	
   	  			vcopy_DE((float *)&Cmplx_Array[2*NL+Step_i*2*NL],(float *)&OFDM_Symbol_RX[0+Step_o*4*NL+Step_i*2*NL],2*2*NL);                
   	  			// Frequency offset compensation (in place)
   	  			Start_Phi = fmodf(Start_Phi_New,OFDM_TWO_PI);
      			Start_Phi_New = Exp_Gen(Start_Phi,Rot_Fr,Cmplx_Array,Delta_Phi_Symbol,2*NL);
      			vmul_c_DDD(&Cmplx_Array[2*NL+Step_i*2*NL],&Cmplx_Array[2*NL+Step_i*2*NL],Cmplx_Array,2*NL);
	    		}
	    		// Output is Cmplx_Array[2NL..6NL-1]
	
	    	// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Dec4x24_Filt_Coeff_V4[0],2*DECFILT_4_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Dec4x24_filt_c_Delay[0],2*(DECFILT_4_LENGTH+1));
	
			// Decimation by 4 (result in Cmplx_Array[0...NL-1]
			Dec4x24_filt_c(&Cmplx_Array[0],&Cmplx_Array[2*NL], 4*NL, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Dec4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(DECFILT_4_LENGTH+1));
	    		
	    	// Save output signal to OFDM_Symbol_RX	[0..NL-1],[NL..2NL-1],[2NL..3NL-1],[3NL..4NL-1][4NL..5NL-1],[5NL..6NL-1],[6NL..7NL-1],[7NL..8NL-1]
	    	// !! be aware that this is inplace operation for OFDM_Symbol_RX
			vcopy_ED((float *)&OFDM_Symbol_RX[0+Step_o*NL],(float *)&Cmplx_Array[0],2*NL);
			}
		//				Output OFDM_Symbol_RX[0..8NL-1]
		}// end of variant 1
		
		
		break;
			
		case 4000:
		
		//==>Variant 1
		if (Filter_RX_Decimation == 2)
		{
		//Variant 1:  x-SFC: /4/2  x-FFT:/2  
		// Input OFDM_Symbol_RX[0..16NL-1]
		
		// /4:
		// Phase increment for one OFDM symbol
		Delta_Phi_Symbol = -(tmp_rot*2.0f)*(float)NL;
	
		for (Step_o=0; Step_o < 4; Step_o++)   //4*4NL
			{         
			for (Step_i=0; Step_i < 2; Step_i++)
	    		{
				// Frequency offset compensation and OFDM received symbol extraction                     
	  			// Save input block of size 2NL complex in Cmplx_Array [2NL..4NL-1] (step 0) and [4NL..6NL-1] (step1)	
   	  			vcopy_DE((float *)&Cmplx_Array[2*NL+Step_i*2*NL],(float *)&OFDM_Symbol_RX[0+Step_o*4*NL+Step_i*2*NL],2*2*NL);                
   	  			// Frequency offset compensation (in place)
   	  			Start_Phi = fmodf(Start_Phi_New,OFDM_TWO_PI);
      			Start_Phi_New = Exp_Gen(Start_Phi,Rot_Fr,Cmplx_Array,Delta_Phi_Symbol,2*NL);
      			vmul_c_DDD(&Cmplx_Array[2*NL+Step_i*2*NL],&Cmplx_Array[2*NL+Step_i*2*NL],Cmplx_Array,2*NL);
	    		}
	    		// Output is Cmplx_Array[2NL..6NL-1]
	
	    	// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Dec4x24_Filt_Coeff[0],2*DECFILT_4_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Dec4x24_filt_c_Delay[0],2*(DECFILT_4_LENGTH+1));
	
			// Decimation by 4 (result in Cmplx_Array[0...NL-1]
			Dec4x24_filt_c(&Cmplx_Array[0],&Cmplx_Array[2*NL], 4*NL, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Dec4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(DECFILT_4_LENGTH+1));
	    		
	    	// Save output signal to OFDM_Symbol_RX	[0..NL-1],[NL..2NL-1],[2NL..3NL-1],[3NL..4NL-1]
	    	// !! be aware that this is inplace operation for OFDM_Symbol_RX
			vcopy_ED((float *)&OFDM_Symbol_RX[0+Step_o*NL],(float *)&Cmplx_Array[0],2*NL);
			}
		//				Output OFDM_Symbol_RX[0..4NL-1]
		// /2:
		// Restore OFDM_symbol into DM Cmplx_Array[2NL..6NL-1]
		vcopy_DE((float *)&Cmplx_Array[2*NL],(float *)&OFDM_Symbol_RX[0],2*4*NL);                
   	  			
		// Restore Filter Coefficients from external Memory to PM 
		vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Dec2x12_Filt_Coeff_V1[0],2*DECFILT_2_LENGTH);
		// Restore Delay Array from external Memory to DM
		vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Dec2x12_filt_c_Delay[0],2*(DECFILT_2_LENGTH+1));
	
		// Decimation by 2
		Dec2x12_filt_c(&Cmplx_Array[0],&Cmplx_Array[2*NL], 4*NL, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

		// Save Delay Array into external memory buffer
		vcopy_ED((float *)&Dec2x12_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(DECFILT_2_LENGTH+1));
	    		
	    // Save output signal to OFDM_Symbol_RX	
		vcopy_ED((float *)&OFDM_Symbol_RX[0],(float *)&Cmplx_Array[0],2*2*NL);
		//				Output OFDM_Symbol_RX[0..2NL-1]   
		}// end of variant 1
		
		//==>Variant 2
		//------------
		if (Filter_RX_Decimation == 8)
		{		
		//Variant 2:  x-FFT:/8  x-SFC: /2
		//	!!! N<=128) (max input block length:16*NL=16*(128+32)=2560cmplx=5120words)
		// Input OFDM_Symbol_RX[0..16NL-1]
		// /2:
		// Phase increment for one OFDM symbol
		Delta_Phi_Symbol = -(tmp_rot*2.0f)*(float)NL;
	
		for (Step_o=0; Step_o < 4; Step_o++)   //4*4NL
			{         
			for (Step_i=0; Step_i < 2; Step_i++)
	    		{
				// Frequency offset compensation and OFDM received symbol extraction                     
	  			// Save input block of size 2NL complex in Cmplx_Array [2NL..4NL-1] (step 0) and [4NL..6NL-1] (step1)	
   	  			vcopy_DE((float *)&Cmplx_Array[2*NL+Step_i*2*NL],(float *)&OFDM_Symbol_RX[0+Step_o*4*NL+Step_i*2*NL],2*2*NL);                
   	  			// Frequency offset compensation (in place)
   	  			Start_Phi = fmodf(Start_Phi_New,OFDM_TWO_PI);
      			Start_Phi_New = Exp_Gen(Start_Phi,Rot_Fr,Cmplx_Array,Delta_Phi_Symbol,2*NL);
      			vmul_c_DDD(&Cmplx_Array[2*NL+Step_i*2*NL],&Cmplx_Array[2*NL+Step_i*2*NL],Cmplx_Array,2*NL);
	    		}
	    		// Output is Cmplx_Array[2NL..6NL-1]
	
	    	// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Dec2x12_Filt_Coeff_V2[0],2*DECFILT_2_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Dec2x12_filt_c_Delay[0],2*(DECFILT_2_LENGTH+1));
	
			// Decimation by 2
			Dec2x12_filt_c(&Cmplx_Array[0],&Cmplx_Array[2*NL], 4*NL, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Dec2x12_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(DECFILT_2_LENGTH+1));
	    		
	    	// Save output signal to OFDM_Symbol_RX	
			vcopy_ED((float *)&OFDM_Symbol_RX[0+Step_o*2*NL],(float *)&Cmplx_Array[0],2*2*NL);
			//				Output OFDM_Symbol_RX[0..2NL-1][2NL..4NL-1][4NL..6NL-1][6NL..8NL-1] 
	    	}
		//				Output OFDM_Symbol_RX[0..8NL-1]
		
		}// end of variant 2
		
		break;
		
	case 4800:
		if (Filter_RX_Decimation == 8)
		{		
		//Variant 4:  x-SFC: *3/5 x-FFT:/8
		//	!!! N<=128) (max input block length:8*5/3*NL=40/3*(128+16)=1920cmplx=3840words)
		// SFC: *3/5   Input OFDM_Symbol_RX[0..40/3*NL-1] = OFDM_Symbol_RX[0..8*NL_SFC-1]

		NL_SFC = 5* ((int)((float)NL/3.0f)); //  NL_SFC=5/3*NL this is what compiler likes to see
		
		// Phase increment for one block of length NL_SFC
		Delta_Phi_Symbol = -(tmp_rot)*(float)NL_SFC;
		
		// *3   		Input:OFDM_Symbol_RX[0..8*NL_SFC-1]
		for (Step_o=0; Step_o < 8; Step_o++)
			{
			// copy OFDM_Symbol_RX to internal Cmplx_Array[5NL..5NL+NL_SFC-1] , NL_SFC complex values
		   	vcopy_DE((float *)&Cmplx_Array[5*NL],(float *)&OFDM_Symbol_RX[0+Step_o*NL_SFC],2*NL_SFC);                
			
		   	// Frequency offset compensation (in place)
   	  			Start_Phi = fmodf(Start_Phi_New,OFDM_TWO_PI);
      			Start_Phi_New = Exp_Gen(Start_Phi,Rot_Fr,Cmplx_Array,Delta_Phi_Symbol,NL_SFC);
      			vmul_c_DDD(&Cmplx_Array[5*NL],&Cmplx_Array[5*NL],Cmplx_Array,NL_SFC);
	    				   	
		   	// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int3x24_Filt_Coeff_V3[0],2*INTFILT_3_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int3x24_filt_c_Delay[0],2*(INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1));
	
			//Amplifying Signal by Interpolation Factor
    		vsmul_f_DD((float *)&Cmplx_Array[5*NL],3.0f,(float *)&Cmplx_Array[5*NL],2*NL_SFC);  
    		
			// Interpolation by 3
			Int3x24_filt_c(&Cmplx_Array[0],&Cmplx_Array[5*NL], NL_SFC, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Int3x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1));
	
			// Decimation by 5 (result in OFDM_Symbol_RX	[0..NL-1] ,[NL..2NL-1],...,[7NL..8NL-1])
			// real part
	    	vcopy_s_ED( (float *)&OFDM_Symbol_RX[0+Step_o*NL], 2*1, (float *)&Cmplx_Array[0], 2*5, NL );
			// imag part
	    	vcopy_s_ED( (((float *)&OFDM_Symbol_RX[0+Step_o*NL])+1), 2*1, (((float *)&Cmplx_Array[0])+1), 2*5, NL );
	    		
			}
			//				Output OFDM_Symbol_RX[0..8NL-1]      
		
		}

		break;
		
	case 6000:
		//==>Variant 2
		//------------
		if (Filter_RX_Decimation == 2) // used for N=512
		{		
		//Variant 2:  x-SFC: /4*3/4 x-FFT:/2
		// max input block length:8*4/3*NL=32/3*(512+64)=6144cmplx=12288words)
		// SFC: /4*3/4   Input OFDM_Symbol_RX[0..32/3*NL-1] = OFDM_Symbol_RX[0..8*NL_SFC-1]

		NL_SFC = 4* ((int)((float)NL/3.0f)); //  NL_SFC=4/3*NL this is what compiler likes to see
					
		// Phase increment for one block of length NL_SFC
		Delta_Phi_Symbol = -(tmp_rot)*(float)NL_SFC;
        	
		   
		// /4  two times 4*NL_SFC Input:  OFDM_Symbol_RX[0..4*NL_SFC-1] OFDM_Symbol_RX[4*NL_SFC...8*NL_SFC-1]
		for (Step_o=0; Step_o < 2; Step_o++)
			{
			// copy OFDM_Symbol_RX part to internal Cmplx_Array[NL_SFC...5*NL_SFC-1] , 4*NL_SFC complex values
		   	vcopy_DE((float *)&Cmplx_Array[NL_SFC],(float *)&OFDM_Symbol_RX[0+Step_o*4*NL_SFC],2*4*NL_SFC);                
			
		   	for (Step_i=0; Step_i < 4; Step_i++)
	    		{
		   	// Frequency offset compensation (in place)
   	  			Start_Phi = fmodf(Start_Phi_New,OFDM_TWO_PI);
      			Start_Phi_New = Exp_Gen(Start_Phi,Rot_Fr,Cmplx_Array,Delta_Phi_Symbol,NL_SFC);
      			vmul_c_DDD(&Cmplx_Array[NL_SFC+Step_i*NL_SFC],&Cmplx_Array[NL_SFC+Step_i*NL_SFC],Cmplx_Array,NL_SFC);
	    		}
		   	
			// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Dec4x24_Filt_Coeff_V3[0],2*DECFILT_4_LENGTH);

			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Dec4x24_filt_c_Delay[0],2*(DECFILT_4_LENGTH+1));

			// Decimation by 4
			Dec4x24_filt_c(&Cmplx_Array[0],&Cmplx_Array[NL_SFC], 4*NL_SFC, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Dec4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(DECFILT_4_LENGTH+1));
	    		
	    	// Save output signal to OFDM_Symbol_RX [0..NL_SFC-1] [NL_SFC..2*NL_SFC-1] 
			vcopy_ED((float *)&OFDM_Symbol_RX[0+Step_o*NL_SFC],(float *)&Cmplx_Array[0],2*NL_SFC);			
			}
			//	Output:OFDM_Symbol_RX[0..2*NL_SFC-1]
		
		// two times NL_SFC complex
		// *3   		Input:OFDM_Symbol_RX[0..2*NL_SFC-1]
		for (Step_o=0; Step_o < 2; Step_o++)
			{
			// copy OFDM_Symbol_RX to internal Cmplx_Array[4NL..4NL+NL_SFC-1] , NL_SFC complex values
		   	vcopy_DE((float *)&Cmplx_Array[4*NL],(float *)&OFDM_Symbol_RX[0+Step_o*NL_SFC],2*NL_SFC);                
			
		   	// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int3x24_Filt_Coeff_V1[0],2*INTFILT_3_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int3x24_filt_c_Delay[0],2*(INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1));
	
			//Amplifying Signal by Interpolation Factor
    		vsmul_f_DD((float *)&Cmplx_Array[4*NL],3.0f,(float *)&Cmplx_Array[4*NL],2*NL_SFC);  
    		
			// Interpolation by 3
			Int3x24_filt_c(&Cmplx_Array[0],&Cmplx_Array[4*NL], NL_SFC, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Int3x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1));
	
			// Decimation by 4 (result in OFDM_Symbol_RX	[0..NL-1] and [NL..2NL-1])
			// real part
	    	vcopy_s_ED( (float *)&OFDM_Symbol_RX[0+Step_o*NL], 2*1, (float *)&Cmplx_Array[0], 2*4, NL );
			// imag part
	    	vcopy_s_ED( (((float *)&OFDM_Symbol_RX[0+Step_o*NL])+1), 2*1, (((float *)&Cmplx_Array[0])+1), 2*4, NL );
	    		
			}
			//				Output OFDM_Symbol_RX[0..2NL-1]       
		}
		//==>Variant 3
		//------------
		if (Filter_RX_Decimation == 4) //used for N=256
		{		
		//Variant 3:  x-SFC: /2*3/4 x-FFT:/4
		//	!!! N<=128) (max input block length:8*4/3*NL=32/3*(128+16)=1536cmplx=3072words)
		// SFC: /2*3/4   Input OFDM_Symbol_RX[0..32/3*NL-1] = OFDM_Symbol_RX[0..8*NL_SFC-1]

		NL_SFC = 4* ((int)((float)NL/3.0f)); //  NL_SFC=4/3*NL this is what compiler likes to see
					
		// Phase increment for one block of length NL_SFC
		Delta_Phi_Symbol = -(tmp_rot)*(float)NL_SFC;
        	
		   
		// /2  four times 2*NL_SFC Input:  OFDM_Symbol_RX[0..2*NL_SFC-1] OFDM_Symbol_RX[2*NL_SFC...4*NL_SFC-1]
		//									OFDM_Symbol_RX[4*NL_SFC..6*NL_SFC-1] OFDM_Symbol_RX[6*NL_SFC...8*NL_SFC-1]
		for (Step_o=0; Step_o < 4; Step_o++)
			{
			// copy OFDM_Symbol_RX part to internal Cmplx_Array[NL_SFC...3*NL_SFC-1] , 2*NL_SFC complex values
		   	vcopy_DE((float *)&Cmplx_Array[NL_SFC],(float *)&OFDM_Symbol_RX[0+Step_o*2*NL_SFC],2*2*NL_SFC);                
			
		   	for (Step_i=0; Step_i < 2; Step_i++)
	    		{
		   	// Frequency offset compensation (in place)
   	  			Start_Phi = fmodf(Start_Phi_New,OFDM_TWO_PI);
      			Start_Phi_New = Exp_Gen(Start_Phi,Rot_Fr,Cmplx_Array,Delta_Phi_Symbol,NL_SFC);
      			vmul_c_DDD(&Cmplx_Array[NL_SFC+Step_i*NL_SFC],&Cmplx_Array[NL_SFC+Step_i*NL_SFC],Cmplx_Array,NL_SFC);
	    		}
		   	
			// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Dec2x12_Filt_Coeff_V1[0],2*DECFILT_2_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Dec2x12_filt_c_Delay[0],2*(DECFILT_2_LENGTH+1));
	
			// Decimation by 2
			Dec2x12_filt_c(&Cmplx_Array[0],&Cmplx_Array[NL_SFC], 2*NL_SFC, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Dec2x12_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(DECFILT_2_LENGTH+1));
	    		
	    	// Save output signal to OFDM_Symbol_RX [0..NL_SFC-1] [NL_SFC..2*NL_SFC-1] [2*NL_SFC..3*NL_SFC-1] [3*NL_SFC..4*NL_SFC-1]
			vcopy_ED((float *)&OFDM_Symbol_RX[0+Step_o*NL_SFC],(float *)&Cmplx_Array[0],2*NL_SFC);			
			}
			//	Output:OFDM_Symbol_RX[0..4*NL_SFC-1]
		
		// four times NL_SFC complex
		// *3   		Input:OFDM_Symbol_RX[0..4*NL_SFC-1]
		for (Step_o=0; Step_o < 4; Step_o++)
			{
			// copy OFDM_Symbol_RX to internal Cmplx_Array[4NL..4NL+NL_SFC-1] , NL_SFC complex values
		   	vcopy_DE((float *)&Cmplx_Array[4*NL],(float *)&OFDM_Symbol_RX[0+Step_o*NL_SFC],2*NL_SFC);                
			
		   	// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int3x24_Filt_Coeff_V1[0],2*INTFILT_3_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int3x24_filt_c_Delay[0],2*(INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1));
	
			//Amplifying Signal by Interpolation Factor
    		vsmul_f_DD((float *)&Cmplx_Array[4*NL],3.0f,(float *)&Cmplx_Array[4*NL],2*NL_SFC);  
    		
			// Interpolation by 3
			Int3x24_filt_c(&Cmplx_Array[0],&Cmplx_Array[4*NL], NL_SFC, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Int3x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1));
	
			// Decimation by 4 (result in OFDM_Symbol_RX	[0..NL-1] and [NL..2NL-1])
			// real part
	    	vcopy_s_ED( (float *)&OFDM_Symbol_RX[0+Step_o*NL], 2*1, (float *)&Cmplx_Array[0], 2*4, NL );
			// imag part
	    	vcopy_s_ED( (((float *)&OFDM_Symbol_RX[0+Step_o*NL])+1), 2*1, (((float *)&Cmplx_Array[0])+1), 2*4, NL );
	    		
			}
			//				Output OFDM_Symbol_RX[0..4NL-1]       
		}
		//==>Variant 3  
		//------------
		if (Filter_RX_Decimation == 8)  // used for N=64,128
		{		
		//Variant 4:  x-SFC: *3/4 x-FFT:/8
		//	!!! N<=128) (max input block length:8*4/3*NL=32/3*(128+16)=1536cmplx=3072words)
		// SFC: *3/4   Input OFDM_Symbol_RX[0..32/3*NL-1] = OFDM_Symbol_RX[0..8*NL_SFC-1]

		NL_SFC = 4* ((int)((float)NL/3.0f)); //  NL_SFC=4/3*NL this is what compiler likes to see
		
		// Phase increment for one block of length NL_SFC
		Delta_Phi_Symbol = -(tmp_rot)*(float)NL_SFC;
		
		// *3   		Input:OFDM_Symbol_RX[0..8*NL_SFC-1]
		for (Step_o=0; Step_o < 8; Step_o++)
			{
			// copy OFDM_Symbol_RX to internal Cmplx_Array[4NL..4NL+NL_SFC-1] , NL_SFC complex values
		   	vcopy_DE((float *)&Cmplx_Array[4*NL],(float *)&OFDM_Symbol_RX[0+Step_o*NL_SFC],2*NL_SFC);                
			
		   	// Frequency offset compensation (in place)
   	  			Start_Phi = fmodf(Start_Phi_New,OFDM_TWO_PI);
      			Start_Phi_New = Exp_Gen(Start_Phi,Rot_Fr,Cmplx_Array,Delta_Phi_Symbol,NL_SFC);
      			vmul_c_DDD(&Cmplx_Array[4*NL],&Cmplx_Array[4*NL],Cmplx_Array,NL_SFC);
	    				   	
		   	// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int3x24_Filt_Coeff_V2[0],2*INTFILT_3_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int3x24_filt_c_Delay[0],2*(INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1));
	
			//Amplifying Signal by Interpolation Factor
    		vsmul_f_DD((float *)&Cmplx_Array[4*NL],3.0f,(float *)&Cmplx_Array[4*NL],2*NL_SFC);  
    		
			// Interpolation by 3
			Int3x24_filt_c(&Cmplx_Array[0],&Cmplx_Array[4*NL], NL_SFC, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Int3x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1));
	
			// Decimation by 4 (result in OFDM_Symbol_RX	[0..NL-1] ,[NL..2NL-1],...,[7NL..8NL-1])
			// real part
	    	vcopy_s_ED( (float *)&OFDM_Symbol_RX[0+Step_o*NL], 2*1, (float *)&Cmplx_Array[0], 2*4, NL );
			// imag part
	    	vcopy_s_ED( (((float *)&OFDM_Symbol_RX[0+Step_o*NL])+1), 2*1, (((float *)&Cmplx_Array[0])+1), 2*4, NL );
	    		
			}
			//				Output OFDM_Symbol_RX[0..8NL-1]      
		
		}

		break;
		
    	
    	
		case 8000:
		// SFC: /4		Input OFDM_Symbol_RX[0..8NL-1]
		
		// Phase increment for one OFDM symbol
		Delta_Phi_Symbol = -(tmp_rot*2.0f)*(float)NL;

		for (Step_o=0; Step_o < 2; Step_o++)
			{         
			for (Step_i=0; Step_i < 2; Step_i++)
	    		{
				// Frequency offset compensation and OFDM received symbol extraction                     
	  			// Save input block of size 2NL complex in Cmplx_Array [2NL..4NL-1] (step 0) and [4NL..6NL-1] (step1)	
   	  			vcopy_DE((float *)&Cmplx_Array[2*NL+Step_i*2*NL],(float *)&OFDM_Symbol_RX[0+Step_o*4*NL+Step_i*2*NL],2*2*NL);                
   	  			// Frequency offset compensation (in place)
   	  			Start_Phi = fmodf(Start_Phi_New,OFDM_TWO_PI);
      			Start_Phi_New = Exp_Gen(Start_Phi,Rot_Fr,Cmplx_Array,Delta_Phi_Symbol,2*NL);
      			vmul_c_DDD(&Cmplx_Array[2*NL+Step_i*2*NL],&Cmplx_Array[2*NL+Step_i*2*NL],Cmplx_Array,2*NL);
	    		}
	    		// Output is Cmplx_Array[2NL..6NL-1]
	
	    	// Restore Filter Coefficients from external Memory to PM 
			//vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Dec4x24_Filt_Coeff[0],2*DECFILT_4_LENGTH);
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Dec4x24_Filt_Coeff_V3[0],2*DECFILT_4_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Dec4x24_filt_c_Delay[0],2*(DECFILT_4_LENGTH+1));
	
			// Decimation by 4 (result in Cmplx_Array[0...NL-1]
			Dec4x24_filt_c(&Cmplx_Array[0],&Cmplx_Array[2*NL], 4*NL, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Dec4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(DECFILT_4_LENGTH+1));
	    		
	    	// Save output signal to OFDM_Symbol_RX	[0..NL-1] and [NL..2NL-1]
			vcopy_ED((float *)&OFDM_Symbol_RX[0+Step_o*NL],(float *)&Cmplx_Array[0],2*NL);
			}
		//				Output OFDM_Symbol_RX[0..2NL-1]                
		break;

		case 9600:
		if (Filter_RX_Decimation == 4)
		{		
		//Variant 5:  x-SFC: *3/5 x-FFT:/4
		//	!!! N<=256) (max input block length:4*5/3*NL=20/3*(256+32)=1920cmplx=3840words)
		// SFC: *3/5   Input OFDM_Symbol_RX[0..20/3*NL-1] = OFDM_Symbol_RX[0..4*NL_SFC-1]

		NL_SFC = 5* ((int)((float)NL/3.0f)); //  NL_SFC=5/3*NL this is what compiler likes to see
		
		// Phase increment for one block of length NL_SFC
		Delta_Phi_Symbol = -(tmp_rot)*(float)NL_SFC;
		
		// *3   		Input:OFDM_Symbol_RX[0..4*NL_SFC-1]
		for (Step_o=0; Step_o < 4; Step_o++)
			{
			// copy OFDM_Symbol_RX to internal Cmplx_Array[5NL..5NL+NL_SFC-1] , NL_SFC complex values
		   	vcopy_DE((float *)&Cmplx_Array[5*NL],(float *)&OFDM_Symbol_RX[0+Step_o*NL_SFC],2*NL_SFC);                
			
		   	// Frequency offset compensation (in place)
   	  			Start_Phi = fmodf(Start_Phi_New,OFDM_TWO_PI);
      			Start_Phi_New = Exp_Gen(Start_Phi,Rot_Fr,Cmplx_Array,Delta_Phi_Symbol,NL_SFC);
      			vmul_c_DDD(&Cmplx_Array[5*NL],&Cmplx_Array[5*NL],Cmplx_Array,NL_SFC);
	    				   	
		   	// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int3x24_Filt_Coeff_V3[0],2*INTFILT_3_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int3x24_filt_c_Delay[0],2*(INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1));
	
			//Amplifying Signal by Interpolation Factor
    		vsmul_f_DD((float *)&Cmplx_Array[5*NL],3.0f,(float *)&Cmplx_Array[5*NL],2*NL_SFC);  
    		
			// Interpolation by 3
			Int3x24_filt_c(&Cmplx_Array[0],&Cmplx_Array[5*NL], NL_SFC, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Int3x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1));
	
			// Decimation by 5 (result in OFDM_Symbol_RX	[0..NL-1] ,[NL..2NL-1],...,[3NL..4NL-1])
			// real part
	    	vcopy_s_ED( (float *)&OFDM_Symbol_RX[0+Step_o*NL], 2*1, (float *)&Cmplx_Array[0], 2*5, NL );
			// imag part
	    	vcopy_s_ED( (((float *)&OFDM_Symbol_RX[0+Step_o*NL])+1), 2*1, (((float *)&Cmplx_Array[0])+1), 2*5, NL );
	    		
			}
			//				Output OFDM_Symbol_RX[0..4NL-1]      
		
		}

		break;

				
		case 12000:
		// SFC: /2*3/4   Input OFDM_Symbol_RX[0..16/3*NL-1] = OFDM_Symbol_RX[0..4*NL_SFC-1]

		NL_SFC = 4* ((int)((float)NL/3.0f)); //  NL_SFC=4/3*NL this is what compiler likes to see
		
		// Phase increment for one block of length NL_SFC
		Delta_Phi_Symbol = -(tmp_rot)*(float)NL_SFC;
        	
		   
		// /2  two times 2*NL_SFC Input:  OFDM_Symbol_RX[0..2*NL_SFC-1] OFDM_Symbol_RX[2*NL_SFC...4*NL_SFC-1]
		for (Step_o=0; Step_o < 2; Step_o++)
			{
			// copy OFDM_Symbol_RX part to internal Cmplx_Array[NL_SFC...3*NL_SFC-1] , 2*NL_SFC complex values
		   	vcopy_DE((float *)&Cmplx_Array[NL_SFC],(float *)&OFDM_Symbol_RX[0+Step_o*2*NL_SFC],2*2*NL_SFC);                
			
		   	for (Step_i=0; Step_i < 2; Step_i++)
	    		{
		   	// Frequency offset compensation (in place)
   	  			Start_Phi = fmodf(Start_Phi_New,OFDM_TWO_PI);
      			Start_Phi_New = Exp_Gen(Start_Phi,Rot_Fr,Cmplx_Array,Delta_Phi_Symbol,NL_SFC);
      			vmul_c_DDD(&Cmplx_Array[NL_SFC+Step_i*NL_SFC],&Cmplx_Array[NL_SFC+Step_i*NL_SFC],Cmplx_Array,NL_SFC);
	    		}
		   	
			// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Dec2x12_Filt_Coeff_V1[0],2*DECFILT_2_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Dec2x12_filt_c_Delay[0],2*(DECFILT_2_LENGTH+1));
	
			// Decimation by 2
			Dec2x12_filt_c(&Cmplx_Array[0],&Cmplx_Array[NL_SFC], 2*NL_SFC, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Dec2x12_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(DECFILT_2_LENGTH+1));
	    		
	    	// Save output signal to OFDM_Symbol_RX [0..NL_SFC-1] [NL_SFC..2*NL_SFC]
			vcopy_ED((float *)&OFDM_Symbol_RX[0+Step_o*NL_SFC],(float *)&Cmplx_Array[0],2*NL_SFC);			
			}
			//	Output:OFDM_Symbol_RX[0..2*NL_SFC]
		
		// two times NL_SFC complex
		// *3   		Input:OFDM_Symbol_RX[0..2*NL_SFC-1]
		for (Step_o=0; Step_o < 2; Step_o++)
			{
			// copy OFDM_Symbol_RX to internal Cmplx_Array[4NL..4NL+NL_SFC-1] , NL_SFC complex values
		   	vcopy_DE((float *)&Cmplx_Array[4*NL],(float *)&OFDM_Symbol_RX[0+Step_o*NL_SFC],2*NL_SFC);                
			
		   	// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int3x24_Filt_Coeff_V1[0],2*INTFILT_3_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int3x24_filt_c_Delay[0],2*(INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1));
	
			//Amplifying Signal by Interpolation Factor
    		vsmul_f_DD((float *)&Cmplx_Array[4*NL],3.0f,(float *)&Cmplx_Array[4*NL],2*NL_SFC);  
    		
			// Interpolation by 3
			Int3x24_filt_c(&Cmplx_Array[0],&Cmplx_Array[4*NL], NL_SFC, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Int3x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1));
	
			// Decimation by 4 (result in OFDM_Symbol_RX	[0..NL-1] and [NL..2NL-1])
			// real part
	    	vcopy_s_ED( (float *)&OFDM_Symbol_RX[0+Step_o*NL], 2*1, (float *)&Cmplx_Array[0], 2*4, NL );
			// imag part
	    	vcopy_s_ED( (((float *)&OFDM_Symbol_RX[0+Step_o*NL])+1), 2*1, (((float *)&Cmplx_Array[0])+1), 2*4, NL );
	    		
			}
			//				Output OFDM_Symbol_RX[0..2NL-1]       
		
		break;
		
		
		case 16000:
		// SFC: /2        Input OFDM_Symbol_RX[0..4NL-1]
		
		// Phase increment for one OFDM symbol
		Delta_Phi_Symbol = -(tmp_rot*2.0f)*(float)NL;

		for (Step_i=0; Step_i < 2; Step_i++)
	    		{
				// Frequency offset compensation and OFDM received symbol extraction                     
	  			// Save input block of size 2NL complex in Cmplx_Array [2NL..4NL-1] (step 0) and [4NL..6NL-1] (step1)	
   	  			vcopy_DE((float *)&Cmplx_Array[2*NL+Step_i*2*NL],(float *)&OFDM_Symbol_RX[0+Step_i*2*NL],2*2*NL);                
   	  			// Frequency offset compensation (in place)
   	  			Start_Phi = fmodf(Start_Phi_New,OFDM_TWO_PI);
      			Start_Phi_New = Exp_Gen(Start_Phi,Rot_Fr,Cmplx_Array,Delta_Phi_Symbol,2*NL);
      			vmul_c_DDD(&Cmplx_Array[2*NL+Step_i*2*NL],&Cmplx_Array[2*NL+Step_i*2*NL],Cmplx_Array,2*NL);
	    		}
	    		// Output is Cmplx_Array[2NL..6NL-1]
/* //debug
// save signal to OFDM_RX	    		
vcopy_ED((float *)&OFDM_Symbol_RX[0],(float *)&Cmplx_Array[2*NL],2*4*NL);
// estimate spec
OFDM_Spectrum_RX();
// restore signal
vcopy_DE((float *)&Cmplx_Array[2*NL],(float *)&OFDM_Symbol_RX[0],2*4*NL);		

if(n_blk_rx_M_QAM_train>80){
   		asm("nop;");
		asm("nop;");
		asm("nop;");
   		}
*/

	    // Restore Filter Coefficients from external Memory to PM 
		vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Dec2x12_Filt_Coeff_V3[0],2*DECFILT_2_LENGTH);
		// Restore Delay Array from external Memory to DM
		vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Dec2x12_filt_c_Delay[0],2*(DECFILT_2_LENGTH+1));
	
		// Decimation by 2
		Dec2x12_filt_c(&Cmplx_Array[0],&Cmplx_Array[2*NL], 4*NL, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

		// Save Delay Array into external memory buffer
		vcopy_ED((float *)&Dec2x12_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(DECFILT_2_LENGTH+1));
	    
		// Save output signal to OFDM_Symbol_RX	
		vcopy_ED((float *)&OFDM_Symbol_RX[0],(float *)&Cmplx_Array[0],4*NL);
		//				Output OFDM_Symbol_RX[0..2NL-1]      

//			Test_FIR(); //debug
/*
			// debug Decimation with out filtering
			// Decimation by 2 (result in OFDM_Symbol_RX	[0..2NL-1])
			// real part
	    	vcopy_s_ED( (float *)&OFDM_Symbol_RX[0], 2*1, (float *)&Cmplx_Array[2*NL], 2*2, 2*NL );
			// imag part
	    	vcopy_s_ED( (((float *)&OFDM_Symbol_RX[0])+1), 2*1, (((float *)&Cmplx_Array[2*NL])+1), 2*2, 2*NL );
*/	    	
    
		break;
		
		
		case 24000:
		// SFC: *3/4        Input OFDM_Symbol_RX[0..8/3*NL-1] = OFDM_Symbol_RX[0..2*NL_SFC-1]
		
		NL_SFC = 4* ((int)((float)NL/3.0f)); //  NL_SFC=4/3*NL this is what compiler likes to see
		// Phase increment for one OFDM symbol
		Delta_Phi_Symbol = -(tmp_rot)*(float)NL_SFC;
        	
		// two times NL_SFC complex
		// *3   		Input:OFDM_Symbol_RX[0..2*NL_SFC-1]
		for (Step_o=0; Step_o < 2; Step_o++)
			{
			// copy OFDM_Symbol_RX to internal Cmplx_Array[4NL..4NL+NL_SFC-1] , NL_SFC complex values
		   	vcopy_DE((float *)&Cmplx_Array[4*NL],(float *)&OFDM_Symbol_RX[0+Step_o*NL_SFC],2*NL_SFC);                
			
			// Frequency offset compensation (in place)
			Start_Phi = fmodf(Start_Phi_New,OFDM_TWO_PI);
      			Start_Phi_New = Exp_Gen(Start_Phi,Rot_Fr,Cmplx_Array,Delta_Phi_Symbol,NL_SFC);
      			vmul_c_DDD(&Cmplx_Array[4*NL],&Cmplx_Array[4*NL],Cmplx_Array,NL_SFC);
	    		
		   	// Restore Filter Coefficients from external Memory to PM 
			vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int3x24_Filt_Coeff_V4[0],2*INTFILT_3_LENGTH);
			// Restore Delay Array from external Memory to DM
			vcopy_DE((float *)IntDec_Filt_Delay,(float *)&Int3x24_filt_c_Delay[0],2*(INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1));
	
			//Amplifying Signal by Interpolation Factor
    		vsmul_f_DD((float *)&Cmplx_Array[4*NL],3.0f,(float *)&Cmplx_Array[4*NL],2*NL_SFC);  
    		
			// Interpolation by 3
			Int3x24_filt_c(&Cmplx_Array[0],&Cmplx_Array[4*NL], NL_SFC, (pm complex_float *)IntDec_Filt_Coeff, IntDec_Filt_Delay);

			// Save Delay Array into external memory buffer
			vcopy_ED((float *)&Int3x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1));
	
	
			// Decimation by 4 (result in Interp_Buf	[0..NL-1] and [NL..2NL-1])
			// real part
	    		//vcopy_s_ED( (float *)&Interp_Buf[0+Step_o*NL], 2*1, (float *)&Cmplx_Array[0], 2*4, NL );
				vcopy_s_ED( (float *)&OFDM_Symbol_RX[0+Step_o*NL], 2*1, (float *)&Cmplx_Array[0], 2*4, NL );
			// imag part
	    		//vcopy_s_ED( (((float *)&Interp_Buf[0+Step_o*NL])+1), 2*1, (((float *)&Cmplx_Array[0])+1), 2*4, NL );
	    		vcopy_s_ED( (((float *)&OFDM_Symbol_RX[0+Step_o*NL])+1), 2*1, (((float *)&Cmplx_Array[0])+1), 2*4, NL );
	    		
		}
			// also test direct copy to OFDM_symbol_RX, it should work
		//	vcopy_EE((float *)&OFDM_Symbol_RX[0],(float *)&Interp_Buf[0],2*2*NL);
				
		break;
		
		case 32000:
		// SFC: /1 (no SFC)
		
		// Phase increment for one OFDM symbol
		Delta_Phi_Symbol = -(tmp_rot*2.0f)*(float)NL;

		// Frequency offset compensation and OFDM received symbol extraction                     
	  	// Save input signal	
   	  	vcopy_DE((float *)&Cmplx_Array[2*N_L],(float *)&OFDM_Symbol_RX[0],4*NL);                

   	  	Start_Phi = fmodf(Start_Phi_New,OFDM_TWO_PI);
      	Start_Phi_New = Exp_Gen(Start_Phi,Rot_Fr,Compl_Exp,Delta_Phi_Symbol,2*NL);
      	vmul_c_DDD(&Cmplx_Array[2*N_L],&Cmplx_Array[2*N_L],Compl_Exp,2*NL);

	  	// Restore input signal	
   	  	vcopy_ED((float *)&OFDM_Symbol_RX[0],(float *)&Cmplx_Array[2*N_L],4*NL);                
		break;
		
		} // end of switch
	
	
	
	}

      	
