/*---------------------------------------------------------------------------*
 * $Workfile: intDec.c $
 * Part of      : ETL600 / Front end DSP
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Interpolation and decimation routines
 * Copyright    : ABB Switzerland Ltd., 2003
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * $Log: /Source/P4LT/FE_DSP/C/intDec.c $
 * 
 * 2     19.08.05 16:53 Chhamaa
 * segment ("seg_dmData") replaced with SEGMENT_DM_DATA2
 * 
 * 1     19.01.05 15:31 Chchlee
 * 
 * 5     20.10.04 17:14 Maag01
 * 
 * 4     20.10.04 9:58 Maag01
 * Int/dec equalization filter added (second stage)
 * 
 * 3     18.10.04 17:58 Maag01
 * Int/dec equalization filter added (first stage)
 * 
 * 2     6.10.04 17:49 Maag01
 * 
 * 1     30.04.04 14:51 Maag01
 * Initial version
 * 
 * 
 *
 *---------------------------------------------------------------------------*/

/* INCLUDE FILES *************************************************************/

#include "basetype.h"
#include "speeddsp.h"

#include "intDec.h"
#include "intDecCoef.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define N_FIR_LP_M2_DELAY	(N_FIR_M2_LP_COEF + 2)
#define N_FIR_LP_M3_DELAY	(N_FIR_M3_LP_COEF + 2)

#define N_FIR_LP_POLY0_M2_DELAY	(N_FIR_M2_LP_POLY0_COEF + 2)
#define N_FIR_LP_POLY1_M2_DELAY	(N_FIR_M2_LP_POLY1_COEF + 2)

#define N_FIR_LP_POLY0_M3_DELAY (N_FIR_M3_LP_POLY0_COEF + 2)
#define N_FIR_LP_POLY1_M3_DELAY (N_FIR_M3_LP_POLY1_COEF + 2)
#define N_FIR_LP_POLY2_M3_DELAY (N_FIR_M3_LP_POLY2_COEF + 2)

#define INT_M6_GAIN	6.0


/* LOCAL VARIABLE DECLARATIONS ***********************************************/

// interpolation/decimation filter delay lines

// seg_dmData used for even word alignment (use only even numbers of vector elements)

float SEGMENT_DM_DATA2 g_firIntM2Poly0DelayI[N_FIR_LP_POLY0_M2_DELAY];
float SEGMENT_DM_DATA2 g_firIntM2Poly1DelayI[N_FIR_LP_POLY1_M2_DELAY];

float SEGMENT_DM_DATA2 g_firIntM2Poly0DelayQ[N_FIR_LP_POLY0_M2_DELAY];
float SEGMENT_DM_DATA2 g_firIntM2Poly1DelayQ[N_FIR_LP_POLY1_M2_DELAY];

float SEGMENT_DM_DATA2 g_firIntM2Poly0DelayCh3I[N_FIR_LP_POLY0_M2_DELAY];
float SEGMENT_DM_DATA2 g_firIntM2Poly1DelayCh3I[N_FIR_LP_POLY1_M2_DELAY];

float SEGMENT_DM_DATA2 g_firIntM2Poly0DelayCh3Q[N_FIR_LP_POLY0_M2_DELAY];
float SEGMENT_DM_DATA2 g_firIntM2Poly1DelayCh3Q[N_FIR_LP_POLY1_M2_DELAY];

float SEGMENT_DM_DATA2 g_firIntM3Poly0DelayI[N_FIR_LP_POLY0_M3_DELAY];
float SEGMENT_DM_DATA2 g_firIntM3Poly1DelayI[N_FIR_LP_POLY1_M3_DELAY];
float SEGMENT_DM_DATA2 g_firIntM3Poly2DelayI[N_FIR_LP_POLY2_M3_DELAY];

float SEGMENT_DM_DATA2 g_firIntM3Poly0DelayQ[N_FIR_LP_POLY0_M3_DELAY];
float SEGMENT_DM_DATA2 g_firIntM3Poly1DelayQ[N_FIR_LP_POLY1_M3_DELAY];
float SEGMENT_DM_DATA2 g_firIntM3Poly2DelayQ[N_FIR_LP_POLY2_M3_DELAY];

float SEGMENT_DM_DATA2 g_firDecM3DelayI[N_FIR_LP_M3_DELAY];
float SEGMENT_DM_DATA2 g_firDecM3DelayQ[N_FIR_LP_M3_DELAY];

float SEGMENT_DM_DATA2 g_firDecM2DelayI[N_FIR_LP_M2_DELAY];
float SEGMENT_DM_DATA2 g_firDecM2DelayQ[N_FIR_LP_M2_DELAY];

float SEGMENT_DM_DATA2 g_firDecM2DelayCh3I[N_FIR_LP_M2_DELAY];
float SEGMENT_DM_DATA2 g_firDecM2DelayCh3Q[N_FIR_LP_M2_DELAY];

float SEGMENT_DM_DATA2 g_firDecM2Poly0DelayI[N_FIR_LP_POLY0_M2_DELAY];
float SEGMENT_DM_DATA2 g_firDecM2Poly1DelayI[N_FIR_LP_POLY1_M2_DELAY];

float SEGMENT_DM_DATA2 g_firDecM2Poly0DelayQ[N_FIR_LP_POLY0_M2_DELAY];
float SEGMENT_DM_DATA2 g_firDecM2Poly1DelayQ[N_FIR_LP_POLY1_M2_DELAY];

float SEGMENT_DM_DATA2 g_firDecM3Poly0DelayI[N_FIR_LP_POLY0_M3_DELAY];
float SEGMENT_DM_DATA2 g_firDecM3Poly1DelayI[N_FIR_LP_POLY1_M3_DELAY];
float SEGMENT_DM_DATA2 g_firDecM3Poly2DelayI[N_FIR_LP_POLY2_M3_DELAY];

float SEGMENT_DM_DATA2 g_firDecM3Poly0DelayQ[N_FIR_LP_POLY0_M3_DELAY];
float SEGMENT_DM_DATA2 g_firDecM3Poly1DelayQ[N_FIR_LP_POLY1_M3_DELAY];
float SEGMENT_DM_DATA2 g_firDecM3Poly2DelayQ[N_FIR_LP_POLY2_M3_DELAY];

// interpolation equalizer state variables
static float dm g_eqIntM2StateI[EQ_M2_STATE];
static float dm g_eqIntM2StateQ[EQ_M2_STATE];

static float dm g_eqIntM2StateCh3I[EQ_M2_STATE];
static float dm g_eqIntM2StateCh3Q[EQ_M2_STATE];

static float dm g_eqIntM3StateI[EQ_M3_STATE];
static float dm g_eqIntM3StateQ[EQ_M3_STATE];


// decimation equalizer state variables
static float dm g_eqDecM2StateI[EQ_M2_STATE];
static float dm g_eqDecM2StateQ[EQ_M2_STATE];

static float dm g_eqDecM2StateCh3I[EQ_M2_STATE];
static float dm g_eqDecM2StateCh3Q[EQ_M2_STATE];

static float dm g_eqDecM3StateI[EQ_M3_STATE];
static float dm g_eqDecM3StateQ[EQ_M3_STATE];


/* GLOBAL VARIABLE DECLARATIONS **********************************************/

/* LOCAL FUNCTION DEFINITIONS ************************************************/

t_DecPolyM2Data g_decPolyM2Data;


/* IMPLEMENTATION ************************************************************/

// optimization is turned off for initialization functions in ext RAM
#pragma optimize_off

/*****************************************************************************
  Function:		initIntPolyM2()	
  Description:	Initializes polyphase interpolation parameters (int. factor: 2)
  				used for APLC channels 1 and 2.	
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:	Call before using intPolyM2() for channels 1 and 2.

******************************************************************************/

void SEGMENT_EXT_CODE initIntPolyM2(t_IntPolyM2Data *intData)
{
	long i;


	for (i = 0; i < EQ_M2_STATE; i++) {

		g_eqIntM2StateI[i] = 0.0;
		g_eqIntM2StateQ[i] = 0.0;
	};

	fir_f_init(g_firIntM2Poly0DelayI, N_FIR_M2_LP_POLY0_COEF);
	fir_f_init(g_firIntM2Poly0DelayQ, N_FIR_M2_LP_POLY0_COEF);
	
	fir_f_init(g_firIntM2Poly1DelayI, N_FIR_M2_LP_POLY1_COEF);
	fir_f_init(g_firIntM2Poly1DelayQ, N_FIR_M2_LP_POLY1_COEF);
		
	intData->poly0DelayI = g_firIntM2Poly0DelayI;
	intData->poly0DelayQ = g_firIntM2Poly0DelayQ;
	
	intData->poly1DelayI = g_firIntM2Poly1DelayI;
	intData->poly1DelayQ = g_firIntM2Poly1DelayQ;

	intData->eqStateI = g_eqIntM2StateI;
	intData->eqStateQ = g_eqIntM2StateQ; 

};


/*****************************************************************************
  Function:		initIntPolyM2Ch3()	
  Description:	Initializes polyphase interpolation parameters (int. factor: 2)
  				used for APLC channel 3.	
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:	Call before using intPolyM2() for channel 3.

******************************************************************************/

void SEGMENT_EXT_CODE initIntPolyM2Ch3(t_IntPolyM2Data *intData)
{
	long i;


	for (i = 0; i < EQ_M2_STATE; i++) {

		g_eqIntM2StateCh3I[i] = 0.0;
		g_eqIntM2StateCh3Q[i] = 0.0;
	};
		
	fir_f_init(g_firIntM2Poly0DelayCh3I, N_FIR_M2_LP_POLY0_COEF);
	fir_f_init(g_firIntM2Poly0DelayCh3Q, N_FIR_M2_LP_POLY0_COEF);
	
	fir_f_init(g_firIntM2Poly1DelayCh3I, N_FIR_M2_LP_POLY1_COEF);
	fir_f_init(g_firIntM2Poly1DelayCh3Q, N_FIR_M2_LP_POLY1_COEF);
	
	intData->poly0DelayI = g_firIntM2Poly0DelayCh3I;
	intData->poly0DelayQ = g_firIntM2Poly0DelayCh3Q;
	
	intData->poly1DelayI = g_firIntM2Poly1DelayCh3I;
	intData->poly1DelayQ = g_firIntM2Poly1DelayCh3Q;

	intData->eqStateI = g_eqIntM2StateCh3I;
	intData->eqStateQ = g_eqIntM2StateCh3Q; 
	
};


/*****************************************************************************
  Function:		initIntPolyM3()	
  Description:	Initializes polyphase interpolation parameters (int. factor: 3)
  				used for the aggregate APLC signal (all channels).	
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:	Call before using intPolyM3().

******************************************************************************/

void SEGMENT_EXT_CODE initIntPolyM3(void)
{
	long i;


	for (i = 0; i < EQ_M3_STATE; i++) {

		g_eqIntM3StateI[i] = 0.0;
		g_eqIntM3StateQ[i] = 0.0;
	};
	
	fir_f_init(g_firIntM3Poly0DelayI, N_FIR_M3_LP_POLY0_COEF);
	fir_f_init(g_firIntM3Poly0DelayQ, N_FIR_M3_LP_POLY0_COEF);
	
	fir_f_init(g_firIntM3Poly1DelayI, N_FIR_M3_LP_POLY1_COEF);
	fir_f_init(g_firIntM3Poly1DelayQ, N_FIR_M3_LP_POLY1_COEF);
	
	fir_f_init(g_firIntM3Poly2DelayI, N_FIR_M3_LP_POLY2_COEF);
	fir_f_init(g_firIntM3Poly2DelayQ, N_FIR_M3_LP_POLY2_COEF);
	
};


/*****************************************************************************
  Function:		initDecM3()	
  Description:	Initializes decimation parameters (dec. factor: 3) used for 
  				the aggregate APLC signal (all channels).	
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:	Call before using decimateM3().

******************************************************************************/

void SEGMENT_EXT_CODE initDecM3(void)
{
	
	fir_f_init(g_firDecM3DelayI, N_FIR_M3_LP_COEF);
	fir_f_init(g_firDecM3DelayQ, N_FIR_M3_LP_COEF);	
	
};


/*****************************************************************************
  Function:		initDecM2()	
  Description:	Initializes decimation parameters (dec. factor: 2) used for 
  				APLC channels 1 and 2.	
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:	Call before using decimateM2().

******************************************************************************/

void SEGMENT_EXT_CODE initDecM2(t_DecM2Data *decData)
{
	long i;


	for (i = 0; i < EQ_M2_STATE; i++) {

		g_eqDecM2StateI[i] = 0.0;
		g_eqDecM2StateQ[i] = 0.0;
	};

	fir_f_init(g_firDecM2DelayI, N_FIR_M2_LP_COEF);
	fir_f_init(g_firDecM2DelayQ, N_FIR_M2_LP_COEF);
	
	decData->delayI = g_firDecM2DelayI;
	decData->delayQ = g_firDecM2DelayQ;
	
	decData->eqStateI = g_eqDecM2StateI;
	decData->eqStateQ = g_eqDecM2StateQ; 
	
};


/*****************************************************************************
  Function:		initDecM2Ch3()	
  Description:	Initializes decimation parameters (dec. factor: 2) used for 
  				APLC channel 3.	
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:	Call before using decimateM2().

******************************************************************************/

void SEGMENT_EXT_CODE initDecM2Ch3(t_DecM2Data *decData)
{
	long i;


	for (i = 0; i < EQ_M2_STATE; i++) {

		g_eqDecM2StateCh3I[i] = 0.0;
		g_eqDecM2StateCh3Q[i] = 0.0;
	};
	
	fir_f_init(g_firDecM2DelayCh3I, N_FIR_M2_LP_COEF);
	fir_f_init(g_firDecM2DelayCh3Q, N_FIR_M2_LP_COEF);
	
	decData->delayI = g_firDecM2DelayCh3I;
	decData->delayQ = g_firDecM2DelayCh3Q;

	decData->eqStateI = g_eqDecM2StateCh3I;
	decData->eqStateQ = g_eqDecM2StateCh3Q; 
	
};


/*****************************************************************************
  Function:		initDecPolyM3()	
  Description:	Initializes polyphase decimation parameters (dec. factor: 3) 
  				used for the aggregate APLC signal (all channels).
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:	Call before using decPolyM3().

******************************************************************************/

void SEGMENT_EXT_CODE initDecPolyM3(void)
{
	long i;


	for (i = 0; i < EQ_M3_STATE; i++) {

		g_eqDecM3StateI[i] = 0.0;
		g_eqDecM3StateQ[i] = 0.0;
	};

	fir_f_init(g_firDecM3Poly0DelayI, N_FIR_M3_LP_POLY0_COEF);
	fir_f_init(g_firDecM3Poly1DelayI, N_FIR_M3_LP_POLY1_COEF);
	fir_f_init(g_firDecM3Poly2DelayI, N_FIR_M3_LP_POLY2_COEF);
	
	fir_f_init(g_firDecM3Poly0DelayQ, N_FIR_M3_LP_POLY0_COEF);
	fir_f_init(g_firDecM3Poly1DelayQ, N_FIR_M3_LP_POLY1_COEF);
	fir_f_init(g_firDecM3Poly2DelayQ, N_FIR_M3_LP_POLY2_COEF);
	
};


/*****************************************************************************
  Function:		initDecPolyM2()	
  Description:	Initializes polyphase decimation parameters (dec. factor: 2) 
  				used for APLC channels 1 and 2.
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:	Call before using decPolyM2().

******************************************************************************/

void SEGMENT_EXT_CODE initDecPolyM2(void)
{
	
	fir_f_init(g_firDecM2Poly0DelayI, N_FIR_M2_LP_POLY0_COEF);
	fir_f_init(g_firDecM2Poly1DelayI, N_FIR_M2_LP_POLY1_COEF);
	
	fir_f_init(g_firDecM2Poly0DelayQ, N_FIR_M2_LP_POLY0_COEF);
	fir_f_init(g_firDecM2Poly1DelayQ, N_FIR_M2_LP_POLY1_COEF);
	
	g_decPolyM2Data.poly0DelayI = g_firDecM2Poly0DelayI;
	g_decPolyM2Data.poly1DelayI = g_firDecM2Poly1DelayI;
	
	g_decPolyM2Data.poly0DelayQ = g_firDecM2Poly0DelayQ;
	g_decPolyM2Data.poly1DelayQ = g_firDecM2Poly1DelayQ;
				
};


// optimization is turned on as specified on the command line
#pragma optimize_as_cmd_line

/*****************************************************************************
  Function:		intPolyM2()	
  Description:	Polyphase interpolation stage (int. factor: 2).
  Inputs:		in: pointer to input sample array (length: 4)
  				param: interpolation paramters
  Outputs:		out: pointer to output sample array (length: 8)
  Return:		None
  Globals:		
  Important:	Call corresponding init. function before using.

******************************************************************************/

void intPolyM2(float *in, float *out, t_IntPolyM2Data *param)
{
	// seg_dmData used for correct alignment
	static float SEGMENT_DM_DATA2 inI[2], inQ[2];
	
	static float SEGMENT_DM_DATA2 outPoly0M2I[2], outPoly0M2Q[2];
	static float SEGMENT_DM_DATA2 outPoly1M2I[2], outPoly1M2Q[2];
	
	
	inI[0] = in[0];
	inI[1] = in[2];
	
	inQ[0] = in[1];
	inQ[1] = in[3];

	// interpolation predistortion filter
	BiIIR(inI, inI, eqIntDecM2Coef, param->eqStateI, EQ_M2_SEC, 2);
	BiIIR(inQ, inQ, eqIntDecM2Coef, param->eqStateQ, EQ_M2_SEC, 2);
	
	// polyphase filtering M = 2, in-phase component
	vfir_f(outPoly0M2I, inI, 2, firM2LpPoly0Coef, N_FIR_M2_LP_POLY0_COEF, param->poly0DelayI);
	vfir_f(outPoly1M2I, inI, 2, firM2LpPoly1Coef, N_FIR_M2_LP_POLY1_COEF, param->poly1DelayI);
	
	// polyphase filtering M = 2, quadrature component
	vfir_f(outPoly0M2Q, inQ, 2, firM2LpPoly0Coef, N_FIR_M2_LP_POLY0_COEF, param->poly0DelayQ);
	vfir_f(outPoly1M2Q, inQ, 2, firM2LpPoly1Coef, N_FIR_M2_LP_POLY1_COEF, param->poly1DelayQ);
	
	// rearrange output samples
	out[0] = outPoly0M2I[0];
	out[1] = outPoly1M2I[0];
	out[2] = outPoly0M2I[1];
	out[3] = outPoly1M2I[1];
	
	out[4] = outPoly0M2Q[0];
	out[5] = outPoly1M2Q[0];
	out[6] = outPoly0M2Q[1];
	out[7] = outPoly1M2Q[1];
	
};


/*****************************************************************************
  Function:		intPolyM3()	
  Description:	Polyphase interpolation stage (int. factor: 3).
  Inputs:		in: pointer to input sample array (length: 8)
  Outputs:		out: pointer to output sample array (length: 24)
  Return:		None
  Globals:		
  Important:	Call corresponding init. function before using.

******************************************************************************/

void intPolyM3(float *in, float *out)
{
	// seg_dmData used for correct alignment
	static float SEGMENT_DM_DATA2 inI[4], inQ[4];
	static float SEGMENT_DM_DATA2 outPoly0M3I[4], outPoly0M3Q[4];
	static float SEGMENT_DM_DATA2 outPoly1M3I[4], outPoly1M3Q[4];
	static float SEGMENT_DM_DATA2 outPoly2M3I[4], outPoly2M3Q[4];


	// interpolation predistortion filter
	BiIIR(in, inI, eqIntDecM3Coef, g_eqIntM3StateI, EQ_M3_SEC, 4);
	BiIIR(&in[4], inQ, eqIntDecM3Coef, g_eqIntM3StateQ, EQ_M3_SEC, 4);
	
	// polyphase filtering M = 3, in phase component
	vfir_f(outPoly0M3I, inI, 4, firM3LpPoly0Coef, N_FIR_M3_LP_POLY0_COEF, g_firIntM3Poly0DelayI);
	vfir_f(outPoly1M3I, inI, 4, firM3LpPoly1Coef, N_FIR_M3_LP_POLY1_COEF, g_firIntM3Poly1DelayI);
	vfir_f(outPoly2M3I, inI, 4, firM3LpPoly2Coef, N_FIR_M3_LP_POLY2_COEF, g_firIntM3Poly2DelayI);
	
	
	// polyphase filtering M = 3, quadrature component	
	vfir_f(outPoly0M3Q, inQ, 4, firM3LpPoly0Coef, N_FIR_M3_LP_POLY0_COEF, g_firIntM3Poly0DelayQ);
	vfir_f(outPoly1M3Q, inQ, 4, firM3LpPoly1Coef, N_FIR_M3_LP_POLY1_COEF, g_firIntM3Poly1DelayQ);
	vfir_f(outPoly2M3Q, inQ, 4, firM3LpPoly2Coef, N_FIR_M3_LP_POLY2_COEF, g_firIntM3Poly2DelayQ);
		
	// output calculation
/*	vcopy_s_DD(out,3,outPoly0M3I,1,4);
	vcopy_s_DD(&out[1],3,outPoly1M3I,1,4);
	vcopy_s_DD(&out[2],3,outPoly2M3I,1,4);
	
	vcopy_s_DD(&out[12],3,outPoly0M3Q,1,4);
	vcopy_s_DD(&out[13],3,outPoly1M3Q,1,4);
	vcopy_s_DD(&out[14],3,outPoly2M3Q,1,4);
	
	vsmul_f_DD(out, INT_M6_GAIN, out, 24);*/
			
	out[0] = INT_M6_GAIN*outPoly0M3I[0];
	out[1] = INT_M6_GAIN*outPoly1M3I[0];
	out[2] = INT_M6_GAIN*outPoly2M3I[0];
	
	out[3] = INT_M6_GAIN*outPoly0M3I[1];
	out[4] = INT_M6_GAIN*outPoly1M3I[1];
	out[5] = INT_M6_GAIN*outPoly2M3I[1];
	
	out[6] = INT_M6_GAIN*outPoly0M3I[2];
	out[7] = INT_M6_GAIN*outPoly1M3I[2];
	out[8] = INT_M6_GAIN*outPoly2M3I[2];
	
	out[9] =  INT_M6_GAIN*outPoly0M3I[3];
	out[10] = INT_M6_GAIN*outPoly1M3I[3];
	out[11] = INT_M6_GAIN*outPoly2M3I[3];
	
	out[12] = INT_M6_GAIN*outPoly0M3Q[0];
	out[13] = INT_M6_GAIN*outPoly1M3Q[0];
	out[14] = INT_M6_GAIN*outPoly2M3Q[0];
	
	out[15] = INT_M6_GAIN*outPoly0M3Q[1];
	out[16] = INT_M6_GAIN*outPoly1M3Q[1];
	out[17] = INT_M6_GAIN*outPoly2M3Q[1];
	
	out[18] = INT_M6_GAIN*outPoly0M3Q[2];
	out[19] = INT_M6_GAIN*outPoly1M3Q[2];
	out[20] = INT_M6_GAIN*outPoly2M3Q[2];
	
	out[21] = INT_M6_GAIN*outPoly0M3Q[3];
	out[22] = INT_M6_GAIN*outPoly1M3Q[3];
	out[23] = INT_M6_GAIN*outPoly2M3Q[3];
};


/*****************************************************************************
  Function:		decimateM3()	
  Description:	Decimation stage (dec. factor: 3).
  Inputs:		in: pointer to input sample array (length: 24)
  Outputs:		out: pointer to output sample array (length: 8)
  Return:		None
  Globals:		
  Important:	Call corresponding init. function before using.

******************************************************************************/

void decimateM3(float *in, float *out)
{
	// seg_dmData used for correct alignment
	static float SEGMENT_DM_DATA2 outSampleI[12];
	static float SEGMENT_DM_DATA2 outSampleQ[12];

		
	// decimation filters (M = 3)	
	vfir_f(outSampleI, in, 12, firM3LpCoef, N_FIR_M3_LP_COEF, g_firDecM3DelayI);
	vfir_f(outSampleQ, &in[12], 12, firM3LpCoef, N_FIR_M3_LP_COEF, g_firDecM3DelayQ);
	
	// decimate
	out[0] = outSampleI[0];
	out[1] = outSampleI[3];
	out[2] = outSampleI[6];
	out[3] = outSampleI[9];
	
	out[4] = outSampleQ[0];
	out[5] = outSampleQ[3];
	out[6] = outSampleQ[6];
	out[7] = outSampleQ[9];
};


/*****************************************************************************
  Function:		decimateM2()	
  Description:	Decimation stage (dec. factor: 2).
  Inputs:		in: pointer to input sample array (length: 8)
  				param: decimation paramters
  Outputs:		out: pointer to output sample array (length: 4)
  Return:		None
  Globals:		
  Important:	Call corresponding init. function before using.

******************************************************************************/

void decimateM2(float *in, float *out, t_DecM2Data *param)
{
	// seg_dmData used for correct alignment
	static float SEGMENT_DM_DATA2 outSampleI[4];
	static float SEGMENT_DM_DATA2 outSampleQ[4];

	static float decOutI[2];
	static float decOutQ[2];
	
	
	// decimation filters (M = 2)
	vfir_f(outSampleI, in, 4, firM2LpCoef, N_FIR_M2_LP_COEF, param->delayI);
	vfir_f(outSampleQ, &in[4], 4, firM2LpCoef, N_FIR_M2_LP_COEF, param->delayQ);

	// decimate
	decOutI[0] = outSampleI[0];
	decOutI[1] = outSampleI[2];
	decOutQ[0] = outSampleQ[0];
	decOutQ[1] = outSampleQ[2];

	// dec filter equalizer
	BiIIR(decOutI, decOutI, eqIntDecM2Coef, param->eqStateI, EQ_M2_SEC, 2);
	BiIIR(decOutQ, decOutQ, eqIntDecM2Coef, param->eqStateQ, EQ_M2_SEC, 2);
		
	// rearrange output samples
	out[0] = decOutI[0];
	out[1] = decOutQ[0];
	out[2] = decOutI[1];
	out[3] = decOutQ[1];
};


/*****************************************************************************
  Function:		decPolyM3()	
  Description:	Polyphase decimation stage (dec. factor: 3).
  Inputs:		in: pointer to input sample array (length: 24)
  Outputs:		out: pointer to output sample array (length: 8)
  Return:		None
  Globals:		
  Important:	Call corresponding init. function before using.

******************************************************************************/

void decPolyM3(float *in, float *out)
{
	// seg_dmData used for correct alignment
	static float SEGMENT_DM_DATA2 inPoly0M3I[4], inPoly0M3Q[4];
	static float SEGMENT_DM_DATA2 inPoly1M3I[4], inPoly1M3Q[4];
	static float SEGMENT_DM_DATA2 inPoly2M3I[4], inPoly2M3Q[4];
	
	static float SEGMENT_DM_DATA2 outPoly0M3I[4], outPoly0M3Q[4];
	static float SEGMENT_DM_DATA2 outPoly1M3I[4], outPoly1M3Q[4];
	static float SEGMENT_DM_DATA2 outPoly2M3I[4], outPoly2M3Q[4];

	static float SEGMENT_DM_DATA2 outI[4], outQ[4];
	

	// polyphase filter input allocation, in-phase component
	inPoly0M3I[0] = in[2];
	inPoly0M3I[1] = in[5];
	inPoly0M3I[2] = in[8];
	inPoly0M3I[3] = in[11];
	
	inPoly1M3I[0] = in[1];
	inPoly1M3I[1] = in[4];
	inPoly1M3I[2] = in[7];
	inPoly1M3I[3] = in[10];
	
	inPoly2M3I[0] = in[0];
	inPoly2M3I[1] = in[3];
	inPoly2M3I[2] = in[6];
	inPoly2M3I[3] = in[9];
	
	// polyphase filter input allocation, quadrature component
	inPoly0M3Q[0] = in[14];
	inPoly0M3Q[1] = in[17];
	inPoly0M3Q[2] = in[20];
	inPoly0M3Q[3] = in[23];
	
	inPoly1M3Q[0] = in[13];
	inPoly1M3Q[1] = in[16];
	inPoly1M3Q[2] = in[19];
	inPoly1M3Q[3] = in[22];
	
	inPoly2M3Q[0] = in[12];
	inPoly2M3Q[1] = in[15];
	inPoly2M3Q[2] = in[18];
	inPoly2M3Q[3] = in[21];
	
	
	// polyphase filtering M = 3, in-phase component
	vfir_f(outPoly0M3I, inPoly0M3I, 4, firM3LpPoly0Coef, N_FIR_M3_LP_POLY0_COEF, g_firDecM3Poly0DelayI);
	vfir_f(outPoly1M3I, inPoly1M3I, 4, firM3LpPoly1Coef, N_FIR_M3_LP_POLY1_COEF, g_firDecM3Poly1DelayI);
	vfir_f(outPoly2M3I, inPoly2M3I, 4, firM3LpPoly2Coef, N_FIR_M3_LP_POLY2_COEF, g_firDecM3Poly2DelayI);
	
	// polyphase filtering M = 3, quadrature component
	vfir_f(outPoly0M3Q, inPoly0M3Q, 4, firM3LpPoly0Coef, N_FIR_M3_LP_POLY0_COEF, g_firDecM3Poly0DelayQ);
	vfir_f(outPoly1M3Q, inPoly1M3Q, 4, firM3LpPoly1Coef, N_FIR_M3_LP_POLY1_COEF, g_firDecM3Poly1DelayQ);
	vfir_f(outPoly2M3Q, inPoly2M3Q, 4, firM3LpPoly2Coef, N_FIR_M3_LP_POLY2_COEF, g_firDecM3Poly2DelayQ);
	
	// polyphase filter output calculation, in-phase component
	outI[0] = outPoly0M3I[0] + outPoly1M3I[0] + outPoly2M3I[0];		
	outI[1] = outPoly0M3I[1] + outPoly1M3I[1] + outPoly2M3I[1];		
	outI[2] = outPoly0M3I[2] + outPoly1M3I[2] + outPoly2M3I[2];		
	outI[3] = outPoly0M3I[3] + outPoly1M3I[3] + outPoly2M3I[3];	
	
	// polyphase filter output calculation, quadrature component
	outQ[0] = outPoly0M3Q[0] + outPoly1M3Q[0] + outPoly2M3Q[0];		
	outQ[1] = outPoly0M3Q[1] + outPoly1M3Q[1] + outPoly2M3Q[1];		
	outQ[2] = outPoly0M3Q[2] + outPoly1M3Q[2] + outPoly2M3Q[2];		
	outQ[3] = outPoly0M3Q[3] + outPoly1M3Q[3] + outPoly2M3Q[3];

	// dec filter equalizer
	BiIIR(outI, out, eqIntDecM3Coef, g_eqDecM3StateI, EQ_M3_SEC, 4);
	BiIIR(outQ, &out[4], eqIntDecM3Coef, g_eqDecM3StateQ, EQ_M3_SEC, 4);
	
};


/*****************************************************************************
  Function:		decPolyM2()	
  Description:	Polyphase decimation stage (dec. factor: 2).	
  Inputs:		in: pointer to input sample array (length: 8)
  Outputs:		out: pointer to output sample array (length: 4)
  Return:		None
  Globals:		
  Important:	Call corresponding init. function before using.

******************************************************************************/

void decPolyM2(float *in, float *out, t_DecPolyM2Data *param)
{
	// seg_dmData used for correct alignment
	static float SEGMENT_DM_DATA2 inPoly0M2I[2], inPoly0M2Q[2];
	static float SEGMENT_DM_DATA2 inPoly1M2I[2], inPoly1M2Q[2];
	
	static float SEGMENT_DM_DATA2 outPoly0M2I[2], outPoly0M2Q[2];
	static float SEGMENT_DM_DATA2 outPoly1M2I[2], outPoly1M2Q[2];

	static float decOutI[2];
	static float decOutQ[2];

	
	// polyphase filter input allocation, in-phase component
	inPoly0M2I[0] = in[1];
	inPoly0M2I[1] = in[3];
	
	inPoly1M2I[0] = in[0];
	inPoly1M2I[1] = in[2];
	
	// polyphase filter input allocation, quadrature component
	inPoly0M2Q[0] = in[5];
	inPoly0M2Q[1] = in[7];
	
	inPoly1M2Q[0] = in[4];
	inPoly1M2Q[1] = in[6];
			
	// polyphase filtering M = 2, in-phase component
	vfir_f(outPoly0M2I, inPoly0M2I, 2, firM2LpPoly0Coef, N_FIR_M2_LP_POLY0_COEF, param->poly0DelayI);
	vfir_f(outPoly1M2I, inPoly1M2I, 2, firM2LpPoly1Coef, N_FIR_M2_LP_POLY1_COEF, param->poly1DelayI);
	
	// polyphase filtering M = 2, quadrature component
	vfir_f(outPoly0M2Q, inPoly0M2Q, 2, firM2LpPoly0Coef, N_FIR_M2_LP_POLY0_COEF, param->poly0DelayQ);
	vfir_f(outPoly1M2Q, inPoly1M2Q, 2, firM2LpPoly1Coef, N_FIR_M2_LP_POLY1_COEF, param->poly1DelayQ);
	
	// output calculations
	decOutI[0] = outPoly0M2I[0] + outPoly1M2I[0];
	decOutI[1] = outPoly0M2I[1] + outPoly1M2I[1];

	decOutQ[0] = outPoly0M2Q[0] + outPoly1M2Q[0];
	decOutQ[1] = outPoly0M2Q[1] + outPoly1M2Q[1];

	// dec filter equalizer
	BiIIR(decOutI, decOutI, eqIntDecM2Coef, param->eqStateI, EQ_M2_SEC, 2);
	BiIIR(decOutQ, decOutQ, eqIntDecM2Coef, param->eqStateQ, EQ_M2_SEC, 2);

	// rearrange output samples
	out[0] = decOutI[0];
	out[2] = decOutI[1];
	
	out[1] = decOutQ[0];
	out[3] = decOutQ[1];
};




 	
