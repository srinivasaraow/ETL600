/*-------------------------------------------------------------------------*
 * $Workfile: SSB.c $
 * Part of	: ETL600 P4LT
 * Language	: C
 * Created by :	H.-J. Maag
 * Remarks	:  
 * Purpose	: SSB  Modulator/Demodulator 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/P4LT/FE_DSP/C/SSB.c $
 * 
 * 17    12-09-14 16:47 Chhamaa
 * Modifications for APLC BW = 2.5 kHz.
 * 
 * 15    9.05.08 18:23 Chhamaa
 * New APLC modes added for MOD above speech.
 * 
 * 10    31.01.06 16:28 Chpeplu
 * new mode 2kHz added
 * 
 * 8     28.10.05 14:43 Chhamaa
 * ASSERTs replaced by default settings
 * 
 * 7     20.07.05 14:30 Chhamaa
 * Limiter added
 * 
 * 6     28.06.05 11:56 Chchlee
 * Cleaned up, ASSERTs consolidated
 * 
 * 5     8.06.05 11:51 Chhamaa
 * txAfSignalMux() modified
 * 
 * 4     12.04.05 16:37 Chhamaa
 * 
 * 3     12.04.05 15:48 Chhamaa
 * SSB AP filters disabled
 * 
 * 2     4.03.05 18:51 Chhamaa
 * Constants related to pilot weight removed
 * 
 * 1     19.01.05 15:31 Chchlee
 * 
 * 21    16.12.04 16:18 Chhamaa
 * txAfSignalMux() created
 * 
 * 20    23.11.04 17:07 Maag01
 * 
 * 19    1.11.04 14:16 Maag01
 * AP filters moved form ssbDemodulator to ssbModulator
 * 
 * 18    28.10.04 18:19 Maag01
 * APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_40I added
 * 
 * 17    18.10.04 17:55 Maag01
 * 
 * 16    7.10.04 14:03 Maag01
 * 
 * 15    6.10.04 17:49 Maag01
 * 
 * 14    5.10.04 18:26 Maag01
 * 
 * 13    23.09.04 18:57 Maag01
 * 
 * 12    15.09.04 18:42 Maag01
 * 
 * 11    14.09.04 16:22 Maag01
 * 
 * 10    9.09.04 15:52 Maag01
 * 
 * 9     26.08.04 15:21 Maag01
 * 
 * 8     2.08.04 14:36 Leeb02
 * N_2 removed
 * 
 * 7     20.07.04 13:38 Maag01
 * 
 * 6     14.07.04 12:18 Leeb02
 * Dependencies removed
 * 
 * 5     10.05.04 18:29 Maag01
 * 
 * 4     7.05.04 17:10 Maag01
 * 
 * 3     4.05.04 16:10 Maag01
 * 
 * 2     30.04.04 16:36 Maag01
 * 
 * 1     30.04.04 14:56 Maag01
 * Initial version based on ETL500 Version 4.20
 * 
 *  
 * Initial Revision
 *-------------------------------------------------------------------------*/

/*-----------------------GLOBAL CONSTANTS----------------------------------*/

#include "basetype.h"
#include "diag.h"
#include "speeddsp.h"

#include "constant.h"
#include "FE_DSP_Const.h"
#include "FE_DSP_Types.h"
#include "SSB_Coef.h"        
#include "SSB.h"   


/*-----------------------LOCAL CONSTANTS-----------------------------------*/

#define Fm_BW2000       1150.0   		// 1. modulation frequency, BW = 2.0 kHz
#define Fm_BW2500		1400.0			// 1. modulation frequency, BW = 2.5 kHz
#define Fm_BW4000       2150.0   		// 1. modulation frequency, BW = 4.0 kHz 
#define Fm2_BW4000      2000.0   		// 2. modulation frequency, BW = 4.0 kHz, 2-ch mode 

#define ENV_SQR_MAX		0.96			// limiter max. envelope squared


/*-----------------------LOCAL DATA DEFINITIONS----------------------------*/

static t_RF_Config RF;

static OscIQType fmModulator1;      					// Oszillator für die Modulation   mit fm
static OscIQType fmDemodulator1;    					// Oszillator für die Demodulation mit fm
static OscIQType fmModulator2;     						// Oszillator für die Modulation   mit fm2
static OscIQType fmDemodulator2;  						// Oszillator für die Demodulation mit fm2
static OscIQType fmModulator3;      					// Oszillator für die Modulation   mit fm (Ch3)
static OscIQType fmDemodulator3;    					// Oszillator für die Demodulation mit fm (Ch3)

static float dm ssbTxFilterStateS_CH1[SSB_STATE_MAX];	// Zustand des Tx-SSB-Filter vom I-Signal des 1.Kanals 
static float dm ssbTxFilterStateC_CH1[SSB_STATE_MAX];	// Zustand des Tx-SSB-Filter vom Q-Signal des 1.Kanals   
static float dm ssbRxFilterStateS_CH1[SSB_STATE_MAX];	// Zustand des Rx-SSB-Filter vom I-Signal des 1.Kanals
static float dm ssbRxFilterStateC_CH1[SSB_STATE_MAX];	// Zustand des Rx-SSB-Filter vom Q-Signal des 1.Kanals     

static float dm ssbTxFilterStateS_CH2[SSB_STATE_MAX];	// Zustand des Tx-SSB-Filter vom I-Signal des 2.Kanals
static float dm ssbTxFilterStateC_CH2[SSB_STATE_MAX];	// Zustand des Tx-SSB-Filter vom Q-Signal des 2.Kanals     
static float dm ssbRxFilterStateS_CH2[SSB_STATE_MAX];	// Zustand des Rx-SSB-Filter vom I-Signal des 2.Kanals
static float dm ssbRxFilterStateC_CH2[SSB_STATE_MAX];	// Zustand des Rx-SSB-Filter vom Q-Signal des 2.Kanals

static float dm ssbTxFilterStateS_CH3[SSB_STATE_MAX];	// Zustand des Tx-SSB-Filter vom I-Signal des 3.Kanals
static float dm ssbTxFilterStateC_CH3[SSB_STATE_MAX];	// Zustand des Tx-SSB-Filter vom Q-Signal des 3.Kanals     
static float dm ssbRxFilterStateS_CH3[SSB_STATE_MAX];	// Zustand des Rx-SSB-Filter vom I-Signal des 3.Kanals
static float dm ssbRxFilterStateC_CH3[SSB_STATE_MAX];	// Zustand des Rx-SSB-Filter vom Q-Signal des 3.Kanals

static float sm1, sm2, sm3, sm4, sm5, sm6, sm7, sm8;          // Vorzeichen SSB-Modulator 2-Ch
static float sd11, sd12, sd13, sd14, sd21, sd22, sd23, sd24;  // Vorzeichen SSB-Demodulator 2-Ch


#define SQR(x) ((x)*(x))


/********************************************************************************  
*	SSB - MODULATOR für 1 Kanal                                                	*
*	============================                                             	*
* 																				*
*                                                                               *
*	Multiplexer und SSB Modulator für 1 -Kanalgerät, symmetrische Erzeugung     *
*	Vorgaben in RR-NTC96-1528                                                   *
*	Definition der Funktionen in :	PS8311.DAT  (bilde Summensignal)            *
*					              	PS8312.DAT  (Skalierung Tx)                 *
*                                  	PS8313.DAT  (SSB Modulator)                 *
*                                                                               *
*	Fehlende Funktionalität in:	                                                *
*	- PS8311.DAT :	Kanal2, Tx TPS Signal CH1, Loop                             *
*	- PS8312.DAT :	-                                                           *
*	- PS8313.DAT :	wählbare Bandbreiten                                        *
*	                                                                            *
*	Zusätzliche Funktionalität:                                                 *
*	- NF Hochpass (TM-NT96-1627/_)                                              *
*	- Post Equalizer und zusätzliche Skalierung wegen der Interpolation        	*
*	  (PS8314.DAT)                                                              *
*	- variable Blocklänge                                                       *
*	                                                                            *
*	Aenderungen im Datenfluss:                                                  *
*	- PS8312.DAT  (Skalierung Tx) kommt nach PS8313.DAT  (SSB Modulator)        *
*	                                                                            *
*	Die Implementierung besteht aus einem einmaligen Teil, welcher beim Auf-	*
*	starten oder bei einer Konfigurationsänderung aufgerufen wird und einem 	*
*	periodischen Teil.															* 
*	  									                                        *
*	Einmaliger Teil                                                             *
*	- wird genannt initSSB1Ch                                                   *
*	- ist für die SSB Modulation und Demodulation gemeinsam                     *
*	- bestimmt aus der Kanallage das richtige Vorzeichen   (*)                  *
*	- bestimmt aus der Bandbreite die Filterkoeffizienten und -länge (TAPS)	 	*
*	  des SSB Tiefpasses (*)													* 
*	- bestimmt aus der Bandbreite die Modulationsfrequenz  und initialisiert den*
*	  komplexen Generator mit den Umsetzfrequenzen 2150 Hz, 1150 Hz oder 		*
*	  1400 Hz(**)																*
*	- initialisiert die Statusspeicher für die folgenden Filter                 *
*		-> SSB Tiefpass                                                         *
*		-> NF Hochpass                                                          *
*		-> Pre/Post Entzerrer                                                   *
*	                                                                            *
*	(*)	 für 1 Kanal Version implementiert                                      *
*	(**) für Bandbreite 4kHz implementiert                                      *
*		                                                                        *
*	periodischen Teil                                                           *
*	- wird genannt ssbModulator1Ch                                              *
*	- setzt sich aus dem Multiplexer, Skalierer und Modulator zusammen          *
*	- führt den NF- Teil der Weaver SSB - Modulation durch                      *
*	- implementiert mit Blockverarbeitung                                       *
*	- skaliert richtig für die folgende Interpolation                           *
*		                                                                        *
*********************************************************************************	
*/


/*------------------------------------------------------------------------------
* void initSSB1Ch(void)											    			*
*                                                                         	 	*
*	Variables:                                                             	 	*
*       IN:		-      	       												  	*
*       OUT:	- 					                                            * 
*                                                                               *
*       extern    	ActualConfigData.HF.HF_Bandbreite                           *
*                 	ActualConfigData.HF.Kanallage                               *
*                                                                               *
*   Purpose:                                                                    *
*   initializes all static variables in this file ssb.c for the one channel     *
*   modulation and demodulation                                                 *
*------------------------------------------------------------------------------*/

#pragma optimize_off

void SEGMENT_EXT_CODE initSSB1Ch_Ch1(t_FrontEndDSP_Config *FE_DSP_Config, t_ssbData1Ch *ssbData)
{
	long i;
	float fm; 
	
	
    switch (FE_DSP_Config->channelMode)
    {
		case APLC_CHANNEL_MODE_A1_20E:
			
			fm = Fm_BW2000;

			ssbData->filterCoef = ssbCoef2kHz;
			ssbData->filterSec = SSB_SEC_2KHZ;

			ssbData->sm1 = 1.0;    
    		ssbData->sm2 = -1.0;  
    		ssbData->sd11 = 1.0;
    		ssbData->sd12 = 1.0;
    		break;

		case APLC_CHANNEL_MODE_A1_20I:

			fm = Fm_BW2000;
			
			ssbData->filterCoef = ssbCoef2kHz;
			ssbData->filterSec = SSB_SEC_2KHZ;

			ssbData->sm1 = 1.0;    
    		ssbData->sm2 = 1.0;  
    		ssbData->sd11 = 1.0;
    		ssbData->sd12 = -1.0;
    		break;

		case APLC_CHANNEL_MODE_A1_25E:
			
			fm = Fm_BW2500;

			ssbData->filterCoef = ssbCoef2_5kHz;
			ssbData->filterSec = SSB_SEC_2_5KHZ;

			ssbData->sm1 = 1.0;    
    		ssbData->sm2 = -1.0;  
    		ssbData->sd11 = 1.0;
    		ssbData->sd12 = 1.0;
    		break;

		case APLC_CHANNEL_MODE_A1_25I:

			fm = Fm_BW2500;
			
			ssbData->filterCoef = ssbCoef2_5kHz;
			ssbData->filterSec = SSB_SEC_2_5KHZ;

			ssbData->sm1 = 1.0;    
    		ssbData->sm2 = 1.0;  
    		ssbData->sd11 = 1.0;
    		ssbData->sd12 = -1.0;
    		break;

		case APLC_CHANNEL_MODE_A1_20E_MAS:	// MOD above speech
		case APLC_CHANNEL_MODE_A1_40E:
		
			fm = Fm_BW4000;

			ssbData->filterCoef = ssbCoef4kHz;
			ssbData->filterSec = SSB_SEC_4KHZ;

			ssbData->sm1 = 1.0;    
    		ssbData->sm2 = -1.0;  
    		ssbData->sd11 = 1.0;
    		ssbData->sd12 = 1.0;
    		break;

		case APLC_CHANNEL_MODE_A1_40I: 
		
			fm = Fm_BW4000;

			ssbData->filterCoef = ssbCoef4kHz;
			ssbData->filterSec = SSB_SEC_4KHZ;

			ssbData->sm1 = 1.0;    
    		ssbData->sm2 = 1.0;  
    		ssbData->sd11 = 1.0;
    		ssbData->sd12 = -1.0;
    		break;    
    				
		default:	// settings as APLC_CHANNEL_MODE_A1_40I: 
		
			fm = Fm_BW4000;

			ssbData->filterCoef = ssbCoef4kHz;
			ssbData->filterSec = SSB_SEC_4KHZ;

			ssbData->sm1 = 1.0;    
    		ssbData->sm2 = 1.0;  
    		ssbData->sd11 = 1.0;
    		ssbData->sd12 = -1.0;
	}
 
	InitGenIQ(&fmModulator1,   fm, AF_SAMPLE_FREQ_4K, 1.0);  
	InitGenIQ(&fmDemodulator1, fm, AF_SAMPLE_FREQ_4K, 1.0); 
	
	ssbData->oscMod =  &fmModulator1;
	ssbData->oscDemod =  &fmDemodulator1;
    
	// initialize SSB filter state arrays
    for (i = 0; i < SSB_STATE_MAX; i++) {
		
		ssbTxFilterStateS_CH1[i] = 0.0;
		ssbTxFilterStateC_CH1[i] = 0.0;
  		ssbRxFilterStateS_CH1[i] = 0.0;
		ssbRxFilterStateC_CH1[i] = 0.0;
	}; 
	
	ssbData->txFilterStateS = ssbTxFilterStateS_CH1;
	ssbData->txFilterStateC = ssbTxFilterStateC_CH1;
	ssbData->rxFilterStateS = ssbRxFilterStateS_CH1;
	ssbData->rxFilterStateC = ssbRxFilterStateC_CH1;
        	
}


void SEGMENT_EXT_CODE initSSB1Ch_Ch3(t_FrontEndDSP_Config *FE_DSP_Config, t_ssbData1Ch *ssbData)
{
	long i;
	float fm;
	

	fm = Fm_BW4000;

	ssbData->filterCoef = ssbCoef4kHz;
	ssbData->filterSec = SSB_SEC_4KHZ;
 	
    switch (FE_DSP_Config->channelMode)
    {	
		case APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_20E:	// MOD above speech
		case APLC_CHANNEL_MODE_A1_40I_A2_40E_A3_40E:
		case APLC_CHANNEL_MODE_A1_32I_A2_24E_A3_24E:
			
			ssbData->sm1 = 1.0;    
    		ssbData->sm2 = -1.0;  
    		ssbData->sd11 = 1.0;
    		ssbData->sd12 = 1.0;
    		break;

		case  APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_40I: 
		
			ssbData->sm1 = 1.0;    
    		ssbData->sm2 = 1.0;  
    		ssbData->sd11 = 1.0;
    		ssbData->sd12 = -1.0;
    		break;
    									
		default:	// settings as APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_40I
		
			ssbData->sm1 = 1.0;    
    		ssbData->sm2 = 1.0;  
    		ssbData->sd11 = 1.0;
    		ssbData->sd12 = -1.0;
				
	}
 
	InitGenIQ(&fmModulator3,   fm, AF_SAMPLE_FREQ_4K, 1.0);  
	InitGenIQ(&fmDemodulator3, fm, AF_SAMPLE_FREQ_4K, 1.0); 
	
	ssbData->oscMod =  &fmModulator3;
	ssbData->oscDemod =  &fmDemodulator3; 
    
	// initialize SSB filter state arrays
    for (i = 0; i < SSB_STATE_MAX; i++) {
		
		ssbTxFilterStateS_CH3[i] = 0.0;
		ssbTxFilterStateC_CH3[i] = 0.0;
  		ssbRxFilterStateS_CH3[i] = 0.0;
		ssbRxFilterStateC_CH3[i] = 0.0;
	};
	
	ssbData->txFilterStateS = ssbTxFilterStateS_CH3;
	ssbData->txFilterStateC = ssbTxFilterStateC_CH3;
	ssbData->rxFilterStateS = ssbRxFilterStateS_CH3;
	ssbData->rxFilterStateC = ssbRxFilterStateC_CH3;
   
}


#pragma optimize_as_cmd_line

/*------------------------------------------------------------------------------*
* void txAfSignalMux()															*
*									                                     		*
* IN:	txAf[], txNsk[], txTestTone[]									     	*
*																				*	
* OUT:	txAfSum[]															 	*
*                                                                               *
* Purpose: Tx AF signal multiplexing                                            *
*------------------------------------------------------------------------------*/

void txAfSignalMux(float txAf[], float txNsk[], float txTestTone[], float txAfSum[])
{
	long i;


	for (i = 0; i < N_APLC_CHANNELS*BLOCK_LENGTH_R_4K; i++) {

		txAfSum[i] = txAf[i] + txNsk[i] + txTestTone[i];
	};
}


/*------------------------------------------------------------------------------*
* void ssbModulator1Ch(float *TxAfPtr,                                     		*
*							float *TxPilotPtr ,                                 *
*							float *TesttonPtr,                                  *
*							float *TxAfSumPtr,                                  *
*							float *TxAfssbModPtr)                               *
*                                                                         	 	*
*	Variables:  - array of length 4                                             *
*               - block computation with block length 2:                        *
*				- Arr[0] = 1. value , Arr[1]= 2.value                           *
*               - Arr[2] = 1. value , Arr[2]= 2.value                  	 		*
*                                                                              	*
*   IN:	    *TxAfPtr              Arr[0], Arr[1]: Af- Signal of Channel 1      	*
* 	 							  Arr[2], Arr[3]: no signal, not used		   	*
*          	*TxPilotPtr	          Arr[0], Arr[1]: Pilot of Channel 1           	*
*      							  Arr[2], Arr[3]: no signal, not used          	*
*			*TesttonPtr			  Arr[0], Arr[1]: Testtone of Channel 1	       	*
*      							  Arr[2], Arr[3]: no signal, not used          	*
*			*TxNsdPtr			  Arr[0], Arr[1]: I, Q component 1. sample		*
*								  Arr[2], Arr[3]: I, Q component 2. sample		*	
*   OUT:	*TxAfSumPtr           Arr[0], Arr[1]: Af+Pilot+Testone of Channel1 	*
*								  Arr[2], Arr[3]: no signal, not used		   	*
*           *TxAfssbModPtr        Arr[0]		: I- Modulated Signal 1. value 	*
*                                 Arr[1]		: Q- Modulated Signal 1. value 	* 
*                                 Arr[2]		: I- Modulated Signal 2. value 	*
*                                 Arr[3]		: Q- Modulated Signal 2. value 	*
*                                                                               *
*                                                                               *
* Purpose:                                                                      *
* is a multiplexer and ssb-modulator for one af-channel                         *
*------------------------------------------------------------------------------*/

void ssbModulator1Ch(float *TxAfPtr, float *TxNsdPtr, float *TxAfssbModPtr, t_ssbData1Ch *ssbData)
{  
  static float sin, cos; 
  static float s_1s[BLOCK_LENGTH_R_4K];
  static float s_1c[BLOCK_LENGTH_R_4K];
  long i;             
    
           
    GenIQAmpCorr(ssbData->oscMod);       
   			            
    for (i = 0; i < BLOCK_LENGTH_R_4K; i++) {
		
		GenIQ(ssbData->oscMod, &sin, &cos);   
		s_1s[i] = sin * (*(TxAfPtr +i));   				 
		s_1c[i] = cos * (*(TxAfPtr +i));   				       	
	}
            
    // SSB Tiefpassfilter     PS8313.DAT
    BiIIR(s_1s, s_1s, ssbData->filterCoef, ssbData->txFilterStateS, ssbData->filterSec, BLOCK_LENGTH_R_4K);   
    BiIIR(s_1c, s_1c, ssbData->filterCoef, ssbData->txFilterStateC, ssbData->filterSec, BLOCK_LENGTH_R_4K);
    
    // NSD Tx samples
    s_1s[0] += *TxNsdPtr;
    s_1c[0] += *(TxNsdPtr + 1);
    s_1s[1] += *(TxNsdPtr + 2);
    s_1c[1] += *(TxNsdPtr + 3);

    for (i = 0; i < BLOCK_LENGTH_R_4K; i++) {

		*(TxAfssbModPtr+2*i)   = s_1c[i] * ssbData->sm1;    
		*(TxAfssbModPtr+2*i+1) = s_1s[i] * ssbData->sm2;
	}
}


/********************************************************************************  
*	SSB - DEMODULATOR für 1 Kanal                                             	*
*	=============================                                               *
*                                                                               *
*   SSB Demodulator für 1 -Kanalgerät, symmetrische Erzeugung                   *
*	Vorgaben in RR-NTC96-1528                                                   *
*	Definition der Funktionen in :	PS8323.DAT  (SSB Demodulator)               *
*					              	PS8324.DAT  (Skalierung Rx)                 *
*                                                                               *
*                                                                               *
*	Fehlende Funktionalität in:	                                                *
*	- PS8323.DAT :                                                              *
*	- PS8324.DAT :	NF Loop                                                     *
*                                                                               *
*	                                                                            *
*	Zusätzliche Funktionalität:                                                 *
*	- Post Equalizer                                                            *
*	- variable Blocklänge                                                       *
*	                                                                            *
*	Aenderungen im Datenfluss:                                                  *
*                                                                               *
*	                                                                            *
*	Die Implementierung besteht aus einem einmaligen Teil, welcher beim Auf-    *
*   starten oder bei einer                                                      *
*	Konfigurationsänderung aufgerufen wird und einem periodischen Teil.         *
*	  									                                        *
*	Einmaliger Teil                                                             *
*	- wird genannt InitSSB                                                      *
*	- ist für die SSB Modulation und Demodulation gemeinsam                     *
*	- bestimmt aus der Kanallage das richtige Vorzeichen                        *
*	- bestimmt aus der Bandbeite die Filterkoeffizienten und -länge (TAPS) des  *
*	  SSB Tiefpasses                                                            *
*	- bestimmt aus der Banbreite die Modultionsfrequenz  und initialisiert den 	*
*	  komplexen Generator mit den Umsetzfrequenzen 2150 Hz, 1150 Hz oder 1400 Hz* 
*	- initialisiert die Statusspeicher für die folgenden Filter                 *
*		-> SSB Tiefpass                                                         *
*		-> NF Hochpass                                                          *
*		-> Pre/Post Entzerrer                                                   *
*		                                                                        *
*	periodischen Teil                                                           *
*	- wird genannt ssbDemodulator                                               *
*	- setzt sich aus dem Demodulator und Skalierer zusammen                     *
*	- führt den NF- Teil der Weaver SSB - Demodulation durch                    *
*	- implementiert mit Blockverarbeitung                                       *
*	- skaliert richtig bezüglich der vorhergehenden Dezimation                  *
*		                                                                        *
*********************************************************************************
*/

/*------------------------------------------------------------------------------
* void ssbDemodulator1Ch(float *RxAfdezimPtr, float *RxAfSsbDemodPtr)    		*
*                                                                         	 	*
*	Variables:                                                             	 	*
*       IN:	     *RxAfdezimPtr 		Pointer to decimated complex signal 		*
*									( Array with length 4 )	                    *
*									Arr[0], Arr[2]: i - signal				   	*
*     							   	Arr[2], Arr[3]: q - signal				    * 
*     							  	       									    *
*       OUT:	 *RxAfSsbDemodPtr   Pointer to demodulated af - signal          *
*									( Array with length 4 )	                    *
*									Arr[0], Arr[1]: af- signal of channel 1   	*
*     							   	Arr[2], Arr[3]: af- signal of channel 2     * 
*                                                                               *
*   Purpose:                                                                    *
*	- is the final part of the weaver methode demodulating ssb signals			*
*   - takes the complex signal and compute the af signal for channel 1   		* 
*   - the af signal of channel 2 is always set to zero amplitude, so it's easier* 
*	  to handle the two channel case. We can use always the same variables of   *
*	  length 4 in the main programm of dspb. We don't have to distinguish bet-	*
*	  ween them. 	                                                            *
*------------------------------------------------------------------------------*/

void ssbDemodulator1Ch(float *RxAfdezimPtr, float *RxAfSsbDemodPtr, t_ssbData1Ch *ssbData) 
{
	static float sin, cos;
	static float i_rx[BLOCK_LENGTH_R_4K]; 
	static float q_rx[BLOCK_LENGTH_R_4K];
	static float s_1s;
	static float s_1c;
	long i;
  
  
	GenIQAmpCorr(ssbData->oscDemod);

	for (i = 0; i < BLOCK_LENGTH_R_4K; i++) {

		i_rx[i] = *(RxAfdezimPtr + 2*i)   * ssbData->sd12;       
		q_rx[i] = *(RxAfdezimPtr + 2*i+1) * ssbData->sd11;    
	}
 
    // SSB filter
    BiIIR(i_rx, i_rx, ssbData->filterCoef, ssbData->rxFilterStateS, ssbData->filterSec, BLOCK_LENGTH_R_4K);
    BiIIR(q_rx, q_rx, ssbData->filterCoef, ssbData->rxFilterStateC, ssbData->filterSec, BLOCK_LENGTH_R_4K);
            
	// Demodulation 1. sample
	GenIQ(ssbData->oscDemod, &sin, &cos);

	s_1s = sin * i_rx[0];    
	s_1c = cos * q_rx[0];        	
	*(RxAfSsbDemodPtr    ) = (s_1c + s_1s);

	// Demodulation 2. sample
	GenIQ(ssbData->oscDemod, &sin, &cos);

	s_1s = sin * i_rx[1];    
	s_1c = cos * q_rx[1];        	
	*(RxAfSsbDemodPtr + 1) = (s_1c + s_1s);
	
}


/********************************************************************************  
*	SSB - MODULATOR für 2 Kanäle                                              	*
*	============================                                              	*
*	                                                                          	*
*	Multiplexer und SSB Modulator für 2 - Kanalgerät, symmetrische Erzeugung  	*
*	                                                                          	*	
*	Referenzen:                                                               	*
*	                                                                          	*
*	- Vorgaben in RR-NTC96-1528                                               	*
*	- Definition der Funktionen in :	PS8311.DAT  (bilde Summensignal)      	*
*						              	PS8312.DAT  (Skalierung Tx)           	*
*        	                          	PS8313.DAT  (SSB Modulator)           	*
*                                                                             	*
*	Fehlende Funktionalität in:	                                              	*
*	                                                                          	*
*	- PS8311.DAT :	Tx TPS Signal CH1                                         	*
*	- PS8312.DAT :	-                                                         	*
*	- PS8313.DAT :	wählbare Bandbreiten                                      	*
*	                                                                          	*
*	Zusätzliche Funktionalität:                                               	*
*	                                                                          	*
*	- NF Hochpass (TM-NT96-1627/_)                                            	*
*	- Post Equalizer und zusätzliche Skalierung wegen der Interpolation       	*
*     (PS8314.DAT)                                                            	*
*	- variable Blocklänge                                                     	*
*	                                                                          	*
*	Aenderungen im Datenfluss:                                                	*
*	                                                                          	*
*	- PS8312.DAT  (Skalierung Tx) kommt nach PS8313.DAT  (SSB Modulator)      	*
*	                                                                          	*
*	Beschreibung:                                                             	*
*	                                                                          	*
*	Die Implementierung besteht aus einem einmaligen Teil, welcher beim Auf-  	*
*	starten oder bei einer Konfigurationsänderung aufgerufen wird und einem   	*
*   periodischen Teil.                                                        	*
*	  									                                      	*
*	a) einmaliger Teil                                                        	*
*	- wird genannt initSSB2Ch                                                  	*
*	- ist für die SSB Modulation und Demodulation gemeinsam                   	*
*	- bestimmt aus der Kanallage das richtige Vorzeichen                      	*
*	- bestimmt aus der Bandbreite die Filterkoeffizienten und -länge (TAPS)   	*
*	  des SSB Tiefpasses (*)                                                  	*
*	- bestimmt aus der Bandbreite die Modulationsfrequenz  und initialisiert  	*
*     den komplexen Generator mit den Umsetzfrequenzen 2150 Hz, 1150 Hz oder  	*
*     1400 Hz    (*)                                                          	*
*	- initialisiert die Statusspeicher für die folgenden Filter               	*
*		-> SSB Tiefpass                                                       	*
*		-> NF Hochpass                                                        	*
*		-> Pre/Post Entzerrer                                                 	*
*	                                                                          	*
*	                                                                          	*
*	(*) für Bandbreite 4kHz implementiert                                     	*
*		                                                                      	*
*	b) periodischer Teil                                                      	*
*	- wird genannt ssbModulator2Ch		                                       	*
*	- setzt sich aus dem Multiplexer, Skalierer und Modulator zusammen        	*
*	- führt den NF- Teil der Weaver SSB - Modulation durch                    	*
*	- implementiert mit Blockverarbeitung                                     	*
*	- skaliert richtig für die folgende Interpolation                         	*
*                                                                             	*
*********************************************************************************	
*/

/*------------------------------------------------------------------------------
* void initSSB2Ch(void)											    			*
*                                                                         	 	*
*	Variables:                                                             	 	*
*       IN:		-      	       												  	*
*       OUT:	- 					                                            * 
*                                                                               *
*       extern    	ActualConfigData.HF.HF_Bandbreite                           *
*                 	ActualConfigData.HF.Kanallage                               *
*                                                                               *
*   Purpose:                                                                    *
*   initializes all static variables in this file ssb.c for the two channel     *
*   modulation and demodulation                                                 *
*------------------------------------------------------------------------------*/

#pragma optimize_off

void SEGMENT_EXT_CODE initSSB2Ch(t_FrontEndDSP_Config *FE_DSP_Config)
{
	long i;
	float fm;   
	float fm2;
	

	fm  = Fm_BW4000;    		
    fm2 = Fm2_BW4000;
	
    switch (FE_DSP_Config->channelMode)
    {  
		case APLC_CHANNEL_MODE_A1_40I_A2_20E:	// MOD above speech
		case APLC_CHANNEL_MODE_A1_40I_A2_40E:
		case APLC_CHANNEL_MODE_A1_40I_A2_40E_A3_40E:
		case APLC_CHANNEL_MODE_A1_32I_A2_24E_A3_24E: 
			
			// CH1_RO_CH2_KU
			sm1  = +1.0; sm4 = +1.0;	// Modulators Kanal 1 , I
    		sm2  = -1.0; sm3 = +1.0;	// Modulators Kanal 1 , Q 
    	    
    	    sm5  = +1.0; sm8 = +1.0;	// Modulators Kanal 2 , I 
    	    sm6  = +1.0; sm7 = -1.0;	// Modulators Kanal 2 , Q
            
   	   	    sd11 = -1.0; sd14 = +1.0;	// Demodulators Kanal 1, I 
	   	    sd12 = +1.0; sd13 = +1.0;	// Demodulators Kanal 1, Q
    	    
    	    sd22 = -1.0; sd23 = +1.0;	// Demodulators Kanal 2, I
    	    sd21 = +1.0; sd24 = +1.0;	// Demodulators Kanal 2, Q

    	    break;

		case APLC_CHANNEL_MODE_A1_40I_A2_40I:
		case APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_20E:	// MOD above speech
		case APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_40I:

			//CH1_RO_CH2_RU:    
			sm1  = +1.0; sm4 = +1.0; 
    		sm2  = -1.0; sm3 = +1.0;
    	    
    	    sm5  = +1.0; sm8 = -1.0;
    	    sm6  = +1.0; sm7 = +1.0; 
    	    
    	    sd11 = -1.0; sd14 = +1.0;
    	    sd12 = +1.0; sd13 = +1.0; 
    	    
    	    sd22 = +1.0; sd23 = -1.0;
    	    sd21 = +1.0; sd24 = +1.0;
    	    
    	    break;
			
		default:	// settings as APLC_CHANNEL_MODE_A1_40I_A2_40I
			
			sm1  = +1.0; sm4 = +1.0; 
    		sm2  = -1.0; sm3 = +1.0;
    	    
    	    sm5  = +1.0; sm8 = -1.0;
    	    sm6  = +1.0; sm7 = +1.0; 
    	    
    	    sd11 = -1.0; sd14 = +1.0;
    	    sd12 = +1.0; sd13 = +1.0; 
    	    
    	    sd22 = +1.0; sd23 = -1.0;
    	    sd21 = +1.0; sd24 = +1.0;

	}
 
	InitGenIQ(&fmModulator1,    fm,  AF_SAMPLE_FREQ_4K, 1.0);  
	InitGenIQ(&fmDemodulator1,  fm,  AF_SAMPLE_FREQ_4K, 1.0); 
	InitGenIQ(&fmModulator2,   fm2, AF_SAMPLE_FREQ_4K, 1.0);  
	InitGenIQ(&fmDemodulator2, fm2, AF_SAMPLE_FREQ_4K, 1.0);  

	// initialize SSB filter states
    for (i = 0; i < SSB_STATE_MAX; i++) { 
    	                 
		ssbTxFilterStateS_CH1[i] = 0.0;
		ssbTxFilterStateC_CH1[i] = 0.0;
  		ssbRxFilterStateS_CH1[i] = 0.0;
		ssbRxFilterStateC_CH1[i] = 0.0;
		
  		ssbTxFilterStateS_CH2[i] = 0.0;
		ssbTxFilterStateC_CH2[i] = 0.0;
  		ssbRxFilterStateS_CH2[i] = 0.0;
		ssbRxFilterStateC_CH2[i] = 0.0;
	};

} 

#pragma optimize_as_cmd_line


/*------------------------------------------------------------------------------
* void ssbModulator2Ch(float *TxAfPtr,                                   		*
*					   float *TxPilotPtr ,                                 		*
*					   float *TesttonPtr,                                  		*
*					   float *TxNsdPtr,
*					   float *TxAfSumPtr,                                  		*
*					   float *TxAfssbModPtr)                               		*
*                                                                         	 	*
*	Variables:  - array of length 4                                             *
*               - block computation with block length 2:                        *
*				- Arr[0] = 1. value , Arr[1]= 2.value                           *
*               - Arr[2] = 1. value , Arr[2]= 2.value                  	 		*
*                                                                               *
*   IN:	    *TxAfPtr              Arr[0], Arr[1]: Af- Signal of Channel 1   	*
*     							  Arr[2], Arr[3]: Af- Signal of Channel 2   	*
*  	       	*TxPilotPtr	          Arr[0], Arr[1]: Pilot of Channel 1        	*
*      							  Arr[2], Arr[3]: Pilot of Channel 2            *
*			*TesttonPtr			  Arr[0], Arr[1]: Testtone of Channel 1	        *
*      							  Arr[2], Arr[3]: Testtone of Channel 2         *
*			*TxNsdPtr			  Arr[0], Arr[1]: I, Q component 1. sample Ch1	*
*								  Arr[2], Arr[3]: I, Q component 2. sample Ch1	*
*								  Arr[4], Arr[5]: I, Q component 1. sample Ch2	*
*								  Arr[6], Arr[7]: I, Q component 2. sample Ch2	*	
*   OUT:	*TxAfSumPtr           Arr[0], Arr[1]: Af+Pilot+Testone of Channel 1 *
*								  Arr[2], Arr[3]: Af+Pilot+Testone of Channel 2 *
*           *TxAfssbModPtr        Arr[0]		: I- Modulated Signal 1. value  *
*                                 Arr[1]		: Q- Modulated Signal 1. value  * 
*                                 Arr[2]		: I- Modulated Signal 2. value  *
*                                 Arr[3]		: Q- Modulated Signal 2. value  *
*                                                                               *
*   Purpose:                                                                    *
*   - is a multiplexer for two af-channels										*
*	- first part of weaver ssb-modulator                                       	*
*	- symmetric modulation of the two af channels                          		*
*------------------------------------------------------------------------------*/


void ssbModulator2Ch(float *TxAfPtr, float *TxNsdPtr, float *TxAfssbModPtr)
{  
	static float sin, cos;	

	static float s_1s[BLOCK_LENGTH_R_4K];     // s_1s,s_1c: Hilbert Signal Paar Kanal 1
	static float s_1c[BLOCK_LENGTH_R_4K]; 
	static float s_2s[BLOCK_LENGTH_R_4K];     // s_2s,s_2c: Hilbert Signal Paar Kanal 2
	static float s_2c[BLOCK_LENGTH_R_4K];

	static float s_1i[BLOCK_LENGTH_R_4K];
	static float s_1q[BLOCK_LENGTH_R_4K]; 
	static float s_2i[BLOCK_LENGTH_R_4K];
	static float s_2q[BLOCK_LENGTH_R_4K];

	static float s_rf_i[BLOCK_LENGTH_R_4K];
	static float s_rf_q[BLOCK_LENGTH_R_4K];
  
	
	GenIQAmpCorr(&fmModulator1);
	GenIQAmpCorr(&fmModulator2);      
	
    GenIQ(&fmModulator1, &sin, &cos);   
    
    s_1s[0] = sin * (*(TxAfPtr +0));   				 
    s_1c[0] = cos * (*(TxAfPtr +0));   
	s_2s[0] = sin * (*(TxAfPtr +2));   				 
    s_2c[0] = cos * (*(TxAfPtr +2));   
    
    GenIQ(&fmModulator1, &sin, &cos);   
    
    s_1s[1] = sin * (*(TxAfPtr +1));   				 
    s_1c[1] = cos * (*(TxAfPtr +1));   
    s_2s[1] = sin * (*(TxAfPtr +3));   				 
    s_2c[1] = cos * (*(TxAfPtr +3));   					       	
            
    // SSB Tiefpassfilter     PS8313.DAT
    BiIIR(s_1s, s_1s, ssbCoef4kHz, ssbTxFilterStateS_CH1, SSB_SEC_4KHZ, BLOCK_LENGTH_R_4K);   
    BiIIR(s_1c, s_1c, ssbCoef4kHz, ssbTxFilterStateC_CH1, SSB_SEC_4KHZ, BLOCK_LENGTH_R_4K);   
    BiIIR(s_2s, s_2s, ssbCoef4kHz, ssbTxFilterStateS_CH2, SSB_SEC_4KHZ, BLOCK_LENGTH_R_4K);   
    BiIIR(s_2c, s_2c, ssbCoef4kHz, ssbTxFilterStateC_CH2, SSB_SEC_4KHZ, BLOCK_LENGTH_R_4K);
       
    // NSD Tx Data Ch1
    s_1s[0] += *TxNsdPtr;
    s_1c[0] += *(TxNsdPtr + 1);
    s_1s[1] += *(TxNsdPtr + 2);
    s_1c[1] += *(TxNsdPtr + 3);

	// NSD Tx Data Ch2
    s_2s[0] += *(TxNsdPtr + 4);
    s_2c[0] += *(TxNsdPtr + 5);
    s_2s[1] += *(TxNsdPtr + 6);
    s_2c[1] += *(TxNsdPtr + 7);
 
   	GenIQ(&fmModulator2, &sin, &cos); 
   	 
    s_1i[0] = sm1 *	s_1c[0] * cos + sm4 * s_1s[0] * sin; 
    s_1q[0] = sm2 *	s_1c[0] * sin + sm3 * s_1s[0] * cos;  
    s_2i[0] = sm5 *	s_2c[0] * cos + sm8 * s_2s[0] * sin;   
    s_2q[0] = sm6 * s_2c[0] * sin + sm7 * s_2s[0] * cos; 
     
 	GenIQ(&fmModulator2, &sin, &cos); 
 	
    s_1i[1] = sm1 *	s_1c[1] * cos + sm4 * s_1s[1] * sin; 
    s_1q[1] = sm2 *	s_1c[1] * sin + sm3 * s_1s[1] * cos; 
    s_2i[1] = sm5 *	s_2c[1] * cos + sm8 * s_2s[1] * sin;  
    s_2q[1] = sm6 * s_2c[1] * sin + sm7 * s_2s[1] * cos; 

 
    s_rf_i[0] =   s_1i[0] + s_2i[0];
    s_rf_q[0] =   s_1q[0] + s_2q[0]; 
    s_rf_i[1] =   s_1i[1] + s_2i[1];
    s_rf_q[1] =   s_1q[1] + s_2q[1];          	 

	*(TxAfssbModPtr   ) = s_rf_i[0];   
    *(TxAfssbModPtr +1) = s_rf_q[0];  
    *(TxAfssbModPtr +2) = s_rf_i[1];
    *(TxAfssbModPtr +3) = s_rf_q[1]; 

}


/********************************************************************************  
*	SSB - DEMODULATOR für 2 Kanäle                                              *
*	==============================                                              *
*                                                                               *
*   SSB Demodulator für 2 -Kanalgerät, symmetrische Erzeugung                   *
*	Vorgaben in RR-NTC96-1528                                                   *
*	Definition der Funktionen in :	PS8323.DAT  (SSB Demodulator)               *
*					              	PS8324.DAT  (Skalierung Rx)                 *
*                                                                               *
*                                                                               *
*	Fehlende Funktionalität in:	                                                *
*	- PS8323.DAT :                                                              *
*	- PS8324.DAT :	NF Loop                                                     *
*                                                                               *
*	                                                                            *
*	Zusätzliche Funktionalität:                                                 *
*	- Post Equalizer                                                            *
*	- variable Blocklänge                                                       *
*	                                                                            *
*	Aenderungen im Datenfluss:                                                  *
*                                                                               *
*	                                                                            *
*	Die Implementierung besteht aus einem einmaligen Teil, welcher beim Auf-	*
*	starten oder bei einer Konfigurationsänderung aufgerufen wird und einem 	*
*	periodischen Teil.                                                          *
*	  									                                        *
*	Einmaliger Teil                                                             *
*	- wird genannt InitSSB                                                      *
*	- ist für die SSB Modulation und Demodulation gemeinsam                     *
*	- bestimmt aus der Kanallage das richtige Vorzeichen                        *
*	- bestimmt aus der Bandbeite die Filterkoeffizienten und -länge (TAPS) des 	*
*	  SSB Tiefpasses 											 				*
*	- bestimmt aus der Banbreite die Modultionsfrequenz  und initialisiert den 	*
*	  komplexen Generator mit den Umsetzfrequenzen 2150 Hz, 1150 Hz oder 1400 Hz* 
*	- initialisiert die Statusspeicher für die folgenden Filter                 *
*		-> SSB Tiefpass                                                         *
*		-> NF Hochpass                                                          *
*		-> Pre/Post Entzerrer                                                   *
*		                                                                        *
*	periodischen Teil                                                           *
*	- wird genannt ssbDemodulator                                               *
*	- setzt sich aus dem Demodulator und Skalierer zusammen                     *
*	- führt den NF- Teil der Weaver SSB - Demodulation durch                    *
*	- implementiert mit Blockverarbeitung                                       *
*	- skaliert richtig bezüglich der vorhergehenden Dezimation                  *
*		                                                                        *
*********************************************************************************
*/


/*------------------------------------------------------------------------------
* void ssbDemodulator2Ch(float *RxAfdezimPtr, float *RxAfSsbDemodPtr)	     	*
*                                                                         	 	*
*	Variables:                                                             	 	*
*       IN:	     *RxAfdezimPtr 		Pointer to decimated complex signal 		*
*									( Array with length 4 )	                    *
*									Arr[0], Arr[2]: i - signal				   	*
*     							   	Arr[2], Arr[3]: q - signal				    * 
*     							  	       									    *
*       OUT:	 *RxAfSsbDemodPtr   Pointer to demodulated af - signal          *
*									( Array with length 4 )	                    *
*									Arr[0], Arr[1]: af- signal of channel 1   	*
*     							   	Arr[2], Arr[3]: af- signal of channel 2     * 
*                                                                               *
*   Purpose:                                                                    *
*	- is the final part of the weaver methode demodulating ssb signals			*
*   - takes the complex signal and compute the af signal for channel 1 + 2 		* 
*------------------------------------------------------------------------------*/

void ssbDemodulator2Ch(float *RxAfdezimPtr, float *RxAfSsbDemodPtr) 
{
	static float sin, cos;

	static float s_1s[BLOCK_LENGTH_R_4K];     // s_1s,s_1c: Hilbert Signal Paar Kanal 1
	static float s_1c[BLOCK_LENGTH_R_4K]; 
	static float s_2s[BLOCK_LENGTH_R_4K];     // s_2s,s_2c: Hilbert Signal Paar Kanal 2
	static float s_2c[BLOCK_LENGTH_R_4K];
  
	static float s_1i[BLOCK_LENGTH_R_4K];
	static float s_1q[BLOCK_LENGTH_R_4K]; 
	static float s_2i[BLOCK_LENGTH_R_4K];
	static float s_2q[BLOCK_LENGTH_R_4K];

	static float s_ss[BLOCK_LENGTH_R_4K]; 	// reelles Spektrum  
	static float s_sc[BLOCK_LENGTH_R_4K]; 	// imag. Spektrum
	static float s_cs[BLOCK_LENGTH_R_4K];   // imag. Spektrum
	static float s_cc[BLOCK_LENGTH_R_4K];  	// reelles Spektrum  

	static float s_rf_i[BLOCK_LENGTH_R_4K];
	static float s_rf_q[BLOCK_LENGTH_R_4K];


  	GenIQAmpCorr(&fmDemodulator1);
  	GenIQAmpCorr(&fmDemodulator2);
    
	s_rf_i[0] = -1.0* *(RxAfdezimPtr); 		// invert sign of in-phase component   
	s_rf_q[0] = *(RxAfdezimPtr +1);        
	s_rf_i[1] = -1.0* *(RxAfdezimPtr +2);	// invert sign of in-phase component   
	s_rf_q[1] = *(RxAfdezimPtr +3);    

    GenIQ(&fmDemodulator2, &sin, &cos); 

    s_ss[0] =  s_rf_i[0] *  sin;   // reelles Spektrum  , 1
    s_sc[0] =  s_rf_i[0] *  cos;   // imag. Spektrum    , 2
    s_cs[0] =  s_rf_q[0] *  sin;   // imag. Spektrum    , 3
    s_cc[0] =  s_rf_q[0] *  cos;   // reelles Spektrum  , 4
    
    GenIQ(&fmDemodulator2, &sin, &cos);
     
    s_ss[1] =  s_rf_i[1] *  sin;   // reelles Spektrum  
    s_sc[1] =  s_rf_i[1] *  cos;   // imag. Spektrum
    s_cs[1] =  s_rf_q[1] *  sin;   // imag. Spektrum
    s_cc[1] =  s_rf_q[1] *  cos;   // reelles Spektrum        
    
    s_1i[0] =  sd11 * s_ss[0] + sd14 * s_cc[0];	// 	Kanal 1:	I-Signal aus 1 , 4
    s_1q[0] =  sd12 * s_sc[0] + sd13 * s_cs[0];	//  			Q-Signal aus 2 , 3
    s_2i[0] =  sd21 * s_ss[0] + sd24 * s_cc[0];	//  Kanal 2:    I-Signal aus 1 , 4
    s_2q[0] =  sd22 * s_sc[0] + sd23 * s_cs[0];	//  			Q-Signal aus 2 , 3

    s_1i[1] =  sd11 * s_ss[1] + sd14 * s_cc[1];	// 	Kanal 1:	I-Signal aus 1 , 4
    s_1q[1] =  sd12 * s_sc[1] + sd13 * s_cs[1];	//  			Q-Signal aus 2 , 3
    s_2i[1] =  sd21 * s_ss[1] + sd24 * s_cc[1];	//  Kanal 2:    I-Signal aus 1 , 4
    s_2q[1] =  sd22 * s_sc[1] + sd23 * s_cs[1];	//  			Q-Signal aus 2 , 3
                              
    // SSB filter
    BiIIR(s_1i, s_1i, ssbCoef4kHz, ssbRxFilterStateC_CH1, SSB_SEC_4KHZ, BLOCK_LENGTH_R_4K);
    BiIIR(s_1q, s_1q, ssbCoef4kHz, ssbRxFilterStateS_CH1, SSB_SEC_4KHZ, BLOCK_LENGTH_R_4K);

    BiIIR(s_2i, s_2i, ssbCoef4kHz, ssbRxFilterStateC_CH2, SSB_SEC_4KHZ, BLOCK_LENGTH_R_4K);
    BiIIR(s_2q, s_2q, ssbCoef4kHz, ssbRxFilterStateS_CH2, SSB_SEC_4KHZ, BLOCK_LENGTH_R_4K);

	// Demodulation	1. sample
	GenIQ(&fmDemodulator1, &sin, &cos);
	
	s_1c[0] = cos * s_1i[0];    // 5
	s_1s[0] = sin * s_1q[0];    // 6
  	s_2c[0] = cos * s_2i[0];    // 8
	s_2s[0] = sin * s_2q[0];    // 7
	
	// Demodulation	2. sample
   	GenIQ(&fmDemodulator1, &sin, &cos);
   	
	s_1c[1] = cos * s_1i[1];    
	s_1s[1] = sin * s_1q[1];   
  	s_2c[1] = cos * s_2i[1];    
	s_2s[1] = sin * s_2q[1];        	
    
	// build reel signal
	*(RxAfSsbDemodPtr     ) = (s_1c[0] + s_1s[0]);    // 5 + 6
	*(RxAfSsbDemodPtr + 1 ) = (s_1c[1] + s_1s[1]);    // 5 + 6

  	*(RxAfSsbDemodPtr + 2 ) = (s_2c[0] + s_2s[0]);    // 7 + 8
	*(RxAfSsbDemodPtr + 3 ) = (s_2c[1] + s_2s[1]);    // 7 + 8
  
}


/*------------------------------------------------------------------------------
* BOOL limiter(complex_float vSamples[], long nSamples)			    			*
*                                                                         	 	*
*	Variables:                                                             	 	*
*       IN:		sample vector, number of samples      	       												  	*
*       OUT:	limited sample vector 					                                            * 
*                                                                               *
*                                                                               *
*   Purpose:                                                                    *
*   Limits the max. envelope of a complex signal							    *
*------------------------------------------------------------------------------*/
/*
BOOL limiter(complex_float vSamples[], long nSamples)
{
	long i;
	static float envSqr;
	static complex_float oldSample = {0,0};
	BOOL flag = FALSE;


	for(i = 0; i < nSamples; i++) {

		envSqr = SQR(vSamples[i].re) + SQR(vSamples[i].im);

		if (envSqr > ENV_SQR_MAX) {

			flag = TRUE;
			vSamples[i].re = oldSample.re;
			vSamples[i].im = oldSample.im;
		}
		else {

			oldSample.re = vSamples[i].re;
			oldSample.im = vSamples[i].im;
		};
	};

	return(flag);
}
*/
