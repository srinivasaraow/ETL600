/*--------------------------------------------------------------------------*
 * $Workfile: NSD_Detector.c $
 * Part of:		NSD570 
 * Language:	C
 * Created by:	P.Plueer
 * Remarks:		
 * Purpose:		
 *--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*
 * $Log: /branches/Source R4/P4LT/NSD_DSP/C/NSD_Detector.c $ *
 * 
 * 38    13-02-20 16:57 Chhamaa
 * Initialization for INT_TELPROT_MODE_HSD modified.
 * 
 * 37    12-09-20 10:24 Chhamaa
 * Modifications for 2.5 kHz TPS mode.
 * 
 * 34    08-02-25 10:17 Chtognu
 * Merged changes from ad.equ.branch:
 * NSD trip and guard alarm boolian added for RUENE (set flag whene enough
 * power in largest/second detector comparing to wide band power used
 * additionaly for the alarm "trip and guard")
 * 
 * 32    28.11.06 10:13 Chpeplu
 * NSD600 Extended: if function nsdCustomTable_packEx() is not successfull
 * -> do not unse NSD channel
 * 
 * 31    24.11.06 13:08 Chpeplu
 * Mode for NSD600 extended added, convert custom table with new functions
 * (nsdCustomTable_packEx(), CreateRxCmdTable_ext())
 * 
 * 30    14.11.06 13:39 Chpeplu
 * Function init_Detector() changed for custom mode, Function
 * SwitchOffNotUsedRxCommands() changed for up to six commands, Function
 * Init_CompensationOfEvaluationTime() changed for custom mode
 * 
 * 29    8.11.06 14:32 Chtognu
 * Bug 392 fixed, new input value for CompensationOfEvaluationTime() to
 * reset prolongation after the guard is detected. Changes active for NSD
 * TPE (2 and 4KHz).
 * 
 * 26    8.02.06 13:27 Chpeplu
 * HSD Mode 2+1 removed, code for configurable integration time removed
 * 
 * 25    31.01.06 16:25 Chpeplu
 * new mode HSD integrated, universal ITS for different evaluation times
 * implemented (Alpha-values, offest for second largest detector)
 * 
 * 23    26.10.05 16:54 Chpeplu
 * F4 replaced with F_4
 * 
 * 22    26.10.05 14:21 Chchlee
 * ASSERT removed
 * 
 * 21    29.08.05 16:57 Chpeplu
 * define TONE1_TO_TONE2_MAX_RATIO added;
 *  trip detector prinzip changed  (ratio of T1 and T2 within 20dB added)!
 * 
 * 20    12.08.05 8:03 Chpeplu
 * compensation times increased: 1.1 for blocking; 1.2 for permissive; 1.3
 * for direct
 * 
 * 19    9.08.05 17:43 Chpeplu
 * new  defines COMPENSATION_BLOCKING, COMPENSATION_PERMISSIVE,
 * COMPENSATION_DIRECT added and compensation time increased
 * 
 * 18    30.06.05 8:35 Chpeplu
 * Code walkthrough Pl
 * 
 * 17    27.06.05 10:15 Chpeplu
 * additional case COMMAND_OFF added in function
 * Init_CompensationOfEvaluationTime()
 * 
 * 16    23.06.05 10:31 Chchlee
 * 2 "break" statements were missing
 * 
 * 15    22.06.05 12:08 Chchlee
 * Code walkthrough
 * 
 * 14    27.05.05 14:07 Chpeplu
 * New Parameter added Channel_active, function Detector_NSD() will only
 * be procced if Channel_active = ON, some  comment removed
 * 
 * 13    24.05.05 14:33 Chpeplu
 * optimizer Ov100 improved, Pragma optimizer off used
 * 
 * 12    23.03.05 13:53 Chpeplu
 * new define COEF_PER_SECTION_LPF added, SEGMENT_EXT_DATA used instead of
 * "segment ("ext_dmda")"
 *  
 * 
 * 11    22.03.05 16:55 Chpeplu
 * select second largest detektor output for tev3 corrected
 * 
 * 10    18.03.05 8:34 Chpeplu
 * Alpha values for Tev3 calulated and added for Ch1 and Ch2
 * 
 * 9     17.03.05 17:30 Chpeplu
 * Third evaluation time for detector implemented and detector runs
 * principle -> but no performance checked yet
 * 
 * 8     14.03.05 13:29 Chpeplu
 * Changes made for up to six LP Commands
 * 
 * 7     11.03.05 16:22 Chpeplu
 * Detector use now DefaultCustomTable
 * 
 * 6     9.03.05 14:05 Chpeplu
 * New include file NSD_CustomTableConverter.h added
 * 
 * 5     9.03.05 10:27 Chpeplu
 * New config used (UsedCommands)
 * 
 * 4     8.03.05 10:44 Chpeplu
 * Init Command code table according costum table (only Test table not
 * costum table from config)!
 * 
 * 3     8.03.05 7:22 Chpeplu
 * Defines NUMBER_OF_APPLICATION_MODES and F_1 ... F_7 moved to
 * NSD_DSP_Types.h
 * 
 * 2     3.03.05 14:04 Chpeplu
 * prepared for NewConfigNSD without custom table
 * 
 * 1     19.01.05 15:33 Chchlee
 * 
 * 21    17.01.05 14:05 Chpeplu
 * Compensation time for tev1 Alpha value only used -> not mix of tev1 and
 * tev2 Alpha values
 * 
 * 20    13.01.05 16:09 Chpeplu
 * Function  init_Detector() moved to extern Code segement!
 * 
 * 19    13.01.05 14:13 Chpeplu
 * Parallel receiver for great level differences of the two tones added;
 * Min. Speech power for detection of rx command added
 * 
 * 18    12.01.05 17:18 Chpeplu
 * Functions Init_CompensationOfEvaluationTime() and
 * CompensationOfEvaluationTime() added
 * 
 * 17    7.12.04 11:54 Chpeplu
 * New function Update_Dev_Status_Det() added
 * 
 * 16    2.12.04 8:06 Chpeplu
 * Defines and struct definition inserted from NSD_Detector.h
 * 
 * 15    1.12.04 17:22 Chpeplu
 * PowerSpeech data update added -> used for Unblocking 
 * 
 * 14    12.11.04 9:45 Plueer01
 * Init functions ResetCommandTable() and SwitchOffNotUsedRxCommands()
 * added to disable all not used and switched off commands in the command
 * table 
 * 
 * 13    19.10.04 14:32 Plueer01
 * Detection of command AC (and  ABCD, BC, D, C)  corrected!
 * 
 * 12    8.09.04 14:25 Plueer01
 * void init_Detector(t_NSD_DSP_Config *g_NSD_Config) -> config struct
 * added
 * 
 * 11    22.07.04 14:56 Plueer01
 * Functions for NSD-detector changed for dual channel use
 * 
 * 10    8.07.04 11:17 Plueer01
 * anges according new NSD-config struct 
 * 
 * 9     7.07.04 10:39 Plueer01
 * Some minor changes made for optimizer
 * 
 * 8     22.06.04 16:16 Plueer01
 * Alpha settings changed according "calalphN.m" for N=22
 * 
 * 7     4.06.04 16:56 Plueer01
 * Addtional functionallity for NSD detector added
 * 
 * 6     1.06.04 16:39 Plueer01
 * Calculation of correlation summe and detector output calculation and
 * selection of largest and second largest detector added
 * 
 * 5     28.05.04 16:45 Plueer01
 * Additional functionallity added
 * 
 * 4     28.05.04 10:43 Plueer01
 * Some small modifications made
 * 
 * 3     27.05.04 14:40 Plueer01
 * Make it compilable
 * 
 * 2     27.05.04 13:57 Plueer01
 * 
 * 1     27.05.04 13:47 Plueer01
 * Inital version
 * 
 *--------------------------------------------------------------------------*/

/* INCLUDE FILES **************************************************************/
#include	<math.h>
//
#include "basetype.h"
#include "NSD_DSP_Types.h"
#include "NSD_Detector.h"
#include "NSD_Transmitter.h"
#include "nsdCustomTable.h"
#include "NSD_CustomTableConverter.h"
#include "diag.h" 
#include "NSD_Monitor.h"
/* LOCAL CONSTANT DECLARATIONS ***********************************************/
//
// defines for evaluation time compensation ********
//
#define	COMPENSATION_BLOCKING		1.1
#define	COMPENSATION_PERMISSIVE		1.2
#define	COMPENSATION_DIRECT			1.3
//
// defines for detector ****************************
// number of adaptive detectors N1 ... N3
#define	ADAPT_NUMB						3

//
#define	NUMBER_OF_PATH_CORRELATOR		4


#define	RX_DETECTOR_SAMPL_FREQ			(AF_SAMPLE_FREQ/2.0)
//
// defines for correlator
#define	NUMBERS_OF_INTEGR_SUM			(ADAPT_NUMB*NUMBER_OF_PATH_CORRELATOR*NUMBER_OF_RX_DET_TONES)
//
#define	NUMBER_OF_CORREL_OUTPUTS		(NUMBER_OF_PATH_CORRELATOR*NUMBER_OF_RX_DET_TONES)
//
#define	GAMMA							0.4
//
//Ratio of tone 1 to tone 2 to accept a correct command is set to 20dB level difference
//(new added after detection bug #195 system test ETL600 Rel. 1)
#define TONE1_TO_TONE2_MAX_RATIO		100.0
// Speech Power with 0dB boost of dual tone command for Tev2 = 1.57
// This value was measured  with upload ITS-Status per HMI in ch1 with FW Version from 10.1.2005
//
// set minimum detection power in speech band to -20dB
#define	MIN_POWER_SPEECH				0.0157
//
// LP-Filter 
//
// number of biquad sections
#define RX_NUM_OF_SEC_LP			2	            	
// LP-filter state array length		
#define RX_LPF_SARRAY_LENGTH		2*RX_NUM_OF_SEC_LP	
//
#define COEF_PER_SECTION_LPF		4
//
// 
// Calculation according math lab-File "LP_NSD_1.m"
 // NSD600 Ch1 Filter
static float pm Ch1_LP_Rx1700Hz[COEF_PER_SECTION_LPF*RX_NUM_OF_SEC_LP + 1] = {
 -6.3217804e-001,  1.5393106e+000,  1.0000000e+000, -2.5368935e-002,
 -8.7789938e-001,  1.6341318e+000,  1.0000000e+000, -1.3423585e+000,
 1.6457508e-002};
// NSD600 Ch2 Filter
static float pm Ch2_LP_Rx1700Hz[COEF_PER_SECTION_LPF*RX_NUM_OF_SEC_LP + 1] = {
 -6.3217804e-001,  1.5393106e+000,  1.0000000e+000, -2.5368935e-002,
 -8.7789938e-001,  1.6341318e+000,  1.0000000e+000, -1.3423585e+000,
 1.6457508e-002};
// Calculation according math lab-File "LP_HSD_1.m"
static float pm Ch1_HSD_LP_Rx1300Hz[COEF_PER_SECTION_LPF*RX_NUM_OF_SEC_LP + 1] = {
  -7.0517402e-001,  1.6487546e+000,  1.0000000e+000, -5.6395697e-001,
  -9.0352713e-001,	1.7575446e+000,  1.0000000e+000, -1.5937789e+000,
  1.3329065e-002};
// ***************************************************************************
// *** Code Table for NSD600 *************************************************
// ***************************************************************************
//****************************************************************************
// Command Table (dummy array) 4 dual tone commands   ************************
//****************************************************************************
//L: low alpha for blocking application
//M: medium alpha for permissive application
//H: high alpha for direct application
//
//		|	0	 |	    F1	    |	   F2	   |      F3	  |	     F4      |      F5     |      F6      |      F7      |
//		|--------|--------------|--------------|--------------|--------------|-------------|--------------|--------------|
//		|L	M	H|L	    M     H |L     M     H |L     M     H |L     M     H |L     M     H|L     M     H |L     M     H |
//		|--------|--------------|--------------|--------------|--------------|-------------|--------------|--------------|
//	0	|0	0	0|0     0     0 |0     0	 0 |0     0     0 |0     0     0 |0     0    0 |0     0     0 |0     0     0 |
//  F1  |0	0	0|0	    0	  0 |A     A	 A |AB    AB   AB |ABC  ABC  ABC |AD    AD  AD |0     0     0 |ABCD ABCD ABCD|
//	F2  |0	0	0|A	    A	  A |0     0	 0 |B     B     B |BC    BC   BC |0     0    0 |BD   BD    BD |0     0     0 |
//  F3  |0	0	0|AB   AB    AB |B     B	 B |0     0     0 |C     C     C |AC    AC  AC |0     0     0 |T     T     T |
//  F4  |0	0	0|ABC  ABC	 ABC|BC   BC	BC |C     C     C |0     0     0 |0     0    0 |CD   CD    CD |0     0     0 |
//  F5  |0	0	0|AD    AD	 AD |0     0	 0 |AC    AC   AC |0     0     0 |0     0    0 |D     D     D |ACD  ACD   ACD|
//  F6  |0	0	0|0	    0	  0 |BD   BD	BD |0     0     0 |CD    CD   CD |D     D    D |0     0     0 |ABD  ABD   ABD|
//  F7  |0	0	0|ABCD ABCD ABCD|0     0	 0 |T     T     T |0     0     0 |ACD  ACD ACD |ABD  ABD  ABD |0     0     0 |
//                |             |      |
//				  |				|	   |				
//                |				|	    -> Command B is permissive 
//				  |				 -> Looptest 
//                 -> Command A blocking
// The following table will be set during the initialisation for blocking, permissive or direct application
//
// 
//
/*
// -> This table will be created with the costum table !!!!
//
long	Ch1_CMD_Table_NSD[NUMBER_OF_APPLICATION_MODES * NUMBER_OF_RX_DET_TONES*2 * NUMBER_OF_RX_DET_TONES*2]={
	0,	 0,	 0,	 0,	         0,	       0,	      0,	     0,	       0,       0,	      0,	   0,	    0,	       0,	     0,         0,	      0,	    0,        0,	   0,	    0,        0,	    0,	      0, 
	0,	 0,	 0,	 0,	         0,	       0,		  0,		 0,		   0,		0,		  0,	   0,		0,		   0,		 0,			0,		  0,		0,        0,	   0,	    0,		COM_T,	  COM_T,	COM_T,
	0,   0,  0,  0,			 0,		   0,		  0,         0,        0,	  COM_CD,	COM_CD,	 COM_CD, COM_ABCD,COM_ABCD,	COM_ABCD,	COM_BCD,  COM_BCD,	COM_BCD,   COM_ACD, COM_ACD, COM_ACD,	COM_ABD, COM_ABD,  COM_ABD,
	0,   0,  0,  0,			 0,        0,		COM_CD,	   COM_CD,	COM_CD,		0,        0,       0,	  COM_D,	COM_D,	  COM_D,	 COM_BD,   COM_BD,	 COM_BD,	COM_AD,	COM_AD,	 COM_AD,	COM_ABC, COM_ABC,  COM_ABC,
	0,	 0,	 0,  0,			 0,		   0,	  COM_ABCD,	  COM_ABCD,COM_ABCD,  COM_D,	COM_D,	 COM_D,		0,         0,        0,		  COM_C,	COM_C,	  COM_C,	COM_BC,	COM_BC,	 COM_BC,	COM_AC,	  COM_AC,   COM_AC,
	0,	 0,	 0,	 0,			 0,		   0,	   COM_BCD,	  COM_BCD,	COM_BCD,  COM_BD,  COM_BD,	COM_BD,	  COM_C,	COM_C,	  COM_C,		0,	      0,	    0,		COM_B,	 COM_B,	  COM_B,	COM_AB,	  COM_AB,	COM_AB,
	0,	 0,	 0,	 0,		     0,	       0,	   COM_ACD,	  COM_ACD,	COM_ACD,  COM_AD,  COM_AD,	COM_AD,	  COM_BC,	COM_BC,	  COM_BC,	  COM_B,	COM_B,	  COM_B,      0,	   0,		0,		 COM_A,	  COM_A,	COM_A,
	0,	 0,	 0,	COM_T,	   COM_T,	COM_T,	   COM_ABD,	  COM_ABD,	COM_ABD,  COM_ABC, COM_ABC,	COM_ABC,  COM_AC,	COM_AC,	  COM_AC,	 COM_AB,   COM_AB,	 COM_AB,	COM_A,	 COM_A,	  COM_A,	  0,	    0,	      0};
*/
//			   |                              |                             |                           |                             |					            |					          |					            |
//       0     |           F1                 |            F2		        |             F3            |             F4              |	           F5		        |		      F6		      |		        F7		        |
//  L   M   H  | L         M          H       |  L          M          H    |  L           M        H   |  L           M         H    |   L		   M	        H   |     L	       M	    H     |    L	    M	      H     |
//
static long Ch1_Correl_to_Freq_Map[ADAPT_NUMB * NO_OF_TONES] = {
		F_4, F_4, F_4, F_4, F_4, F_4, F_3, F_3, F_3, F_5, F_5, F_5, F_2, F_2, F_2, F_6, F_6, F_6, F_1, F_1, F_1, F_7, F_7, F_7};
//
//
/*
// -> This table will be created with the costum table !!!!
//
long	Ch2_CMD_Table_NSD[NUMBER_OF_APPLICATION_MODES * NUMBER_OF_RX_DET_TONES*2 * NUMBER_OF_RX_DET_TONES*2]={
	0,	 0,	 0,	 0,	         0,	       0,	      0,	     0,	       0,       0,	      0,	   0,	    0,	       0,	     0,         0,	      0,	    0,        0,	   0,	    0,        0,	    0,	      0, 
	0,	 0,	 0,	 0,	         0,	       0,		  0,		 0,		   0,		0,		  0,	   0,		0,		   0,		 0,			0,		  0,		0,        0,	   0,	    0,		COM_T,	  COM_T,	COM_T,
	0,   0,  0,  0,			 0,		   0,		  0,         0,        0,	  COM_CD,	COM_CD,	 COM_CD, COM_ABCD,COM_ABCD,	COM_ABCD,	COM_BCD,  COM_BCD,	COM_BCD,   COM_ACD, COM_ACD, COM_ACD,	COM_ABD, COM_ABD,  COM_ABD,
	0,   0,  0,  0,			 0,        0,		COM_CD,	   COM_CD,	COM_CD,		0,        0,       0,	  COM_D,	COM_D,	  COM_D,	 COM_BD,   COM_BD,	 COM_BD,	COM_AD,	COM_AD,	 COM_AD,	COM_ABC, COM_ABC,  COM_ABC,
	0,	 0,	 0,  0,			 0,		   0,	  COM_ABCD,	  COM_ABCD,COM_ABCD,  COM_D,	COM_D,	 COM_D,		0,         0,        0,		  COM_C,	COM_C,	  COM_C,	COM_BC,	COM_BC,	 COM_BC,	COM_AC,	  COM_AC,   COM_AC,
	0,	 0,	 0,	 0,			 0,		   0,	   COM_BCD,	  COM_BCD,	COM_BCD,  COM_BD,  COM_BD,	COM_BD,	  COM_C,	COM_C,	  COM_C,		0,	      0,	    0,		COM_B,	 COM_B,	  COM_B,	COM_AB,	  COM_AB,	COM_AB,
	0,	 0,	 0,	 0,		     0,	       0,	   COM_ACD,	  COM_ACD,	COM_ACD,  COM_AD,  COM_AD,	COM_AD,	  COM_BC,	COM_BC,	  COM_BC,	  COM_B,	COM_B,	  COM_B,      0,	   0,		0,		 COM_A,	  COM_A,	COM_A,
	0,	 0,	 0,	COM_T,	   COM_T,	COM_T,	   COM_ABD,	  COM_ABD,	COM_ABD,  COM_ABC, COM_ABC,	COM_ABC,  COM_AC,	COM_AC,	  COM_AC,	 COM_AB,   COM_AB,	 COM_AB,	COM_A,	 COM_A,	  COM_A,	  0,	    0,	      0};
*/
 //			   |                              |                             |                           |                             |					            |					          |					            |
//       0     |           F1                 |            F2		        |             F3            |             F4              |	           F5		        |		      F6		      |		        F7		        |
//  L   M   H  | L         M          H       |  L          M          H    |  L           M        H   |  L           M         H    |   L		   M	        H   |     L	       M	    H     |    L	    M	      H     |
//
static long Ch2_Correl_to_Freq_Map[ADAPT_NUMB * NO_OF_TONES] = {
		F_4, F_4, F_4, F_4, F_4, F_4, F_3, F_3, F_3, F_5, F_5, F_5, F_2, F_2, F_2, F_6, F_6, F_6, F_1, F_1, F_1, F_7, F_7, F_7};
//
static long Correl_to_Freq_Map_HSD[ADAPT_NUMB * NO_OF_TONES] = {
		F_3, F_3, F_3, F_3, F_3, F_3, F_2, F_2, F_2, F_4, F_4, F_4, F_1, F_1, F_1, F_5, F_5, F_5, F_0, F_0, F_0, F_0, F_0, F_0};
//
// ****************************************************************************
// *** New NSD600 Code Table according to Custom table ************************
// ****************************************************************************
// Ch1 code table
static unsigned long Ch1_COMMAND_Table_NSD[NUMBER_OF_APPLICATION_MODES * NUMBER_OF_RX_DET_TONES*2 * NUMBER_OF_RX_DET_TONES*2];
// Ch2 code table
static unsigned long Ch2_COMMAND_Table_NSD[NUMBER_OF_APPLICATION_MODES * NUMBER_OF_RX_DET_TONES*2 * NUMBER_OF_RX_DET_TONES*2];
//

 // Alpha settings Ch1 
 // adaptive detector for tev1 and tev2, tev3 (calculation of alpha values see mathlab file calalphN.m)
static SEGMENT_EXT_DATA float Ch1_Alpha_Setting[NUMBER_OF_APPLICATION_MODES*ADAPT_NUMB] ={
	0.968576,  0.987500, 0.996854,	0.774500,  0.847280, 0.914510, 0.503203, 0.585964, 0.684174 	
};	
//	tev1	    tev1		tev1		tev2	tev2		tev2	tev3		tev3		tev3
//	low			med			high		low		med			high	low			med			high
// 1*10-4     1*10-6      1*10-9      1*10-4    1*10-6      1*10-9	1*10-4		1*10-6      1*10-9
//
 // Alpha settings Ch2 
 // adaptive detector for tev1 and tev2, tev3 (calculation of alpha values see mathlab file calalphN.m)
static SEGMENT_EXT_DATA float Ch2_Alpha_Setting[NUMBER_OF_APPLICATION_MODES*ADAPT_NUMB] ={
	0.968576,  0.987500, 0.996854,	0.774500,  0.847280, 0.914510, 0.503203, 0.585964, 0.684174 	
};	
//	tev1	    tev1		tev1		tev2	tev2		tev2	tev3		tev3		tev3
//	low			med			high		low		med			high	low			med			high
// 1*10-4     1*10-6      1*10-9      1*10-4    1*10-6      1*10-9	1*10-4		1*10-6      1*10-9
//
// Alpha settings HSD600 Version 0.63
// adaptive detector for tev1 and tev2, tev3 (calculation of alpha values see mathlab file calalphN.m)
static SEGMENT_EXT_DATA float Ch1_Alpha_Setting_HSD[NUMBER_OF_APPLICATION_MODES*ADAPT_NUMB] ={
// Alpha values for N=28 1*10-3, 1*10-6, 1*10-9
 0.936859,  0.985041,	0.996433,	0.694848,  0.833688, 0.908692, 0.426996,  0.568438, 0.673611	
};	
//	tev1	    tev1		tev1		tev2	tev2		tev2	tev3		tev3		tev3
//	low			med			high		low		med			high	low			med			high
// 1*10-4     1*10-6      1*10-9      1*10-4    1*10-6      1*10-9	1*10-4		1*10-6      1*10-9
//
// start value to evaluate second largest detector in case that first detector is the largest 
#define	START_SELECT_SECOND_NSD		6
//
				
/* LOCAL TYPE DECLARATIONS ***************************************************/
typedef struct {
	float Detector_freq[NUMBER_OF_RX_DET_TONES];	// only used for init -> use local variable to save memory
	long *ptr_Correl_to_freq_map;					// pointer to table for correlatoroutput  to frequncy mapping (1...7)
	unsigned long *PtrCOMTAB;						// pointer to Frequency to command mapping
	unsigned long  Integration_time;				// Integration time used (in samples for 5.333kHz)
	float RxIF_Sin;									// IF frequency sine value
	float RxIF_Cos;									// IF frequency cos value
	float sinDeltaRxIF;								// delta phi for sin Rx IF
	float cosDeltaRxIF;								// delta phi for cos Rx IF
	float pm *LP_Coef;								// pointer to filter coeffizients for LP Filter after frequnecy shift to zero
	float dm LP_FilterState1[RX_LPF_SARRAY_LENGTH]; // Filter state values of section 1
	float dm LP_FilterState2[RX_LPF_SARRAY_LENGTH]; // Filter state values of section 2
	float NCorrePower[ADAPT_NUMB];					// correction factor for integration time N1, N2, N3
	float *ptr_sumPowerN1;							// pointer to integration summ of power N1
	float *ptr_sumPowerN2;							// pointer to integration summ of power N2
	float *ptr_sumPowerN3;							// pointer to integration summ of power N3
	float OutputWideBand[ADAPT_NUMB];				// Power within NSD channel for N1, N2, N3
	float Power_Verif[ADAPT_NUMB];					// Verification sum for Power N1, N2, N3
	float *startAdrPowerN1;							// start adress of integration sum N1
	float *startAdrPowerN2;							// start adress of integration sum N2
	float *startAdrPowerN3;							// start adress of integration sum N3
	float Sin_Value[NUMBER_OF_RX_DET_TONES];		// local variable could also be used instead of a struct variabel
	float Cos_Value[NUMBER_OF_RX_DET_TONES];		// local variable could also be used instead of a struct variabel
	float Sin_DetArg[NUMBER_OF_RX_DET_TONES];		// sin delta phi for all detector frequencies 
	float Cos_DetArg[NUMBER_OF_RX_DET_TONES];		// cos delta phi for all detector frequencies 
	float DetSin[NUMBER_OF_RX_DET_TONES];			// det input * sin for all frequencies
	float DetCos[NUMBER_OF_RX_DET_TONES];			// det. onput * cos for all frequencies
	float *ptr_Sum_Out;								// actual pointer to the integration sum 
	float Sum_Out[ADAPT_NUMB * NUMBER_OF_PATH_CORRELATOR * NUMBER_OF_RX_DET_TONES]; // output value of all integrations sum
	float Sum_Inp[NUMBER_OF_CORREL_OUTPUTS];		// input for correlation sum for each detector
	float *ptrSum_F[ADAPT_NUMB * NUMBER_OF_RX_DET_TONES * NUMBER_OF_PATH_CORRELATOR]; //actual pointer integration sum for each correlator
	float Sum_Verif[ADAPT_NUMB * NUMBER_OF_RX_DET_TONES * NUMBER_OF_PATH_CORRELATOR]; // verification sum for each integration
	float *endptrSum_F[ADAPT_NUMB * NUMBER_OF_RX_DET_TONES * NUMBER_OF_PATH_CORRELATOR];	// start pointer for each integr. sum
	float *startptrSum_F[ADAPT_NUMB * NUMBER_OF_RX_DET_TONES * NUMBER_OF_PATH_CORRELATOR];	// end pointer for each integr. sum
	float OutputDet[ADAPT_NUMB * NO_OF_TONES];		// actual detector output power for all frequencies
	long Largest_Detektor[ADAPT_NUMB];				// largest detector output N1 and N2, N3
	long Second_Detektor[ADAPT_NUMB];				// second largest detector ouput for N1 and N2, N3
	long Activ_freq;								// activ detector frequency 1
	long Activ_freq_1;								// activ detector frequency 2
	float Alpha_1[NUMBER_OF_APPLICATION_MODES*ADAPT_NUMB];	// alpha values for all applications(blocking perm. direct) and all integration times N1,N2, N3
	float Lamda[NUMBER_OF_APPLICATION_MODES*ADAPT_NUMB];	// lamda values for all applications(blocking, per. direct) and all integr. times N1, N2, N3	
	BOOL  Channel_active;							// on if channel is active
	BOOL  HSD_On;									// True if HSD is configured
	long  Start_Select_Second;						// start value to select second largest detector
	long  Offset_1;									// Offset if mode f0 = Fdet 0
//	long *Ptr_Com;
} t_NSD_Detector;
//
typedef struct {

	BOOL	Channel_Active;										// on/off switch 
	long	CompensationTime[MAX_NUMBER_LP_COMMANDS];			// compensation time for each command 
	long	CounterCompensationTime[MAX_NUMBER_LP_COMMANDS];	// counter for the compensation time for each command

} t_EvaluationTimeCompensation_Type;
/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL VARIABLES ***********************************************************/
//
static t_EvaluationTimeCompensation_Type t_EvalComp_Ch1, t_EvalComp_Ch2;
//
static t_NSD_Detector t_Ch1_Detector, t_Ch2_Detector;
 
t_NSD_Power_Levels * ptr_NSD_Power_Speech_Guard;
//
 // power sum Array for three evaluation times
 // Channel 1
 SEGMENT_EXT_DATA float PowerArray1_N1[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float PowerArray1_N2[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float PowerArray1_N3[MAX_INTEGR_TIME*2*2];
 //
 //Channel 2
 SEGMENT_EXT_DATA float PowerArray2_N1[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float PowerArray2_N2[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float PowerArray2_N3[MAX_INTEGR_TIME*2*2];
 //
 // correlator sum Arrays for all frequencies an three different integration times t1, t2, t3
 //
 // Ch1 
 SEGMENT_EXT_DATA float Sum1F1AN1_N[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum1F1AN2_N[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum1F1AN3_N[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum1F1BN1_N[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum1F1BN2_N[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum1F1BN3_N[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum1F1CN1_P[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum1F1CN2_P[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum1F1CN3_P[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum1F1DN1_P[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum1F1DN2_P[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum1F1DN3_P[MAX_INTEGR_TIME*2*2];

 SEGMENT_EXT_DATA float Sum1F2AN1_N[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum1F2AN2_N[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum1F2AN3_N[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum1F2BN1_N[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum1F2BN2_N[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum1F2BN3_N[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum1F2CN1_P[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum1F2CN2_P[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum1F2CN3_P[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum1F2DN1_P[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum1F2DN2_P[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum1F2DN3_P[MAX_INTEGR_TIME*2*2];

 SEGMENT_EXT_DATA float Sum1F3AN1_N[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum1F3AN2_N[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum1F3AN3_N[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum1F3BN1_N[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum1F3BN2_N[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum1F3BN3_N[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum1F3CN1_P[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum1F3CN2_P[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum1F3CN3_P[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum1F3DN1_P[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum1F3DN2_P[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum1F3DN3_P[MAX_INTEGR_TIME*2*2];

 SEGMENT_EXT_DATA float Sum1F4AN1_N[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum1F4AN2_N[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum1F4AN3_N[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum1F4BN1_N[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum1F4BN2_N[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum1F4BN3_N[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum1F4CN1_P[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum1F4CN2_P[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum1F4CN3_P[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum1F4DN1_P[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum1F4DN2_P[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum1F4DN3_P[MAX_INTEGR_TIME*2*2];
//
// Ch2
 SEGMENT_EXT_DATA float Sum2F1AN1_N[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum2F1AN2_N[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum2F1AN3_N[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum2F1BN1_N[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum2F1BN2_N[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum2F1BN3_N[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum2F1CN1_P[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum2F1CN2_P[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum2F1CN3_P[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum2F1DN1_P[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum2F1DN2_P[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum2F1DN3_P[MAX_INTEGR_TIME*2*2];

 SEGMENT_EXT_DATA float Sum2F2AN1_N[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum2F2AN2_N[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum2F2AN3_N[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum2F2BN1_N[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum2F2BN2_N[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum2F2BN3_N[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum2F2CN1_P[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum2F2CN2_P[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum2F2CN3_P[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum2F2DN1_P[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum2F2DN2_P[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum2F2DN3_P[MAX_INTEGR_TIME*2*2];

 SEGMENT_EXT_DATA float Sum2F3AN1_N[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum2F3AN2_N[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum2F3AN3_N[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum2F3BN1_N[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum2F3BN2_N[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum2F3BN3_N[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum2F3CN1_P[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum2F3CN2_P[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum2F3CN3_P[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum2F3DN1_P[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum2F3DN2_P[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum2F3DN3_P[MAX_INTEGR_TIME*2*2];

 SEGMENT_EXT_DATA float Sum2F4AN1_N[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum2F4AN2_N[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum2F4AN3_N[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum2F4BN1_N[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum2F4BN2_N[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum2F4BN3_N[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum2F4CN1_P[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum2F4CN2_P[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum2F4CN3_P[MAX_INTEGR_TIME*2*2];
 SEGMENT_EXT_DATA float Sum2F4DN1_P[MAX_INTEGR_TIME];
 SEGMENT_EXT_DATA float Sum2F4DN2_P[MAX_INTEGR_TIME*2];
 SEGMENT_EXT_DATA float Sum2F4DN3_P[MAX_INTEGR_TIME*2*2];
//
//
// float *ptr_RxIF_Sin_Ch1, *ptr_RxIF_Cos_Ch1;
// float *ptr_RxIF_Sin_Ch2, *ptr_RxIF_Cos_Ch2;
//
// detector settings 
//
// float *ptr_DetSin_Ch1, *ptr_DetCos_Ch1;
// float *ptr_DetSin_Ch2, *ptr_DetCos_Ch2;

 static long	*ptr_Correl_to_freq_map;
 static unsigned long *PtrCOMTAB;

 //
 static t_Custom2CommandTable C2C_Table;
 static t_NSDCustomTable UnpCustomTable;

 static SEGMENT_EXT_DATA t_NSDExtCustomTable localCustomTable;

/* LOCAL FUNCTIONS ***********************************************************/
static  void SwitchOffNotUsedRxCommands(unsigned long *ptr_CommandTable, long UsedRxCommands);
static void ResetCommandTable(unsigned long *ptr_CommandTable);

/* IMPLEMENTATION ************************************************************/
#pragma optimize_off

/*****************************************************************************
  Function:		ResetCommandTable()
  Description:	This function is called, if no NSD is configured to reset the rx command tabel zu zero!
				There will be not even one correct frequency combination to detect a rx command!
  Inputs:		Startadress of command table
  Outputs:		None
  Return:		None
  Globals:		
  Important:	The size of the command table have to be the same as the following expression:
				NUMBER_OF_APPLICATION_MODES * NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2

*******************************************************************************/
void ResetCommandTable(unsigned long *ptr_CommandTable)
{
	long i_temp1;

	for (i_temp1 =0; i_temp1< NUMBER_OF_APPLICATION_MODES * NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2 ;i_temp1++)
	{
		*ptr_CommandTable++ = 0x00000000;
	}
}
/*****************************************************************************
  Function:		SwitchOffNotUsedCommands()
  Description:	.
  Inputs:		Startadress of command table, Configured used rx commands (Bit0 (Com A) - Bit3 (Com_D))
  Outputs:		None
  Return:		None
  Globals:		
  Important:	This function is only useable  for up to 4 commands!
				The size of the command table have to be the same as the following expression:
				NUMBER_OF_APPLICATION_MODES * NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2

*******************************************************************************/
void SwitchOffNotUsedRxCommands(unsigned long *ptr_CommandTable, long UsedRxCommands)
{
	int i = 0;
	int i_temp = 0;
	long RxMask = 0;
	unsigned long *ptr_Temp;

	// @Pl_ext
	RxMask = (UsedRxCommands&ALL_LP_COMMANDS_A_F);

	for (i=0; i<MAX_NUMBER_LP_COMMANDS;i++)
	{
		ptr_Temp = ptr_CommandTable;		// set start address of RX Command Table

		if((RxMask&(BIT_0_SET<<i)) == 0x00000000)
		{
			//
			// Rx Comnmand is not used -> remove command and command combinations in the Rx Command table
			//
			for (i_temp =0; i_temp< NUMBER_OF_APPLICATION_MODES * NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2 ;i_temp++)
				{
					// Search command in the command table
					// delete all commands and command combinations of "not used rx commands"
					//
					if(((*ptr_Temp)&(BIT_0_SET<<i)) == 0x00000000)
					{
						// do nothing command is used
					}
					else
					{
						// delete command or command combination for "not used command"
						//Ch1_COMMAND_Table_NSD[i_temp] = 0x00000000;
						*ptr_Temp = 0x00000000;
					}
					ptr_Temp++;
				}		
		}
		else
		{
			// Command is used do not reset commands in Rx Command Table
		}

	}
}
/*****************************************************************************
  Function:		init_detector()
  Description:	This function will do the initialisation of the two NSD instances at all.
  Inputs:		Configuration of NSD's
  Outputs:		None
  Return:		None
  Globals:		
  Important:	-

*******************************************************************************/
//
//
void SEGMENT_EXT_CODE init_Detector(t_NSD_DSP_Config *g_NSD_Config)
{
	int		i, i_temp;
	float	f_temp;
	long	l_Array[MAX_NUMBER_LP_COMMANDS];
	//
	long    l_N_HSD;
	// @Pl_ext
	BOOL success;
	//
	long TPE_Basic_Mode = 0;
	long TPE1_Custom_Mode = 0;
	long TPE2_Custom_Mode = 0;
	//
	// @Pl_ext
	TPE_Basic_Mode = (g_NSD_Config->mode & INT_TPE_MODE_MASK);
	TPE1_Custom_Mode = (g_NSD_Config->mode & INT_TPE1_MODE_EXT_MASK);
	TPE2_Custom_Mode = (g_NSD_Config->mode & INT_TPE2_MODE_EXT_MASK);
	//
	t_Ch1_Detector.HSD_On = FALSE;
	t_Ch2_Detector.HSD_On = FALSE;
	//
/*	// ****************************************************
	// *** init Command Code Table according custom table *
	// ****************************************************
	//
	nsdCustomTable_pack (FALSE, UnpCustomTable, g_NSD_Config->NSD[0].customTable);
	ConvertCustom2CommandTable(UnpCustomTable, C2C_Table);
	// 
	GenerateDetectorCodeTable(C2C_Table, Ch1_COMMAND_Table_NSD);
	//
	nsdCustomTable_pack (FALSE, UnpCustomTable, g_NSD_Config->NSD[1].customTable);
	ConvertCustom2CommandTable(UnpCustomTable, C2C_Table);
	// 
	GenerateDetectorCodeTable(C2C_Table, Ch2_COMMAND_Table_NSD);
	//
*/
	// ****************************************************
	// *** init LP-Filter *********************************
	// ****************************************************
	for (i=0;i<RX_LPF_SARRAY_LENGTH;i++)
  	{   
		// reset filter state values
		// Ch1
   		t_Ch1_Detector.LP_FilterState1[i]=0.0;
   		t_Ch1_Detector.LP_FilterState2[i]=0.0;
		// Ch2
		t_Ch2_Detector.LP_FilterState1[i]=0.0;
   		t_Ch2_Detector.LP_FilterState2[i]=0.0;
		//
   	}
	// ******************************************
	// *** set filter coefficients **************
	// ******************************************
	// Ch1
	t_Ch1_Detector.LP_Coef = Ch1_LP_Rx1700Hz;
	// Ch2
	t_Ch2_Detector.LP_Coef = Ch2_LP_Rx1700Hz;
	// ****************************************************
	// *** set transfer frequency Rx **********************
	// ****************************************************
	// set channel 1 centre frequency
	f_temp = ((TWO_PI*(NSD_CENTRE_FREQ))/(1.0*(AF_SAMPLE_FREQ)));
	//
	t_Ch1_Detector.sinDeltaRxIF = sinf(f_temp);
	t_Ch1_Detector.cosDeltaRxIF = cosf(f_temp);
	//
	// set channel 2 centre frequency 
	f_temp = ((TWO_PI*(NSD_CENTRE_FREQ))/(1.0*(AF_SAMPLE_FREQ)));
	//
	t_Ch2_Detector.sinDeltaRxIF = sinf(f_temp);
	t_Ch2_Detector.cosDeltaRxIF = cosf(f_temp);
	// ****************************************************
	// *** init values for quadratur-Oscillator Rx IF frequency
	// ****************************************************
	// Ch1
	t_Ch1_Detector.RxIF_Sin = 0.0;
	t_Ch1_Detector.RxIF_Cos = 1.0;
	// used for AmpCorr() in Main Ch1
	//ptr_RxIF_Sin_Ch1 = &(t_Ch1_Detector.RxIF_Sin);
	//ptr_RxIF_Cos_Ch1 = &(t_Ch1_Detector.RxIF_Cos);
	// Ch2
	t_Ch2_Detector.RxIF_Sin = 0.0;
	t_Ch2_Detector.RxIF_Cos = 1.0;
	// used for AmpCorr() in Main Ch1
	//ptr_RxIF_Sin_Ch2 = &(t_Ch2_Detector.RxIF_Sin);
	//ptr_RxIF_Cos_Ch2 = &(t_Ch2_Detector.RxIF_Cos);
	// ****************************************************
	// *** init wideband power data  **********************
	// ****************************************************
	// Ch1
	t_Ch1_Detector.OutputWideBand[0] = 0.0;			// Reset power tev1
    t_Ch1_Detector.OutputWideBand[1] = 0.0;			// Reset power tev2
    t_Ch1_Detector.OutputWideBand[2] = 0.0;			// Reset power tev3
	// Ch2
	t_Ch2_Detector.OutputWideBand[0] = 0.0;			// Reset power tev1
    t_Ch2_Detector.OutputWideBand[1] = 0.0;			// Reset power tev2
	t_Ch2_Detector.OutputWideBand[2] = 0.0;			// Reset power tev3
	// 
	// ******************************************
	// *** Reset PowerArrays for N1, N2, N3
	// ******************************************
	//
  	t_Ch1_Detector.ptr_sumPowerN1 = PowerArray1_N1;
	t_Ch2_Detector.ptr_sumPowerN1 = PowerArray2_N1;
  	for (i=0;i<MAX_INTEGR_TIME;i++)
  	{
  	 	PowerArray1_N1[i] = 0.0;        	// Reset PowerArray N1 Ch1
		PowerArray2_N1[i] = 0.0;        	// Reset PowerArray N1 Ch2
  	}
	//
  	t_Ch1_Detector.ptr_sumPowerN2 = PowerArray1_N2;
	t_Ch2_Detector.ptr_sumPowerN2 = PowerArray2_N2;
  	for (i=0;i<(MAX_INTEGR_TIME*2);i++)
  	{
  	 	PowerArray1_N2[i] = 0.0;        	// Reset PowerArray N2 Ch1
		PowerArray2_N2[i] = 0.0;        	// Reset PowerArray N2 Ch2
  	}
	//
	t_Ch1_Detector.ptr_sumPowerN3 = PowerArray1_N3;
	t_Ch2_Detector.ptr_sumPowerN3 = PowerArray2_N3;
  	for (i=0;i<(MAX_INTEGR_TIME*2*2);i++)
  	{
  	 	PowerArray1_N3[i] = 0.0;        	// Reset PowerArray N3 Ch1
		PowerArray2_N3[i] = 0.0;        	// Reset PowerArray N3 Ch2
  	}
	// ******************************************
	// *** set Start adress power Array N1, N2 and N3 for Ch1 and Ch2
	// ******************************************
	//
	t_Ch1_Detector.startAdrPowerN1 = PowerArray1_N1;
	t_Ch2_Detector.startAdrPowerN1 = PowerArray2_N1;
	//
	t_Ch1_Detector.startAdrPowerN2 = PowerArray1_N2;
	t_Ch2_Detector.startAdrPowerN2 = PowerArray2_N2;
	//
	t_Ch1_Detector.startAdrPowerN3 = PowerArray1_N3;
	t_Ch2_Detector.startAdrPowerN3 = PowerArray2_N3;
	// ******************************************
	// *** set power correction factor  *********
	// ******************************************
	// Ch1
	t_Ch1_Detector.NCorrePower[0] = 1.0 / INTEGR_TIME_DETECTOR;
    t_Ch1_Detector.NCorrePower[1] = 1.0 / (INTEGR_TIME_DETECTOR*2);
	t_Ch1_Detector.NCorrePower[2] = 1.0 / (INTEGR_TIME_DETECTOR*2*2);
	// Ch2
	t_Ch2_Detector.NCorrePower[0] = 1.0 / INTEGR_TIME_DETECTOR;
    t_Ch2_Detector.NCorrePower[1] = 1.0 / (INTEGR_TIME_DETECTOR*2);
	t_Ch2_Detector.NCorrePower[2] = 1.0 / (INTEGR_TIME_DETECTOR*2*2);
	//
	// ******************************************
	// *** set integration time *****************
	// ******************************************
	t_Ch1_Detector.Integration_time = INTEGR_TIME_DETECTOR;
	t_Ch2_Detector.Integration_time = INTEGR_TIME_DETECTOR;
	// ******************************************
	// ***Reset Verification sum of Power N1, N2, N3*
	// ******************************************
	// Ch1
	t_Ch1_Detector.Power_Verif[0] = 0.0;
  	t_Ch1_Detector.Power_Verif[1] = 0.0;
	t_Ch1_Detector.Power_Verif[2] = 0.0;
	// Ch2
	t_Ch2_Detector.Power_Verif[0] = 0.0;
  	t_Ch2_Detector.Power_Verif[1] = 0.0;
	t_Ch2_Detector.Power_Verif[2] = 0.0;
	// 
	// ****************************************************
	// *** init Detector frequencies **********************
	// ****************************************************
	// Ch1
		f_temp = 1.0*RX_DETECTOR_SAMPL_FREQ/INTEGR_TIME_DETECTOR;	// set hub frequency
		//
		t_Ch1_Detector.Detector_freq[0] = 0.0;								// set frequency 1:	 0 +/- Fhub/2
		t_Ch1_Detector.Detector_freq[1] = t_Ch1_Detector.Detector_freq[0] + f_temp;		// set frequency 2:  0 +/- 3/2 * Fhub
		t_Ch1_Detector.Detector_freq[2] = t_Ch1_Detector.Detector_freq[1] + f_temp;		// set frequency 3:  0 +/- 5/2 * Fhub
		t_Ch1_Detector.Detector_freq[3] = t_Ch1_Detector.Detector_freq[2] + f_temp;		// set frequency 4:  0 +/- 7/2 * Fhub
		//
		t_Ch1_Detector.Sin_DetArg[0] = sinf(TWO_PI*t_Ch1_Detector.Detector_freq[0]/RX_DETECTOR_SAMPL_FREQ);
		t_Ch1_Detector.Cos_DetArg[0] = cosf(TWO_PI*t_Ch1_Detector.Detector_freq[0]/RX_DETECTOR_SAMPL_FREQ);
		//
		t_Ch1_Detector.Sin_DetArg[1] = sinf(TWO_PI*t_Ch1_Detector.Detector_freq[1]/RX_DETECTOR_SAMPL_FREQ);
		t_Ch1_Detector.Cos_DetArg[1] = cosf(TWO_PI*t_Ch1_Detector.Detector_freq[1]/RX_DETECTOR_SAMPL_FREQ);
		//
		t_Ch1_Detector.Sin_DetArg[2] = sinf(TWO_PI*t_Ch1_Detector.Detector_freq[2]/RX_DETECTOR_SAMPL_FREQ);
		t_Ch1_Detector.Cos_DetArg[2] = cosf(TWO_PI*t_Ch1_Detector.Detector_freq[2]/RX_DETECTOR_SAMPL_FREQ);
		//
		t_Ch1_Detector.Sin_DetArg[3] = sinf(TWO_PI*t_Ch1_Detector.Detector_freq[3]/RX_DETECTOR_SAMPL_FREQ);
		t_Ch1_Detector.Cos_DetArg[3] = cosf(TWO_PI*t_Ch1_Detector.Detector_freq[3]/RX_DETECTOR_SAMPL_FREQ);
		//
	// Ch2 
		f_temp = 1.0*RX_DETECTOR_SAMPL_FREQ/INTEGR_TIME_DETECTOR;	// set hub frequency
		//
		t_Ch2_Detector.Detector_freq[0] = 0.0;								// set frequency 1:	 0 +/- Fhub/2
		t_Ch2_Detector.Detector_freq[1] = t_Ch2_Detector.Detector_freq[0] + f_temp;		// set frequency 2:  0 +/- 3/2 * Fhub
		t_Ch2_Detector.Detector_freq[2] = t_Ch2_Detector.Detector_freq[1] + f_temp;		// set frequency 3:  0 +/- 5/2 * Fhub
		t_Ch2_Detector.Detector_freq[3] = t_Ch2_Detector.Detector_freq[2] + f_temp;		// set frequency 4:  0 +/- 7/2 * Fhub
		//
		t_Ch2_Detector.Sin_DetArg[0] = sinf(TWO_PI*t_Ch2_Detector.Detector_freq[0]/RX_DETECTOR_SAMPL_FREQ);
		t_Ch2_Detector.Cos_DetArg[0] = cosf(TWO_PI*t_Ch2_Detector.Detector_freq[0]/RX_DETECTOR_SAMPL_FREQ);
		//
		t_Ch2_Detector.Sin_DetArg[1] = sinf(TWO_PI*t_Ch2_Detector.Detector_freq[1]/RX_DETECTOR_SAMPL_FREQ);
		t_Ch2_Detector.Cos_DetArg[1] = cosf(TWO_PI*t_Ch2_Detector.Detector_freq[1]/RX_DETECTOR_SAMPL_FREQ);
		//
		t_Ch2_Detector.Sin_DetArg[2] = sinf(TWO_PI*t_Ch2_Detector.Detector_freq[2]/RX_DETECTOR_SAMPL_FREQ);
		t_Ch2_Detector.Cos_DetArg[2] = cosf(TWO_PI*t_Ch2_Detector.Detector_freq[2]/RX_DETECTOR_SAMPL_FREQ);
		//
		t_Ch2_Detector.Sin_DetArg[3] = sinf(TWO_PI*t_Ch2_Detector.Detector_freq[3]/RX_DETECTOR_SAMPL_FREQ);
		t_Ch2_Detector.Cos_DetArg[3] = cosf(TWO_PI*t_Ch2_Detector.Detector_freq[3]/RX_DETECTOR_SAMPL_FREQ);
		//
		//
	// ******************************************
	// ***	Init TempArgument to zero *********** 
	// ******************************************
	for (i=0;i<NUMBER_OF_RX_DET_TONES;i++)
	{
		// init values for quadratur-Oscillator detector frequencies
		// Ch1
		t_Ch1_Detector.DetSin[i] = 0.0;
		t_Ch1_Detector.DetCos[i] = 1.0;
		// Ch2
		t_Ch2_Detector.DetSin[i] = 0.0;
		t_Ch2_Detector.DetCos[i] = 1.0;
	}
	// Used for AmpCorr() in main 
	// Ch1
	//ptr_DetSin_Ch1 = &t_Ch1_Detector.DetSin[0];	
	//ptr_DetCos_Ch1 = &t_Ch1_Detector.DetCos[0];
	// Ch2
	//ptr_DetSin_Ch2 = &t_Ch2_Detector.DetSin[0];	
	//ptr_DetCos_Ch2 = &t_Ch2_Detector.DetCos[0];
	//
	// ******************************************
	// *** set pointer integration sum correl ***   
	// ******************************************
	// Ch1
		t_Ch1_Detector.ptrSum_F[0] = Sum1F1AN1_N;
		t_Ch1_Detector.ptrSum_F[1] = Sum1F1AN2_N;
		t_Ch1_Detector.ptrSum_F[2] = Sum1F1AN3_N;
		t_Ch1_Detector.ptrSum_F[3] = Sum1F1BN1_N;
		t_Ch1_Detector.ptrSum_F[4] = Sum1F1BN2_N;
		t_Ch1_Detector.ptrSum_F[5] = Sum1F1BN3_N;
		t_Ch1_Detector.ptrSum_F[6] = Sum1F1CN1_P;
		t_Ch1_Detector.ptrSum_F[7] = Sum1F1CN2_P;
		t_Ch1_Detector.ptrSum_F[8] = Sum1F1CN3_P;
		t_Ch1_Detector.ptrSum_F[9] = Sum1F1DN1_P;
		t_Ch1_Detector.ptrSum_F[10] = Sum1F1DN2_P;
		t_Ch1_Detector.ptrSum_F[11] = Sum1F1DN3_P;
		//
		t_Ch1_Detector.ptrSum_F[12] = Sum1F2AN1_N;
		t_Ch1_Detector.ptrSum_F[13] = Sum1F2AN2_N;
		t_Ch1_Detector.ptrSum_F[14] = Sum1F2AN3_N;
		t_Ch1_Detector.ptrSum_F[15] = Sum1F2BN1_N;
		t_Ch1_Detector.ptrSum_F[16] = Sum1F2BN2_N;
		t_Ch1_Detector.ptrSum_F[17] = Sum1F2BN3_N;
		t_Ch1_Detector.ptrSum_F[18] = Sum1F2CN1_P;
		t_Ch1_Detector.ptrSum_F[19] = Sum1F2CN2_P;
		t_Ch1_Detector.ptrSum_F[20] = Sum1F2CN3_P;
		t_Ch1_Detector.ptrSum_F[21] = Sum1F2DN1_P;
		t_Ch1_Detector.ptrSum_F[22] = Sum1F2DN2_P;
		t_Ch1_Detector.ptrSum_F[23] = Sum1F2DN3_P;
		//
		t_Ch1_Detector.ptrSum_F[24] = Sum1F3AN1_N;
		t_Ch1_Detector.ptrSum_F[25] = Sum1F3AN2_N;
		t_Ch1_Detector.ptrSum_F[26] = Sum1F3AN3_N;
		t_Ch1_Detector.ptrSum_F[27] = Sum1F3BN1_N;
		t_Ch1_Detector.ptrSum_F[28] = Sum1F3BN2_N;
		t_Ch1_Detector.ptrSum_F[29] = Sum1F3BN3_N;
		t_Ch1_Detector.ptrSum_F[30] = Sum1F3CN1_P;
		t_Ch1_Detector.ptrSum_F[31] = Sum1F3CN2_P;
		t_Ch1_Detector.ptrSum_F[32] = Sum1F3CN3_P;
		t_Ch1_Detector.ptrSum_F[33] = Sum1F3DN1_P;
		t_Ch1_Detector.ptrSum_F[34] = Sum1F3DN2_P;
		t_Ch1_Detector.ptrSum_F[35] = Sum1F3DN3_P;
		//
		t_Ch1_Detector.ptrSum_F[36] = Sum1F4AN1_N;
		t_Ch1_Detector.ptrSum_F[37] = Sum1F4AN2_N;
		t_Ch1_Detector.ptrSum_F[38] = Sum1F4AN3_N;
		t_Ch1_Detector.ptrSum_F[39] = Sum1F4BN1_N;
		t_Ch1_Detector.ptrSum_F[40] = Sum1F4BN2_N;
		t_Ch1_Detector.ptrSum_F[41] = Sum1F4BN3_N;
		t_Ch1_Detector.ptrSum_F[42] = Sum1F4CN1_P;
		t_Ch1_Detector.ptrSum_F[43] = Sum1F4CN2_P;
		t_Ch1_Detector.ptrSum_F[44] = Sum1F4CN3_P;
		t_Ch1_Detector.ptrSum_F[45] = Sum1F4DN1_P;
		t_Ch1_Detector.ptrSum_F[46] = Sum1F4DN2_P;
		t_Ch1_Detector.ptrSum_F[47] = Sum1F4DN3_P;
	//
	// Ch2
		t_Ch2_Detector.ptrSum_F[0] = Sum2F1AN1_N;
		t_Ch2_Detector.ptrSum_F[1] = Sum2F1AN2_N;
		t_Ch2_Detector.ptrSum_F[2] = Sum2F1AN3_N;
		t_Ch2_Detector.ptrSum_F[3] = Sum2F1BN1_N;
		t_Ch2_Detector.ptrSum_F[4] = Sum2F1BN2_N;
		t_Ch2_Detector.ptrSum_F[5] = Sum2F1BN3_N;
		t_Ch2_Detector.ptrSum_F[6] = Sum2F1CN1_P;
		t_Ch2_Detector.ptrSum_F[7] = Sum2F1CN2_P;
		t_Ch2_Detector.ptrSum_F[8] = Sum2F1CN3_P;
		t_Ch2_Detector.ptrSum_F[9] = Sum2F1DN1_P;
		t_Ch2_Detector.ptrSum_F[10] = Sum2F1DN2_P;
		t_Ch2_Detector.ptrSum_F[11] = Sum2F1DN3_P;
		//
		t_Ch2_Detector.ptrSum_F[12] = Sum2F2AN1_N;
		t_Ch2_Detector.ptrSum_F[13] = Sum2F2AN2_N;
		t_Ch2_Detector.ptrSum_F[14] = Sum2F2AN3_N;
		t_Ch2_Detector.ptrSum_F[15] = Sum2F2BN1_N;
		t_Ch2_Detector.ptrSum_F[16] = Sum2F2BN2_N;
		t_Ch2_Detector.ptrSum_F[17] = Sum2F2BN3_N;
		t_Ch2_Detector.ptrSum_F[18] = Sum2F2CN1_P;
		t_Ch2_Detector.ptrSum_F[19] = Sum2F2CN2_P;
		t_Ch2_Detector.ptrSum_F[20] = Sum2F2CN3_P;
		t_Ch2_Detector.ptrSum_F[21] = Sum2F2DN1_P;
		t_Ch2_Detector.ptrSum_F[22] = Sum2F2DN2_P;
		t_Ch2_Detector.ptrSum_F[23] = Sum2F2DN3_P;
		//
		t_Ch2_Detector.ptrSum_F[24] = Sum2F3AN1_N;
		t_Ch2_Detector.ptrSum_F[25] = Sum2F3AN2_N;
		t_Ch2_Detector.ptrSum_F[26] = Sum2F3AN3_N;
		t_Ch2_Detector.ptrSum_F[27] = Sum2F3BN1_N;
		t_Ch2_Detector.ptrSum_F[28] = Sum2F3BN2_N;
		t_Ch2_Detector.ptrSum_F[29] = Sum2F3BN3_N;
		t_Ch2_Detector.ptrSum_F[30] = Sum2F3CN1_P;
		t_Ch2_Detector.ptrSum_F[31] = Sum2F3CN2_P;
		t_Ch2_Detector.ptrSum_F[32] = Sum2F3CN3_P;
		t_Ch2_Detector.ptrSum_F[33] = Sum2F3DN1_P;
		t_Ch2_Detector.ptrSum_F[34] = Sum2F3DN2_P;
		t_Ch2_Detector.ptrSum_F[35] = Sum2F3DN3_P;
		//
		t_Ch2_Detector.ptrSum_F[36] = Sum2F4AN1_N;
		t_Ch2_Detector.ptrSum_F[37] = Sum2F4AN2_N;
		t_Ch2_Detector.ptrSum_F[38] = Sum2F4AN3_N;
		t_Ch2_Detector.ptrSum_F[39] = Sum2F4BN1_N;
		t_Ch2_Detector.ptrSum_F[40] = Sum2F4BN2_N;
		t_Ch2_Detector.ptrSum_F[41] = Sum2F4BN3_N;
		t_Ch2_Detector.ptrSum_F[42] = Sum2F4CN1_P;
		t_Ch2_Detector.ptrSum_F[43] = Sum2F4CN2_P;
		t_Ch2_Detector.ptrSum_F[44] = Sum2F4CN3_P;
		t_Ch2_Detector.ptrSum_F[45] = Sum2F4DN1_P;
		t_Ch2_Detector.ptrSum_F[46] = Sum2F4DN2_P;
		t_Ch2_Detector.ptrSum_F[47] = Sum2F4DN3_P;
		//
	// ******************************************
	// *** set start- and end-pointer correl sum*
	// ******************************************
	// Ch1
		for (i=0;i<NUMBER_OF_RX_DET_TONES * NUMBER_OF_PATH_CORRELATOR;i++)
		{
			t_Ch1_Detector.startptrSum_F[ADAPT_NUMB*i] = t_Ch1_Detector.ptrSum_F[ADAPT_NUMB*i];
			t_Ch1_Detector.endptrSum_F[ADAPT_NUMB*i] = t_Ch1_Detector.ptrSum_F[ADAPT_NUMB*i] + INTEGR_TIME_DETECTOR -1;

			t_Ch1_Detector.startptrSum_F[ADAPT_NUMB*i +1] = t_Ch1_Detector.ptrSum_F[ADAPT_NUMB*i+1];
			t_Ch1_Detector.endptrSum_F[ADAPT_NUMB*i+1] = t_Ch1_Detector.ptrSum_F[ADAPT_NUMB*i+1] + INTEGR_TIME_DETECTOR*2 -1;

			t_Ch1_Detector.startptrSum_F[ADAPT_NUMB*i +2] = t_Ch1_Detector.ptrSum_F[ADAPT_NUMB*i+2];
			t_Ch1_Detector.endptrSum_F[ADAPT_NUMB*i+2] = t_Ch1_Detector.ptrSum_F[ADAPT_NUMB*i+2] + INTEGR_TIME_DETECTOR*2*2 -1;
		}
		//
		t_Ch1_Detector.ptr_Sum_Out = t_Ch1_Detector.Sum_Out;
	// Ch2
		for (i=0;i<NUMBER_OF_RX_DET_TONES * NUMBER_OF_PATH_CORRELATOR;i++)
		{
			t_Ch2_Detector.startptrSum_F[ADAPT_NUMB*i] = t_Ch2_Detector.ptrSum_F[ADAPT_NUMB*i];
			t_Ch2_Detector.endptrSum_F[ADAPT_NUMB*i] = t_Ch2_Detector.ptrSum_F[ADAPT_NUMB*i] + INTEGR_TIME_DETECTOR -1;

			t_Ch2_Detector.startptrSum_F[ADAPT_NUMB*i +1] = t_Ch2_Detector.ptrSum_F[ADAPT_NUMB*i+1];
			t_Ch2_Detector.endptrSum_F[ADAPT_NUMB*i+1] = t_Ch2_Detector.ptrSum_F[ADAPT_NUMB*i+1] + INTEGR_TIME_DETECTOR*2 -1;

			t_Ch2_Detector.startptrSum_F[ADAPT_NUMB*i +2] = t_Ch2_Detector.ptrSum_F[ADAPT_NUMB*i+2];
			t_Ch2_Detector.endptrSum_F[ADAPT_NUMB*i+2] = t_Ch2_Detector.ptrSum_F[ADAPT_NUMB*i+2] + INTEGR_TIME_DETECTOR*2*2 -1;
		}
		//
		t_Ch2_Detector.ptr_Sum_Out = t_Ch2_Detector.Sum_Out;
	//
	// ******************************************
	// *** reset all integeration summes to zero*
	// ******************************************
	// Ch1 
		for (i = 0; i<NUMBER_OF_RX_DET_TONES * NUMBER_OF_PATH_CORRELATOR; i++)
		{
			for (i_temp =0; i_temp< MAX_INTEGR_TIME; i_temp++)
			{
				*(t_Ch1_Detector.ptrSum_F[ADAPT_NUMB*i])++ = 0.0;
			}
			t_Ch1_Detector.ptrSum_F[ADAPT_NUMB*i] = t_Ch1_Detector.startptrSum_F[ADAPT_NUMB*i];
			t_Ch1_Detector.Sum_Out[ADAPT_NUMB*i] = 0.0;
			//
			for (i_temp =0; i_temp< MAX_INTEGR_TIME*2; i_temp++)
			{
				*(t_Ch1_Detector.ptrSum_F[ADAPT_NUMB*i+1])++ = 0.0;
			}
			t_Ch1_Detector.Sum_Out[ADAPT_NUMB*i+1] = 0.0;
			t_Ch1_Detector.ptrSum_F[ADAPT_NUMB*i+1] = t_Ch1_Detector.startptrSum_F[ADAPT_NUMB*i+1];
			//
			for (i_temp =0; i_temp< MAX_INTEGR_TIME*2*2; i_temp++)
			{
				*(t_Ch1_Detector.ptrSum_F[ADAPT_NUMB*i+2])++ = 0.0;
			}
			t_Ch1_Detector.Sum_Out[ADAPT_NUMB*i+2] = 0.0;
			t_Ch1_Detector.ptrSum_F[ADAPT_NUMB*i+2] = t_Ch1_Detector.startptrSum_F[ADAPT_NUMB*i+2];
		}
	// Ch2 Pl-Last
		for (i = 0; i<NUMBER_OF_RX_DET_TONES * NUMBER_OF_PATH_CORRELATOR; i++)
		{
			for (i_temp =0; i_temp< MAX_INTEGR_TIME; i_temp++)
			{
				*(t_Ch2_Detector.ptrSum_F[ADAPT_NUMB*i])++ = 0.0;
			}
			t_Ch2_Detector.ptrSum_F[ADAPT_NUMB*i] = t_Ch2_Detector.startptrSum_F[ADAPT_NUMB*i];
			t_Ch2_Detector.Sum_Out[ADAPT_NUMB*i] = 0.0;
			//
			for (i_temp =0; i_temp< MAX_INTEGR_TIME*2; i_temp++)
			{
				*(t_Ch2_Detector.ptrSum_F[ADAPT_NUMB*i+1])++ = 0.0;
			}
			t_Ch2_Detector.Sum_Out[ADAPT_NUMB*i+1] = 0.0;
			t_Ch2_Detector.ptrSum_F[ADAPT_NUMB*i+1] = t_Ch2_Detector.startptrSum_F[ADAPT_NUMB*i+1];
			//
			for (i_temp =0; i_temp< MAX_INTEGR_TIME*2*2; i_temp++)
			{
				*(t_Ch2_Detector.ptrSum_F[ADAPT_NUMB*i+2])++ = 0.0;
			}
			t_Ch2_Detector.Sum_Out[ADAPT_NUMB*i+2] = 0.0;
			t_Ch2_Detector.ptrSum_F[ADAPT_NUMB*i+2] = t_Ch2_Detector.startptrSum_F[ADAPT_NUMB*i+2];
		}
	//
	// ****************************************************
	// *** set Freq. to command mapping *******************
	// ****************************************************
	// Ch1
	t_Ch1_Detector.PtrCOMTAB = Ch1_COMMAND_Table_NSD;
	t_Ch1_Detector.ptr_Correl_to_freq_map = Ch1_Correl_to_Freq_Map;
	// Ch2
	t_Ch2_Detector.PtrCOMTAB = Ch2_COMMAND_Table_NSD;
	t_Ch2_Detector.ptr_Correl_to_freq_map = Ch2_Correl_to_Freq_Map;
	//
	// 
/*	// ****************************************************
	// *** Set command table for Ch1 **********************
	// ****************************************************
	// Ch1
	l_Array[0] = g_NSD_Config->NSD[0].CommandApplication[0];
	l_Array[1] = g_NSD_Config->NSD[0].CommandApplication[1];
	l_Array[2] = g_NSD_Config->NSD[0].CommandApplication[2];
	l_Array[3] = g_NSD_Config->NSD[0].CommandApplication[3];
	l_Array[4] = g_NSD_Config->NSD[0].CommandApplication[4];
	l_Array[5] = g_NSD_Config->NSD[0].CommandApplication[5];

	for (i=0; i<MAX_NUMBER_LP_COMMANDS;i++)
		{
			switch (l_Array[i])
			{
			case BLOCKING:
				// do nothing let all commands in commmand table
				break;
			case PERMISSIVE:
				// delete command for blocking application in command table
				//#pragma vector_for
				for (i_temp =0; i_temp< NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2;i_temp++)
				{
					Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(BIT_0_SET<<i));
					Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(COM_T));
				}
				break;
			case DIRECT:
				// delete command for blocking and permissive applications in command table
				//#pragma vector_for 
				for (i_temp =0; i_temp< NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2  ;i_temp++)
				{
					Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(BIT_0_SET<<i));
					Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]&(~(BIT_0_SET<<i));
					Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(COM_T));
					Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]&(~(COM_T));
				}
				break;
			case COMMAND_OFF:
				// delete command in command table
				for (i_temp =0; i_temp< NUMBER_OF_APPLICATION_MODES * NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2 ;i_temp++)
				{
					// delete all command combination of "switched off commands"
					//
					if((Ch1_COMMAND_Table_NSD[i_temp]&(BIT_0_SET<<i)) == 0x00000000)
					{
						// do nothing 		
					}
					else
					{
						// delete command combination for "switched off commands"
						Ch1_COMMAND_Table_NSD[i_temp] = 0;
					}

				}
				break;
			default:
				ASSERT(FALSE);
			}
		}
	// ************************************************************************
	// *** Switch off all "not used commands" and command combinations for Ch1*
	// ************************************************************************
	// 
	SwitchOffNotUsedRxCommands(&Ch1_COMMAND_Table_NSD[0], g_NSD_Config->NSD[0].UsedCommands );
	//
	// *****************************************************************************************
	// *** Do Ch2 init for detector ************************************************************
	// *****************************************************************************************
	// ****************************************************
	// *** Set command table for Ch2 **********************
	// ****************************************************
	// Ch2
	l_Array[0] = g_NSD_Config->NSD[1].CommandApplication[0];
	l_Array[1] = g_NSD_Config->NSD[1].CommandApplication[1];
	l_Array[2] = g_NSD_Config->NSD[1].CommandApplication[2];
	l_Array[3] = g_NSD_Config->NSD[1].CommandApplication[3];
	l_Array[4] = g_NSD_Config->NSD[1].CommandApplication[4];
	l_Array[5] = g_NSD_Config->NSD[1].CommandApplication[5];
	for (i=0; i<MAX_NUMBER_LP_COMMANDS;i++)
		{
			switch (l_Array[i])
			{
			case BLOCKING:
				// do nothing let all commands in commmand table
				break;
			case PERMISSIVE:
				// delete command for blocking application in command table
//				#pragma vector_for
				for (i_temp =0; i_temp< NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2;i_temp++)
				{
					Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(BIT_0_SET<<i));
					Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(COM_T));
				}
				break;
			case DIRECT:
				// delete command for blocking and permissive applications in command table
//				#pragma vector_for
				for (i_temp =0; i_temp< NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2  ;i_temp++)
				{
					Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(BIT_0_SET<<i));
					Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]=Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]&(~(BIT_0_SET<<i));
					Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(COM_T));
					Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]=Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]&(~(COM_T));
				}
				break;
			case COMMAND_OFF:
				// delete command in command table
				for (i_temp =0; i_temp< NUMBER_OF_APPLICATION_MODES * NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2 ;i_temp++)
				{
					//
					// delete all command combination of "switched off commands"
					//
					if((Ch2_COMMAND_Table_NSD[i_temp]&(BIT_0_SET<<i)) == 0x00000000)
					{
						// do nothing 		
					}
					else
					{
						// delete command combination for "switched off commands"
						Ch2_COMMAND_Table_NSD[i_temp] = 0;
					}
				}
				break;
			default:
				ASSERT(FALSE);
			}
		}
	// ************************************************************************
	// ***Switch off all "not used commands" and command combinations for Ch2 *
	// ************************************************************************
	//
	SwitchOffNotUsedRxCommands(&Ch2_COMMAND_Table_NSD[0], g_NSD_Config->NSD[1].UsedCommands);
	//
*/
	// ****************************************************
	// *** set Alpha values and Lamda values    ***********
	// ****************************************************
	// Ch1 and Ch2
	for(i=0;i<NUMBER_OF_APPLICATION_MODES*ADAPT_NUMB;i++)
	{
		t_Ch1_Detector.Alpha_1[i]  = Ch1_Alpha_Setting[i];
		t_Ch1_Detector.Lamda[i] = (1.0/(1.0 - Ch1_Alpha_Setting[i]));
		t_Ch2_Detector.Alpha_1[i]  = Ch2_Alpha_Setting[i];
		t_Ch2_Detector.Lamda[i] = (1.0/(1.0 - Ch2_Alpha_Setting[i]));
	}
	// ********************************************************************
	// *** NSD off settings for Ch1 and Ch2 Code Table reset  *************
	// ********************************************************************
	//
	switch(TPE_Basic_Mode)
	{
		case INT_TELPROT_MODE_NONE:
			ResetCommandTable(&Ch1_COMMAND_Table_NSD[0]);	// reset all commands in command table for ch1
			ResetCommandTable(&Ch2_COMMAND_Table_NSD[0]);	// reset all commands in command table for ch2
			t_Ch1_Detector.Channel_active = OFF;
			t_Ch2_Detector.Channel_active = OFF;
			break;
		case INT_TELPROT_MODE_AES:
			// AES is not supported in this version
			ResetCommandTable(&Ch1_COMMAND_Table_NSD[0]);	// reset all commands in command table for ch1
			ResetCommandTable(&Ch2_COMMAND_Table_NSD[0]);	// reset all commands in command table for ch2
			t_Ch1_Detector.Channel_active = OFF;
			t_Ch2_Detector.Channel_active = OFF;
			break;
		case INT_TELPROT_MODE_NSD1_APLC1:
		case INT_TELPROT_MODE_NSD1_DPLC:
		case INT_TELPROT_MODE_HSD_25:
			if(TPE1_Custom_Mode == 0)
			{
				// no custom mode activ 
				//
				// ****************************************************
				// *** init Command Code Table according custom table *
				// ****************************************************
				//
				nsdCustomTable_pack (FALSE, UnpCustomTable, g_NSD_Config->NSD[0].customTable);
				ConvertCustom2CommandTable(UnpCustomTable, C2C_Table);
				// 
				GenerateDetectorCodeTable(C2C_Table, Ch1_COMMAND_Table_NSD);
				//
				t_Ch1_Detector.Channel_active = ON;
			}
			else 
			{
				// Custom mode activ do init with extended custom table!
				//
				// -> to do @Pl_ext
				//@Pl_ext
				// -> unpack custom table
				success = nsdCustomTable_packEx(FALSE,  &localCustomTable, g_NSD_Config->NSD[0].customTable);
				//
				if(success)
				{
					// create Detector Command table
					CreateRxCmdTable_ext(Ch1_COMMAND_Table_NSD, &localCustomTable);
					t_Ch1_Detector.Channel_active = ON;
				}
				else
				{
					// unpack custom table not successfull!!!
					ResetCommandTable(&Ch1_COMMAND_Table_NSD[0]);
					t_Ch1_Detector.Channel_active = OFF;
				}
			}
			//
			// ****************************************************
			// *** Set command table for Ch1 **********************
			// ****************************************************
			// Ch1
			l_Array[0] = g_NSD_Config->NSD[0].CommandApplication[0];
			l_Array[1] = g_NSD_Config->NSD[0].CommandApplication[1];
			l_Array[2] = g_NSD_Config->NSD[0].CommandApplication[2];
			l_Array[3] = g_NSD_Config->NSD[0].CommandApplication[3];
			l_Array[4] = g_NSD_Config->NSD[0].CommandApplication[4];
			l_Array[5] = g_NSD_Config->NSD[0].CommandApplication[5];

			for (i=0; i<MAX_NUMBER_LP_COMMANDS;i++)
			{
				switch (l_Array[i])
				{
					case BLOCKING:
						// do nothing let all commands in commmand table
						break;
					case PERMISSIVE:
						// delete command for blocking application in command table
						//#pragma vector_for
						for (i_temp =0; i_temp< NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2;i_temp++)
						{
							Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(BIT_0_SET<<i));
							Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(COM_T));
						}
						break;
					case DIRECT:
						// delete command for blocking and permissive applications in command table
						//#pragma vector_for 
						for (i_temp =0; i_temp< NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2  ;i_temp++)
						{
							Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(BIT_0_SET<<i));
							Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]&(~(BIT_0_SET<<i));
							Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(COM_T));
							Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]&(~(COM_T));
						}
						break;
					case COMMAND_OFF:
						// delete command in command table
						for (i_temp =0; i_temp< NUMBER_OF_APPLICATION_MODES * NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2 ;i_temp++)
						{
							// delete all command combination of "switched off commands"
							//
							if((Ch1_COMMAND_Table_NSD[i_temp]&(BIT_0_SET<<i)) == 0x00000000)
							{
								// do nothing 		
							}
							else
							{
								// delete command combination for "switched off commands"
								Ch1_COMMAND_Table_NSD[i_temp] = 0;
							}

						}
						break;
					default:
						ASSERT(FALSE);
				}
			}
			// ************************************************************************
			// *** Switch off all "not used commands" and command combinations for Ch1*
			// ************************************************************************
			// 
			SwitchOffNotUsedRxCommands(&Ch1_COMMAND_Table_NSD[0], g_NSD_Config->NSD[0].UsedCommands );
			//
			ResetCommandTable(&Ch2_COMMAND_Table_NSD[0]);	// reset all commands in command table for ch2
			t_Ch2_Detector.Channel_active = OFF;
			t_Ch1_Detector.Start_Select_Second = START_SELECT_SECOND_NSD;
			t_Ch1_Detector.Offset_1 = ADAPT_NUMB;
			break;
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC:
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2:
			if(TPE1_Custom_Mode == 0)
			{
				// no custom mode activ 
				//
				// ****************************************************
				// *** init Command Code Table according custom table *
				// ****************************************************
				//
				nsdCustomTable_pack (FALSE, UnpCustomTable, g_NSD_Config->NSD[0].customTable);
				ConvertCustom2CommandTable(UnpCustomTable, C2C_Table);
				// 
				GenerateDetectorCodeTable(C2C_Table, Ch1_COMMAND_Table_NSD);
				//
				t_Ch1_Detector.Channel_active = ON;
			}
			else 
			{
				// Custom mode activ for Ch1 do init with extended custom table!
				//
				// -> to do @Pl_ext
				//
				// !!!!! This mode is not yet supported by HMI600
				//
				//@Pl_ext
				// -> unpack custom table
				success = nsdCustomTable_packEx(FALSE,  &localCustomTable, g_NSD_Config->NSD[0].customTable);
				if(success)
				{
					// create Detector Command table
					CreateRxCmdTable_ext(Ch1_COMMAND_Table_NSD, &localCustomTable);
					t_Ch1_Detector.Channel_active = ON;
					//
				}
				else
				{
					// unpack custom table not successfull!!!
					ResetCommandTable(&Ch1_COMMAND_Table_NSD[0]);
					t_Ch1_Detector.Channel_active = OFF;
				}
			}
			//
			if(TPE2_Custom_Mode == 0)
			{
				// ****************************************************
				// *** init Command Code Table according custom table *
				// ****************************************************
				//
				nsdCustomTable_pack (FALSE, UnpCustomTable, g_NSD_Config->NSD[1].customTable);
				ConvertCustom2CommandTable(UnpCustomTable, C2C_Table);
				// 
				GenerateDetectorCodeTable(C2C_Table, Ch2_COMMAND_Table_NSD);
				//
				t_Ch2_Detector.Channel_active = ON;

			}
			else
			{
				// Custom mode activ for Ch2 do init with extended custom table!
				//
				// -> to do @Pl_ext
				//
				// !!!!! This mode is not yet supported by HMI600
				//
				//@Pl_ext
				// -> unpack custom table
				success = nsdCustomTable_packEx(FALSE,  &localCustomTable, g_NSD_Config->NSD[1].customTable);
				//
				if(success)
				{
					// create Detector Command table
					CreateRxCmdTable_ext(Ch2_COMMAND_Table_NSD, &localCustomTable);
					t_Ch2_Detector.Channel_active = ON;
					//
				}
				else
				{
					// unpack custom table not successfull!!!
					ResetCommandTable(&Ch2_COMMAND_Table_NSD[0]);
					t_Ch2_Detector.Channel_active = OFF;
				}
			}
			//
			// ****************************************************
			// *** Set command table for Ch1 **********************
			// ****************************************************
			// Ch1
			l_Array[0] = g_NSD_Config->NSD[0].CommandApplication[0];
			l_Array[1] = g_NSD_Config->NSD[0].CommandApplication[1];
			l_Array[2] = g_NSD_Config->NSD[0].CommandApplication[2];
			l_Array[3] = g_NSD_Config->NSD[0].CommandApplication[3];
			l_Array[4] = g_NSD_Config->NSD[0].CommandApplication[4];
			l_Array[5] = g_NSD_Config->NSD[0].CommandApplication[5];

			for (i=0; i<MAX_NUMBER_LP_COMMANDS;i++)
				{
					switch (l_Array[i])
					{
					case BLOCKING:
						// do nothing let all commands in commmand table
						break;
					case PERMISSIVE:
						// delete command for blocking application in command table
						//#pragma vector_for
						for (i_temp =0; i_temp< NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2;i_temp++)
						{
							Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(BIT_0_SET<<i));
							Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(COM_T));
						}
						break;
					case DIRECT:
						// delete command for blocking and permissive applications in command table
						//#pragma vector_for 
						for (i_temp =0; i_temp< NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2  ;i_temp++)
						{
							Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(BIT_0_SET<<i));
							Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]&(~(BIT_0_SET<<i));
							Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(COM_T));
							Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]&(~(COM_T));
						}
						break;
					case COMMAND_OFF:
						// delete command in command table
						for (i_temp =0; i_temp< NUMBER_OF_APPLICATION_MODES * NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2 ;i_temp++)
						{
							// delete all command combination of "switched off commands"
							//
							if((Ch1_COMMAND_Table_NSD[i_temp]&(BIT_0_SET<<i)) == 0x00000000)
							{
								// do nothing 		
							}
							else
							{
								// delete command combination for "switched off commands"
								Ch1_COMMAND_Table_NSD[i_temp] = 0;
							}

						}
						break;
					default:
						ASSERT(FALSE);
					}
				}
			// ************************************************************************
			// *** Switch off all "not used commands" and command combinations for Ch1*
			// ************************************************************************
			// 
			SwitchOffNotUsedRxCommands(&Ch1_COMMAND_Table_NSD[0], g_NSD_Config->NSD[0].UsedCommands );
			//
			// *****************************************************************************************
			// *** Do Ch2 init for detector ************************************************************
			// *****************************************************************************************
			// ****************************************************
			// *** Set command table for Ch2 **********************
			// ****************************************************
			// Ch2
			l_Array[0] = g_NSD_Config->NSD[1].CommandApplication[0];
			l_Array[1] = g_NSD_Config->NSD[1].CommandApplication[1];
			l_Array[2] = g_NSD_Config->NSD[1].CommandApplication[2];
			l_Array[3] = g_NSD_Config->NSD[1].CommandApplication[3];
			l_Array[4] = g_NSD_Config->NSD[1].CommandApplication[4];
			l_Array[5] = g_NSD_Config->NSD[1].CommandApplication[5];
			for (i=0; i<MAX_NUMBER_LP_COMMANDS;i++)
				{
					switch (l_Array[i])
					{
					case BLOCKING:
						// do nothing let all commands in commmand table
						break;
					case PERMISSIVE:
						// delete command for blocking application in command table
			//				#pragma vector_for
						for (i_temp =0; i_temp< NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2;i_temp++)
						{
							Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(BIT_0_SET<<i));
							Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(COM_T));
						}
						break;
					case DIRECT:
						// delete command for blocking and permissive applications in command table
			//				#pragma vector_for
						for (i_temp =0; i_temp< NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2  ;i_temp++)
						{
							Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(BIT_0_SET<<i));
							Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]=Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]&(~(BIT_0_SET<<i));
							Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(COM_T));
							Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]=Ch2_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]&(~(COM_T));
						}
						break;
					case COMMAND_OFF:
						// delete command in command table
						for (i_temp =0; i_temp< NUMBER_OF_APPLICATION_MODES * NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2 ;i_temp++)
						{
							//
							// delete all command combination of "switched off commands"
							//
							if((Ch2_COMMAND_Table_NSD[i_temp]&(BIT_0_SET<<i)) == 0x00000000)
							{
								// do nothing 		
							}
							else
							{
								// delete command combination for "switched off commands"
								Ch2_COMMAND_Table_NSD[i_temp] = 0;
							}
						}
						break;
					default:
						ASSERT(FALSE);
					}
				}
			// ************************************************************************
			// ***Switch off all "not used commands" and command combinations for Ch2 *
			// ************************************************************************
			//
			SwitchOffNotUsedRxCommands(&Ch2_COMMAND_Table_NSD[0], g_NSD_Config->NSD[1].UsedCommands);
			//
			// all command tables are used do not reset the table
			t_Ch1_Detector.Start_Select_Second = START_SELECT_SECOND_NSD;
			t_Ch2_Detector.Start_Select_Second = START_SELECT_SECOND_NSD;
			t_Ch1_Detector.Offset_1 = ADAPT_NUMB;
			t_Ch2_Detector.Offset_1 = ADAPT_NUMB;
			break;
		case INT_TELPROT_MODE_HSD:
			if(TPE1_Custom_Mode == 0)
			{
				// no custom mode activ 
				//
				// ****************************************************
				// *** init Command Code Table according custom table *
				// ****************************************************
				//
				nsdCustomTable_pack (FALSE, UnpCustomTable, g_NSD_Config->NSD[0].customTable);
				ConvertCustom2CommandTable(UnpCustomTable, C2C_Table);
				// 
				GenerateDetectorCodeTable(C2C_Table, Ch1_COMMAND_Table_NSD);
				//
				t_Ch1_Detector.Channel_active = ON;
				t_Ch2_Detector.Channel_active = OFF;
				//
				t_Ch1_Detector.HSD_On = TRUE;
				//
			}
			else 
			{
				// Custom mode activ for HSD Ch1 not supported mode!!!
				//
				// !!!!! This mode is not yet supported!!!!!!!!!!!
				//
				t_Ch1_Detector.Channel_active = OFF;
				t_Ch2_Detector.Channel_active = OFF;
				//
			}
			ResetCommandTable(&Ch2_COMMAND_Table_NSD[0]);	// reset all commands in command table for ch2
			// Ch1 filter coeff for HSD
			t_Ch1_Detector.LP_Coef = Ch1_HSD_LP_Rx1300Hz;
			//
			//
			l_N_HSD = INTEGR_TIME_DETECTOR_HSD;
			//
			// new version for selectable N
			//
						// set Integration time 
			t_Ch1_Detector.Integration_time = l_N_HSD;
			// ******************************************
			// *** set power correction factor  *********
			// ******************************************
			// Ch1
			t_Ch1_Detector.NCorrePower[0] = 1.0 / l_N_HSD;
			t_Ch1_Detector.NCorrePower[1] = 1.0 / (l_N_HSD*2);
			t_Ch1_Detector.NCorrePower[2] = 1.0 / (l_N_HSD*2*2);
			//
			// ****************************************************
			// *** init Detector frequencies **********************
			// ****************************************************
			// Ch1
				f_temp = 1.0*RX_DETECTOR_SAMPL_FREQ/l_N_HSD;	// set hub frequency
				//
				//
				t_Ch1_Detector.Detector_freq[0] = 0.0;											// set frequency 1:	 0 +/-  0*Fhub
				t_Ch1_Detector.Detector_freq[1] = t_Ch1_Detector.Detector_freq[0] + f_temp;		// set frequency 2:  0 +/-  1*Fhub
				t_Ch1_Detector.Detector_freq[2] = t_Ch1_Detector.Detector_freq[1] + f_temp;		// set frequency 3:  0 +/-  2*Fhub
				t_Ch1_Detector.Detector_freq[3] = t_Ch1_Detector.Detector_freq[2] + f_temp;		// set frequency 4:  0 +/-  3*Fhub
				//
				t_Ch1_Detector.Sin_DetArg[0] = sinf(TWO_PI*t_Ch1_Detector.Detector_freq[0]/RX_DETECTOR_SAMPL_FREQ);
				t_Ch1_Detector.Cos_DetArg[0] = cosf(TWO_PI*t_Ch1_Detector.Detector_freq[0]/RX_DETECTOR_SAMPL_FREQ);
				//
				t_Ch1_Detector.Sin_DetArg[1] = sinf(TWO_PI*t_Ch1_Detector.Detector_freq[1]/RX_DETECTOR_SAMPL_FREQ);
				t_Ch1_Detector.Cos_DetArg[1] = cosf(TWO_PI*t_Ch1_Detector.Detector_freq[1]/RX_DETECTOR_SAMPL_FREQ);
				//
				t_Ch1_Detector.Sin_DetArg[2] = sinf(TWO_PI*t_Ch1_Detector.Detector_freq[2]/RX_DETECTOR_SAMPL_FREQ);
				t_Ch1_Detector.Cos_DetArg[2] = cosf(TWO_PI*t_Ch1_Detector.Detector_freq[2]/RX_DETECTOR_SAMPL_FREQ);
				//
				t_Ch1_Detector.Sin_DetArg[3] = sinf(TWO_PI*t_Ch1_Detector.Detector_freq[3]/RX_DETECTOR_SAMPL_FREQ);
				t_Ch1_Detector.Cos_DetArg[3] = cosf(TWO_PI*t_Ch1_Detector.Detector_freq[3]/RX_DETECTOR_SAMPL_FREQ);
				//
			// ******************************************
			// *** set start- and end-pointer correl sum*
			// ******************************************
			// Ch1
				for (i=0;i<NUMBER_OF_RX_DET_TONES * NUMBER_OF_PATH_CORRELATOR;i++)
				{
					t_Ch1_Detector.startptrSum_F[ADAPT_NUMB*i] = t_Ch1_Detector.ptrSum_F[ADAPT_NUMB*i];
					t_Ch1_Detector.endptrSum_F[ADAPT_NUMB*i] = t_Ch1_Detector.ptrSum_F[ADAPT_NUMB*i] + l_N_HSD -1;

					t_Ch1_Detector.startptrSum_F[ADAPT_NUMB*i +1] = t_Ch1_Detector.ptrSum_F[ADAPT_NUMB*i+1];
					t_Ch1_Detector.endptrSum_F[ADAPT_NUMB*i+1] = t_Ch1_Detector.ptrSum_F[ADAPT_NUMB*i+1] + l_N_HSD*2 -1;

					t_Ch1_Detector.startptrSum_F[ADAPT_NUMB*i +2] = t_Ch1_Detector.ptrSum_F[ADAPT_NUMB*i+2];
					t_Ch1_Detector.endptrSum_F[ADAPT_NUMB*i+2] = t_Ch1_Detector.ptrSum_F[ADAPT_NUMB*i+2] + l_N_HSD*2*2 -1;
				}
				//
				t_Ch1_Detector.ptr_Sum_Out = t_Ch1_Detector.Sum_Out;
			// set channel 1 centre frequency for HSD
			f_temp = ((TWO_PI*(HSD_CENTRE_FREQ))/(1.0*(AF_SAMPLE_FREQ)));
			//
			t_Ch1_Detector.sinDeltaRxIF = sinf(f_temp);
			t_Ch1_Detector.cosDeltaRxIF = cosf(f_temp);
			//
			// HSD600 mode 3+1
			// 
			t_Ch1_Detector.ptr_Correl_to_freq_map = Correl_to_Freq_Map_HSD;
			//
			t_Ch1_Detector.Start_Select_Second = START_SELECT_SECOND_NSD;
			t_Ch1_Detector.Offset_1 = ADAPT_NUMB;
			//
			// ****************************************************
			// *** set Alpha values and Lamda values    ***********
			// ****************************************************
			// Ch1 HSD
			for(i=0;i<NUMBER_OF_APPLICATION_MODES*ADAPT_NUMB;i++)
			{
				t_Ch1_Detector.Alpha_1[i]  = Ch1_Alpha_Setting_HSD[i];
				t_Ch1_Detector.Lamda[i] = (1.0/(1.0 - Ch1_Alpha_Setting_HSD[i]));
			}
			//

			//
			// ****************************************************
			// *** Set command table for Ch1 **********************
			// ****************************************************
			// Ch1
			l_Array[0] = g_NSD_Config->NSD[0].CommandApplication[0];
			l_Array[1] = g_NSD_Config->NSD[0].CommandApplication[1];
			l_Array[2] = g_NSD_Config->NSD[0].CommandApplication[2];
			l_Array[3] = g_NSD_Config->NSD[0].CommandApplication[3];
			l_Array[4] = g_NSD_Config->NSD[0].CommandApplication[4];
			l_Array[5] = g_NSD_Config->NSD[0].CommandApplication[5];

			for (i=0; i<MAX_NUMBER_LP_COMMANDS;i++)
			{
				switch (l_Array[i])
				{
					case BLOCKING:
						// do nothing let all commands in commmand table
						break;
					case PERMISSIVE:
						// delete command for blocking application in command table
						//#pragma vector_for
						for (i_temp =0; i_temp< NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2;i_temp++)
						{
							Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(BIT_0_SET<<i));
							Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(COM_T));
						}
						break;
					case DIRECT:
						// delete command for blocking and permissive applications in command table
						//#pragma vector_for 
						for (i_temp =0; i_temp< NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2  ;i_temp++)
						{
							Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(BIT_0_SET<<i));
							Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]&(~(BIT_0_SET<<i));
							Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES]&(~(COM_T));
							Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]=Ch1_COMMAND_Table_NSD[i_temp*NUMBER_OF_APPLICATION_MODES+1]&(~(COM_T));
						}
						break;
					case COMMAND_OFF:
						// delete command in command table
						for (i_temp =0; i_temp< NUMBER_OF_APPLICATION_MODES * NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2 ;i_temp++)
						{
							// delete all command combination of "switched off commands"
							//
							if((Ch1_COMMAND_Table_NSD[i_temp]&(BIT_0_SET<<i)) == 0x00000000)
							{
								// do nothing 		
							}
							else
							{
								// delete command combination for "switched off commands"
								Ch1_COMMAND_Table_NSD[i_temp] = 0;
							}

						}
						break;
					default:
						ASSERT(FALSE);
				}
			}

			// disable rx commands based on G4AI output configuration (UsedCommands)
			SwitchOffNotUsedRxCommands(&Ch1_COMMAND_Table_NSD[0], g_NSD_Config->NSD[0].UsedCommands);

			break;
		default:
			ASSERT(FALSE);
	}
	ptr_NSD_Power_Speech_Guard = getPtr_NSD_Power_Speech_Guard();
}

/*****************************************************************************
  Function:		Get_Ptr_Det_Amp_Correction()
  Description:	get the pointer to the actual sine and cos value for Amplitude correction 
  Inputs:		
  Outputs:		None
  Return:		
  Globals:		
  Important:	.
*******************************************************************************/
void SEGMENT_EXT_CODE Get_Ptr_Det_Amp_Correction(float **ptr_Sin_Ch1, float **ptr_Cos_Ch1, float **ptr_Sin_Ch2, float **ptr_Cos_Ch2)
{
	*ptr_Sin_Ch1 = &t_Ch1_Detector.DetSin[0];
	*ptr_Cos_Ch1 = &t_Ch1_Detector.DetCos[0];

	*ptr_Sin_Ch2 = &t_Ch2_Detector.DetSin[0];
	*ptr_Cos_Ch2 = &t_Ch2_Detector.DetCos[0];
}

/*****************************************************************************
  Function:		Get_Ptr_RXIF_Amp_Correction()
  Description:	get the pointer to the actual sine and cos value for Amplitude correction 
  Inputs:		
  Outputs:		None
  Return:		
  Globals:		
  Important:	.
*******************************************************************************/
void SEGMENT_EXT_CODE Get_Ptr_RXIF_Amp_Correction(float **ptr_Rx_Sin_Ch1, float **ptr_Rx_Cos_Ch1, float **ptr_Rx_Sin_Ch2, float **ptr_Rx_Cos_Ch2)
{

	*ptr_Rx_Sin_Ch1 = &(t_Ch1_Detector.RxIF_Sin);
	*ptr_Rx_Cos_Ch1 = &(t_Ch1_Detector.RxIF_Cos);

	*ptr_Rx_Sin_Ch2 = &(t_Ch2_Detector.RxIF_Sin);
	*ptr_Rx_Cos_Ch2 = &(t_Ch2_Detector.RxIF_Cos);

}

#pragma optimize_as_cmd_line

/*****************************************************************************
  Function:		Detector_NSD()
  Description:	
  Inputs:		start pointer of 2 AF-Samples (10.66kHz), Channel of NSD ( 1 or 2)
  Outputs:		None
  Return:		Active received Command
  Globals:		
  Important:	.

*******************************************************************************/

long Detector_NSD(float *Rx_LP_Input, long Channel, BOOL *detectorOutputActive)
{
	t_NSD_Detector *ptr_t_NSD_Det;
	float 	PowerN; 
	float	Input_sin[NUMBER_OF_RX_SAMPLE], Input_cos[NUMBER_OF_RX_SAMPLE];
	float  Sin_Filt_Out[NUMBER_OF_RX_SAMPLE], Cos_Filt_Out[NUMBER_OF_RX_SAMPLE];
	//
	register float *PtrCos_Value, *PtrSin_Value, *tempptr, Oldvalue, Oldvalue_1, Oldvalue_2, New_value, New_value_1, New_value_2;
	register float *ptrSin_inp, *ptrCos_inp, *PtrDetSin_0, *PtrDetCos_0;
	float *PtrDeltaArg;
	register int temp = 0, i, i_temp;
	float CorrelInp[2], Correl_x[2], Correl_y[2], RxIF_Sin_new, RxIF_Cos_new;
	volatile long  Rx_Command = 0;
	//
	//
	//
	ptr_t_NSD_Det = (Channel == TPE1) ? &t_Ch1_Detector:&t_Ch2_Detector;

 if(ptr_t_NSD_Det->Channel_active == ON)
 {	
	//
	// NSD channel is on 
	//
	ptr_Correl_to_freq_map = ptr_t_NSD_Det->ptr_Correl_to_freq_map;
	PtrCOMTAB = ptr_t_NSD_Det->PtrCOMTAB;

	tempptr = Rx_LP_Input;
	ptrSin_inp = Input_sin;
	ptrCos_inp = Input_cos;
	//
	RxIF_Sin_new = ptr_t_NSD_Det->RxIF_Sin * ptr_t_NSD_Det->cosDeltaRxIF + ptr_t_NSD_Det->RxIF_Cos * ptr_t_NSD_Det->sinDeltaRxIF;
	RxIF_Cos_new = ptr_t_NSD_Det->RxIF_Cos * ptr_t_NSD_Det->cosDeltaRxIF - ptr_t_NSD_Det->RxIF_Sin * ptr_t_NSD_Det->sinDeltaRxIF;

	Input_sin[0] = *tempptr * RxIF_Sin_new;		// sample 1
	Input_cos[0] = *tempptr++ * RxIF_Cos_new;

	ptr_t_NSD_Det->RxIF_Sin = RxIF_Sin_new;
	ptr_t_NSD_Det->RxIF_Cos = RxIF_Cos_new;

	RxIF_Sin_new = ptr_t_NSD_Det->RxIF_Sin * ptr_t_NSD_Det->cosDeltaRxIF + ptr_t_NSD_Det->RxIF_Cos * ptr_t_NSD_Det->sinDeltaRxIF;
	RxIF_Cos_new = ptr_t_NSD_Det->RxIF_Cos * ptr_t_NSD_Det->cosDeltaRxIF - ptr_t_NSD_Det->RxIF_Sin * ptr_t_NSD_Det->sinDeltaRxIF;

	Input_sin[1] = *tempptr * RxIF_Sin_new;		// sample 2
	Input_cos[1] = *tempptr++ * RxIF_Cos_new;

	ptr_t_NSD_Det->RxIF_Sin = RxIF_Sin_new; // save also in RxIF_Sin for AmpCorr() functin in DSP_Main
	ptr_t_NSD_Det->RxIF_Cos = RxIF_Cos_new; // save also in RxIF_Cos for AmpCorr() functin in DSP_Main
 
	// ******************************************************************
	// ***  do the LP-filtering      ************************************
	// ******************************************************************
	//
	BiIIR(Input_sin, Sin_Filt_Out, ptr_t_NSD_Det->LP_Coef, ptr_t_NSD_Det->LP_FilterState1, RX_NUM_OF_SEC_LP, NUMBER_OF_RX_SAMPLE);
	BiIIR(Input_cos, Cos_Filt_Out, ptr_t_NSD_Det->LP_Coef, ptr_t_NSD_Det->LP_FilterState2, RX_NUM_OF_SEC_LP, NUMBER_OF_RX_SAMPLE);
	//
// *********************************************************************************
// -> after this point fs=5.333kHz *************************************************
// *********************************************************************************
	//
	// ******************************************************************
	// *** do power calculation N1, N2,    ******************************
	// ******************************************************************
	// do scaling filter output
	CorrelInp[1] = Sin_Filt_Out[1]*2.0;	// do scaling sin*sin = 1/2 ()
	CorrelInp[0] = Cos_Filt_Out[1]*2.0;	// do scaling sin*cos = 1/2 ()
	//
	PowerN = CorrelInp[1] * CorrelInp[1] + CorrelInp[0] * CorrelInp[0];
	//
	New_value   = PowerN * ptr_t_NSD_Det->NCorrePower[0];  // evaluation time 1
	New_value_1 = PowerN * ptr_t_NSD_Det->NCorrePower[1];  // evaluation time 2
	New_value_2 = PowerN * ptr_t_NSD_Det->NCorrePower[2];  // evaluation time 3
	// do read and write from or to external memory without interruption of other code
	Oldvalue = *(ptr_t_NSD_Det->ptr_sumPowerN1);
	*(ptr_t_NSD_Det->ptr_sumPowerN1)++ = New_value;
	//
	Oldvalue_1 = *(ptr_t_NSD_Det->ptr_sumPowerN2);
	*(ptr_t_NSD_Det->ptr_sumPowerN2)++ = New_value_1;
	//
	Oldvalue_2 = *(ptr_t_NSD_Det->ptr_sumPowerN3);
	*(ptr_t_NSD_Det->ptr_sumPowerN3)++ = New_value_2;
	//Calculate PowerN1
	ptr_t_NSD_Det->OutputWideBand[0] = ptr_t_NSD_Det->OutputWideBand[0] - Oldvalue + New_value;
	ptr_t_NSD_Det->Power_Verif[0] += New_value;
	if (ptr_t_NSD_Det->ptr_sumPowerN1 > (ptr_t_NSD_Det->startAdrPowerN1 + ptr_t_NSD_Det->Integration_time -1)) 
		{
   			ptr_t_NSD_Det->ptr_sumPowerN1 = ptr_t_NSD_Det->startAdrPowerN1;  
   			ptr_t_NSD_Det->OutputWideBand[0] = ptr_t_NSD_Det->Power_Verif[0];
   			ptr_t_NSD_Det->Power_Verif[0] = 0.0;
		}
	// 
	// Calculate PowerN2 ----------------------------------------------
	ptr_t_NSD_Det->OutputWideBand[1] = ptr_t_NSD_Det->OutputWideBand[1] - Oldvalue_1 + New_value_1 ;  
	ptr_t_NSD_Det->Power_Verif[1] += New_value_1;
	if (ptr_t_NSD_Det->ptr_sumPowerN2 > (ptr_t_NSD_Det->startAdrPowerN2 + ptr_t_NSD_Det->Integration_time*2 -1)) 
		{
   			ptr_t_NSD_Det->ptr_sumPowerN2 = ptr_t_NSD_Det->startAdrPowerN2; 
			ptr_t_NSD_Det->OutputWideBand[1] = ptr_t_NSD_Det->Power_Verif[1]; 
			ptr_t_NSD_Det->Power_Verif[1] = 0.0;
		}
	// Calculate PowerN3 ----------------------------------------------
	ptr_t_NSD_Det->OutputWideBand[2] = ptr_t_NSD_Det->OutputWideBand[2] - Oldvalue_2 + New_value_2 ;  
	ptr_t_NSD_Det->Power_Verif[2] += New_value_2;
	if (ptr_t_NSD_Det->ptr_sumPowerN3 > (ptr_t_NSD_Det->startAdrPowerN3 + ptr_t_NSD_Det->Integration_time*2*2 -1)) 
		{
   			ptr_t_NSD_Det->ptr_sumPowerN3 = ptr_t_NSD_Det->startAdrPowerN3;
			ptr_t_NSD_Det->OutputWideBand[2] = ptr_t_NSD_Det->Power_Verif[2]; 
			ptr_t_NSD_Det->Power_Verif[2] = 0.0;
		}
	//
	// ******************************************************************
	// *** End power calcualtion ****************************************
	// ******************************************************************
	//
	// ******************************************************************
	// *** Calculate Cos- and Sin- value for  F1...Fx  ******************
	// ******************************************************************
	//
	// init Pointers --------------------------------------------------- 
	// 
	// Calculate sin and cos for all detector frequencies
	//
	#pragma vector_for 
  	for(i=0;i<NUMBER_OF_RX_DET_TONES;i++)
		{
			  ptr_t_NSD_Det->Sin_Value[i] = ptr_t_NSD_Det->DetSin[i] * ptr_t_NSD_Det->Cos_DetArg[i] + ptr_t_NSD_Det->DetCos[i] * ptr_t_NSD_Det->Sin_DetArg[i];
			  ptr_t_NSD_Det->Cos_Value[i] = ptr_t_NSD_Det->DetCos[i] * ptr_t_NSD_Det->Cos_DetArg[i] - ptr_t_NSD_Det->DetSin[i] * ptr_t_NSD_Det->Sin_DetArg[i];

			  ptr_t_NSD_Det->DetSin[i] = ptr_t_NSD_Det->Sin_Value[i];
			  ptr_t_NSD_Det->DetCos[i] = ptr_t_NSD_Det->Cos_Value[i];

		}
	//
	PtrSin_Value = ptr_t_NSD_Det->Sin_Value;
	PtrCos_Value = ptr_t_NSD_Det->Cos_Value;
	tempptr = CorrelInp;
	for (i=0;i<NUMBER_OF_CORREL_OUTPUTS;i+=4)
		{
			//
			Correl_x[0] = *tempptr * *PtrCos_Value;			// x1 = Cos_Filt_out * cos (fx)
			Correl_x[1] = *tempptr++ * *PtrSin_Value;		// x2 = Cos_Filt_out * sin (fx)
			Correl_y[0] = *tempptr * *PtrSin_Value++;		// y1 = Sin_Filt_Out * sin (fx)
			Correl_y[1] = *tempptr-- * *PtrCos_Value++;		// y2 = Sin_Filt_Out * cos (fx)
			//
			ptr_t_NSD_Det->Sum_Inp[i] = Correl_x[0] + Correl_y[0];			// Sum input = x1 + y1
			ptr_t_NSD_Det->Sum_Inp[i+1] = Correl_x[1] - Correl_y[1];		// Sum input = x2 - y2
			ptr_t_NSD_Det->Sum_Inp[i+2] = Correl_y[0] - Correl_x[0];
			ptr_t_NSD_Det->Sum_Inp[i+3] = Correl_y[1] + Correl_x[1];
		}

	// ****************************************************************
	// *** Begin update Correl Output  ******************************** 
	// ****************************************************************
	ptr_t_NSD_Det->ptr_Sum_Out = ptr_t_NSD_Det->Sum_Out;
	temp = 0;
	for(i=0;i<NUMBER_OF_CORREL_OUTPUTS;i++)
		{
								
			New_value = ptr_t_NSD_Det->Sum_Inp[i] * ptr_t_NSD_Det->NCorrePower[0];
			New_value_1 = ptr_t_NSD_Det->Sum_Inp[i] * ptr_t_NSD_Det->NCorrePower[1];
			New_value_2 = ptr_t_NSD_Det->Sum_Inp[i] * ptr_t_NSD_Det->NCorrePower[2];

			Oldvalue = *(ptr_t_NSD_Det->ptrSum_F[temp]);			// get oldest value integration sum
			*(ptr_t_NSD_Det->ptrSum_F[temp])++ = New_value;			// write new value to integration sum 

			Oldvalue_1 = *(ptr_t_NSD_Det->ptrSum_F[temp+1]);		// get oldest value integration sum
			*(ptr_t_NSD_Det->ptrSum_F[temp+1])++ = New_value_1;		// write new value to integration sum 

			Oldvalue_2 = *(ptr_t_NSD_Det->ptrSum_F[temp+2]);		// get oldest value integration sum
			*(ptr_t_NSD_Det->ptrSum_F[temp+2])++ = New_value_2;		// write new value to integration sum 
		
			*(ptr_t_NSD_Det->ptr_Sum_Out) = *(ptr_t_NSD_Det->ptr_Sum_Out) - Oldvalue + New_value; // Sum_Out N1
			ptr_t_NSD_Det->Sum_Verif[temp] +=New_value;			// update verification summe N1
			ptr_t_NSD_Det->Sum_Verif[temp+1] +=New_value_1;		// update verification summe N2
			ptr_t_NSD_Det->Sum_Verif[temp+2] +=New_value_2;		// update verification summe N3
			//ptr_Sum_Out++;
	
			if(ptr_t_NSD_Det->endptrSum_F[temp] < ptr_t_NSD_Det->ptrSum_F[temp])
			{
				ptr_t_NSD_Det->ptrSum_F[temp] = ptr_t_NSD_Det->startptrSum_F[temp];
				*(ptr_t_NSD_Det->ptr_Sum_Out) = ptr_t_NSD_Det->Sum_Verif[temp];	// set Sum_Out N1 equal to verification summe 
				ptr_t_NSD_Det->Sum_Verif[temp] = 0.0;							// reset verification summe N1
			}
			ptr_t_NSD_Det->ptr_Sum_Out++;	//
			//
			*(ptr_t_NSD_Det->ptr_Sum_Out) = *(ptr_t_NSD_Det->ptr_Sum_Out) - Oldvalue_1 + New_value_1;		// Sum_Out N2
			if(ptr_t_NSD_Det->endptrSum_F[temp+1] < ptr_t_NSD_Det->ptrSum_F[temp+1])
			{
				ptr_t_NSD_Det->ptrSum_F[temp+1] = ptr_t_NSD_Det->startptrSum_F[temp+1];
				*(ptr_t_NSD_Det->ptr_Sum_Out) = ptr_t_NSD_Det->Sum_Verif[temp+1];	// set Sum_Out N2 equal to verification summe 
				ptr_t_NSD_Det->Sum_Verif[temp+1] = 0.0;								// reset verification summe N2
			}
			//
			ptr_t_NSD_Det->ptr_Sum_Out++;  // 
			//
			*(ptr_t_NSD_Det->ptr_Sum_Out) = *(ptr_t_NSD_Det->ptr_Sum_Out) - Oldvalue_2 + New_value_2;		// Sum_Out N3
			if(ptr_t_NSD_Det->endptrSum_F[temp+2] < ptr_t_NSD_Det->ptrSum_F[temp+2])
			{
				ptr_t_NSD_Det->ptrSum_F[temp+2] = ptr_t_NSD_Det->startptrSum_F[temp+2];
				*(ptr_t_NSD_Det->ptr_Sum_Out) = ptr_t_NSD_Det->Sum_Verif[temp+2];	// set Sum_Out N3 equal to verification summe 
				ptr_t_NSD_Det->Sum_Verif[temp+2] = 0.0;								// reset verification summe N3
			}
			//
			ptr_t_NSD_Det->ptr_Sum_Out++;  // 
			//
			temp +=ADAPT_NUMB;
		}
	// ****************************************************************
	// *** End update Correl Output    ******************************** 
	// ****************************************************************
	//
	// ****************************************************************
	// *** calculate detector outputs *********************************
	// ****************************************************************
	//
	for (i=0; i< ADAPT_NUMB*NO_OF_TONES ;i+=ADAPT_NUMB)
	{
		ptr_t_NSD_Det->OutputDet[i]  = ptr_t_NSD_Det->Sum_Out[2*i]   * ptr_t_NSD_Det->Sum_Out[2*i]   + ptr_t_NSD_Det->Sum_Out[2*i+ADAPT_NUMB]   * ptr_t_NSD_Det->Sum_Out[2*i+ADAPT_NUMB];
		ptr_t_NSD_Det->OutputDet[i+1]= ptr_t_NSD_Det->Sum_Out[2*i+1] * ptr_t_NSD_Det->Sum_Out[2*i+1] + ptr_t_NSD_Det->Sum_Out[2*i+ADAPT_NUMB+1] * ptr_t_NSD_Det->Sum_Out[2*i+ADAPT_NUMB+1];
		ptr_t_NSD_Det->OutputDet[i+2]= ptr_t_NSD_Det->Sum_Out[2*i+2] * ptr_t_NSD_Det->Sum_Out[2*i+2] + ptr_t_NSD_Det->Sum_Out[2*i+ADAPT_NUMB+2] * ptr_t_NSD_Det->Sum_Out[2*i+ADAPT_NUMB+2];
	}
	//
	// ****************************************************************
	// *** Reset not used Correl Outputs for HSD **********************
	// ****************************************************************
	//
	if (ptr_t_NSD_Det->HSD_On == TRUE)
	{
		//for (i= ADAPT_NUMB * NO_OF_TONES_HSD; i< ADAPT_NUMB*NO_OF_TONES ;i+=ADAPT_NUMB)
		//{
			ptr_t_NSD_Det->OutputDet[18]	= 0.0;
			ptr_t_NSD_Det->OutputDet[19]	= 0.0;
			ptr_t_NSD_Det->OutputDet[20]	= 0.0;

			ptr_t_NSD_Det->OutputDet[21]	= 0.0;
			ptr_t_NSD_Det->OutputDet[22]	= 0.0;
			ptr_t_NSD_Det->OutputDet[23]	= 0.0;
		//}
	}
	else
	{
		// NSD or AES is on -> do nothing
	}
	//
	//
	// ****************************************************************
	// *** Begin Select largest (two) detektors  **********************
	// ****************************************************************
	// ***********************************************
	// *** Select largest (two) detektors ************
	// ***********************************************
	//
		ptr_t_NSD_Det->Largest_Detektor[0] = 0;
		ptr_t_NSD_Det->Largest_Detektor[1] = 1;
		ptr_t_NSD_Det->Largest_Detektor[2] = 2;
		ptr_t_NSD_Det->Second_Detektor[0] = 0;
		ptr_t_NSD_Det->Second_Detektor[1] = 1;
		ptr_t_NSD_Det->Second_Detektor[2] = 2;
		tempptr = ptr_t_NSD_Det->OutputDet;
		//
		// select largest detektor tev1 
		//
		for (i=0; i<ADAPT_NUMB*NO_OF_TONES; i+=ADAPT_NUMB)
		{
		//	
			if (*tempptr < ptr_t_NSD_Det->OutputDet[i])  
			{
				ptr_t_NSD_Det->Largest_Detektor[0] = i;
				tempptr = &(ptr_t_NSD_Det->OutputDet[i]);	
			}
			else
			{	
			}
		}
		//
		// select largest detektor tev2 
		//
		tempptr = &(ptr_t_NSD_Det->OutputDet[1]);
		for (i=1; i<ADAPT_NUMB*NO_OF_TONES; i+=ADAPT_NUMB)
		{
			//
			if (*tempptr < ptr_t_NSD_Det->OutputDet[i]) 
			{
				tempptr = &(ptr_t_NSD_Det->OutputDet[i]);
				ptr_t_NSD_Det->Largest_Detektor[1] = i;
			}
			else
			{			
			}
		}
		//
		// select largest detektor tev3 
		//
		tempptr = &(ptr_t_NSD_Det->OutputDet[2]);
		for (i=2; i<ADAPT_NUMB*NO_OF_TONES; i+=ADAPT_NUMB)
		{
			//
			if (*tempptr < ptr_t_NSD_Det->OutputDet[i]) 
			{
				tempptr = &(ptr_t_NSD_Det->OutputDet[i]);
				ptr_t_NSD_Det->Largest_Detektor[2] = i;
			}
			else
			{			
			}
		}
	// ***********************************************
	// *** Select second largest detektors ***********
	// ***********************************************
		//
		// set start value to select second largest detektor output for tev1
		//
		tempptr = &(ptr_t_NSD_Det->OutputDet[0]);
		i_temp = 0;
		//
		if((ptr_t_NSD_Det->Largest_Detektor[0] == 0)||(ptr_t_NSD_Det->Largest_Detektor[0] == (0+ptr_t_NSD_Det->Offset_1))) 
		{
			tempptr = &(ptr_t_NSD_Det->OutputDet[ptr_t_NSD_Det->Start_Select_Second]);
			ptr_t_NSD_Det->Second_Detektor[0] = ptr_t_NSD_Det->Start_Select_Second;
			i_temp = ptr_t_NSD_Det->Start_Select_Second;
		}
		//
		// select second largest detektor tev1 without largest detektor
		//
		for (i=i_temp; i<ADAPT_NUMB*NO_OF_TONES; i+=ADAPT_NUMB)
		{
		//	
			if ((*tempptr < ptr_t_NSD_Det->OutputDet[i]) && (i!= ptr_t_NSD_Det->Largest_Detektor[0]))  
				{
					ptr_t_NSD_Det->Second_Detektor[0] = i;
					tempptr = &(ptr_t_NSD_Det->OutputDet[i]);	
				}
			else
				{	
				}
		}
		//
		// set start value to select second largest detektor output for tev2
		//
		tempptr = &(ptr_t_NSD_Det->OutputDet[1]);
		i_temp = 1;
		//
		if((ptr_t_NSD_Det->Largest_Detektor[1] == 1)||(ptr_t_NSD_Det->Largest_Detektor[1] == (1+ptr_t_NSD_Det->Offset_1))) 
			{
				tempptr = &(ptr_t_NSD_Det->OutputDet[ptr_t_NSD_Det->Start_Select_Second+1]);
				ptr_t_NSD_Det->Second_Detektor[1] = ptr_t_NSD_Det->Start_Select_Second+1;
				i_temp = ptr_t_NSD_Det->Start_Select_Second+1;
			}
			
		// select second largest detektor N2 -> tev2
		for (i=i_temp; i<ADAPT_NUMB*NO_OF_TONES; i+=ADAPT_NUMB)
		{
			//
			if ((*tempptr < ptr_t_NSD_Det->OutputDet[i]) && (i != ptr_t_NSD_Det->Largest_Detektor[1]))  
				{
					tempptr = &(ptr_t_NSD_Det->OutputDet[i]);
					ptr_t_NSD_Det->Second_Detektor[1] = i;
				}
				else
				{			
				}
		}
		//
		// set start value to select second largest detektor output for tev3
		//
		tempptr = &(ptr_t_NSD_Det->OutputDet[2]);
		i_temp = 2;
		//
		if((ptr_t_NSD_Det->Largest_Detektor[2] == 2)||(ptr_t_NSD_Det->Largest_Detektor[2] == (2+ptr_t_NSD_Det->Offset_1))) 
			{
				tempptr = &(ptr_t_NSD_Det->OutputDet[ptr_t_NSD_Det->Start_Select_Second+2]);
				ptr_t_NSD_Det->Second_Detektor[2] = ptr_t_NSD_Det->Start_Select_Second+2;
				i_temp = ptr_t_NSD_Det->Start_Select_Second+2;
			}
			
		// select second largest detektor N3 -> tev3
		for (i=i_temp; i<ADAPT_NUMB*NO_OF_TONES; i+=ADAPT_NUMB)
		{
			//
			if ((*tempptr < ptr_t_NSD_Det->OutputDet[i]) && (i != ptr_t_NSD_Det->Largest_Detektor[2]))  
				{
					tempptr = &(ptr_t_NSD_Det->OutputDet[i]);
					ptr_t_NSD_Det->Second_Detektor[2] = i;
				}
				else
				{			
				}
		}
		// ************************************************************
		// *** End Select largest (two) detektors  ********************
		// ************************************************************
		//
		// ************************************************************
		// *** Decision trip or guard *********************************
		// ************************************************************
		if(ptr_t_NSD_Det->OutputWideBand[1] > MIN_POWER_SPEECH)			// tev2 speech power
		{
			// *******************************************************************
			// *** min speech power is ok -> do trip detection
			// *******************************************************************
			i_temp = 0;	// Tev 1 active
			for (temp=0; temp<NUMBER_OF_APPLICATION_MODES*ADAPT_NUMB;temp++)
				{
					if (temp >= NUMBER_OF_APPLICATION_MODES ) i_temp= 1;			// Tev 2 active
					if (temp >= NUMBER_OF_APPLICATION_MODES*2 ) i_temp= 2;			// Tev 3 active
					// **********************************************************************
					// ** do dual tone detection for minimum level differences of the two tones
					// **********************************************************************
					if((ptr_t_NSD_Det->OutputDet[ptr_t_NSD_Det->Largest_Detektor[i_temp]] + ptr_t_NSD_Det->OutputDet[ptr_t_NSD_Det->Second_Detektor[i_temp]]) > (ptr_t_NSD_Det->Alpha_1[temp]*(ptr_t_NSD_Det->OutputWideBand[i_temp] )))
					{
						// set flag whene enough power in largest/second detector comparing to wide band power
						*detectorOutputActive = TRUE; // change_gn NSD trip and guard alarm boolian added

						// trip decision
						if(ptr_t_NSD_Det->OutputDet[ptr_t_NSD_Det->Largest_Detektor[i_temp]]*GAMMA < ptr_t_NSD_Det->OutputDet[ptr_t_NSD_Det->Second_Detektor[i_temp]] )
						{
							// trip activ
							ptr_t_NSD_Det->Activ_freq	= *(ptr_Correl_to_freq_map + ptr_t_NSD_Det->Largest_Detektor[i_temp]);
							ptr_t_NSD_Det->Activ_freq_1	= *(ptr_Correl_to_freq_map + ptr_t_NSD_Det->Second_Detektor[i_temp]);
							Rx_Command |= *(PtrCOMTAB + ptr_t_NSD_Det->Activ_freq_1 * NUMBER_OF_APPLICATION_MODES * NO_OF_TONES  +  ptr_t_NSD_Det->Activ_freq*NUMBER_OF_APPLICATION_MODES + temp - i_temp*NUMBER_OF_APPLICATION_MODES);
						}
						else
						{
							// trip not active !!!!
							
						}
					}
					else
					{
						// reset flag when to less power in detector
						*detectorOutputActive = FALSE; // change_gn NSD trip and guard alarm boolian added
					}
					// ************************************************************************
					// ** do dual tone detection for maximum level differences of the two tones
					// ************************************************************************
					// if((Lamda*(P-T1-T2)) < (P-T1))
					if((ptr_t_NSD_Det->Lamda[temp] *(ptr_t_NSD_Det->OutputWideBand[i_temp] - ptr_t_NSD_Det->OutputDet[ptr_t_NSD_Det->Largest_Detektor[i_temp]] - ptr_t_NSD_Det->OutputDet[ptr_t_NSD_Det->Second_Detektor[i_temp]])) < (ptr_t_NSD_Det->OutputWideBand[i_temp] - ptr_t_NSD_Det->OutputDet[ptr_t_NSD_Det->Largest_Detektor[i_temp]]))
					{
						// set flag whene enough power in largest/second detector comparing to wide band power
						*detectorOutputActive = TRUE; // change_gn NSD trip and guard alarm boolian added
						
						// if (T1 < T2 * TONE1_TO_TONE2_MAX_RATIO)
						if(ptr_t_NSD_Det->OutputDet[ptr_t_NSD_Det->Largest_Detektor[i_temp]]  < (TONE1_TO_TONE2_MAX_RATIO * ptr_t_NSD_Det->OutputDet[ptr_t_NSD_Det->Second_Detektor[i_temp]]))
						{
							// trip active
							ptr_t_NSD_Det->Activ_freq	= *(ptr_Correl_to_freq_map + ptr_t_NSD_Det->Largest_Detektor[i_temp]);
							ptr_t_NSD_Det->Activ_freq_1	= *(ptr_Correl_to_freq_map + ptr_t_NSD_Det->Second_Detektor[i_temp]);
							Rx_Command |= *(PtrCOMTAB + ptr_t_NSD_Det->Activ_freq_1 * NUMBER_OF_APPLICATION_MODES * NO_OF_TONES  +  ptr_t_NSD_Det->Activ_freq*NUMBER_OF_APPLICATION_MODES + temp - i_temp*NUMBER_OF_APPLICATION_MODES);
						}
						else
						{
							// trip not active tone 1 to tone 2 ratio is higher then 20dB
						}
					}
					else
					{
							// trip not active !!!
					}
				}
		}
		else
		{
			// Speech power too low do not evaluate trip!!!!!
			*detectorOutputActive = FALSE; 	// change_gn NSD trip and guard alarm boolian added
		}
	// ************************************************************************
	// *** update power level Speechband for monitoring functions   ***********
	// ************************************************************************
	if(Channel == TPE1)
	{
		// update for Ch1
		ptr_NSD_Power_Speech_Guard->SpeechPowerCh1[0] = ptr_t_NSD_Det->OutputWideBand[0];
		ptr_NSD_Power_Speech_Guard->SpeechPowerCh1[1] = ptr_t_NSD_Det->OutputWideBand[1];
		ptr_NSD_Power_Speech_Guard->SpeechPowerCh1[2] = ptr_t_NSD_Det->OutputWideBand[2];
	}
	else
	{
		// update for Ch2
		ptr_NSD_Power_Speech_Guard->SpeechPowerCh2[0] = ptr_t_NSD_Det->OutputWideBand[0];
		ptr_NSD_Power_Speech_Guard->SpeechPowerCh2[1] = ptr_t_NSD_Det->OutputWideBand[1];
		ptr_NSD_Power_Speech_Guard->SpeechPowerCh2[2] = ptr_t_NSD_Det->OutputWideBand[2];
	}
	//
 }
 else
 {
	// channel is off do nothing
	Rx_Command = 0;
 }
 //
 return (Rx_Command);
}

/*****************************************************************************
  Function:		Update_Dev_Status_Det(void)
  Description:	
  Inputs:		Pointer to Developer Status struct
  Outputs:		None
  Return:		
  Globals:		
  Important:	.

*******************************************************************************/
void SEGMENT_EXT_CODE Update_Dev_Status_Det(t_NSD_DevelopperStatus_Type *ptr_Develop_Status_NSD)
{

	//
	ptr_Develop_Status_NSD->ActivFreq_LargestCh1 = t_Ch1_Detector.Activ_freq;	// tev2
	ptr_Develop_Status_NSD->ActivFreq_SecondCh1 = t_Ch1_Detector.Activ_freq_1;	// tev2
	ptr_Develop_Status_NSD->PowerLargestDet_Ch1 = t_Ch1_Detector.OutputDet[t_Ch1_Detector.Largest_Detektor[0]]; // tev1
	ptr_Develop_Status_NSD->PowerSecondDet_Ch1 = t_Ch1_Detector.OutputDet[t_Ch1_Detector.Second_Detektor[0]];	// tev1
	ptr_Develop_Status_NSD->SpeechPower_tev1Ch1 = t_Ch1_Detector.OutputWideBand[0];
	ptr_Develop_Status_NSD->SpeechPower_tev2Ch1 = t_Ch1_Detector.OutputWideBand[1];
	ptr_Develop_Status_NSD->f_Development_5 = t_Ch1_Detector.OutputWideBand[2];
	//
	ptr_Develop_Status_NSD->ActivFreq_LargestCh2 = t_Ch2_Detector.Activ_freq;	// tev2
	ptr_Develop_Status_NSD->ActivFreq_SecondCh2 = t_Ch2_Detector.Activ_freq_1;	// tev2
	ptr_Develop_Status_NSD->PowerLargestDet_Ch2 = t_Ch2_Detector.OutputDet[t_Ch1_Detector.Largest_Detektor[0]];	// tev1
	ptr_Develop_Status_NSD->PowerSecondDet_Ch2 = t_Ch2_Detector.OutputDet[t_Ch1_Detector.Second_Detektor[0]];	// tev1
	ptr_Develop_Status_NSD->SpeechPower_tev1Ch2 = t_Ch2_Detector.OutputWideBand[0];
	ptr_Develop_Status_NSD->SpeechPower_tev2Ch2 = t_Ch2_Detector.OutputWideBand[1];
//	ptr_Develop_Status_NSD->f_Development_6 = t_Ch2_Detector.OutputWideBand[2];
	ptr_Develop_Status_NSD->f_Development_6 = t_Ch1_Detector.Alpha_1[0];

}
//
/*****************************************************************************
  Function:		Init_CompensationOfEvaluationTime(t_NSD_DSP_Config *g_NSD_Config)
  Description:	
  Inputs:		t_NSD_DSP_Config *g_NSD_Config
  Outputs:		-
  Return:		
  Globals:		
  Important:	Compensation times for Tev1 

*******************************************************************************/
void SEGMENT_EXT_CODE Init_CompensationOfEvaluationTime(t_NSD_DSP_Config *g_NSD_Config)
{

	unsigned long	i;
	//
	// @Pl_ext
	long TPE_Basic_Mode = 0;
	//
	TPE_Basic_Mode = (g_NSD_Config->mode & INT_TPE_MODE_MASK);

	switch(TPE_Basic_Mode)
	{
		case INT_TELPROT_MODE_NONE:
			t_EvalComp_Ch1.Channel_Active = OFF;
			t_EvalComp_Ch2.Channel_Active = OFF;
			//
			break;
		case INT_TELPROT_MODE_AES:
			// init Ch1 data 
			t_EvalComp_Ch1.Channel_Active = OFF;
			t_EvalComp_Ch2.Channel_Active = OFF;
			//
			break;
		case INT_TELPROT_MODE_NSD1_APLC1:
		case INT_TELPROT_MODE_NSD1_DPLC:
		case INT_TELPROT_MODE_HSD_25:
			// init Ch1 data 
			t_EvalComp_Ch1.Channel_Active = ON;
			for(i=0;i<MAX_NUMBER_LP_COMMANDS;i++)
			{
				switch(g_NSD_Config->NSD[0].CommandApplication[i])
				{
				case BLOCKING:
					t_EvalComp_Ch1.CompensationTime[i] = (long)(INTEGR_TIME_DETECTOR * Ch1_Alpha_Setting[0] * COMPENSATION_BLOCKING);
					break;

				case PERMISSIVE:
					t_EvalComp_Ch1.CompensationTime[i] = (long)(INTEGR_TIME_DETECTOR * Ch1_Alpha_Setting[1] * COMPENSATION_PERMISSIVE);
					break;

				case DIRECT:
					t_EvalComp_Ch1.CompensationTime[i] = (long)(INTEGR_TIME_DETECTOR * Ch1_Alpha_Setting[2] * COMPENSATION_DIRECT);
					break;

				case COMMAND_OFF:
					t_EvalComp_Ch1.CompensationTime[i] = 1;	// do not compensate evaluation time for not used commands
					break;
				default:
					ASSERT(FALSE);
				}
			}
			//
			t_EvalComp_Ch2.Channel_Active = OFF;
			break;
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC:
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2:
			// init ch1 
			t_EvalComp_Ch1.Channel_Active = ON;
			for(i=0;i<MAX_NUMBER_LP_COMMANDS;i++)
			{
				switch(g_NSD_Config->NSD[0].CommandApplication[i])
				{
				case BLOCKING:
					t_EvalComp_Ch1.CompensationTime[i] = (long)(INTEGR_TIME_DETECTOR * Ch1_Alpha_Setting[0] * COMPENSATION_BLOCKING);
					break;

				case PERMISSIVE:
					t_EvalComp_Ch1.CompensationTime[i] = (long)(INTEGR_TIME_DETECTOR * Ch1_Alpha_Setting[2] * COMPENSATION_PERMISSIVE);
					break;

				case DIRECT:
					t_EvalComp_Ch1.CompensationTime[i] = (long)(INTEGR_TIME_DETECTOR * Ch1_Alpha_Setting[4] * COMPENSATION_DIRECT);
					break;

				case COMMAND_OFF:
					t_EvalComp_Ch1.CompensationTime[i] = 1;	// do not compensate evaluation time for not used commands
					break;

				default:
					ASSERT(FALSE);
				}
			}
			//
			// init ch2
			t_EvalComp_Ch2.Channel_Active = ON;
			for(i=0;i<MAX_NUMBER_LP_COMMANDS;i++)
			{
				switch(g_NSD_Config->NSD[1].CommandApplication[i])
				{
				case BLOCKING:
					t_EvalComp_Ch2.CompensationTime[i] = (long)(INTEGR_TIME_DETECTOR * Ch2_Alpha_Setting[0] * COMPENSATION_BLOCKING);
					break;

				case PERMISSIVE:
					t_EvalComp_Ch2.CompensationTime[i] = (long)(INTEGR_TIME_DETECTOR * Ch2_Alpha_Setting[2] * COMPENSATION_PERMISSIVE);
					break;

				case DIRECT:
					t_EvalComp_Ch2.CompensationTime[i] = (long)(INTEGR_TIME_DETECTOR * Ch2_Alpha_Setting[4] * COMPENSATION_DIRECT);
					break;

				case COMMAND_OFF:
					t_EvalComp_Ch2.CompensationTime[i] = 1;	// do not compensate evaluation time for not used commands
					break;

				default:
					ASSERT(FALSE);
				}
			}
			//
			break;
		case INT_TELPROT_MODE_HSD:
			t_EvalComp_Ch1.Channel_Active = ON;
			for(i=0;i<MAX_NUMBER_LP_COMMANDS;i++)
			{
				switch(g_NSD_Config->NSD[0].CommandApplication[i])
				{
				case BLOCKING:
					t_EvalComp_Ch1.CompensationTime[i] = (long)(INTEGR_TIME_DETECTOR * Ch1_Alpha_Setting[0] * COMPENSATION_BLOCKING);
					break;

				case PERMISSIVE:
					t_EvalComp_Ch1.CompensationTime[i] = (long)(INTEGR_TIME_DETECTOR * Ch1_Alpha_Setting[1] * COMPENSATION_PERMISSIVE);
					break;

				case DIRECT:
					t_EvalComp_Ch1.CompensationTime[i] = (long)(INTEGR_TIME_DETECTOR * Ch1_Alpha_Setting[2] * COMPENSATION_DIRECT);
					break;

				case COMMAND_OFF:
					t_EvalComp_Ch1.CompensationTime[i] = 1;	// do not compensate evaluation time for not used commands
					break;
				default:
					ASSERT(FALSE);
				}
			}
			//
			t_EvalComp_Ch2.Channel_Active = OFF;
			break;
		default:
			ASSERT(FALSE);
	}
	
}

/*****************************************************************************
  Function:		long CompensationOfEvaluationTime(long RxCommand, long Channel)
  Description:	
  Inputs:		RxCommand, Channel
  Outputs:		RxCommand
  Return:		
  Globals:		
  Important:	

*******************************************************************************/
long SEGMENT_EXT_CODE CompensationOfEvaluationTime(long RxCommand, long Channel, BOOL StopCompensation)
{
	long RxInput, RxOutput, i;
	t_EvaluationTimeCompensation_Type *ptr_EvalComp;
	//
	RxInput = RxCommand;
	RxOutput = RxCommand;
	// **************************************************************
	// *** select channel *******************************************
	// **************************************************************
	if(Channel == TPE1)
	{
		// set pointer for Ch1
		ptr_EvalComp = &t_EvalComp_Ch1;
	}
	else
	{
		// set pointer for Ch2
		ptr_EvalComp = &t_EvalComp_Ch2;
	}
	//
	if(ptr_EvalComp->Channel_Active == ON)
	{
		// **********************************************************
		// *** Channel is active do compensation ********************
		// **********************************************************
		for (i = 0;i<MAX_NUMBER_LP_COMMANDS;i++)
		{
			// ******************************************************
			// *** set hold times for active commands  **************
			// ******************************************************
			if((RxInput&(BIT0<<i)) != 0)
			{
				// selected command is activ -> set hold time
				ptr_EvalComp->CounterCompensationTime[i] = ptr_EvalComp->CompensationTime[i];
			}
			else
			{

			}
			//______________________________________________________________			
			// changegn
			if (StopCompensation)
			{
				ptr_EvalComp->CounterCompensationTime[i] = 0;

			}
			// changegn end
			//_______________________________________________________________

			// ******************************************************
			// *** hold rx commands *********************************
			// ******************************************************
			if(ptr_EvalComp->CounterCompensationTime[i] > 0)
			{
				RxOutput |= (BIT0<<i);							// set hold command in output word
				ptr_EvalComp->CounterCompensationTime[i]--;		// decrement hold time
			}
			else
			{
				// hold time not activ do not set commadn and do not decrement hold time
			}
		}
	}
	else
	{
		// channel is not active -> do nothing
	}
	return(RxOutput);
}
