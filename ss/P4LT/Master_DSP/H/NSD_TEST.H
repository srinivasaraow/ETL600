/*---------------------------------------------------------------------------*
 * $Workfile: NSD_TEST.H $
 * Part of	: NSD550
 * Language	: C
 * Created by: P. Plüer
 * Remarks	:  
 * Purpose	:  
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 1/NSD_DSP/H/NSD_TEST.H $
 * 
 * 5     1.02.05 15:58 Chpeplu
 * struct NSDTestDataStatus changed and function Testsignal_Gen() changed
 * for I and Q signal
 * 
 * 4     28.01.05 14:35 Chpeplu
 * getNSDTestsignal_Gen_Konfig function changed
 * 
 * 3     28.01.05 8:16 Chpeplu
 * function declaration changed for NSD Testsignals
 * 
 * 2     19.05.04 14:50 Nohl01
 * 
 * 1     10.03.04 12:14 Nohl01
 * Initial version
 * Based on MMI500 v. 3.20
 *---------------------------------------------------------------------------*/ 
 
#ifndef		_NSD_TEST
#define		_NSD_TEST 

#include "basetype.h" 

 #define	NUMBER_OF_KEY	16	// used by HMI600
 #define	ROW				4
 #define	COLUMN			4 
 
 //
 #define	PI_F		3.1415927 
 #define	BLOCK_LEN	2
 //
 // defines frequencies for MFC forward
 #define	F_MFC_F1	1380.0
 #define	F_MFC_F2	1500.0
 #define	F_MFC_F3	1620.0
 #define	F_MFC_F4	1740.0
 #define	F_MFC_F5	1860.0
 #define	F_MFC_F6	1980.0
 // defines frequencies for MFC backward
 #define	B_MFC_F1	1140.0
 #define	B_MFC_F2	1020.0
 #define	B_MFC_F3	900.0
 #define	B_MFC_F4	780.0
 #define	B_MFC_F5	660.0
 #define	B_MFC_F6	540.0
 //
 // defines for TestNumber
 #define	NO_TEST		0
 #define	DTMF_TEST	1
 #define	MFC_TEST	2
 #define	ALIEN		3
 // defines for MFC_Direction
 #define	FORWARD		8	// used by HMI600
 #define	BACK		9	// used by HMI600
 #define	BOTH_DIR	10	// used by HMI600
 // defines for SweepDirection
 #define	DIR_UP				1	// used by HMI600
 #define	DIR_DOWN			2	// used by HMI600
 #define	DIR_UP_AND_DOWN		3	// used by HMI600 
 // level for upper group (F1) P = -6.0dBm +-2dB
 // factor corrects 0dB
 #define	DTMF_LEVEL_F1       1.0
 // level for the lower group (F2) P = -8.0dBm +-2.0dB
 // factor corrects only -2dB
 #define	DTMF_LEVEL_F2		0.794		
 //
 typedef struct {
 long	TestNumber;     			// 0: no test; 1:DTMF; 2: MFC; 3:Alien
 float	LevelDTMF;                  //
 long	SendTimeDTMF;
 long	BreakTimeDTMF;
 int	KeySelectDTMF[NUMBER_OF_KEY];
 float 	LevelMFC;
 long	SendTimeMFC;
 long	BreakTimeMFC;
 int	KeySelectMFC[NUMBER_OF_KEY - 1];
 int	MFC_Direction;
 float	LevelAlien;
 float	StartFreq;
 float	StopFreq;
 long	SweepTime;
 long	SweepDirection; 
 long	ChangeFlag;
 } NSDTestDataConfig;	// used by HMI600 

 // defines for StatusDTMF
 #define	START_UP			12
 #define	SEND_KEY			13
 #define	BREAK_S				14
 #define	GET_NEXT_KEY		15 
 
 // defines for StatusMFC
 #define	START_UP_MFC		16
 #define	SEND_MFC			17
 #define	BREAK_MFC			18
 #define	GET_NEXT_MFC		19 
 // defines for StatusAlien
 #define	START_UP_ALIEN		30
 #define	SEND_ALIEN			31
 #define	RESTART_SWEEP		32

 typedef struct {
 long	SendTimeCounter;
 long	BreakTimeCounter;
 int	KeyCounterDTMF;
 int	KeyCounterMFC;
 long	StatusDTMF;
 long	StatusMFC;
 long	StatusAlien;
 float	SweepDeltaFreq;
 float	SweepTempFreq;
 long	SweepTimeCounterSet;
 long	SweepTimeCounter;
 int	Sign_Delta_Sweep;
 float	DeltaArgF1;
 float	DeltaArgF2;
 float	TempArgF1[BLOCK_LEN];
 float	TempArgF2[BLOCK_LEN];
 float	SumSignal_I[BLOCK_LEN];
 float	SumSignal_Q[BLOCK_LEN];
 float	SignalF1_I[BLOCK_LEN];
 float	SignalF1_Q[BLOCK_LEN];
 float	SignalF2_I[BLOCK_LEN];
 float  SignalF2_Q[BLOCK_LEN];
 float	SendLevelF1;
 float	SendLevelF2;

 }NSDTestDataStatus;

// ****************************************************************************
 // function declaration for NSD_TEST.C ****************************************
 // ****************************************************************************
 NSDTestDataConfig * getNSDTestsignal_Gen_Konfig();
 void SEGMENT_EXT_CODE initNSD_Testsignal(void);
 void SEGMENT_EXT_CODE NSD600_Testsignal(void);
 void SEGMENT_EXT_CODE SM_DTMF(void);
 void SEGMENT_EXT_CODE SM_MFC(void);
 void SEGMENT_EXT_CODE SM_Alien(void);
 void SEGMENT_EXT_CODE Testsignal_Gen(float *Tx_Test_Signal_Q, float *Tx_Test_Signal_I);                                      
 //
      


#endif
