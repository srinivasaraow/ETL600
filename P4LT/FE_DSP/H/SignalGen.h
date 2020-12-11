/*-------------------------------------------------------------------------*
 * $Workfile: SignalGen.h $
 * Part of	: ETL600 / P4LT
 * Language	: C
 * Created by: 
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/FE_DSP/H/SignalGen.h $
 * 
 * 1     19.01.05 15:32 Chchlee
 * 
 * 1     30.04.04 15:17 Maag01
 * Initial version based on ETL500 Version 4.20 (signal_p.c)
 * 
 * Initial Revision
 *-------------------------------------------------------------------------*/
#ifndef _SIGNAL_GEN
#define _SIGNAL_GEN 
   
typedef struct  OscIQ {

	float tps_tx_sin;
	float tps_tx_cos;            
	float sin_b;
	float cos_b;
	float amplitude;
	float f_sample;
	float f_transmit;

} OscIQType;

typedef struct OscIQ *OscIQPtr;

// function prototypes

void GenIQAmpCorr(OscIQPtr );
void InitGenIQ(OscIQPtr ,float , float , float );
void GenIQ(OscIQPtr , float *, float * );
//void noise_gen_float(float *buffer_ptr);
//void InitTwoToneGenerator(void);
//void TwoToneGenerator(float *TxPtr);
//void TwoToneGenerator2Channel(float *TxPtr);   

#endif	
