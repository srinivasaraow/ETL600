/*-------------------------------------------------------------------------*
 * $Workfile: intDec.c $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : D. Cachin
 * Remarks      :  
 * Purpose      : Interpolation/Decimation by 8
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/intDec.c $
 * 
 * 1     4.07.07 11:50 Chhamaa
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/
 
/* INCLUDE FILES *************************************************************/

#include <filter.h>

#include "Basetype_O4CV.h"
#include "O4CV_Const.h"
#include "intDec.h"
#include "sport2.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define		RATIO                     8		// decimation ratio = 64kHz/8kHz
#define		NBR_COEFFS				272		// # coefficients (padding to 
											// n*RATIO with 0)
#define		NBR_COEFFS_DIV_RATIO	 34		// # coefficients / RATIO

// length of 30ms buffers in #samples
#define		SAMP_PER_BUF_8K	         240


/* LOCAL TYPE DECLARATIONS ***************************************************/

/* LOCAL VARIABLE DECLARATIONS ***********************************************/

// maping strategie:	- align everything to 4 Bytes boundary
//						- filter coefficients in L1_data_b
//						- filter in-/output and history in L1_data_a

// coefficients for ALL decimation filters
#pragma align 4				  
#pragma section("L1_data_b")
static	fract16 coeffs_dec[NBR_COEFFS];

// coefficients for ALL interpolation filters 
#pragma align 4
#pragma section("L1_data_b")
static	fract16 coeffs_int[NBR_COEFFS];

// filter histories for the decimation filters
#pragma align 4
//#pragma section("L1_data_a")
#pragma section("L1_Scratch")
static fract16 hist_dec[N_CHANNELS_ON_BOARD][NBR_COEFFS];

// filter histories for the interpolation filters
#pragma align 4
#pragma section("L1_data_a")
//#pragma section("L1_Scratch")
static fract16 hist_int [N_CHANNELS_ON_BOARD][NBR_COEFFS];

// filter states
static fir_state_fr16 fstate_dec[N_CHANNELS_ON_BOARD];
static fir_state_fr16 fstate_int[N_CHANNELS_ON_BOARD];


/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL FUNCTION DEFINITIONS ************************************************/

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		void intDec_interpolate()
  Description:	interpolation of 8kHz samples to 64kHz using ADI library func. 
				fir_interp_fr16()   
  Inputs:		pointers to sample buffers (number of input samples: 240,
  				number of output samples: 1920)
  Outputs:		interpolated sammples in output buffer
  Return:		none
  Globals:      filter history and state
  Important:    --
*******************************************************************************/

void intDec_interpolate(INT16 *pBufferIn, INT16 *pBufferOut, INT16 channel)
{
    fir_interp_fr16(pBufferIn, pBufferOut, SAMP_PER_BUF_8K, &fstate_int[channel]);
}


/*****************************************************************************
  Function:		void intDec_decimate()
  Description:	decimation of 64kHz samples to 8kHz using ADI library function 
				fir_decima_fr16()  
  Inputs:		pointers to sample buffers (number of input samples: 1920,
  				number of output samples: 240)
  Outputs:		decimated sammples in output buffer
  Return:		none
  Globals:      filter history and state
  Important:    --
*******************************************************************************/

void intDec_decimate(INT16 *pBufferIn, INT16 *pBufferOut, INT16 channel)
{
	fir_decima_fr16(pBufferIn, pBufferOut, BUFFER_SIZE_4W_IF, &fstate_dec[channel]); 
}


/*****************************************************************************
  Function:		void intDec_init (void)
  Description:	initialisation function for the decimation & interp. filtering.
				N_CHANNELS_ON_BOARD dec. and int filter are initialised with coeffs, 
				empty filter history and own fir_state_fr16 structs
  Inputs:		none
  Outputs:		none
  Return:		none
  Globals:      statics:
				- coeffs_dec[] & coeffs_int[] are initialised with filt.-coeefs
				- fstate_dec[] & fstate_int[] are initialised
				- hist_dec  [] & hist_int  [] are initialised to 0
  Important:    --
*******************************************************************************/

void intDec_init(void) {
     
    fract16 *pDec, *pInt;
	INT16	i, j;

    // initialisation of filter structs:
    //
	for (i=0;i<N_CHANNELS_ON_BOARD;i++) {		// the dec. filters:
	
		fstate_dec[i].h	= &coeffs_dec [0];		// - same coeffs for all filt. 
		fstate_dec[i].d	= &hist_dec[i][0];		// - individual filter 
		fstate_dec[i].p	= &hist_dec[i][0];		//   histories
		fstate_dec[i].k	= NBR_COEFFS;			// - lenght (dec.: full length)
		fstate_dec[i].l	= RATIO;				// - decimation ratio

												// the int. filters: 
		fstate_int[i].h	= &coeffs_int [0];		// - same coeffs for all filt.  
		fstate_int[i].d	= &hist_int[i][0];		// - individual filter  
		fstate_int[i].p	= &hist_int[i][0];		//   histories 
		fstate_int[i].k	= NBR_COEFFS_DIV_RATIO;	// - lenght (int.: length/RATIO) 
		fstate_int[i].l	= RATIO;				// - interpolation ratio 

		for (j=0;j<NBR_COEFFS;j++) {
		    
			hist_dec [i][j] = 0;				// init filt. history to 0
			hist_int [i][j] = 0;				// 			"
		}
	}	

	
	// initilaisation of filter coefficients
	// 
	pDec = &coeffs_dec[0];	pInt = &coeffs_int[0];
	
	// Decimation filter: 
	*pDec++	= 0x0019;		// b0
	*pDec++	= 0x0010;		// b1
	*pDec++	= 0x0013;		// b2 
	*pDec++	= 0x0016;		// ...
	*pDec++	= 0x0017;
	*pDec++	= 0x0016;
	*pDec++	= 0x0013;
	*pDec++	= 0x000E;
	*pDec++	= 0x0007;
	*pDec++	= 0xFFFF;
	*pDec++	= 0xFFF6;
	*pDec++	= 0xFFEE;
	*pDec++	= 0xFFE6;
	*pDec++	= 0xFFE1;
	*pDec++	= 0xFFDD;
	*pDec++	= 0xFFDD;
	*pDec++	= 0xFFE1;
	*pDec++	= 0xFFE7;
	*pDec++	= 0xFFEF;
	*pDec++	= 0xFFF9;
	*pDec++	= 0x0003;
	*pDec++	= 0x000C;
	*pDec++	= 0x0013;
	*pDec++	= 0x0017;
	*pDec++	= 0x0017;
	*pDec++	= 0x0013;
	*pDec++	= 0x000C;
	*pDec++	= 0x0001;
	*pDec++	= 0xFFF5;
	*pDec++	= 0xFFE9;
	*pDec++	= 0xFFDE;
	*pDec++	= 0xFFD6;
	*pDec++	= 0xFFD3;
	*pDec++	= 0xFFD4;
	*pDec++	= 0xFFDB;
	*pDec++	= 0xFFE6;
	*pDec++	= 0xFFF5;
	*pDec++	= 0x0005;
	*pDec++	= 0x0016;
	*pDec++	= 0x0023;
	*pDec++	= 0x002D;
	*pDec++	= 0x0031;
	*pDec++	= 0x002E;
	*pDec++	= 0x0024;
	*pDec++	= 0x0015;
	*pDec++	= 0x0002;
	*pDec++	= 0xFFED;
	*pDec++	= 0xFFD9;
	*pDec++	= 0xFFC8;
	*pDec++	= 0xFFBD;
	*pDec++	= 0xFFBA;
	*pDec++	= 0xFFC0;
	*pDec++	= 0xFFCE;
	*pDec++	= 0xFFE3;
	*pDec++	= 0xFFFD;
	*pDec++	= 0x0018;
	*pDec++	= 0x0031;
	*pDec++	= 0x0046;
	*pDec++	= 0x0052;
	*pDec++	= 0x0053;
	*pDec++	= 0x004A;
	*pDec++	= 0x0036;
	*pDec++	= 0x001A;
	*pDec++	= 0xFFF9;
	*pDec++	= 0xFFD7;
	*pDec++	= 0xFFB8;
	*pDec++	= 0xFFA0;
	*pDec++	= 0xFF93;
	*pDec++	= 0xFF94;
	*pDec++	= 0xFFA3;
	*pDec++	= 0xFFBF;
	*pDec++	= 0xFFE4;
	*pDec++	= 0x000F;
	*pDec++	= 0x003B;
	*pDec++	= 0x0061;
	*pDec++	= 0x007D;
	*pDec++	= 0x0089;
	*pDec++	= 0x0084;
	*pDec++	= 0x006E;
	*pDec++	= 0x0048;
	*pDec++	= 0x0016;
	*pDec++	= 0xFFDE;
	*pDec++	= 0xFFA7;
	*pDec++	= 0xFF79;
	*pDec++	= 0xFF59;
	*pDec++	= 0xFF4E;
	*pDec++	= 0xFF59;
	*pDec++	= 0xFF7A;
	*pDec++	= 0xFFAF;
	*pDec++	= 0xFFF2;
	*pDec++	= 0x003B;
	*pDec++	= 0x0081;
	*pDec++	= 0x00BA;
	*pDec++	= 0x00DE;
	*pDec++	= 0x00E8;
	*pDec++	= 0x00D3;
	*pDec++	= 0x00A1;
	*pDec++	= 0x0056;
	*pDec++	= 0xFFFB;
	*pDec++	= 0xFF9A;
	*pDec++	= 0xFF3F;
	*pDec++	= 0xFEF6;
	*pDec++	= 0xFECC;
	*pDec++	= 0xFEC7;
	*pDec++	= 0xFEEB;
	*pDec++	= 0xFF37;
	*pDec++	= 0xFFA4;
	*pDec++	= 0x0026;
	*pDec++	= 0x00B0;
	*pDec++	= 0x012E;
	*pDec++	= 0x0190;
	*pDec++	= 0x01C7;
	*pDec++	= 0x01C5;
	*pDec++	= 0x0187;
	*pDec++	= 0x010D;
	*pDec++	= 0x0060;
	*pDec++	= 0xFF90;
	*pDec++	= 0xFEB5;
	*pDec++	= 0xFDE8;
	*pDec++	= 0xFD45;
	*pDec++	= 0xFCEA;
	*pDec++	= 0xFCEC;
	*pDec++	= 0xFD5C;
	*pDec++	= 0xFE43;
	*pDec++	= 0xFF9D;
	*pDec++	= 0x015D;
	*pDec++	= 0x036C;
	*pDec++	= 0x05A9;
	*pDec++	= 0x07EE;
	*pDec++	= 0x0A14;
	*pDec++	= 0x0BF1;
	*pDec++	= 0x0D63;
	*pDec++	= 0x0E4C;
	*pDec++	= 0x0E9C;
	*pDec++	= 0x0E4C;
	*pDec++	= 0x0D63;
	*pDec++	= 0x0BF1;
	*pDec++	= 0x0A14;
	*pDec++	= 0x07EE;
	*pDec++	= 0x05A9;
	*pDec++	= 0x036C;
	*pDec++	= 0x015D;
	*pDec++	= 0xFF9D;
	*pDec++	= 0xFE43;
	*pDec++	= 0xFD5C;
	*pDec++	= 0xFCEC;
	*pDec++	= 0xFCEA;
	*pDec++	= 0xFD45;
	*pDec++	= 0xFDE8;
	*pDec++	= 0xFEB5;
	*pDec++	= 0xFF90;
	*pDec++	= 0x0060;
	*pDec++	= 0x010D;
	*pDec++	= 0x0187;
	*pDec++	= 0x01C5;
	*pDec++	= 0x01C7;
	*pDec++	= 0x0190;
	*pDec++	= 0x012E;
	*pDec++	= 0x00B0;
	*pDec++	= 0x0026;
	*pDec++	= 0xFFA4;
	*pDec++	= 0xFF37;
	*pDec++	= 0xFEEB;
	*pDec++	= 0xFEC7;
	*pDec++	= 0xFECC;
	*pDec++	= 0xFEF6;
	*pDec++	= 0xFF3F;
	*pDec++	= 0xFF9A;
	*pDec++	= 0xFFFB;
	*pDec++	= 0x0056;
	*pDec++	= 0x00A1;
	*pDec++	= 0x00D3;
	*pDec++	= 0x00E8;
	*pDec++	= 0x00DE;
	*pDec++	= 0x00BA;
	*pDec++	= 0x0081;
	*pDec++	= 0x003B;
	*pDec++	= 0xFFF2;
	*pDec++	= 0xFFAF;
	*pDec++	= 0xFF7A;
	*pDec++	= 0xFF59;
	*pDec++	= 0xFF4E;
	*pDec++	= 0xFF59;
	*pDec++	= 0xFF79;
	*pDec++	= 0xFFA7;
	*pDec++	= 0xFFDE;
	*pDec++	= 0x0016;
	*pDec++	= 0x0048;
	*pDec++	= 0x006E;
	*pDec++	= 0x0084;
	*pDec++	= 0x0089;
	*pDec++	= 0x007D;
	*pDec++	= 0x0061;
	*pDec++	= 0x003B;
	*pDec++	= 0x000F;
	*pDec++	= 0xFFE4;
	*pDec++	= 0xFFBF;
	*pDec++	= 0xFFA3;
	*pDec++	= 0xFF94;
	*pDec++	= 0xFF93;
	*pDec++	= 0xFFA0;
	*pDec++	= 0xFFB8;
	*pDec++	= 0xFFD7;
	*pDec++	= 0xFFF9;
	*pDec++	= 0x001A;
	*pDec++	= 0x0036;
	*pDec++	= 0x004A;
	*pDec++	= 0x0053;
	*pDec++	= 0x0052;
	*pDec++	= 0x0046;
	*pDec++	= 0x0031;
	*pDec++	= 0x0018;
	*pDec++	= 0xFFFD;
	*pDec++	= 0xFFE3;
	*pDec++	= 0xFFCE;
	*pDec++	= 0xFFC0;
	*pDec++	= 0xFFBA;
	*pDec++	= 0xFFBD;
	*pDec++	= 0xFFC8;
	*pDec++	= 0xFFD9;
	*pDec++	= 0xFFED;
	*pDec++	= 0x0002;
	*pDec++	= 0x0015;
	*pDec++	= 0x0024;
	*pDec++	= 0x002E;
	*pDec++	= 0x0031;
	*pDec++	= 0x002D;
	*pDec++	= 0x0023;
	*pDec++	= 0x0016;
	*pDec++	= 0x0005;
	*pDec++	= 0xFFF5;
	*pDec++	= 0xFFE6;
	*pDec++	= 0xFFDB;
	*pDec++	= 0xFFD4;
	*pDec++	= 0xFFD3;
	*pDec++	= 0xFFD6;
	*pDec++	= 0xFFDE;
	*pDec++	= 0xFFE9;
	*pDec++	= 0xFFF5;
	*pDec++	= 0x0001;
	*pDec++	= 0x000C;
	*pDec++	= 0x0013;
	*pDec++	= 0x0017;
	*pDec++	= 0x0017;
	*pDec++	= 0x0013;
	*pDec++	= 0x000C;
	*pDec++	= 0x0003;
	*pDec++	= 0xFFF9;
	*pDec++	= 0xFFEF;
	*pDec++	= 0xFFE7;
	*pDec++	= 0xFFE1;
	*pDec++	= 0xFFDD;
	*pDec++	= 0xFFDD;
	*pDec++	= 0xFFE1;
	*pDec++	= 0xFFE6;
	*pDec++	= 0xFFEE;
	*pDec++	= 0xFFF6;
	*pDec++	= 0xFFFF;
	*pDec++	= 0x0007;
	*pDec++	= 0x000E;
	*pDec++	= 0x0013;
	*pDec++	= 0x0016;
	*pDec++	= 0x0017;
	*pDec++	= 0x0016;
	*pDec++	= 0x0013;
	*pDec++	= 0x0010;		// ...
	*pDec++	= 0x0019;		// b266
	*pDec++	= 0x0000;		// b267		padd with 0 to length n x RATIO
	*pDec++	= 0x0000;		// b268					"
	*pDec++	= 0x0000;		// b269					"
	*pDec++	= 0x0000;		// b270					"
	*pDec++	= 0x0000;		// b271					"	

	// Interpolation filter: 
	*pInt++ = 0x00C5;		// b0
	*pInt++ = 0x003A;		// b1
	*pInt++ = 0xFF05;		// b2
	*pInt++ = 0x00B9;		// ...
	*pInt++ = 0xFE97;
	*pInt++ = 0x0167;
	*pInt++ = 0xFE41;
	*pInt++ = 0x018B;
	*pInt++ = 0xFEB5;
	*pInt++ = 0x007B;
	*pInt++ = 0x00AE;
	*pInt++ = 0xFD7B;
	*pInt++ = 0x0508;
	*pInt++ = 0xF756;
	*pInt++ = 0x0E2B;
	*pInt++ = 0xE74F;
	*pInt++ = 0x3F72;
	*pInt++ = 0x5F89;
	*pInt++ = 0xEAE4;
	*pInt++ = 0x0868;
	*pInt++ = 0xFD1C;
	*pInt++ = 0xFFD7;
	*pInt++ = 0x01DA;
	*pInt++ = 0xFD39;
	*pInt++ = 0x0308;
	*pInt++ = 0xFCFF;
	*pInt++ = 0x028C;
	*pInt++ = 0xFDD4;
	*pInt++ = 0x016F;
	*pInt++ = 0xFED8;
	*pInt++ = 0x005D;
	*pInt++ = 0xFF79;
	*pInt++ = 0xFFB3;
	*pInt++ = 0x009B;
	*pInt++ = 0x0080;
	*pInt++ = 0xFFF9;
	*pInt++ = 0xFF36;
	*pInt++ = 0x009A;
	*pInt++ = 0xFEA3;
	*pInt++ = 0x0186;
	*pInt++ = 0xFDEB;
	*pInt++ = 0x022C;
	*pInt++ = 0xFDBC;
	*pInt++ = 0x01D7;
	*pInt++ = 0xFEEF;
	*pInt++ = 0xFF93;
	*pInt++ = 0x02B2;
	*pInt++ = 0xF9B5;
	*pInt++ = 0x0C39;
	*pInt++ = 0xE75F;
	*pInt++ = 0x509E;
	*pInt++ = 0x509E;
	*pInt++ = 0xE75F;
	*pInt++ = 0x0C39;
	*pInt++ = 0xF9B5;
	*pInt++ = 0x02B2;
	*pInt++ = 0xFF93;
	*pInt++ = 0xFEEF;
	*pInt++ = 0x01D7;
	*pInt++ = 0xFDBC;
	*pInt++ = 0x022C;
	*pInt++ = 0xFDEB;
	*pInt++ = 0x0186;
	*pInt++ = 0xFEA3;
	*pInt++ = 0x009A;
	*pInt++ = 0xFF36;
	*pInt++ = 0xFFF9;
	*pInt++ = 0x0080;
	*pInt++ = 0x009B;
	*pInt++ = 0xFFB3;
	*pInt++ = 0xFF79;
	*pInt++ = 0x005D;
	*pInt++ = 0xFED8;
	*pInt++ = 0x016F;
	*pInt++ = 0xFDD4;
	*pInt++ = 0x028C;
	*pInt++ = 0xFCFF;
	*pInt++ = 0x0308;
	*pInt++ = 0xFD39;
	*pInt++ = 0x01DA;
	*pInt++ = 0xFFD7;
	*pInt++ = 0xFD1C;
	*pInt++ = 0x0868;
	*pInt++ = 0xEAE4;
	*pInt++ = 0x5F89;
	*pInt++ = 0x3F72;
	*pInt++ = 0xE74F;
	*pInt++ = 0x0E2B;
	*pInt++ = 0xF756;
	*pInt++ = 0x0508;
	*pInt++ = 0xFD7B;
	*pInt++ = 0x00AE;
	*pInt++ = 0x007B;
	*pInt++ = 0xFEB5;
	*pInt++ = 0x018B;
	*pInt++ = 0xFE41;
	*pInt++ = 0x0167;
	*pInt++ = 0xFE97;
	*pInt++ = 0x00B9;
	*pInt++ = 0xFF05;
	*pInt++ = 0x003A;
	*pInt++ = 0x00C5;
	*pInt++ = 0x00AE;
	*pInt++ = 0xFF6E;
	*pInt++ = 0xFFC8;
	*pInt++ = 0x000A;
	*pInt++ = 0xFF32;
	*pInt++ = 0x0124;
	*pInt++ = 0xFE00;
	*pInt++ = 0x029B;
	*pInt++ = 0xFC9B;
	*pInt++ = 0x03E4;
	*pInt++ = 0xFBC6;
	*pInt++ = 0x0408;
	*pInt++ = 0xFCCC;
	*pInt++ = 0x0132;
	*pInt++ = 0x0300;
	*pInt++ = 0xF218;
	*pInt++ = 0x6B15;
	*pInt++ = 0x2D45;
	*pInt++ = 0xEA2C;
	*pInt++ = 0x0E34;
	*pInt++ = 0xF636;
	*pInt++ = 0x0698;
	*pInt++ = 0xFBD2;
	*pInt++ = 0x023E;
	*pInt++ = 0xFF21;
	*pInt++ = 0xFFC9;
	*pInt++ = 0x00BF;
	*pInt++ = 0xFEC6;
	*pInt++ = 0x011B;
	*pInt++ = 0xFEB3;
	*pInt++ = 0x00B9;
	*pInt++ = 0xFEEC;
	*pInt++ = 0x0070;
	*pInt++ = 0x0000;
	*pInt++ = 0x00B6;
	*pInt++ = 0xFF31;
	*pInt++ = 0x0019;
	*pInt++ = 0xFFA9;
	*pInt++ = 0xFFA7;
	*pInt++ = 0x00AA;
	*pInt++ = 0xFE70;
	*pInt++ = 0x0251;
	*pInt++ = 0xFCA1;
	*pInt++ = 0x0449;
	*pInt++ = 0xFACA;
	*pInt++ = 0x05D0;
	*pInt++ = 0xF9F5;
	*pInt++ = 0x057C;
	*pInt++ = 0xFC84;
	*pInt++ = 0xFCE8;
	*pInt++ = 0x7263;
	*pInt++ = 0x1B5D;
	*pInt++ = 0xEF3D;
	*pInt++ = 0x0C82;
	*pInt++ = 0xF65F;
	*pInt++ = 0x073D;
	*pInt++ = 0xFAC5;
	*pInt++ = 0x0370;
	*pInt++ = 0xFDF4;
	*pInt++ = 0x00D3;
	*pInt++ = 0xFFE5;
	*pInt++ = 0xFF68;
	*pInt++ = 0x00AC;
	*pInt++ = 0xFEF1;
	*pInt++ = 0x009A;
	*pInt++ = 0xFEEC;
	*pInt++ = 0x0098;
	*pInt++ = 0x0000;
	*pInt++ = 0x00AF;
	*pInt++ = 0xFF04;
	*pInt++ = 0x0062;
	*pInt++ = 0xFF47;
	*pInt++ = 0x002B;
	*pInt++ = 0x0010;
	*pInt++ = 0xFF18;
	*pInt++ = 0x01B4;
	*pInt++ = 0xFD17;
	*pInt++ = 0x0424;
	*pInt++ = 0xFA6D;
	*pInt++ = 0x06F2;
	*pInt++ = 0xF7B3;
	*pInt++ = 0x0970;
	*pInt++ = 0xF5A6;
	*pInt++ = 0x0AE8;
	*pInt++ = 0x74E3;
	*pInt++ = 0x0AE8;
	*pInt++ = 0xF5A6;
	*pInt++ = 0x0970;
	*pInt++ = 0xF7B3;
	*pInt++ = 0x06F2;
	*pInt++ = 0xFA6D;
	*pInt++ = 0x0424;
	*pInt++ = 0xFD17;
	*pInt++ = 0x01B4;
	*pInt++ = 0xFF18;
	*pInt++ = 0x0010;
	*pInt++ = 0x002B;
	*pInt++ = 0xFF47;
	*pInt++ = 0x0062;
	*pInt++ = 0xFF04;
	*pInt++ = 0x00AF;
	*pInt++ = 0x0000;
	*pInt++ = 0x0098;
	*pInt++ = 0xFEEC;
	*pInt++ = 0x009A;
	*pInt++ = 0xFEF1;
	*pInt++ = 0x00AC;
	*pInt++ = 0xFF68;
	*pInt++ = 0xFFE5;
	*pInt++ = 0x00D3;
	*pInt++ = 0xFDF4;
	*pInt++ = 0x0370;
	*pInt++ = 0xFAC5;
	*pInt++ = 0x073D;
	*pInt++ = 0xF65F;
	*pInt++ = 0x0C82;
	*pInt++ = 0xEF3D;
	*pInt++ = 0x1B5D;
	*pInt++ = 0x7263;
	*pInt++ = 0xFCE8;
	*pInt++ = 0xFC84;
	*pInt++ = 0x057C;
	*pInt++ = 0xF9F5;
	*pInt++ = 0x05D0;
	*pInt++ = 0xFACA;
	*pInt++ = 0x0449;
	*pInt++ = 0xFCA1;
	*pInt++ = 0x0251;
	*pInt++ = 0xFE70;
	*pInt++ = 0x00AA;
	*pInt++ = 0xFFA7;
	*pInt++ = 0xFFA9;
	*pInt++ = 0x0019;
	*pInt++ = 0xFF31;
	*pInt++ = 0x00B6;
	*pInt++ = 0x0000;
	*pInt++ = 0x0070;
	*pInt++ = 0xFEEC;
	*pInt++ = 0x00B9;
	*pInt++ = 0xFEB3;
	*pInt++ = 0x011B;
	*pInt++ = 0xFEC6;
	*pInt++ = 0x00BF;
	*pInt++ = 0xFFC9;
	*pInt++ = 0xFF21;
	*pInt++ = 0x023E;
	*pInt++ = 0xFBD2;
	*pInt++ = 0x0698;
	*pInt++ = 0xF636;
	*pInt++ = 0x0E34;
	*pInt++ = 0xEA2C;
	*pInt++ = 0x2D45;
	*pInt++ = 0x6B15;
	*pInt++ = 0xF218;
	*pInt++ = 0x0300;
	*pInt++ = 0x0132;
	*pInt++ = 0xFCCC;
	*pInt++ = 0x0408;
	*pInt++ = 0xFBC6;
	*pInt++ = 0x03E4;
	*pInt++ = 0xFC9B;
	*pInt++ = 0x029B;
	*pInt++ = 0xFE00;
	*pInt++ = 0x0124;
	*pInt++ = 0xFF32;
	*pInt++ = 0x000A;
	*pInt++ = 0xFFC8;		// b268
	*pInt++ = 0xFF6E;		// b269
	*pInt++ = 0x00AE;		// b270
	*pInt++ = 0x0000;		// b271

}

