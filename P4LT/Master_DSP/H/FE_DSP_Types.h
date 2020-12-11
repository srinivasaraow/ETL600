/*-------------------------------------------------------------------------*
 * $Workfile: FE_DSP_Types.h $
 * Part of      : ETL600 / Front end DSP
 * Language     : C
 * Created by   : Author
 * Remarks      :  
 * Purpose      : Global type declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/FE_DSP/H/FE_DSP_Types.h $
 * 
 * 3     12.04.05 11:07 Chhamaa
 * operationMode removed from struct t_FrontEndDSP_Config
 * 
 * 2     16.02.05 16:05 Chhamaa
 * 
 * 12    8.02.05 11:08 Chhamaa
 * t_TestToneConfig removed from t_FrontEndDSP_Config
 * 
 * 11    27.01.05 18:30 Chhamaa
 * equalizerFirCoef[] added to t_FrontEndDSP_Config
 * 
 * 10    26.11.04 15:19 Maag01
 * t_FrontEndDSP_Status modified
 * 
 * 9     16.09.04 15:33 Maag01
 * 
 * 8     7.09.04 14:05 Maag01
 * 
 * 7     26.08.04 11:33 Maag01
 * 
 * 6     14.07.04 12:19 Leeb02
 * Dependencies removed
 * 
 * 5     28.05.04 13:35 Maag01
 * 
 * 4     26.04.04 18:18 Maag01
 * t_FrontEndDSP_Status added
 * 
 * 3     7.04.04 11:05 Leeb02
 * 
 * 2     15.03.04 16:49 Maag01
 * 
 * 1     5.03.04 15:32 Maag01
 * Initial Version
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef FE_DSP_TYPES_H
#define FE_DSP_TYPES_H

/* INCLUDE FILES ***********************************************************/

#include "APLC_Types.h"
#include "RF_Types.h"
#include "NSK_Types.h"
#include "equalizerFirFilter.h"


/* GLOBAL CONSTANT DECLARATIONS ********************************************/


/* GLOBAL TYPE DECLARATIONS ************************************************/


typedef struct {

	t_PilotModemConfig		pilot[N_APLC_CHANNELS];
	t_EqualizerConfig		equalizer[N_APLC_CHANNELS];
	t_EqualizerFirCoef		equalizerFirCoef[N_APLC_CHANNELS];
	t_RF_Config				RF;
	float					rxAFLevelAdjust[N_APLC_CHANNELS];
	long					channelMode;
	unsigned long			apFilterFlags;		// group delay equalizer flages for APLC channels
	BOOL					AFC_Enabled;
	float					weightOFDM_Signal;
	float					weightNSK_Signal[N_NSK_MODEMS];
	long					DPLC_BW;
	long					intTeleprotMode;
	long					rfPepMax;

} t_FrontEndDSP_Config;


typedef struct {
	
	t_APLC_Status			APLC;
	
} t_FrontEndDSP_Status; 



#endif
