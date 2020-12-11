/*-------------------------------------------------------------------------*
 * $Workfile: SSB.h $
 * Part of	: ETL600
 * Language	: C
 * Created by	: 
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source HSD600/P4LT/FE_DSP/H/SSB.h $
 * 
 * 4     31.01.06 16:28 Chpeplu
 * struct renamed t_ssbData1Ch and parameter list changed
 * 
 * 2     8.06.05 11:50 Chhamaa
 * txAfSignalMux() function definition modified
 * 
 * 1     19.01.05 15:32 Chchlee
 * 
 * 7     16.12.04 16:18 Chhamaa
 * txAfSignalMux() created
 * 
 * 6     1.11.04 14:17 Maag01
 * AP filters moved form ssbDemodulator to ssbModulator
 * 
 * 5     23.09.04 18:57 Maag01
 * 
 * 4     15.09.04 18:42 Maag01
 * 
 * 3     14.09.04 16:21 Maag01
 * 
 * 2     20.07.04 13:38 Maag01
 * 
 * 1     30.04.04 15:18 Maag01
 * Initial version based on ETL500 Version 4.20
 * 
 * Initial Revision
 *-------------------------------------------------------------------------*/
 
#ifndef _SSB
#define _SSB

#include "SignalGen.h"

/*-----------------------GLOBAL DATA DEFINITIONS---------------------------*/

typedef struct {
	
	// sign of SSB modulator/ demodulator
	float sm1;	 
	float sm2; 
	float sd11;
	float sd12;
	
	// oscillator paramerters
	OscIQType *oscMod;
	OscIQType *oscDemod;  
	
	// SSB filter parameters
	float pm *filterCoef;		// pointer to SSB filter coefficients
	int filterSec;				// number of SSB filter sections

	// pointers to ssb filter states 
	float *txFilterStateS;
	float *txFilterStateC;
	float *rxFilterStateS;
	float *rxFilterStateC;
	
} t_ssbData1Ch;


/*-----------------------GLOBAL FUNCTION DEFINITIONS-----------------------*/

void setHFConfig(void);

void initSSB1Ch_Ch1(t_FrontEndDSP_Config *FE_DSP_Config, t_ssbData1Ch *ssbData);
void initSSB1Ch_Ch3(t_FrontEndDSP_Config *FE_DSP_Config, t_ssbData1Ch *ssbData);
void initSSB2Ch(t_FrontEndDSP_Config *FE_DSP_Config);

void txAfSignalMux(float txAf[], float txNsk[], float txTestTone[], float txAfSum[]);

void ssbModulator1Ch(float *TxAfPtr, float *TxNsdPtr, float *TxAfssbModPtr, t_ssbData1Ch *ssbData);
void ssbModulator2Ch(float *TxAfPtr, float *TxNsdPtr, float *TxAfssbModPtr);

void ssbDemodulator1Ch(float *RxAfdezimPtr, float *RxAfSsbDemodPtr, t_ssbData1Ch *ssbData); 
void ssbDemodulator2Ch(float *RxAfdezimPtr, float *RxAfSsbDemodPtr);
      
#endif
