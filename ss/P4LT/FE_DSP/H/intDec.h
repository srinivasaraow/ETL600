/*---------------------------------------------------------------------------*
 * $Workfile: intDec.h $
 * Part of      : ETL600 / Front end DSP
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Global function and type declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/FE_DSP/H/intDec.h $
 * 
 * 1     19.01.05 15:32 Chchlee
 * 
 * 2     18.10.04 17:58 Maag01
 * 
 * 1     30.04.04 15:04 Maag01
 * Initial version
 * 
 *
 *---------------------------------------------------------------------------*/

#ifndef _INT_DEC
#define _INT_DEC


/* GLOBAL TYPE DECLARATIONS **************************************************/

typedef struct {
	
	float *poly0DelayI;
	float *poly1DelayI;
	float *poly0DelayQ;
	float *poly1DelayQ;

	float *eqStateI;
	float *eqStateQ;
	
} t_IntPolyM2Data;


typedef struct {
	
	float *delayI;
	float *delayQ;

	float *eqStateI;
	float *eqStateQ;

} t_DecM2Data;


typedef struct {
	
	float *poly0DelayI;
	float *poly1DelayI;
	float *poly0DelayQ;
	float *poly1DelayQ;

	float *eqStateI;
	float *eqStateQ;
	
} t_DecPolyM2Data;

 
/* PUBLIC FUNCTIONS **********************************************************/

void initIntPolyM2(t_IntPolyM2Data *intData);
void initIntPolyM2Ch3(t_IntPolyM2Data *intData);
void initIntPolyM3(void);

void intPolyM2(float *in, float *out, t_IntPolyM2Data *param);
void intPolyM3(float *in, float *out);

void initDecM3(void);
void initDecPolyM3(void);
void initDecM2(t_DecM2Data *decData);
void initDecM2Ch3(t_DecM2Data *decData);

void decimateM3(float *in, float *out);
void decPolyM3(float *in, float *out);
void decimateM2(float *in, float *out, t_DecM2Data *param);

#endif
