/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: Modulation_Init_QAM4.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Initialize QAM4 Modulation Parameters
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/Modulation_Init_QAM4.c $
 * 
 * 13    25.07.08 14:05 Chhamaa
 * 
 * 7     5.04.06 15:28 Chhamaa
 * 
 * 7     17.03.06 14:41 Chhamaa
 * 
 * 5     13.09.05 13:23 Ats
 * Exchange from TLC
 * 
 * 4     26.04.05 8:42 Chalgyg
 * OFDM_DSP_V0_09R
 * 
 * 2     24.02.05 7:28 Chchlee
 * 
 * 1     19.01.05 15:39 Chchlee
 * 
 * 6     22.12.04 9:30 Chchlee
 * 
 * 5     2.12.04 17:54 Chhamaa
 * 
 * 4     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       15.02.2003 DLK
 * Modified to use with universal TCM codec
 *       21.01.2004 vgp 
 * Split QAM4 part into own function
 *		 06.04.2004 PL
 * Initialization of QAM_Index_TX & QAM_Index_RX before TCM Codec init
 *       09.04.2004 vgp
 * implemeted new bandwidths 28,24,20,16,12,8,4 kHz
 *		 07.07.2004 PL
*****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "../h/OFDM_Defs.h"
#include "../h/OFDM_cfg_types.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/tcm/TCM_Codec_externs.h"
#include "../h/util/util.h"

int M_max;

void segment ( "seg_ext_pmco" ) Modulation_Init_QAM4()
	{
    int   TCM_Init_4QAM_Codec(int len2d, int *Tmp_vec);
	
    //int   i, M_max;
	int   i;
	
    switch ( BW )
    {
    case 32000:
    case 24000:
    case 16000:
    case 12000:
//    case  8000:
//	  case  6000:
//    case  4000:	
		switch ( N )
			{
			case 64:
			  M_QAM4 = 48;
			  M_max = 48;
			 break;
			case 128:
			  M_QAM4 = 96;
			  M_max = 106;
			 break;
			case 256:
			  M_QAM4 = 192;
			  M_max = 212;
			 break;
			case 512:
			  M_QAM4 = 384;
			  M_max = 424;
			 break;
			}
	break;

	case 2000:
		switch ( N )
			{
			case 64:
			  M_QAM4 = 40;
			  M_max = 48;
			 break;
			case 128:
			  M_QAM4 = 88;
			  M_max = 92;
			 break;
			case 256:
			  M_QAM4 = 160;
			  M_max = 190;
			 break;
			case 512:
			  M_QAM4 = 320;
			  M_max = 354;
			 break;
			}
	break;

	case 4000:
		switch (BW4_8_Mode)
		{
			case std_FN:
				switch ( N )
				{
				case 64:
			  	 M_QAM4 = 48; // not used
			  	 M_max = 48;
			 	break;
				case 128:
			  	 M_QAM4 = 96; // active
			  	 M_max = 108; //08.03.06 instead of 106
			 	break;
				case 256:
			  	 M_QAM4 = 192; // active
			  	 M_max = 212;
			 	break;
				case 512:
			  	 M_QAM4 = 384; // active
			  	 M_max = 424;
			 	break;
				}
			break;
			case high_FN:
				switch ( N )
				{
			   	case 64:
    			  M_QAM4 = 34; // not used
			      M_max = 36; //34
			    break;
			    case 128:
    			  M_QAM4 = 66; // not used
			      M_max = 70;
			    break;
    			case 256:
    			  M_QAM4 = 132; // not used
			      M_max = 140;
			    break;
    			case 512: // not used
                    //not allowed
    			  M_QAM4 = 240;
			      M_max = 280;
			    break;
				}
			break;
		}
		if (FN == 4800) //BW 4000 Low Delay
		{
		switch ( N )
				{
			   	case 64:
    			  M_QAM4 = 38; // used for Low Delay (BW=4000, N=64)
			      M_max = 44;  //44 is max, tested 8.3.06
			    break;
			    case 128:
    			  M_QAM4 = 0;
			      M_max = 0;
			    break;
    			case 256:
    			  M_QAM4 = 0;
			      M_max = 0;
			    break;
    			case 512:
                  M_QAM4 =0;
			      M_max = 0;
			    break;
				}	
		}
	break;
	
	case  3500:
		switch (BW4_8_Mode)
		{
			case std_FN:
				switch ( N )
				{
				case 64:
			  	 M_QAM4 = 44; // active
			  	 M_max = 48; // BW3.5 LD N=64
			 	break;
				case 128:
			  	 M_QAM4 = 96; // active
			  	 M_max = 102; //08.03.06 instead of 100
			 	break;
				case 256: // active
			  	 M_QAM4 = 192;
			  	 M_max = 212;
			 	break;
				case 512: // active
			  	 M_QAM4 = 384;
			  	 M_max = 424;
			 	break;
			 	}
			break;
			case high_FN:
			switch ( N )
				{
			   	case 64:
    			  M_QAM4 = 30; // not used
			      M_max = 30;
			    break;
			    case 128:
    			  M_QAM4 = 60; // not used
			      M_max = 68;
			    break;
    			case 256:
    			  M_QAM4 = 120; // not used
			      M_max = 140;
			    break;
    			case 512:
                    //not allowed
    			  M_QAM4 = 240; // not used
			      M_max = 280;
			    break;
				}
			break;
			
			default:
			break;
		}
		
		if (FN == 4800) //BW 3500
		{
		switch ( N )
				{
			   	case 64:
    			  M_QAM4 = 36; // not used
			      M_max = 38;
			    break;
			    case 128:
    			  M_QAM4 = 0; // not used
			      M_max = 0;
			    break;
    			case 256:
    			  M_QAM4 = 0; // not used
			      M_max = 0;
			    break;
    			case 512:
                  M_QAM4 =0; // not used
			      M_max = 0;
			    break;
				}	
		}
	
	break;
	
	case 6000:
		switch ( N )
			{
			case 64:
			  M_QAM4 = 46; // active
			  M_max = 46; //
			 break; 
			case 128:
			  M_QAM4 = 96; // active
			  M_max =  98; // 96=BW5300 , 104=BW5720, Test mit 98 für Performance
			 break;
			case 256:
			  M_QAM4 = 192; // active
			  M_max = 206; //
			 break;
			case 512:
			  M_QAM4 = 384; // active
			  M_max = 412;
			 break;
			}
	break;

	case 8000:
		switch (BW4_8_Mode)
		{
			case std_FN:
			switch ( N )
				{
				case 64:
			  	 M_QAM4 = 48; // not used
			  	 M_max = 50;  //
			 	break;
				case 128:
			  	 M_QAM4 = 96; // active High Delay (BW=8000, N=128)
			  	 M_max = 108; //
			 	break;
				case 256:
			  	 M_QAM4 = 192; // active
			  	 M_max = 212;
			 	break;
				case 512:
			  	 M_QAM4 = 384; // active
			  	 M_max = 424;
			 	break;
				}
			break;
			case high_FN:
			if (FN==12000)
			{
				switch ( N )
					{
					case 64:
			  	 	M_QAM4 = 34;
			  	 	M_max = 34;
			 		break;
					case 128:
			  	 	M_QAM4 = 60;
			  	 	M_max = 68;
			 		break;
					case 256:
			  	 	M_QAM4 = 120;
			  	 	M_max = 140;
			 		break;
					case 512:
			  	 	M_QAM4 = 240;
			  	 	M_max = 280;
			 		break;
					}
			}
			else
			if (FN==9600)
			{
				switch ( N )
					{
					case 64:
			  	 	M_QAM4 = 40;
			  	 	M_max = 42;
			 		break;
					case 128:
			  	 	M_QAM4 = 80; // active Low Delay (BW=8000, N=64(HMI))
			  	 	M_max = 92; //
			 		break;
					case 256:
			  	 	M_QAM4 = 160;
			  	 	M_max = 176;
			 		break;
					case 512:
			  	 	M_QAM4 = 320;
			  	 	M_max = 354;
			 		break;
					}
			}
			break;
		}
	break;
	
	
	case  7500:
		switch (BW4_8_Mode)
		{
			case std_FN:	// FN=8000
				switch ( N )
				{
				case 64:
			  	 M_QAM4 = 48; // not used
			  	 M_max = 48;
			 	break;
				case 128:
			  	 M_QAM4 = 96; // active High Delay (BW=7500, N=128)
			  	 M_max = 108; //
			 	break;
				case 256: // active
			  	 M_QAM4 = 192;
			  	 M_max = 212;
			 	break;
				case 512: // active
			  	 M_QAM4 = 384;
			  	 M_max = 424;
			 	break;
			 	}
			break;
			case high_FN:	// FN=12000
			if (FN==12000)
			{
				switch ( N )
					{
					case 64:
			  	 	M_QAM4 = 34;// not used
			  	 	M_max = 34;
			 		break;
					case 128:
			  	 	M_QAM4 = 60;// not used
			  	 	M_max = 66;
			 		break;
					case 256:
			  	 	M_QAM4 = 120;// not used
			  	 	M_max = 140;
			 		break;
					case 512:
			  	 	M_QAM4 = 240;// not used
			  	 	M_max = 280;
			 		break;
					}
			} else
			if (FN==9600)
			{
				switch ( N )
					{
					case 64:
			  	 	M_QAM4 = 40;// not used
			  	 	M_max = 42;
			 		break;
					case 128:
			  	 	M_QAM4 = 80; // active for Low Delay (BW=7500, N=64(HMI))
			  	 	M_max = 86; //86 is max, tested 8.3.06
			 		break;
					case 256:
			  	 	M_QAM4 = 160;// not used
			  	 	M_max = 174;
			 		break;
					case 512:
			  	 	M_QAM4 = 320;// not used
			  	 	M_max = 350;
			 		break;
					}
			}
			break;
		}
		
	break;
	
	
	case  11500:
		switch ( N )
			{
			case 64:
			  M_QAM4 = 48;
			  M_max = 48;
			 break;
			case 128:
			  M_QAM4 = 96;
			  M_max = 106;
			 break;
			case 256: 
			  M_QAM4 = 192;
			  M_max = 212;
			 break;
			case 512: 
			  M_QAM4 = 384;
			  M_max = 424;
			 break;
			 
			}
	break;
	
	case 28000:
		switch ( N )
			{
			case 64:
			  M_QAM4 = 42;
			  M_max = 42;
			 break;
			case 128:
			  M_QAM4 = 84;
			  M_max =  92;
			 break;
			case 256:
			  M_QAM4 = 168;
			  M_max = 186;
			 break;
			case 512:
			  M_QAM4 = 336;
			  M_max = 370;
			 break;
			}
	break;
	
	case 20000:
		switch ( N )
			{
			case 64:
			  M_QAM4 = 40;
			  M_max = 40;
			 break;
			case 128:
			  M_QAM4 = 80;
			  M_max =  88;
			 break;
			case 256:
			  M_QAM4 = 160;
			  M_max = 176;
			 break;
			case 512:
			  M_QAM4 = 320;
			  M_max = 354;
			 break;
			}
	break;
    }
    
	if ( M_QAM4 > 484 ) exit(0);

	// Number of channels during QAM4 transmission
	M0_QAM4 = M_QAM4 + N_pil;

	
	M_TX = M_QAM4;
	M_RX = M_QAM4;
	
	M0_TX = M0_QAM4;
	M0_RX = M0_QAM4;
	
	
//============== TCM configuration parameters     ====================

	// ***** !! QAM_Index_TX, QAM_Index_RX, Max_Index_TX, Min_Index_TX, Max_Index_RX, Min_Index_RX, 
	// ***** !! are independet of QAM4 modulation. Reason: Channel measurement has to be performed
	// ***** !! in max range  M_max
	
	// Init QAM TX index array 

    for (i=0; i<N; i++)
    	{
        Fl_Array_R[N+i] = 0.5f*(1.0f - cosf(OFDM_TWO_PI*(float)i/(float)N));
    	}
	Fl_Array_R[N+Pilot_ind0] = -1.0f;
	Fl_Array_R[N+Pilot_ind1] = -2.0f;
	
    vsorti_f_DDD(&Fl_Array_R[N2], (int *)&Fl_Array_R[0], &Fl_Array_R[N], N );

	Min_Index_TX = vmin_i_D((int *)&Fl_Array_R[0],M_max);
	Max_Index_TX = vmax_i_D((int *)&Fl_Array_R[0],M_max);
    vcopy_ED((float *)&QAM_Index_TX[0],(float *)&Fl_Array_R[0],N);
	
	// Init QAM RX index array 

    for (i=0; i<N; i++)
    	{
        Fl_Array_R[N+i] = 0.5f*(1.0f - cosf(OFDM_TWO_PI*(float)i/(float)N));
    	}
	Fl_Array_R[N+Pilot_ind0] = -1.0f;
	Fl_Array_R[N+Pilot_ind1] = -2.0f;
	
    vsorti_f_DDD(&Fl_Array_R[N2], (int *)&Fl_Array_R[0], &Fl_Array_R[N], N );

	Min_Index_RX = vmin_i_D((int *)&Fl_Array_R[0],M_max);
	Max_Index_RX = vmax_i_D((int *)&Fl_Array_R[0],M_max);
	
//	Min_Index_RX_QAM4 = vmin_i_D((int *)&Fl_Array_R[0],M_QAM4);
//	Max_Index_RX_QAM4 = vmax_i_D((int *)&Fl_Array_R[0],M_QAM4);
	
	
    vcopy_ED((float *)&QAM_Index_RX[0],(float *)&Fl_Array_R[0],N);
 
	// Init trellis codec in QPSK mode
    TCM_Init_4QAM_Codec(M_QAM4, (int *)Fl_Array_R);	                   

	Xmax      = sqrt(2.0f)*(Delta_TX/2.0f)*(sqrtf((float)QAM4) - 1.0f);
	
	Amp_Pilot = 2.0f*Xmax;
	
	
	P_OFDM_Spectrum = (OFDM_QAM_Energy_TX + 
	                  (float)N_pil*Amp_Pilot*Amp_Pilot)/
	                  (float)N;

	// QAM scaling
	Scale_QAM_TX=sqrtf(P_OFDM_TX/P_OFDM_Spectrum);
	
	// Pilots scaling
	Amp_Pilot_RX = Amp_Pilot;
	
	Amp_Pilot_TX =Amp_Pilot*sqrtf(P_OFDM_TX/P_OFDM_Spectrum);
	
	Amp_Pilot_TX_Ref = Amp_Pilot*sqrtf(P_OFDM_TX/P_OFDM_Spectrum);
	Ref_Level = (OFDM_PILOTS_NUM*Amp_Pilot_TX_Ref*Amp_Pilot_TX_Ref);

	Preeq.re = 1.0f;
	Preeq.im = 0.0f;
	Alpha_Pil = 1.0f;
	
   	}


