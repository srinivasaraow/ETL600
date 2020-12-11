/*-------------------------------------------------------------------------*
 * $Workfile: tdm_int_types.h $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Dr. Vladimir Potapov
 * Remarks      : no boost switches, yet 
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source R4/P4LT/NSK_DSP/H/tdm_int_types.h $
 * 
 * 34    10-11-24 14:30 Chhamaa
 * Struct t_NSK_Alarm modified.
 * 
 * 33    10-10-27 13:14 Chhamaa
 * New alarms added in struct t_NSK_Alarm: 
 * - transitNoSync
 * - transitMissingRxData
 * 
 * 30    18.03.08 17:07 Chchlee
 * OFDM_SYNCSTATE_... Definitions added
 * 
 * 29    08-02-25 9:58 Chtognu
 * Merged changes from ad.equ.branch:
 * ALM_DCD_NSK1..4 added
 * 
 * 28    07-08-28 13:12 Chchlee
 * Additional path for injection of NSK signals after allpass filter added
 * 
 * 27    1.03.07 17:37 Chhamaa
 * New time slots for O4CV subframe start flags added.
 * 
 * 26    24.01.07 10:07 Chchlee
 * Comment added: tpe1/2TestCfgActiveWarning: not used
 * 
 * 25    10.01.07 10:14 Chhamaa
 * O4CV time slots added
 * 
 * 23    18.09.06 17:21 Chhamaa
 * tpe1CommandOutputsBlockedAlarm added in struct t_NSD_Alarm
 * 
 * 22    21.08.06 15:19 Chchlee
 * OFDM syncstate added
 * 
 * 20    1.06.06 13:14 Chalgyg
 * G.703 LOS Alarm added to t_NSK_Alarm Struct.
 * 
 * 18    5.04.06 16:08 Chhamaa
 * 
 * 16    5.04.06 14:07 Chtognu
 * TPE1/2_Suppress_Alarms bits added  in t_NSD_Events
 * 
 * 15    23.03.06 15:49 Chmibuh
 * t_MasterDSPOutputFlags.reset_R1LA added
 * 
 * 14    10.03.06 14:25 Chalgyg
 * LAN not booted and LAN not connected is written to the NSK alarm
 * struct.
 * 
 * 13    8.03.06 15:55 Chmibuh
 * t_MasterDSPOutputFlags and t_UC_Alarm expanded
 * 
 * 11    26.10.05 11:15 Chchlee
 * "illegalConfig" bit in all DSP Alarm structs introduced
 * 
 * 10    11.10.05 17:23 Chchlee
 * OFDM rx definitions activated again
 * 
 * 9     11.10.05 16:45 Chchlee
 * Unused definitions commented out
 * 
 * 8     7.09.05 15:37 Chhamaa
 * dacInitFailure added in struct t_UC_Alarm
 * 
 * 7     2.06.05 9:45 Chpeplu
 * In Bit Feld t_NSD_Events new members Manual_LT_Sent and
 * Manual_LT_Failed for Ch1 and Ch2 added 
 * 
 * 6     23.05.05 17:05 Chpeplu
 * struct t_NSD_Events added, NSD_Debug_Word0_TS83 replaced
 * 
 * 5     19.05.05 11:21 Chalgyg
 * Subprint Alarm added to NSK Alarm Struct
 * 
 * 4     17.05.05 16:48 Chchlee
 * error --> configAlm 
 * 
 * 3     16.02.05 14:51 Chhamaa
 * New time slot added: t_UpConvSettings
 * 
 * 2     21.01.05 7:42 Chpeplu
 * in struct t_Boost_Mgr_Input reserved corrected to 26
 * 
 * 1     19.01.05 15:50 Chchlee
 * 
 * 42    19.01.05 12:56 Chpeplu
 * in struct t_Boost_Mgr_Input members TPE1_Pilot_Enable and
 * TPE2_Pilot_Enable removed
 * 
 * 41    18.01.05 17:13 Chpeplu
 * in sturct t_Boost_Mgr_Output Pilot enable Ch1...3 and agc blocking
 * ch1..3 added
 * 
 * 40    17.01.05 17:52 Chchlee
 * rxRfOverflow bit moved back from NSK to FE alarm
 * 
 * 39    14.01.05 17:22 Chpeplu
 * New time slot APLC_SNR_Pilot[] from FE to NSD added
 * 
 * 38    11.01.05 17:43 Chhamaa
 * 
 * 37    11.01.05 10:50 Chpeplu
 * New NSD interface alarms added in bitfield t_NSD_Alarm
 * (tpe1RxContinuousAlarm, tpe2RxContinuousAlarm)
 * 
 * 36    6.01.05 15:59 Chharyt
 * t_Frame_data removed. Now first TS is to sync the NSD -->NSD_Sync_TS00
 * 
 * 35    6.01.05 9:36 Chhamaa
 * New time slot added: t_MasterDSPOutputFlags
 * 
 * 34    5.01.05 14:38 Chalgyg
 * Last change undone.
 * 
 * 33    5.01.05 14:18 Chalgyg
 * OFDM_FBFF_state_TS65 added.
 * 
 * 32    5.01.05 13:56 Chchlee
 * rxRfOverflow alarm moved from FE to NSK DSP
 * 
 * 31    3.01.05 9:28 Chchlee
 * 
 * 30    20.12.04 9:31 Chchlee
 * slot for tdmBusAlarmWord added in mask 3
 * 
 * 29    13.12.04 16:27 Chchlee
 * pilotSNRAlarmCh1-3 added
 * 
 * 28    26.11.04 15:20 Maag01
 * t_FE_Alarm modified
 * 
 * 27    24.11.04 9:41 Leeb02
 * bug in t_OFDM_Alarm struct fixed
 * 
 * 26    23.11.04 9:27 Leeb02
 * Some bits to the status slot added
 * 
 * 25    5.11.04 11:21 Maag01
 * Type of EOC data in t_AllDSP_input_tdm_block changed to t_EOC_data
 * 
 * 24    3.11.04 13:53 Maag01
 * Status Bits added to t_EOC_data
 * 
 * 23    5.10.04 7:46 Leeb02
 * 
 * 22    5.10.04 7:46 Leeb02
 * t_OFDM_Alarm: "error" field added
 * 
 * 21    29.09.04 11:08 Leeb02
 * 
 * 19    28.09.04 11:59 Plueer01
 * struct t_Boost_Mgr_Input extended with "TPE1_Pilot_Enable" and
 * "TPE2_Pilot_Enable" and 
 * 
 * 18    22.09.04 18:21 Leeb02
 * currentDatarateIx modified
 * 
 * 17    30.08.04 11:51 Rytz01
 * t_XXX_output_tdm_block with debug words enhanced
 * 
 * 16    27.08.04 8:29 Rytz01
 * debug words added to UC TDM output block struct
 * 
 * 15    11.08.04 16:51 Leeb02
 * 
 * 14    11.08.04 15:12 Leeb02
 * 
 *-------------------------------------------------------------------------*/


//---------------------------------------------------------------------------
// COPYRIGHT:  (c) 2003 TL-Consulting GmbH, Frankfurt am Main 
//
// MODULE      $Workfile: tdm_int_types.h $
//
// PROJECT:    P4LT system board of ETL600
//
// COMPILER    VisualDSP 3.0, service pack 1
//
// DATE        06-DEC-2003
//
// AUTHOR:     Dr. Vladimir Potapov
//  
// ABSTRACT    P4LT TDM data types definition
//             
//                  
// PREMISES:   
//
// REMARKS:    Reference doc. TM-PTUKT03-1637
//  
// HISTORY:    06.12.03  Initial Version, vgp
//
//---------------------------------------------------------------------------


#ifndef TDM_INT_TYPES_H
#define TDM_INT_TYPES_H

#ifndef _WIN32
    #include <complex.h>
#endif

#include "basetype.h"
#include "tdm_init.h"

#define N_APLC_CHANNELS      3

#if N_APLC_CHANNELS != 3
	#error Number of APLC channels must be 3!!
#endif


/***************************************************************/
/******                                                   ******/
/******        P4LT TDM time slots data types             ******/
/******        for each DSP on the P4LT board             ******/
/******                                                   ******/
/****** Reference Doc no.: TM-PTUKT03-1637  11/04/2003    ******/
/******                    TM-PTUKT03-1637  12/05/2003    ******/ 
/******                                                   ******/
/***************************************************************/

/* Boost Switches data type definition */

typedef struct 
{
	unsigned long    reserved         :26;
	BOOL             TPE1_Start_or_Rx : 1; 
	BOOL             TPE2_Start_or_Rx : 1; 
	BOOL             TPE1_Test        : 1; 
	BOOL             TPE2_Test        : 1; 
	BOOL             TPE1_BoostReq    : 1; 
	BOOL             TPE2_BoostReq    : 1; 
}  t_Boost_Mgr_Input;


typedef struct 
{
	unsigned long    reserved         :24;
	BOOL             disconnectOFDM   : 1;    // to FE DSP from BoostManager
	BOOL             disconnect       : 1;    // to NSK DSP from BoostManager
	BOOL			 Pilot_Enable_Ch1 : 1;	  // to FE DSP from PilotController
	BOOL			 Pilot_Enable_Ch2 : 1;	  // to FE DSP from PilotController
	BOOL			 Pilot_Enable_Ch3 : 1;	  // to FE DSP from PilotController
	BOOL			 AGC_Blocking_Ch1 : 1;	  // to FE DSP from PilotController
	BOOL			 AGC_Blocking_Ch2 : 1;	  // to FE DSP from PilotController
	BOOL			 AGC_Blocking_Ch3 : 1;	  // to FE DSP from PilotController
}  t_Boost_Mgr_Output;

/* E-Wire Signalling data type definition */
typedef struct 
{
	unsigned long        reserved:29;          
	BOOL                 channel3: 1;  // Signalling bit channel 3
	BOOL                 channel2: 1;  // Signalling bit channel 2
	BOOL                 channel1: 1;  // Signalling bit channel 1
} t_E_wire_data;

/* M-Wire Signalling data type definition */
typedef struct 
{
	unsigned long        reserved:29;          
	BOOL                 channel3: 1;  // Signalling bit channel 3
	BOOL                 channel2: 1;  // Signalling bit channel 2
	BOOL                 channel1: 1;  // Signalling bit channel 1
} t_M_wire_data;


/* EOC data type definition */
typedef struct 
{
	unsigned long	reserved:	   10;
	BOOL			dataBreakCh3:	1;	// EOC data transmission ch3 interrupted
	BOOL			dataBreakCh2:	1;	// EOC data transmission ch2 interrupted
	BOOL			dataBreakCh1:	1;	// EOC data transmission ch1 interrupted
	BOOL			bufferFullCh3:	1;	// EOC buffer ch3 full
	BOOL			bufferFullCh2:	1;	// EOC buffer ch2 full
	BOOL			bufferFullCh1:	1;	// EOC buffer ch1 full
	unsigned long	data:		   11;	// EOC Data Word
	unsigned long	channel_id:		5;	// EOC Channel ID

} t_EOC_data;


#ifdef _WIN32 // to allow for compilation under Windows

	typedef struct
	{
		float re, im;
	}
	complex_float;

#endif

/* OFDM input/output signal data type definition */
typedef complex_float t_OFDM_io_signal[OFDM_IO_SIGNAL_CSAMPLES];

typedef struct
{
	unsigned long		reserved		    :12;
	
	BOOL				pilotSNRAlarmCh1	:1;  
	BOOL				pilotSNRAlarmCh2	:1;  
	BOOL				pilotSNRAlarmCh3	:1;  

	BOOL				rxPilotFailCh1		:1;  
	BOOL				rxPilotFailCh2		:1;  
	BOOL				rxPilotFailCh3		:1;  
	
	BOOL				pilotLevelAlarmCh1	:1;  
	BOOL				pilotLevelAlarmCh2	:1;  
	BOOL				pilotLevelAlarmCh3	:1;  

	BOOL				agcFailCh1			:1;  
	BOOL				agcFailCh2			:1;  
	BOOL				agcFailCh3			:1;  

	BOOL				afcFail				:1;  

	BOOL				rxRfOverflow   	    :1;  // flag from ADC

	BOOL				progMemCRCErr		:1;
	BOOL				idleTimeAlarm		:1;
	BOOL				illegalConfig		:1;

	unsigned long		lifesign			:3;

} t_FE_Alarm;


typedef struct
{
	unsigned long		reserved                              :1;

	// link alarms TPE1
	BOOL				tpe1RxLowLevelAlarm                   :1;
	BOOL				tpe1RxHighLevelAlarm                  :1;
	BOOL				tpe1SNRAlarm                          :1;
	BOOL				tpe1NoTripAndNoGuardAlarm             :1;
	BOOL				tpe1TripAndGuardAlarm                 :1;
	BOOL				tpe1LoopTestAlarm                     :1;

	// interface alarms TPE1
	BOOL				tpe1OutputSetToPredefinedStateAlarm   :1;
	BOOL				tpe1RxContinuousAlarm				  :1;
	BOOL				tpe1CommandOutputsBlockedAlarm		  :1;

	// warnings TPE1
	BOOL				tpe1TestCfgActiveWarning              :1;  // not used
	BOOL				tpe1LocalLoopActiveWarning            :1;
	BOOL				tpe1LocalLoopTestWarning              :1;
	BOOL				tpe1TestSignalActiveWarning           :1;

	// link alarms TPE2
	BOOL				tpe2RxLowLevelAlarm                   :1;
	BOOL				tpe2RxHighLevelAlarm                  :1;
	BOOL				tpe2SNRAlarm                          :1;
	BOOL				tpe2NoTripAndNoGuardAlarm             :1;
	BOOL				tpe2TripAndGuardAlarm                 :1;
	BOOL				tpe2LoopTestAlarm                     :1;

	// interface alarms TPE2
	BOOL				tpe2OutputSetToPredefinedStateAlarm   :1;
	BOOL				tpe2RxContinuousAlarm				  :1;

	// warnings TPE2
	BOOL				tpe2TestCfgActiveWarning              :1;  // not used
	BOOL				tpe2LocalLoopActiveWarning            :1;
	BOOL				tpe2LocalLoopTestWarning              :1;
	BOOL				tpe2TestSignalActiveWarning           :1;

	// hardware alarms (common for TPE1&2)
	BOOL				progMemCRCErr                         :1;
	BOOL				idleTimeAlarm                         :1;
	BOOL				illegalConfig						  :1;

	// lifesign
	unsigned long		lifesign                              :3;

} t_NSD_Alarm;

typedef struct
{
	unsigned long		reserved		    	:20;

	BOOL				TPE1_LT_Sent			:1;
	BOOL				TPE1_LT_Reflected		:1;
	BOOL				TPE1_LT_Failed			:1;
	BOOL				TPE1_Manual_LT_Sent		:1;
	BOOL				TPE1_Manual_LT_Failed	:1;
	BOOL				TPE1_Suppress_Alarms  	:1; // instructs monitoring to suppress the following alarms:
													// low SNR in channel 1
													// no rx pilot detected in channel 1
													// low pilot level detected in channel 1
													// agc blocking in channel 1
													// afc failed
	BOOL				TPE2_LT_Sent			:1;
	BOOL				TPE2_LT_Reflected		:1;
	BOOL				TPE2_LT_Failed			:1;
	BOOL				TPE2_Manual_LT_Sent		:1;
	BOOL				TPE2_Manual_LT_Failed	:1;
	BOOL				TPE2_Suppress_Alarms  	:1; // instructs monitoring to suppress the following alarms:
													// low SNR in channel 2
													// no rx pilot detected in channel 2
													// low pilot level detected in channel 2
													// agc blocking in channel 2
													// afc failed
} t_NSD_Events;

typedef struct
{
	unsigned long		reserved				:2;

	BOOL				transitNoSync			:1;
	BOOL				transitMissingRxData	:1;

	BOOL				G703LOSAlarm			:1;

	BOOL				LANNotBooted			:1;
	BOOL				LANAlarm				:1;
	BOOL				LANWarning				:1;
	
	BOOL				SubprintAlarm			:1;

	BOOL				LinkAlm1				:1;
	BOOL				LinkAlm2				:1;
	BOOL				LinkAlm3				:1;
	BOOL				LinkAlm4				:1;

	BOOL				DCDAlm1					:1;
	BOOL				DCDAlm2					:1;
	BOOL				DCDAlm3					:1;
	BOOL				DCDAlm4					:1;

	BOOL				JabberTmo1				:1;
	BOOL				JabberTmo2				:1;
	BOOL				JabberTmo3				:1;
	BOOL				JabberTmo4				:1;
	BOOL				JabberTmo5				:1;
	BOOL				JabberTmo6				:1;

	BOOL				muxLinkAlarm			:1;

	BOOL				testModeWarning			:1;

	BOOL				FPGALoadFailure			:1;
	BOOL				progMemCRCErr			:1;
	BOOL				idleTimeAlarm			:1;
	BOOL				illegalConfig			:1;
	unsigned long		lifesign				:3;

} t_NSK_Alarm;

typedef struct
{
	unsigned long		reserved        :24;

	BOOL				requestSPIbus	:1;
	BOOL				dacInitFailure	:1;
	BOOL				progMemCRCErr   :1;
	BOOL				idleTimeAlarm   :1;
	BOOL				illegalConfig	:1;
	unsigned long		lifesign        :3;

} t_UC_Alarm;


#define	OFDM_SYNCSTATE_Mode_Detection		 0
#define	OFDM_SYNCSTATE_Tone2_Searching	 	 1
#define OFDM_SYNCSTATE_Tone2_detected		 2
#define OFDM_SYNCSTATE_NoiseEst_finished     3
#define	OFDM_SYNCSTATE_FreqEst_finished	     4
#define	OFDM_SYNCSTATE_LFM_finished		     5
#define	OFDM_SYNCSTATE_QAM4_detected		 6
#define	OFDM_SYNCSTATE_Prefix_detected		 7
#define	OFDM_SYNCSTATE_ChannelData_received  8
#define	OFDM_SYNCSTATE_Optim_1_finished	     9
#define	OFDM_SYNCSTATE_Optim_2_finished	    10
#define	OFDM_SYNCSTATE_Optim_3_finished	    11
#define	OFDM_SYNCSTATE_Suffix_detected		12
#define	OFDM_SYNCSTATE_Training_running	    13
#define	OFDM_SYNCSTATE_Training_finished	14
#define	OFDM_SYNCSTATE_V11_sync			    15

typedef struct
{
	unsigned long		reserved           :13;
    unsigned long		ofdm_syncstate     :8; //PL16.05.2006
	BOOL				progMemCRCErr      :1;
	BOOL				idleTimeAlarm      :1;
	BOOL				pilotAlm           :1;
	BOOL				syncAlm            :1;
	BOOL                configAlm		   :1;
	unsigned long	    currentDatarateIx  :3; // Index to current datarate. Range: 0-4, 7 means "not in sync"
	unsigned long		lifesign           :3;

} t_OFDM_Alarm;

typedef struct
{
	unsigned long		reserved				:23;

	BOOL				reset_R1LA				:1;		// For NSK DSP; TRUE = reset LAN
	BOOL				requestSPIbusAllowed	:1;		// For UC DSP; TRUE = SPI bus is free
	// flags used by equalizer functions
	BOOL				pnGenEnableCh1			:1;		// enable pseudo noise generator on ch1			
	BOOL				pnGenEnableCh2			:1;		// enable pseudo noise generator on ch2
	BOOL				pnGenEnableCh3			:1;		// enable pseudo noise generator on ch3
	BOOL				eqAfcDisable			:1;		
	BOOL				eqAgcDisableCh1			:1;	
	BOOL				eqAgcDisableCh2			:1;		
	BOOL				eqAgcDisableCh3			:1;	

} t_MasterDSPOutputFlags;

typedef struct
{
	unsigned long		checksum		: 8;
	unsigned long		reserved        : 3;
	unsigned long		channelMode	    : 1;	// erect: 0, inverted: 1
	unsigned long		frequency       :20;	// [Hz]

} t_UpConvSettings;


/***************************************************************/
/******                                                   ******/
/******       All DSPs TDM I/O data type definition       ******/
/******                                                   ******/
/***************************************************************/

// Since Alpha1 (preventive), we decided to receive every timeslot on every DSP (except for the OFDM DSP) 
// This will make debugging of internal TDM bus communication much 
// easier. Rx interrupts will occur synchronous an all DSPs.

// Definitions taken from TM-PTUKT03-1637, easier naming
// Please note that casting the DMA Rx buffer to this format might 
// be dangerous for future compiler releases (we know AD)
// We can still decide to go back to the former typedef if we need to save 
// DMA buffer space when we know we solved all SPORT/DMA problems.

// WARNING: Never change value N_APLC_CHANNELS=3 - Impact on order in DMA buffer
typedef struct
{
	unsigned long				NSD_Sync_TS00;
	unsigned long               MasterDSPServiceData_TS01;
	float						APLC_TxSignalChannel[N_APLC_CHANNELS];			// TS02, TS03, TS04
	t_M_wire_data				M_WireSignallingAllChannels_TS05;
	t_EOC_data					EOC_TxData_Channel_1_2_3_TS06;
	t_Boost_Mgr_Output			BoostMgrOutput_TS07;
	t_FE_Alarm			    	FE_DSP_AlarmStatusData_TS08;
	unsigned long				FE_DSP_ServiceData_TS09;
	t_NSD_Alarm				    NSD_DSP_AlarmStatusData_TS10;
	unsigned long				NSD_DSP_ServiceData_TS11;
	unsigned long				Channel1_InputCommandStatusData_TS12;
	unsigned long				Channel1_OutputCommandStatusData_TS13;
	unsigned long				Channel2_InputCommandStatusData_TS14;
	unsigned long				Channel2_OutputCommandStatusData_TS15;
	t_OFDM_Alarm				OFDM_DSP_AlarmStatusData_TS16;
	unsigned long				OFDM_DSP_ServiceData_TS17;
	t_NSK_Alarm				    NSK600_DSP_AlarmStatusData_TS18;
	unsigned long				NSK600_DSP_ServiceData_TS19;
	t_UC_Alarm				    UpConverter_DSP_AlarmStatusData_TS20;
	unsigned long				UpConverter_DSP_ServiceData_TS21;
	float						APLC_RxSignalNotEqualized[N_APLC_CHANNELS];		// TS22, TS23, TS24
	float						APLC_RxSignalEqualized[N_APLC_CHANNELS];		// TS25, TS26, TS27
	t_E_wire_data				E_WireSignallingAllChannels_TS28;
	t_EOC_data					EOC_RxData_Channel_1_2_3_TS29;
	complex_float				OFDM_Modem_RxSignal[6];							// (TS30, TS31), (TS32, TS33), (TS34, TS35), (TS36, TS37), (TS38, TS39), (TS40, TS41)
	complex_float				OFDM_Modem_TxSignal[6];							// (TS42, TS43), (TS44, TS45), (TS46, TS47), (TS48, TS49), (TS50, TS51), (TS52, TS53)
	float						NSK600_ModemTxSignal[N_APLC_CHANNELS];			// TS54, TS55, TS56
	complex_float               NSD_TxSignal[N_APLC_CHANNELS];					// (TS57, TS58), (TS59, TS60), (TS61, TS62)
	t_Boost_Mgr_Input   		BoostMgrInput_TS63;

	unsigned long				TdmBusAlarmWord_TS64;
	t_MasterDSPOutputFlags		MasterDSPOutputFlags_TS65;
	float						APLC_SNR_Pilot[N_APLC_CHANNELS-1];				// Unused03_TS66;Unused04_TS67;
	t_UpConvSettings			UpConvSettings_TS68;

	unsigned long				O4CV_RxSubframeStartFlags_TS69;
	unsigned long				O4CV_RxDataCh1_4_TS70;
	unsigned long				O4CV_RxDataCh5_8_TS71;
	unsigned long				O4CV_RxDataCh9_12_TS72;
	unsigned long				O4CV_RxDataCh13_16_TS73;

	unsigned long				O4CV_TxSubframeStartFlags_TS74;
	unsigned long				O4CV_TxDataCh1_4_TS75;
	unsigned long				O4CV_TxDataCh5_8_TS76;
	unsigned long				O4CV_TxDataCh9_12_TS77;
	unsigned long				O4CV_TxDataCh13_16_TS78;

	unsigned long				Unused16_TS79;
	unsigned long				FE_DebugWord0_TS80;
	unsigned long				FE_DebugWord1_TS81;
	unsigned long				FE_DebugWord2_TS82;
	t_NSD_Events				NSD_Events_TS83;
	unsigned long				NSD_DebugWord1_TS84;
	unsigned long				NSD_DebugWord2_TS85;
	unsigned long				NSK_DebugWord0_TS86;
	unsigned long				NSK_DebugWord1_TS87;
	unsigned long				NSK_DebugWord2_TS88;
	unsigned long				OFDM_DebugWord0_TS89;
	unsigned long				OFDM_DebugWord1_TS90;
	unsigned long				OFDM_DebugWord2_TS91;
	float						NSK600_ModemTxSignal_noAP[N_APLC_CHANNELS];		// TS92, TS93, TS94
	unsigned long				FrameEnd_TS95;
} t_AllDSP_input_tdm_block;




/***************************************************************/
/******                                                   ******/
/******    Master DSPs TDM I/O data type definition       ******/
/******                                                   ******/
/***************************************************************/

/* Master DSP TDM input block data type definition */
/* not used because all DSPs receive all slots, i.e. t_AllDSP_input_tdm_block
typedef struct
{
	t_FE_Alarm                  fe_alarm;
	unsigned long               fe_service_data;
	unsigned long               nsd_service_data;
	t_NSD_Alarm                 nsd_alarm;
	unsigned long               ch1_input_cmd_status;
	unsigned long               ch1_output_cmd_status;
	unsigned long               ch2_input_cmd_status;
	unsigned long               ch2_output_cmd_status;
	t_OFDM_Alarm                ofdm_alarm;
	unsigned long               ofdm_service_data;
	t_NSK_Alarm                 nsk_alarm;
	unsigned long               nsk_service_data;
	t_UC_Alarm                  uc_alarm;
	unsigned long               uc_service_data;
	float                       aplc_rx_ne[N_APLC_CHANNELS];
	float                       aplc_rx_eq[N_APLC_CHANNELS];
	t_E_wire_data               ewire_rx;
	t_EOC_data                  eoc_rx;
	t_Boost_Mgr_Input           boostManagerInput;
} t_Master_input_tdm_block;
*/

/* Master DSP TDM output block data type definition */
typedef struct
{
	unsigned long				NSD_Sync;
	unsigned long				service_data;
	float						aplc_tx[N_APLC_CHANNELS];
	t_M_wire_data               mwire_tx;
	//unsigned long				mwire_tx;
	t_EOC_data					eoc_tx;
	t_Boost_Mgr_Output          boostManagerOutput;
	unsigned long				tdmBusAlarmWord;
	t_MasterDSPOutputFlags		flags;
	t_UpConvSettings			upConvSettings;
	unsigned long				O4CV_TxSubframeStartFlags;
	unsigned long				O4CV_TxDataCh1_4;
	unsigned long				O4CV_TxDataCh5_8;
	unsigned long				O4CV_TxDataCh9_12;
	unsigned long				O4CV_TxDataCh13_16;				
	unsigned long				frame_end;
}  t_Master_output_tdm_block;


/***************************************************************/
/******                                                   ******/
/******       NSD DSPs TDM I/O data type definition       ******/
/******                                                   ******/
/***************************************************************/

/* NSD DSP TDM input block data type definition */

/* not used because all DSPs receive all slots, i.e. t_AllDSP_input_tdm_block
typedef struct
{
	unsigned long           NSD_Sync;
	unsigned long           service_data;
	float                   aplc_rx_ne[N_APLC_CHANNELS];
	unsigned long			tdmBusAlarmWord;
	float					SNR_aplc_rx[N_APLC_CHANNELS-1];
} t_NSD_input_tdm_block;
*/

/* NSD DSP TDM output block data type definition */
typedef struct
{
	t_NSD_Alarm					alarm;
	unsigned long				service_data;
	unsigned long				ch1_input_cmd_status;
	unsigned long				ch1_output_cmd_status;
	unsigned long				ch2_input_cmd_status;
	unsigned long				ch2_output_cmd_status;
	complex_float				nsd_tx[N_APLC_CHANNELS];
	t_Boost_Mgr_Input			boostManagerInput;
	t_NSD_Events				NSD_Events;
	unsigned long				debug_word_1;
	unsigned long				debug_word_2;

} t_NSD_output_tdm_block;

/***************************************************************/
/******                                                   ******/
/******     NSK DSPs TDM I/O data type definition         ******/
/******                                                   ******/
/***************************************************************/

/* NSK DSP TDM input block data type definition */
/* not used because all DSPs receive all slots, i.e. t_AllDSP_input_tdm_block
typedef struct 
{
	unsigned long           NSD_Sync;
	unsigned long           service_data;
	t_Boost_Mgr_Output      boostManagerOutput;
	float                   aplc_rx_eq[N_APLC_CHANNELS];
} t_NSK_input_tdm_block;
*/

/* NSK DSP TDM output block data type definition */
typedef struct 
{
	t_NSK_Alarm                 alarm;
	unsigned long               service_data;
	float                       nsk_tx[N_APLC_CHANNELS];
	unsigned long				debug_word_0;
	unsigned long				debug_word_1;
	unsigned long				debug_word_2;
	float                       nsk_tx_noAP[N_APLC_CHANNELS]; // signals to be injected after allpass
} t_NSK_output_tdm_block;

/***************************************************************/
/******                                                   ******/
/******      FE DSPs TDM I/O data type definition         ******/
/******                                                   ******/
/***************************************************************/

/* FE TDM input block data type definition */
/* not used because all DSPs receive all slots, i.e. t_AllDSP_input_tdm_block
typedef struct 
{
	unsigned long				NSD_Sync;
	unsigned long				service_data;
	float						aplc_tx[N_APLC_CHANNELS];
	t_M_wire_data               mwire_tx;
//	unsigned long				mwire_tx;
	t_EOC_data					eoc_tx;
	t_Boost_Mgr_Output          boostManagerOutput;
	t_OFDM_io_signal			ofdm_tx;
	float						nsk_tx[N_APLC_CHANNELS];
	complex_float				nsd_tx[N_APLC_CHANNELS];
	unsigned long				frame_end;
} t_FE_input_tdm_block;
*/

/* FE TDM output block data type definition */
typedef struct 
{
	t_FE_Alarm					alarm;
	unsigned long				service_data;
	float						aplc_rx_ne[N_APLC_CHANNELS];
	float						aplc_rx_eq[N_APLC_CHANNELS];
	t_E_wire_data				ewire_rx;
	t_EOC_data					eoc_rx;
	t_OFDM_io_signal			ofdm_rx;
	float						SNR_aplc_rx[N_APLC_CHANNELS-1];
	unsigned long				debug_word_0;
	unsigned long				debug_word_1;
	unsigned long				debug_word_2;
} t_FE_output_tdm_block;

/***************************************************************/
/******                                                   ******/
/******    OFDM DSPs TDM I/O data type definition         ******/
/******                                                   ******/
/***************************************************************/


/* OFDM TDM input block data type definition */
typedef struct
{
	unsigned long		   NSD_Sync;
	unsigned long          service_data;
	t_OFDM_io_signal       ofdm_rx;
	unsigned long		   frame_end;
}  t_OFDM_input_tdm_block;


/* OFDM TDM output block data type definition */
typedef struct 
{
	t_OFDM_Alarm               alarm;
	unsigned long              service_data;
	t_OFDM_io_signal           ofdm_tx;
} t_OFDM_output_tdm_block;

/***************************************************************/
/******                                                   ******/
/******    UC DSPs TDM I/O data type definition           ******/
/******                                                   ******/
/***************************************************************/


/* UC TDM input block data type definition */
/* not used because all DSPs receive all slots, i.e. t_AllDSP_input_tdm_block
typedef struct
{
	unsigned long           NSD_Sync;
	unsigned long           service_data;
}  t_UC_input_tdm_block;
*/

/* UC TDM output block data type definition */
typedef struct 
{
	t_UC_Alarm              alarm;
	unsigned long           service_data;
} t_UC_output_tdm_block;


#endif
