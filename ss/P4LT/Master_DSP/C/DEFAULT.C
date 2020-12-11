/*-------------------------------------------------------------------------*
 * $Workfile: DEFAULT.C $
 * Part of	: ETL600
 * Language	: C
 * Created by	: 
 * Remarks	:  
 * Purpose	: Initializes the P4LT configuration to default values
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/HMI600/C/DEFAULT.C $
 * 
 * 107   19-05-23 9:20 Chthbae
 * Changes for final G4AR board
 * 
 * 106   28.10.13 15:05 Chmaste
 * Command output link failure - Hold time default value changed from 0 to
 * 1 for AES
 * 
 * 105   21.09.12 8:38 Chmaste
 * 2.5 kHz NSD teleprotection system
 * 
 * 104   22.03.12 13:36 Chthbae
 * 
 * 103   20.03.12 12:13 Chthbae
 * Bug in Tx/Rx filter calculation fixed
 * 
 * 102   1.03.12 12:11 Chthbae
 * Default settings for RF filters changed for R4
 * 
 * 101   24.06.11 9:36 Chthbae
 * 
 * 100   11-01-19 15:04 Chmazar
 * 
 * 99    10-11-18 13:44 Chhamaa
 * Default configuration powerSupplyVoltage modified.
 * 
 * 98    10-10-28 11:25 Chhamaa
 * InitDefaultConfigP4LTSystem() modified (equipmentType added).
 * 
 * 96    17.11.08 13:47 Chsanoh
 * - Default boost ratio AES new 6 dB instead of auto-select max.
 * - APLC 2 kHz und DPLC 2 kHz (ModAboveSpeech). With AES in APLC, pilot =
 * 2220 Hz; with NSD in APLC or no TPE in APLC, pilot = 2160 Hz.
 * - Signaling rate (normal /slow)
 * - Config view adaptations
 * - Cosmetics
 * 
 * 95    08-10-16 11:54 Chmabur1
 * Changes due to merge from VoIP Branch into MAS Branch
 * 
 * 95    18.09.08 14:01 Chsanoh
 * Implementation of robust header commpression 
 *
 * 94    11.09.08 10:23 Chsanoh
 * Bug 451 fixed.
 * 
 * 93    8.09.08 10:32 Chsanoh
 * Various small bugs and cosmetics fixed.
 * 
 * 92    6.06.08 15:54 Chsanoh
 * 3rd MAS implementation phase
 * 
 * 90    22.02.08 13:05 Chsanoh
 * All adaptive equalizer features added from branch "Source AdaptEqu"
 * labeled "adEQ TestV 5.0".
 * 
 * 89    5.09.07 13:56 Chrearp
 * Added default values for the t_NSK_EqualizerCfg struct.
 * 
 * 88    23.07.07 16:54 Chtognu
 * NSK adaptive Equalizer struct added:
 * InitDefaultConfigP4LTNskEqualizer()
 * 
 * 87    07-07-08 16:04 Chmaflu
 * Modifications for added SNMP destination trap port
 * 
 * 86    07-07-06 14:08 Chmaflu
 * Add to function InitDefaultConfigO4CV(..) the O4CV data rate defaults 
 * 
 * 85    07-03-21 11:08 Chchlee
 * tcp inactivity timeout removed
 * 
 * 84    27.02.07 14:51 Chhamaa
 * InitDefaultConfigO4CV() modified
 * 
 * 83    30.01.07 17:21 Chhamaa
 * Function InitDefaultConfigO4CV() added. 
 * 
 * 82    22.01.07 18:26 Chmaflu
 * "Change one parameter Type of the function 
 * getTableEntryFromCustomTableFile(..) because of crash in HMI Release
 * build"
 * 
 * 81    19.01.07 15:31 Chhamaa
 * O4CV modifications
 * 
 * 79    14.12.06 11:50 Chmaflu
 * "NSD extended modification:
 * Add new defaults for NSDSystem.cpp Dialog
 * Add new function getTableEntryFromCustomTableFile() 
 * Add ifdef __cplusplus to compile used functions successful under the
 * DSP and under the Visual C++ environment "
 * 
 * 78    4.10.06 10:48 Chsanoh
 * RxGuardOnOffDelay initialized to 0 within function
 * InitDefaultConfigNSD600().
 * 
 * 77    19.09.06 15:50 Chsanoh
 * Bug concerning TANG window default values changed.
 * 
 * 76    14.09.06 15:15 Chsanoh
 * TANG window and Rx guard signaling implemented in corresponding masks.
 * 
 * 75    13.09.06 17:01 Chsanoh
 * Bug 373 corrected.
 * 
 * 74    8.09.06 16:56 Chsanoh
 * - NSD, HSD, and AES configuration masks harmonized.
 * - Default values, read and write operations for AES group A and group B
 * tabs adapted.
 * Changes according to input from M. Strittmatter from 04.09.06.
 * 
 * 73    28.08.06 10:11 Chsanoh
 * Bug 375 corrected.
 * 
 * 72    25.08.06 19:17 Chchlee
 * DEFAULT_EQUIPMENT_ADDRESS = 1
 * 
 * 71    25.08.06 13:44 Chsanoh
 * Classes
 * - CAES600GroupB
 * - CAES600Interface
 * - CAES600Interface0
 * - CAES600Mode
 * - CAES600System
 * - CAES600Unblock
 * revised.
 * 
 * 69    18.07.06 16:07 Chchlee
 * subnetMask corrected
 * 
 * 68    18.07.06 15:34 Chchlee
 * Some default changed
 * 
 * 67    28.04.06 13:36 Chsanoh
 * Allpass filter for group delay equalization activated as default for
 * all APLC channels.
 * 
 * 65    19.04.06 16:33 Chrobif
 * Default values of SNMP alarm polling interval modified.
 * 
 * 64    13.04.06 12:02 Chsanoh
 * Function InitDefaultRF_FilterSettings() adapted to Rev. D of document
 * TM-PTUKT05-1542 "Default settings for ETL600 RF filters".
 * 
 * 63    7.04.06 16:25 Chrobif
 * Note regarding use of InitDefaultConfigR1LA added.
 * 
 * 62    7.04.06 16:02 Chrobif
 * Initialization of member mode in t_R1LA_BridgingIpRouting_Config added.
 * 
 * 61    7.04.06 15:18 Chrobif
 * New line at the end of file added.
 * 
 * 60    7.04.06 14:46 Chrobif
 * InitDefaultConfigR1LA added
 * 
 * 59    5.04.06 10:09 Chsanoh
 * Auto-select standard filter introduced for Tx RF and Rx RF filter
 * tuning.
 * 
 * 58    24.02.06 16:14 Chsanoh
 * HSD detail configuration:
 * - Detail configuration enabled under <Configuration> <NSD600...>.
 * - Corresponding dialog adapted to HSD (new resource with ID
 * IDD_HSD600_SYSTEM).
 * 
 * Configuration view:
 * - Section "Pilot settings APLC channel 1" extended by "Power guard [W]"
 * and "Power guard [dBm]".
 * 
 * AGC blocking level:
 * - If persistent command transmission is enabled, the AGC blocking level
 * range is limited to 16 dB below the nominal pilot signal level.
 * 
 * Transmit and receive filters with a bandwidth of 4 kHz:
 * - HMI600 newly takes into account the 4 kHz Tx RF filters and the 4 kHz
 * Rx RF filters for calculation of the following Tx RF and Rx RF filter
 * parameters:
 *      - Default filter center frequency
 *      - Default filter bandwidth
 * - 4 kHz Tx RF filters and 4 kHz Rx RF filters tunable and testable
 * under <Equipment> <Tuning & Testing...> Remark: No Tx RF filter main
 * tables for 4 kHz available yet."
 * 
 * 57    3.02.06 10:17 Chsanoh
 * HSD basic configuration, HSD custom table, HSD pilot frequency and
 * allpass filter, HSD speech filter (cut off frequency) for service phone
 * implemented.
 * 
 * 55    19.10.05 15:19 Chchlee
 * NUMBER_OF_V*_PORTS_P* modified
 * 
 * 54    12.09.05 17:41 Chsanoh
 * Default alarms for R1BC changed. Default jumper settings for G4AI
 * changed.
 * 
 * 53    3.08.05 16:50 Chsanoh
 * InitDefaultRF_FilterSettings() adapted.
 * 
 * 52    28.07.05 16:35 Chsanoh
 * Calculation of default settings for ETL600 RF filters adapted.
 * 
 * 51    11.07.05 15:20 Chsanoh
 * Calculation of default filter center frequencies adapted!
 * 
 * 50    30.06.05 16:20 Chsanoh
 * 
 * 49    10.06.05 17:38 Chsanoh
 * 
 * 48    3.06.05 17:23 Chsanoh
 * 
 * 47    3.06.05 10:19 Chmibuh
 * 
 * 46    31.05.05 16:59 Chsanoh
 * Rx filter is plugged as default.
 * 
 * 45    27.05.05 8:39 Chsanoh
 * 
 * 44    25.05.05 9:41 Chchlee
 * P4LT_CONFIGURATION_VERSION added
 * 
 * 43    24.05.05 17:14 Chpeplu
 * new custom Table added UnpackedTableCh1_2_2 and UnpackedTableCh1_3_1
 * 
 * 42    23.05.05 15:19 Chsanoh
 * 
 * 41    20.05.05 16:04 Chsanoh
 * 
 * 40    19.05.05 13:46 Chsanoh
 * 
 * 39    19.05.05 13:19 Chalgyg
 * Defines changed.
 * 
 * 38    19.05.05 13:17 Chsanoh
 * 
 * 37    19.05.05 8:50 Chchlee
 * __max replaced by MY_MAX
 * 
 * 36    19.05.05 7:58 Chchlee
 * #include "stdlib.h"  for __max added
 * 
 * 35    18.05.05 18:37 Chsanoh
 * 
 * 34    18.05.05 17:45 Chsanoh
 * 
 * 33    17.05.05 17:02 Chsanoh
 * 
 * 32    11.05.05 18:10 Chsanoh
 * 
 * 31    10.05.05 15:37 Chsanoh
 * 
 * 30    9.05.05 18:07 Chsanoh
 * 
 * 29    4.05.05 17:13 Chsanoh
 * 
 * 28    29.04.05 12:51 Chutpra
 * Add Default because of global Port Structure
 * 
 * 27    22.04.05 13:29 Chpeplu
 * defaultUnpackedTableCh1 changed for new Version 05 DTMF-secure 
 * 
 * 26    13.04.05 17:15 Chsanoh
 * 
 * 25    13.04.05 12:00 Chpeplu
 * New defaultUnpackedTableCh1 for better MFC security according to new
 * Version from Be from 11.4.2005
 * 
 * 24    5.04.05 10:30 Chpeplu
 * new defaultUnpackedTableCh1 secure against DTMF and MFC 
 * 
 * 23    1.04.05 9:23 Chpeplu
 * new defaultUnpackedTableCh1 -> command BD changed to F1/F5 (old
 * (F3/F5))
 * 
 * 22    23.03.05 17:20 Chsanoh
 * 
 * 21    21.03.05 17:42 Chchlee
 * MEMSET used
 * 
 * 20    16.03.05 10:28 Chpeplu
 * pack custom table simplified
 * 
 * 19    11.03.05 15:34 Chpeplu
 * pack custom table corrected!
 * 
 * 18    11.03.05 10:09 Chpttra1
 * 
 * 17    11.03.05 9:14 Chpttra1
 * 
 * 16    11.03.05 8:58 Chpeplu
 * Renamed function InitDefaultConfigNSD550() to InitDefaultConfigNSD600()
 * and packed custom table added to default config NSD600 
 * 
 * 15    10.03.05 16:26 Chpttra1
 * 
 * 14    10.03.05 12:10 Chpttra1
 * Presision of default threshold for AES was changed (it should be not
 * less than 7 digits)
 * 
 * 13    10.03.05 12:03 Chhesch5
 * 
 * 12    10.03.05 12:01 Chhesch5
 * 
 * 11    10.03.05 10:47 Chhesch5
 * 
 * 10    10.03.05 9:46 Chhesch5
 * 
 * 9     9.03.05 16:47 Chhesch5
 * 
 * 8     9.03.05 13:34 Chhesch5
 * 
 * 7     9.03.05 9:40 Chhesch5
 * new config structs
 * 
 * 6     7.03.05 16:43 Chchlee
 * t_NSDCustomTable added
 * 
 * 5     4.03.05 16:21 Chhesch5
 * 
 * 4     25.02.05 17:21 Chhesch5
 * 
 * 3     22.02.05 16:16 Chchlee
 * newline added at end of file to avoid ADSP compiler warning
 * 
 * 2     22.02.05 13:10 Chhesch5
 * 
 * 1     21.02.05 15:24 Chhesch5
 * Initial checkin
 * Initializes the P4LT configuration to default values
 *-------------------------------------------------------------------------*/

#include "default.h"
#include "nsdCustomTable.h"

// Round a Frequency down to next 500
long FreqRoundDown(long value)
{
    long nRest = value % 500;
    return value - nRest;
}


/***************************************************************************************************
Function:		void InitDefaultConfigP4LT(t_P4LT_Config* pP4LTConfig)
Description:	Initializes the P4LT configuration (all except NSD and AES)
				to default values
Inputs:			None
Outputs:		t_P4LT_Config* pConfig		ptr to a t_P4LT_Config data struct
Return:			None
Globals:		
Important:		pP4LTConfig will be modified
***************************************************************************************************/
void SEGMENT_EXT_CODE InitDefaultConfigP4LT(t_P4LT_Config* pP4LTConfig)
{

	MEMSET(pP4LTConfig, 0, sizeof(*pP4LTConfig)); 

	// set the P4LT configuration version
	pP4LTConfig->configVersion = P4LT_CONFIGURATION_VERSION;

	InitDefaultConfigP4LTAplc( pP4LTConfig );
	InitDefaultConfigP4LTSystem( pP4LTConfig );	
	InitDefaultConfigP4LTOfdm( pP4LTConfig );
	InitDefaultConfigP4LTNsk( pP4LTConfig );
	InitDefaultConfigP4LTMux( pP4LTConfig );
	InitDefaultConfigITS( pP4LTConfig );
	InitDefaultConfigO4CV( pP4LTConfig );
	InitDefaultConfigP4LTNskEqualizer( pP4LTConfig );
}





/***************************************************************************************************
Function:		void InitDefaultConfigP4LT(t_P4LT_Config* pP4LTConfig)
Description:	Initializes the ITS (internal teleprot.) that is common to NSD and AES
				(at the moment only the ITS mode).

Inputs:			None
Outputs:		t_P4LT_Config* pConfig		ptr to a t_P4LT_Config data struct
Return:			None
Globals:		
Important:		pP4LTConfig will be modified
***************************************************************************************************/
void SEGMENT_EXT_CODE InitDefaultConfigITS(t_P4LT_Config* pP4LTConfig)
{
		
	pP4LTConfig->ITS.mode = DEFAULT_INT_TELEPROT_MODE;
	

}

/***************************************************************************************************
Function:		void InitDefaultRF_FilterSettings(t_P4LT_Config* pP4LT_Config, BOOL* pbCovering)
Description:	Initializes the following parameters of the Tx RF and the Rx RF filter according to
				TM-PTUKT05-1542 "Default settings for ETL600 RF filters":
				- Tx RF filter:	- End-stop position of coil 1 and coil 2
								- bandwidth 
								- center frequency
				- Rx RF filter:	- bandwidth 
								- center frequency

Inputs:			t_P4LT_Config* pP4LT_Config: Pointer to t_P4LT_Config struct containing the end-stop
											 position of coil 1 and 2 (only Tx RF filter), the 
											 bandwidth and the center frequency of the Tx RF and the 
											 Rx RF filter to be initialized.
				BOOL* pbCovering:			 Pointer to BOOL to hold whether the Tx filter entirely 
											 covers the Rx and Tx band or not. TRUE if it covers
											 entirely, FALSE if not.
											 If this parameter is equal to NULL it will be ignored.
Outputs:		t_P4LT_Config* pP4LT_Config: Pointer to t_P4LT_Config struct containing the end-stop
											 position of coil 1 and 2 (only Tx RF filter), the 
											 bandwidth and the center frequency of the Tx RF and the 
											 Rx RF filter which have been initialized.
				BOOL* pbCovering:			 Pointer to BOOL stating whether, in the case of 
											 operation without Rx filter, the Tx filter entirely
											 covers the Rx and Tx band or not.
											 If NULL, this parameter will be ignored.
Return:			None
Globals:		None
Important:		pP4LT_Config will be modified.
***************************************************************************************************/
void SEGMENT_EXT_CODE InitDefaultRF_FilterSettings(t_P4LT_Config* pP4LT_Config, BOOL* pbCovering)
{
	//////////////////////////////////
	// 1. Calculate basic designations
	//////////////////////////////////
	long lNominalBandwidth	 = 0;	// [Hz]
	long lNominalTxFreq		 = 0;	// [Hz]
	long lNominalRxFreq		 = 0;	// [Hz]
	long lFreqGap			 = 0;	// [Hz]
	long lTxBandCenterFreq	 = 0;	// [Hz]
	long lRxBandCenterFreq	 = 0;	// [Hz]
	long lMidFreq			 = 0;	// [Hz]
	long lTxFilterCenterFreq = 0;	// [Hz]
	long lRxFilterCenterFreq = 0;	// [Hz]
	long lTxFilterBandwidth  = 0;	// [kHz]
	long lRxFilterBandwidth  = 0;	// [kHz]
	BOOL bAdjacentBands		 = FALSE;
	BOOL bCovering			 = TRUE;
	
	GET_RF_BANDWIDTH(pP4LT_Config, lNominalBandwidth);
	lNominalTxFreq = pP4LT_Config->system.RF.txFrequency;
	lNominalRxFreq = pP4LT_Config->system.RF.rxFrequency;
	lFreqGap = labs(lNominalTxFreq - lNominalRxFreq) - lNominalBandwidth;
	
	switch(pP4LT_Config->system.RF.channelMode)
	{
		case RF_CHANNEL_MODE_ERECT:
		{
			lTxBandCenterFreq = lNominalTxFreq + (lNominalBandwidth / 2);
			lRxBandCenterFreq = lNominalRxFreq + (lNominalBandwidth / 2);

			break;
		}

		case RF_CHANNEL_MODE_INVERTED:
		{
			lTxBandCenterFreq = lNominalTxFreq - (lNominalBandwidth / 2);
			lRxBandCenterFreq = lNominalRxFreq - (lNominalBandwidth / 2);

			break;
		}
		
		default:
		{
			/* invalid case <tbd> log error */
			break;
		}
	}

	lMidFreq = (lTxBandCenterFreq + lRxBandCenterFreq) / 2;

    if(lNominalBandwidth != 2500) {
        bAdjacentBands = (lFreqGap <= DEFAULT_TX_RX_BAND_ADJACENT_GAP);
    } else {
        bAdjacentBands = (lFreqGap <= DEFAULT_TX_RX_BAND_ADJACENT_GAP_2_5);
    }

	if(pP4LT_Config->system.equipmentType == EQ_TYPE_ETL600_R3)
	{
		//////////////////////////////////
		// 2. ETL operation with Rx filter [Release 3]
		//////////////////////////////////
		if(pP4LT_Config->system.hwSettings.hybridAndP4RX_Info & P4RX_PLUGGED)
		{
			////////////////////////////////////////////////////////////
			// 2.1. Nominal bandwidth of 2 kHz, adjacent frequency bands
			////////////////////////////////////////////////////////////
			if ((lNominalBandwidth == 2000) && bAdjacentBands) {
				if (lTxBandCenterFreq <= (FIRST_TX_FILTER_BARRIER_FREQUENCY + 1000)) {
					lTxFilterBandwidth = TX_FILTER_BW_4KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_4KHZ;

					lTxFilterCenterFreq = __min(FIRST_TX_FILTER_BARRIER_FREQUENCY, lTxBandCenterFreq + SIGNUM(lRxBandCenterFreq - lTxBandCenterFreq) * 1000);
					lRxFilterCenterFreq = lRxBandCenterFreq + SIGNUM(lTxBandCenterFreq - lRxBandCenterFreq) * 1000;
                } else {
					lTxFilterBandwidth = TX_FILTER_BW_16KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_8KHZ;

					lTxFilterCenterFreq = lMidFreq;
					lRxFilterCenterFreq = lMidFreq;
				}
			}
			
			////////////////////////////////////////////////////////////
			// 2.2. Nominal bandwidth of 4 kHz, adjacent frequency bands [Release 3]
			////////////////////////////////////////////////////////////
			if ((lNominalBandwidth == 4000) && bAdjacentBands) {
				if (lTxBandCenterFreq <= (FIRST_TX_FILTER_BARRIER_FREQUENCY + 2000)) {
					lTxFilterBandwidth = TX_FILTER_BW_8KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_8KHZ;

					lTxFilterCenterFreq = __min(FIRST_TX_FILTER_BARRIER_FREQUENCY, lTxBandCenterFreq + SIGNUM(lRxBandCenterFreq - lTxBandCenterFreq) * 2000);
					lRxFilterCenterFreq = lRxBandCenterFreq + SIGNUM(lTxBandCenterFreq - lRxBandCenterFreq) * 2000;
                } else {
					lTxFilterBandwidth = TX_FILTER_BW_16KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_8KHZ;

					lTxFilterCenterFreq = lMidFreq;
					lRxFilterCenterFreq = lRxBandCenterFreq + SIGNUM(lTxBandCenterFreq - lRxBandCenterFreq) * 2000;
				}
			}

			////////////////////////////////////////////////////////////
			// 2.3. Nominal bandwidth of 8 kHz, adjacent frequency bands [Release 3]
			////////////////////////////////////////////////////////////
			if ((lNominalBandwidth == 8000) && bAdjacentBands) {
				if (lTxBandCenterFreq <= FIRST_TX_FILTER_BARRIER_FREQUENCY) {
					lTxFilterBandwidth = TX_FILTER_BW_8KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_8KHZ;

					lTxFilterCenterFreq = lTxBandCenterFreq;
					lRxFilterCenterFreq = lRxBandCenterFreq;
                } else {
					lTxFilterBandwidth = TX_FILTER_BW_16KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_8KHZ;

					lTxFilterCenterFreq = lTxBandCenterFreq + SIGNUM(lRxBandCenterFreq - lTxBandCenterFreq) * 4000;
					lRxFilterCenterFreq = lRxBandCenterFreq;
				}
			}
			
			////////////////////////////////////////////////////////////////////////
			// 2.4. Nominal bandwidth of 2, 4 or 8 kHz, non-adjacent frequency bands [Release 3]
			////////////////////////////////////////////////////////////////////////
			
			/////////////////////////////////////////////////////////////////
			// 2.4.1 Nominal bandwidth of 2 kHz, non-adjacent frequency bands [Release 3]
			/////////////////////////////////////////////////////////////////
			if ((lNominalBandwidth == 2000) && !bAdjacentBands) {
				if (lTxBandCenterFreq <= FIRST_TX_FILTER_BARRIER_FREQUENCY) {
					lTxFilterBandwidth = TX_FILTER_BW_4KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_4KHZ;

					lTxFilterCenterFreq = MY_MAX(lTxBandCenterFreq, 20000);
					lRxFilterCenterFreq = MY_MAX(lRxBandCenterFreq, 20000);
                } else {
					lTxFilterBandwidth = TX_FILTER_BW_16KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_8KHZ;

					lTxFilterCenterFreq = lTxBandCenterFreq;
					lRxFilterCenterFreq = lRxBandCenterFreq;
				}
			}
			
			//////////////////////////////////////////////////////////////////////
			// 2.4.2 Nominal bandwidth of 4 or 8 kHz, non-adjacent frequency bands [Release 3]
			//////////////////////////////////////////////////////////////////////
			if (((lNominalBandwidth == 4000) || (lNominalBandwidth == 8000)) && !bAdjacentBands) {
				if (lTxBandCenterFreq <= FIRST_TX_FILTER_BARRIER_FREQUENCY) {
					lTxFilterBandwidth = TX_FILTER_BW_8KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_8KHZ;

					lTxFilterCenterFreq = MY_MAX(lTxBandCenterFreq, 20000);
					lRxFilterCenterFreq = MY_MAX(lRxBandCenterFreq, 20000);
                } else {
					lTxFilterBandwidth = TX_FILTER_BW_16KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_8KHZ;

					lTxFilterCenterFreq = lTxBandCenterFreq;
					lRxFilterCenterFreq = lRxBandCenterFreq;
				}
			}

			//////////////////////////////////////////
			// 2.5 Nominal bandwidth higher than 8 kHz [Release 3]
			//////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////
			// 2.5.1 Nominal bandwidth higher than 8 kHz, adjacent frequency bands [Release 3]
			//////////////////////////////////////////////////////////////////////
			if ((lNominalBandwidth > 8000) && bAdjacentBands) {
				if (lNominalBandwidth <= 16000) {
					lTxFilterBandwidth = TX_FILTER_BW_16KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_16KHZ;
                } else {
					lTxFilterBandwidth = TX_FILTER_BW_32KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_32KHZ;
				}
				lTxFilterCenterFreq = lTxBandCenterFreq + SIGNUM(lRxBandCenterFreq - lTxBandCenterFreq) * (((KILO * lTxFilterBandwidth) - lNominalBandwidth) / 2);
				lRxFilterCenterFreq = lRxBandCenterFreq + SIGNUM(lTxBandCenterFreq - lRxBandCenterFreq) * (((KILO * lRxFilterBandwidth) - lNominalBandwidth) / 2);
			}

			//////////////////////////////////////////////////////////////////////////
			// 2.5.2 Nominal bandwidth higher than 8 kHz, non-adjacent frequency bands [Release 3]
			//////////////////////////////////////////////////////////////////////////
			if ((lNominalBandwidth > 8000) && !bAdjacentBands) {
				if (lNominalBandwidth <= 16000) {
					lTxFilterBandwidth = TX_FILTER_BW_16KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_16KHZ;
                } else {
					lTxFilterBandwidth = TX_FILTER_BW_32KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_32KHZ;
				}
				lTxFilterCenterFreq = MY_MAX(lTxBandCenterFreq, 16000 + ((KILO * lTxFilterBandwidth) / 2));
				lRxFilterCenterFreq = MY_MAX(lRxBandCenterFreq, 16000 + ((KILO * lRxFilterBandwidth) / 2));
			}
		}

		/////////////////////////////////////
		// 3. ETL operation without Rx filter [Release 3]
		/////////////////////////////////////
		else
		{
			switch(lNominalBandwidth)
			{
				case 2000: {
					if(lFreqGap == 0)										{lTxFilterBandwidth = (lMidFreq <= FIRST_TX_FILTER_BARRIER_FREQUENCY) ? TX_FILTER_BW_4KHZ : TX_FILTER_BW_16KHZ;}
					else if((lFreqGap >= 500) && (lFreqGap <= 4000))		{lTxFilterBandwidth = TX_FILTER_BW_8KHZ;}
					else if((lFreqGap >= 4500) && (lFreqGap <= 12000))		{lTxFilterBandwidth = TX_FILTER_BW_16KHZ;}
					else if((lFreqGap >= 12500) && (lFreqGap <= 28000))		{lTxFilterBandwidth = TX_FILTER_BW_32KHZ;}
					else if((lFreqGap >= 28500))							{lTxFilterBandwidth = TX_FILTER_BW_32KHZ; bCovering = FALSE;}
                }	break;
			
				case 4000: {
					if(lFreqGap == 0)										{lTxFilterBandwidth = (lMidFreq <= FIRST_TX_FILTER_BARRIER_FREQUENCY) ? TX_FILTER_BW_8KHZ : TX_FILTER_BW_16KHZ;}
					else if((lFreqGap >= 500) && (lFreqGap <= 8000))		{lTxFilterBandwidth = TX_FILTER_BW_16KHZ;}
					else if((lFreqGap >= 8500) && (lFreqGap <= 16000))		{lTxFilterBandwidth = TX_FILTER_BW_32KHZ;}
					else if((lFreqGap >= 16500) && (lFreqGap <= 24000))		{lTxFilterBandwidth = TX_FILTER_BW_32KHZ;}
					else if((lFreqGap >= 24500))							{lTxFilterBandwidth = TX_FILTER_BW_32KHZ; bCovering = FALSE;}

				}	break;

				case 8000: {
					if(lFreqGap == 0)										{lTxFilterBandwidth = TX_FILTER_BW_16KHZ;}
					else if((lFreqGap >= 500) && (lFreqGap <= 8000))		{lTxFilterBandwidth = TX_FILTER_BW_32KHZ;}
					else if((lFreqGap >= 8500) && (lFreqGap <= 16000))		{lTxFilterBandwidth = TX_FILTER_BW_32KHZ;}
					else if((lFreqGap >= 16500) && (lFreqGap <= 24000))		{lTxFilterBandwidth = TX_FILTER_BW_32KHZ; bCovering = FALSE;}
					else if((lFreqGap >= 24500))							{lTxFilterBandwidth = TX_FILTER_BW_32KHZ; bCovering = FALSE;}

				}	break;

				case 12000: {
					if(lFreqGap == 0)										{lTxFilterBandwidth = TX_FILTER_BW_32KHZ;}
					else if((lFreqGap >= 500) && (lFreqGap <= 8000))		{lTxFilterBandwidth = TX_FILTER_BW_32KHZ;}
					else if((lFreqGap >= 8500) && (lFreqGap <= 16000))		{lTxFilterBandwidth = TX_FILTER_BW_32KHZ; bCovering = FALSE;}
					else if((lFreqGap >= 16500) && (lFreqGap <= 24000))		{lTxFilterBandwidth = TX_FILTER_BW_32KHZ; bCovering = FALSE;}
					else if((lFreqGap >= 24500))							{lTxFilterBandwidth = TX_FILTER_BW_32KHZ; bCovering = FALSE;}

				}	break;

				case 16000: {
					if(lFreqGap == 0)										{lTxFilterBandwidth = TX_FILTER_BW_32KHZ;}
					else if((lFreqGap >= 500) && (lFreqGap <= 8000))		{lTxFilterBandwidth = TX_FILTER_BW_32KHZ; bCovering = FALSE;}
					else if((lFreqGap >= 8500) && (lFreqGap <= 16000))		{lTxFilterBandwidth = TX_FILTER_BW_32KHZ; bCovering = FALSE;}
					else if((lFreqGap >= 16500) && (lFreqGap <= 24000))		{lTxFilterBandwidth = TX_FILTER_BW_32KHZ; bCovering = FALSE;}
					else if((lFreqGap >= 24500))							{lTxFilterBandwidth = TX_FILTER_BW_32KHZ; bCovering = FALSE;}

				}	break;
				
				default: {
					if(lFreqGap == 0)										{lTxFilterBandwidth = TX_FILTER_BW_32KHZ; bCovering = FALSE;}
					else if((lFreqGap >= 500) && (lFreqGap <= 8000))		{lTxFilterBandwidth = TX_FILTER_BW_32KHZ; bCovering = FALSE;}
					else if((lFreqGap >= 8500) && (lFreqGap <= 16000))		{lTxFilterBandwidth = TX_FILTER_BW_32KHZ; bCovering = FALSE;}
					else if((lFreqGap >= 16500) && (lFreqGap <= 24000))		{lTxFilterBandwidth = TX_FILTER_BW_32KHZ; bCovering = FALSE;}
					else if((lFreqGap >= 24500))							{lTxFilterBandwidth = TX_FILTER_BW_32KHZ; bCovering = FALSE;}

				}	break;
			}
			
			lTxFilterCenterFreq = MY_MAX(lMidFreq, 16000 + ((KILO * lTxFilterBandwidth) / 2));
			
			lRxFilterBandwidth = 0;
			lRxFilterCenterFreq = 0;
		}
	}
	else
	{
		if(pP4LT_Config->system.hwSettings.hybridAndP4RX_Info & P4RX_PLUGGED)
		{
			//////////////////////////////////
			// 6. ETL operation with Rx filter [Release 4]
			//////////////////////////////////

			////////////////////////////////////////////////////////////
			// 6.1. Nominal bandwidth of 2 kHz, adjacent frequency bands [Release 4]
			////////////////////////////////////////////////////////////
			if ((lNominalBandwidth == 2000) && bAdjacentBands) {
				if (lTxBandCenterFreq <= FIRST_RX_FILTER_BARRIER_FREQUENCY_R4_2 && lRxBandCenterFreq <= FIRST_RX_FILTER_BARRIER_FREQUENCY_R4_2) {
					lTxFilterBandwidth = TX_FILTER_BW_4KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_4KHZ;

					lTxFilterCenterFreq = lTxBandCenterFreq + SIGNUM(lRxBandCenterFreq - lTxBandCenterFreq) * 1000;
					lRxFilterCenterFreq = lRxBandCenterFreq + SIGNUM(lTxBandCenterFreq - lRxBandCenterFreq) * 1000;
                } else {
					lTxFilterBandwidth = TX_FILTER_BW_8KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_8KHZ;

					lTxFilterCenterFreq = lMidFreq;
					lRxFilterCenterFreq = lMidFreq;
				}
			}

			////////////////////////////////////////////////////////////
			// 6.2 Nominal bandwidth of 2.5 kHz, adjacent frequency bands [Release 4]
			////////////////////////////////////////////////////////////
			if ((lNominalBandwidth == 2500) && bAdjacentBands) {
				if (lTxBandCenterFreq <= FIRST_RX_FILTER_BARRIER_FREQUENCY_R4_2_5 && lRxBandCenterFreq <= FIRST_RX_FILTER_BARRIER_FREQUENCY_R4_2_5) {
					lTxFilterBandwidth = TX_FILTER_BW_4KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_4KHZ;

					lTxFilterCenterFreq = lTxBandCenterFreq + SIGNUM(lRxBandCenterFreq - lTxBandCenterFreq) * 750;
					lRxFilterCenterFreq = lRxBandCenterFreq + SIGNUM(lTxBandCenterFreq - lRxBandCenterFreq) * 750;
                } else {
					lTxFilterBandwidth = TX_FILTER_BW_8KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_8KHZ;

                    lTxFilterCenterFreq = FreqRoundDown(lMidFreq);  // round down
					lRxFilterCenterFreq = FreqRoundDown(lMidFreq);  // round down
				}
			}

			////////////////////////////////////////////////////////////
			// 6.3. Nominal bandwidth of 4 kHz, adjacent frequency bands [Release 4]
			////////////////////////////////////////////////////////////
			if ((lNominalBandwidth == 4000) && bAdjacentBands) {
				if (lTxBandCenterFreq <= FIRST_RX_FILTER_BARRIER_FREQUENCY_R4_4 && lRxBandCenterFreq <= FIRST_RX_FILTER_BARRIER_FREQUENCY_R4_4) {
					lTxFilterBandwidth = TX_FILTER_BW_4KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_4KHZ;

					lTxFilterCenterFreq = lTxBandCenterFreq;
					lRxFilterCenterFreq = lRxBandCenterFreq;
                } else {
					lTxFilterBandwidth = TX_FILTER_BW_8KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_8KHZ;

					lTxFilterCenterFreq = lTxBandCenterFreq + SIGNUM(lRxBandCenterFreq - lTxBandCenterFreq) * 2000;
					lRxFilterCenterFreq = lRxBandCenterFreq + SIGNUM(lTxBandCenterFreq - lRxBandCenterFreq) * 2000;
				}
			}

			/////////////////////////////////////////////////////////////////////
			// 6.4. Nominal bandwidth of 2 or 4 kHz, non adjacent frequency bands [Release 4]
			/////////////////////////////////////////////////////////////////////
			if ((lNominalBandwidth == 2000 || lNominalBandwidth == 4000) && !bAdjacentBands) {
				if (lTxBandCenterFreq <= FIRST_TX_FILTER_BARRIER_FREQUENCY_R4) {
					lTxFilterBandwidth = TX_FILTER_BW_4KHZ;
					lTxFilterCenterFreq = lTxBandCenterFreq;
                } else {
					lTxFilterBandwidth = TX_FILTER_BW_8KHZ;
					lTxFilterCenterFreq = lTxBandCenterFreq;
				}

				if (lRxBandCenterFreq <= FIRST_TX_FILTER_BARRIER_FREQUENCY_R4) {
					lRxFilterBandwidth = RX_FILTER_BW_4KHZ;
					lRxFilterCenterFreq = lRxBandCenterFreq;
                } else {
					lRxFilterBandwidth = RX_FILTER_BW_8KHZ;
					lRxFilterCenterFreq = lRxBandCenterFreq;
				}
			}

			/////////////////////////////////////////////////////////////////////
			// 6.5. Nominal bandwidth of 2 or 4 kHz, non adjacent frequency bands [Release 4]
			/////////////////////////////////////////////////////////////////////
			if ((lNominalBandwidth == 2500) && !bAdjacentBands) {
				if (lTxBandCenterFreq <= FIRST_TX_FILTER_BARRIER_FREQUENCY_R4) {
					lTxFilterBandwidth = TX_FILTER_BW_4KHZ;
					lTxFilterCenterFreq = lTxBandCenterFreq - 250;
                } else {
					lTxFilterBandwidth = TX_FILTER_BW_8KHZ;
					lTxFilterCenterFreq = lTxBandCenterFreq - 250;
				}

				if (lRxBandCenterFreq <= FIRST_TX_FILTER_BARRIER_FREQUENCY_R4) {
					lRxFilterBandwidth = RX_FILTER_BW_4KHZ;
					lRxFilterCenterFreq = lRxBandCenterFreq - 250;
                } else {
					lRxFilterBandwidth = RX_FILTER_BW_8KHZ;
					lRxFilterCenterFreq = lRxBandCenterFreq - 250;
				}
			}

			/////////////////////////////////////////////////////////////////////
			// 6.6. Nominal bandwidth higher 4 kHz, non adjacent frequency bands [Release 4]
			/////////////////////////////////////////////////////////////////////
			if (lNominalBandwidth >= 8000) {
				if (lNominalBandwidth == 8000) {
					lTxFilterBandwidth = TX_FILTER_BW_8KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_8KHZ;
				}
				else if (lNominalBandwidth == 12000 || lNominalBandwidth == 16000) {
					lTxFilterBandwidth = TX_FILTER_BW_16KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_16KHZ;
				}
				else if (lNominalBandwidth > 16000) {
					lTxFilterBandwidth = TX_FILTER_BW_32KHZ;
					lRxFilterBandwidth = RX_FILTER_BW_32KHZ;
				}

				if (bAdjacentBands) {
					lTxFilterCenterFreq = lTxBandCenterFreq + SIGNUM(lRxBandCenterFreq - lTxBandCenterFreq) * (lTxFilterBandwidth * 1000 - lNominalBandwidth) / 2;
					lRxFilterCenterFreq = lRxBandCenterFreq + SIGNUM(lTxBandCenterFreq - lRxBandCenterFreq) * (lRxFilterBandwidth * 1000 - lNominalBandwidth) / 2;
                } else {
					lTxFilterCenterFreq = lTxBandCenterFreq;
					lRxFilterCenterFreq = lRxBandCenterFreq;
				}
			}
        } else {
			/////////////////////////////////////
			// 7. ETL operation without Rx filter [Release 4]
			/////////////////////////////////////
			lTxFilterCenterFreq = lMidFreq;

			lRxFilterBandwidth = 0;
			lRxFilterCenterFreq = 0;

            if (lNominalBandwidth != 2500) {    // Table 4
			    if (lFreqGap == 0) {
				    if (lNominalBandwidth == 2000) {
					    if (lMidFreq <= 500000) {
						    lTxFilterBandwidth = TX_FILTER_BW_4KHZ;
                        } else {
						    lTxFilterBandwidth = TX_FILTER_BW_8KHZ;
					    }
				    }
				    else if (lNominalBandwidth == 4000) {
					    lTxFilterBandwidth = TX_FILTER_BW_8KHZ;
                    }
                    else if (lNominalBandwidth == 8000) {
					    lTxFilterBandwidth = TX_FILTER_BW_16KHZ;
                    }
                    else {
					    lTxFilterBandwidth = TX_FILTER_BW_32KHZ;
					    if (lNominalBandwidth > 16000) {
						    bCovering = FALSE;
					    }
				    }
			    }
			    else if (lFreqGap >= 500 && lFreqGap <= 4000)
			    {
				    if (lNominalBandwidth == 2000) {
					    lTxFilterBandwidth = TX_FILTER_BW_8KHZ;
				    }
				    else if (lNominalBandwidth == 4000) {
					    lTxFilterBandwidth = TX_FILTER_BW_16KHZ;
				    }
				    else {
					    lTxFilterBandwidth = TX_FILTER_BW_32KHZ;
					    if (lNominalBandwidth > 12000) {
						    bCovering = FALSE;
					    }
				    }
			    }
			    else if (lFreqGap >= 4500 && lFreqGap <= 8000)
			    {
				    if (lNominalBandwidth <= 4000) {
					    lTxFilterBandwidth = TX_FILTER_BW_16KHZ;
				    }
				    else {
					    lTxFilterBandwidth = TX_FILTER_BW_32KHZ;
					    if (lNominalBandwidth > 12000) {
						    bCovering = FALSE;
					    }
				    }
			    }
			    else if (lFreqGap >= 8500 && lFreqGap <= 12000) {
				    if (lNominalBandwidth <= 2000) {
					    lTxFilterBandwidth = TX_FILTER_BW_16KHZ;
                    } 
                    else {
					    lTxFilterBandwidth = TX_FILTER_BW_32KHZ;
					    if (lNominalBandwidth > 8000) {
						    bCovering = FALSE;
					    }
				    }
			    }
			    else if (lFreqGap >= 12500 && lFreqGap <= 16000) {
				    lTxFilterBandwidth = TX_FILTER_BW_32KHZ;
				    if (lNominalBandwidth > 8000) {
					    bCovering = FALSE;
				    }
			    }
			    else if (lFreqGap >= 16500 && lFreqGap <= 24000) {
				    lTxFilterBandwidth = TX_FILTER_BW_32KHZ;
				    if (lNominalBandwidth > 4000) {
					    bCovering = FALSE;
				    }
			    }
			    else if (lFreqGap >= 24500 && lFreqGap <= 28000) {
				    lTxFilterBandwidth = TX_FILTER_BW_32KHZ;
				    if (lNominalBandwidth > 2000) {
					    bCovering = FALSE;
				    }
			    }
			    else {
				    lTxFilterBandwidth = TX_FILTER_BW_32KHZ;
				    bCovering = FALSE;
			    }
            } else {
                // Tx filter bandwidths for ETL600 operation without Rx filter (Bn = 2.5 kHz)
                lTxFilterCenterFreq = FreqRoundDown(lTxFilterCenterFreq);  // round down

                if (lFreqGap >= 0 && lFreqGap <= 3000) {
                    lTxFilterBandwidth = TX_FILTER_BW_8KHZ;
                }
                else if (lFreqGap >= 3500 && lFreqGap <= 11000) {
                    lTxFilterBandwidth = TX_FILTER_BW_16KHZ;
                }
                else if (lFreqGap >= 11500 && lFreqGap <= 27000) {
                    lTxFilterBandwidth = TX_FILTER_BW_32KHZ;
                }
                else {
				    lTxFilterBandwidth = TX_FILTER_BW_32KHZ;
				    bCovering = FALSE;
                }
            }
		}
	}

	if (pbCovering != NULL) {
		*pbCovering = bCovering;
	}

	// Set end-stop of coil 1 and coil 2 to neutral position.
	pP4LT_Config->system.hwSettings.txFilterBandwidthAndCoilEndStop &= ~TX_COIL1_END_STOP_POS_MASK;
	pP4LT_Config->system.hwSettings.txFilterBandwidthAndCoilEndStop &= ~TX_COIL2_END_STOP_POS_MASK;
	pP4LT_Config->system.hwSettings.txFilterBandwidthAndCoilEndStop |= TX_COIL1_NEUTRAL_POS;
	pP4LT_Config->system.hwSettings.txFilterBandwidthAndCoilEndStop |= TX_COIL2_NEUTRAL_POS;

	// Save filter center frequencies and filter bandwidths to configuration.
	pP4LT_Config->system.hwSettings.txFilterBandwidthAndCoilEndStop &= ~TX_FILTER_BW;
	pP4LT_Config->system.hwSettings.rxFilterBandwidth				&= ~RX_FILTER_BW;
	pP4LT_Config->system.hwSettings.txFilterBandwidthAndCoilEndStop |= (unsigned long)lTxFilterBandwidth;
	pP4LT_Config->system.hwSettings.rxFilterBandwidth				|= (unsigned long)lRxFilterBandwidth;
	
	if ((lTxFilterCenterFreq % TX_FILTER_CENTER_FREQUENCY_STEP) > TX_FILTER_CENTER_FREQUENCY_ROUNDING_EDGE) {
		pP4LT_Config->system.hwSettings.txFilterCenterFrequency = lTxFilterCenterFreq - (lTxFilterCenterFreq % TX_FILTER_CENTER_FREQUENCY_STEP) + TX_FILTER_CENTER_FREQUENCY_STEP;
    } else {
		pP4LT_Config->system.hwSettings.txFilterCenterFrequency = lTxFilterCenterFreq - (lTxFilterCenterFreq % TX_FILTER_CENTER_FREQUENCY_STEP);
	}
	
	if((lRxFilterCenterFreq % RX_FILTER_CENTER_FREQUENCY_STEP) > RX_FILTER_CENTER_FREQUENCY_ROUNDING_EDGE) {
		pP4LT_Config->system.hwSettings.rxFilterCenterFrequency = lRxFilterCenterFreq - (lRxFilterCenterFreq % RX_FILTER_CENTER_FREQUENCY_STEP) + RX_FILTER_CENTER_FREQUENCY_STEP;
    } else {
		pP4LT_Config->system.hwSettings.rxFilterCenterFrequency = lRxFilterCenterFreq - (lRxFilterCenterFreq % RX_FILTER_CENTER_FREQUENCY_STEP);
	}
}



/***************************************************************************************************
Function:		InitDefaultConfigNSD600(t_IntTeleprotConfig* pConfig,
										t_NSDExtCustomTableFile customTableFile)
Description:	Initializes the NSD600 part of the P4LT configuration to default values
Inputs:			None
Outputs:		t_IntTeleprotConfig* pConfig	           ptr to a t_IntTeleprotConfig conifg data struct
				t_NSDExtCustomTableFile customTableFile    structure which contains all custom tables 
Return:			None
Globals:		
Important:		pConfig will be modified
***************************************************************************************************/
void SEGMENT_EXT_CODE InitDefaultConfigNSD600(t_IntTeleprotConfig* pConfig,
											  t_NSDExtCustomTableFile* customTableFile)
{
	int cmd;
	int inst;

	///////////////////////////////////
	//Init local variables for NSD Extended
	///////////////////////////////////

	unsigned long lCustomModeId = 0; 
	t_NSDExtCustomTableFileEntry customTableEntry;
	t_NSDExtCustomTable  unpackedCustomTable;
	

	// ********************************************************************************************
	// *** default table for 4 independent commands (version 05 DTMF-secure )   ***************
	// ********************************************************************************************
	t_NSDCustomTable defaultUnpackedTable = 
	{	
		// command 
		// A          B          C          D          E          F						Combination		Command
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 1:	F1 / F2			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 2:	F1 / F3			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 3:	F1 / F4			not used
		{TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 4:	F1 / F5			ABCD
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 5:	F1 / F6			not used
		{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 6:	F1 / F7			Test
		{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 7:	F2 / F3			CD
		{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 8:	F2 / F4			BD
		{TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 9:	F2 / F5			BCD
		{TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 10:	F2 / F6			ACD
		{TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 11:	F2 / F7			ABD
		{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 12:	F3 / F4			D
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 13:	F3 / F5			not used
		{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 14:	F3 / F6			AD
		{TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 15:	F3 / F7			ABC
		{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 16:	F4 / F5			C
		{TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 17:	F4 / F6			BC
		{TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 18:	F4 / F7			AC
		{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 19:	F5 / F6			B
		{TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 20:	F5 / F7			AB
		{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 21:	F6 / F7			A
	};


	// ********************************************************************************************
	// *** default table for 3 + 1 commands (HSD mode)   ***************
	// ********************************************************************************************
	t_NSDCustomTable defaultUnpackedTableHSD = 
	{	
		// command 
		// A          B          C          D          E          F						Combination		Command
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_1, TERNARY_X, TERNARY_X}, // row 1:	F1 / F2			D
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 2:	F1 / F3			(not used)
		{TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 3:	F1 / F4			ABC
		{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 4:	F1 / F5			Test
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 5:	F1 / F6			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 6:	F1 / F7			not used
		{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 7:	F2 / F3			C
		{TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 8:	F2 / F4			BC
		{TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 9:	F2 / F5			AC
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 10:	F2 / F6			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 11:	F2 / F7			not used
		{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 12:	F3 / F4			B
		{TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 13:	F3 / F5			AB
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 14:	F3 / F6			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 15:	F3 / F7			not used
		{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 16:	F4 / F5			A
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 17:	F4 / F6			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 18:	F4 / F7			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 19:	F5 / F6			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 20:	F5 / F7			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 21:	F6 / F7			not used
	};

	//
	// ********************************************************************************************
	// *** Test Table for 2+2 Custom Table          ***********************************************
	// ********************************************************************************************
/*	t_NSDCustomTable UnpackedTableCh1_2_2 = 
	{	
		// command 
		// A          B          C          D          E          F						Combination		Command
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 1:	F1 / F2			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 2:	F1 / F3			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 3:	F1 / F4			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 4:	F1 / F5			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 5:	F1 / F6			not used
		{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 6:	F1 / F7			Test
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 7:	F2 / F3			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 8:	F2 / F4			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 9:	F2 / F5			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 10:	F2 / F6			not used
		{TERNARY_X, TERNARY_X, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 11:	F2 / F7			CD
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 12:	F3 / F4			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 13:	F3 / F5			not used
		{TERNARY_X, TERNARY_X, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 14:	F3 / F6			D
		{TERNARY_X, TERNARY_X, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 15:	F3 / F7			C
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 16:	F4 / F5			not used
		{TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 17:	F4 / F6			AB
		{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 18:	F4 / F7			B
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 19:	F5 / F6			not used
		{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 20:	F5 / F7			A
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 21:	F6 / F7			not used
	};
*/
	// -> Table tested i.O with FW Version NSD600 Version 0.24 (2005-05-24)
	//
	//
	// ********************************************************************************************
	// *** Test Table for 3+1 Custom Table          ***********************************************
	// ********************************************************************************************
/*	t_NSDCustomTable UnpackedTableCh1_3_1 = 
	{	
		// command 
		// A          B          C          D          E          F						Combination		Command
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 1:	F1 / F2			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 2:	F1 / F3			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 3:	F1 / F4			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 4:	F1 / F5			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 5:	F1 / F6			not used
		{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 6:	F1 / F7			Test
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 7:	F2 / F3			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 8:	F2 / F4			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_1, TERNARY_X, TERNARY_X}, // row 9:	F2 / F5			D
		{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 10:	F2 / F6			C
		{TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 11:	F2 / F7			AC
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 12:	F3 / F4			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 13:	F3 / F5			not used
		{TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 14:	F3 / F6			BC
		{TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 15:	F3 / F7			ABC
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 16:	F4 / F5			not used
		{TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 17:	F4 / F6			AB
		{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 18:	F4 / F7			B
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 19:	F5 / F6			not used
		{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 20:	F5 / F7			A
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 21:	F6 / F7			not used
	};
*/
	//
	// -> Table tested i.O with FW Version NSD600 Version 0.24 (2005-05-24)
	//
	// ********************************************************************************************
	// *** default table for 4 independent commands (version 2 DTMF and MFC secure) **********
	// ********************************************************************************************
/*	t_NSDCustomTable Four_Indep_Cmd_MFC_SecUpTable2 = 
	{	
		// command 
		// A          B          C          D          E          F						Combination		Command
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 1:	F1 / F2			not used
		{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 2:	F1 / F3			CD
		{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 3:	F1 / F4			BD
		{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 4:	F1 / F5			AD
		{TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 5:	F1 / F6			ABD
		{TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 6:	F1 / F7			ABCD
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 7:	F2 / F3			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 8:	F2 / F4			not used
		{TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 9:	F2 / F5			ACD
		{TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 10:	F2 / F6			BCD
		{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 11:	F2 / F7			D
		{TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 12:	F3 / F4			BC
		{TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 13:	F3 / F5			AC
		{TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 14:	F3 / F6			ABC
		{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 15:	F3 / F7			C
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 16:	F4 / F5			not used
		{TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 17:	F4 / F6			AB
		{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 18:	F4 / F7			B
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 19:	F5 / F6			not used
		{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 20:	F5 / F7			A
		{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 21:	F6 / F7			T
	};
*/
	//
	// Table tested i.o. Pl (Version 2 from Be for solution DTMF and MFC secure) 
	//
	// ********************************************************************************************
	// *** Table for 4 independent commands (version 1 DTMF and MFC secure)                **********
	// ********************************************************************************************
/*	t_NSDCustomTable Four_Indep_Cmd_MFC_SecUpTable1 = 
	{	
		// command 
		// A          B          C          D          E          F						Combination		Command
		{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 1:	F1 / F2			T
		{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 2:	F1 / F3			A
		{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 3:	F1 / F4			B
		{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 4:	F1 / F5			C
		{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 5:	F1 / F6			D
		{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 6:	F1 / F7			CD
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 7:	F2 / F3			not used
		{TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 8:	F2 / F4			AB
		{TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 9:	F2 / F5			AC
		{TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 10:	F2 / F6			BC
		{TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 11:	F2 / F7			ABCD
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 12:	F3 / F4			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 13:	F3 / F5			not used
		{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 14:	F3 / F6			AD
		{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 15:	F3 / F7			BD
		{TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 16:	F4 / F5			ABC
		{TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 17:	F4 / F6			ABD
		{TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 18:	F4 / F7			ACD
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 19:	F5 / F6			not used
		{TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 20:	F5 / F7			BCD
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 21:	F6 / F7			not used
	};
*/
	// Table tested i.o. Pl! (Version 1 from Be for solution DTMF and MFC secure)

	//
	// ********************************************************************************************
	// *** default table for 4 independent commands (new version first DTMF-secure )***************
	// ********************************************************************************************
/*	t_NSDCustomTable Four_Indep_DTMF_SecurCmdTable = 
	{	
		// command 
		// A          B          C          D          E          F						Combination		Command
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 1:	F1 / F2			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 2:	F1 / F3			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 3:	F1 / F4			not used
		{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 4:	F1 / F5			BD
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 5:	F1 / F6			not used
		{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 6:	F1 / F7			Test
		{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 7:	F2 / F3			CD
		{TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 8:	F2 / F4			ABCD
		{TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 9:	F2 / F5			BCD
		{TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 10:	F2 / F6			ACD
		{TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 11:	F2 / F7			ABD
		{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 12:	F3 / F4			D
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 13:	F3 / F5			not used
		{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 14:	F3 / F6			AD
		{TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 15:	F3 / F7			ABC
		{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 16:	F4 / F5			C
		{TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 17:	F4 / F6			BC
		{TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 18:	F4 / F7			AC
		{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 19:	F5 / F6			B
		{TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 20:	F5 / F7			AB
		{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 21:	F6 / F7			A
	};
*/
	// Table tested i.o. Pl!

	//
	// ********************************************************************************************
	// *** default table for 4 independent commands (old version) *********************************
	// ********************************************************************************************
/*	t_NSDCustomTable Old_four_IndependendCmdTable1 = 
	{	
		// command 
		// A          B          C          D          E          F						Combination		Command
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 1:	F1 / F2			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 2:	F1 / F3			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 3:	F1 / F4			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 4:	F1 / F5			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 5:	F1 / F6			not used
		{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 6:	F1 / F7			Test
		{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 7:	F2 / F3			CD
		{TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 8:	F2 / F4			ABCD
		{TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 9:	F2 / F5			BCD
		{TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 10:	F2 / F6			ACD
		{TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 11:	F2 / F7			ABD
		{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 12:	F3 / F4			D
		{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 13:	F3 / F5			BD
		{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 14:	F3 / F6			AD
		{TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 15:	F3 / F7			ABC
		{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 16:	F4 / F5			C
		{TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 17:	F4 / F6			BC
		{TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 18:	F4 / F7			AC
		{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 19:	F5 / F6			B
		{TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 20:	F5 / F7			AB
		{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 21:	F6 / F7			A
	};
*/
	// Table tested i.o. Pl!

	//
	// ********************************************************************************************
	// *** Test table for 3 commands with priority of command C over command A and B **************
	// ********************************************************************************************
/*	t_NSDCustomTable TestTableThreeCmdWithPriority = 
	{	
		// command 
		// A          B          C          D          E          F						Combination		Command
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 1:	F1 / F2			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 2:	F1 / F3			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 3:	F1 / F4			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 4:	F1 / F5			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 5:	F1 / F6			not used
		{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X, TERNARY_X}, // row 6:	F1 / F7			Test
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 7:	F2 / F3			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 8:	F2 / F4			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 9:	F2 / F5			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 10:	F2 / F6			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 11:	F2 / F7			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 12:	F3 / F4			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 13:	F3 / F5			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 14:	F3 / F6			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 15:	F3 / F7			not used
		{TERNARY_X, TERNARY_X, TERNARY_1, TERNARY_X, TERNARY_X, TERNARY_X}, // row 16:	F4 / F5			C
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 17:	F4 / F6			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 18:	F4 / F7			not used
		{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X, TERNARY_X}, // row 19:	F5 / F6			B
		{TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X, TERNARY_X}, // row 20:	F5 / F7			AB
		{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X, TERNARY_X}, // row 21:	F6 / F7			A
	};
*/
	// Table tested i.o Pl!

	// initialize data struct t_IntTeleprotConfig.NSD to default values

	// initialize member "t_IntTeleprotConfig ITS" of pConfig
	//pConfig->mode = DEFAULT_INT_TELEPROT_MODE;

	
  
	// set default values that are the identical for both TPE instances and != 0	
	//iterate through NSD instances
	for(inst=0; inst < NUM_OF_NSD_INST;inst++) {
		// set default values that are not command specific
		pConfig->u_NSD_AES.NSD[inst].StartInputActivTime	= DEFAULT_NSD_START_INPUT_ACTIVE_TIME;
		pConfig->u_NSD_AES.NSD[inst].UnblockingPickupTime	= DEFAULT_NSD_UNBLOCKING_PICKUP_TIME;
		pConfig->u_NSD_AES.NSD[inst].UnblockingPulsDuration	= DEFAULT_NSD_UNBLOCKING_PULS_DURATION;
		pConfig->u_NSD_AES.NSD[inst].UnblockingThreshold	= DEFAULT_NSD_UNBLOCKING_THRESHOLD;

		pConfig->u_NSD_AES.NSD[inst].NsdMode = 0;
		pConfig->u_NSD_AES.NSD[inst].NsdMode |= NSD_INIT;
		
		
		if(IS_HSD600((*pConfig)) || IS_HSD600_25((*pConfig)))
		{
			pConfig->u_NSD_AES.NSD[inst].NsdMode &= ~NSD_MODE_BOOST_RATIO;
		}

		pConfig->u_NSD_AES.NSD[inst].TimeBetweenCL_Test		= DEFAULT_NSD_CYCLIC_LOOP_TEST_TIME;

		pConfig->u_NSD_AES.NSD[inst].Guard_Alarm_Threshold	= DEFAULT_NSD_GUARD_ALARM_THRES;
		
		pConfig->u_NSD_AES.NSD[inst].OutputsAtRxAlarm			= OUTPUT_AT_RX_ALARM_UNAFFECTED;
		pConfig->u_NSD_AES.NSD[inst].OutputsAtRxAlarmHoldTime	= DEFAULT_NSD_OUT_FAILURE_HOLD;	
		pConfig->u_NSD_AES.NSD[inst].OutputsAtRxAlarmPickupTime	= DEFAULT_NSD_OUT_FAILURE_PICKUP_TIME;

		if(IS_HSD600((*pConfig)) || IS_HSD600_25((*pConfig)))	// HSD configured.
		{
			pConfig->u_NSD_AES.NSD[inst].BoostLevel	= DEFAULT_HSD_BOOST_RATIO;
		}

		else	
		{
			pConfig->u_NSD_AES.NSD[inst].BoostLevel	= -1.0F;	// @NS@ Must be negative because of the 
																// config checker.
		}

		pConfig->u_NSD_AES.NSD[inst].maxBoostLevelinDB	= 0.0F;
		pConfig->u_NSD_AES.NSD[inst].TimedTxPowerReduction = DEFAULT_NSD_TIMED_TX_POWER_REDUCTION_TIME;
		pConfig->u_NSD_AES.NSD[inst].RxGuardOnOffDelay = 0;
		
		///////////////////////////////////////////////////////////////////////////////////////////
		// Pack custom table for TPE1 (default NSD table for NSD600, default HSD table for HSD600, 
		// table(0..n) for NSD600 extended)
		///////////////////////////////////////////////////////////////////////////////////////////

		if(inst == 0 )
		{	
			
			if(IS_HSD600((*pConfig)))
			{
				nsdCustomTable_pack(TRUE, defaultUnpackedTableHSD, pConfig->u_NSD_AES.NSD[inst].customTable);
			}
            else if (IS_HSD600_25((*pConfig)))
            {
				nsdCustomTable_pack(TRUE, defaultUnpackedTable, pConfig->u_NSD_AES.NSD[inst].customTable);
            }
			else if(IS_NSD600_EXTENDED_TPE1((*pConfig)))
			{
				if(customTableFile->nrOfEntries > 0)
				{
					//Check mode from NSD Extended TPE1
					lCustomModeId = ((pConfig->mode & INT_TPE1_MODE_EXT_MASK) >> GET_SET_LSB(INT_TPE1_MODE_EXT_MASK));

					//Get the right TableEntry for this mode from The CustomTableFile
					customTableEntry = getTableEntryFromCustomTableFile(lCustomModeId,customTableFile);
					if(customTableEntry.id > 0)
					{

						///////////////////////////////////////////////////////////////////////
						//Write packed CustomTable into Config for NSD instance TPE1
						///////////////////////////////////////////////////////////////////////

						unpackedCustomTable =  customTableEntry.customTable;
						nsdCustomTable_packEx(TRUE,&unpackedCustomTable,pConfig->u_NSD_AES.NSD[inst].customTable);
					}
						
				}
			}	
			else 
			{
				//Custom table for NSD mode
				nsdCustomTable_pack(TRUE, defaultUnpackedTable, pConfig->u_NSD_AES.NSD[inst].customTable);
			}
		}
		
		/////////////////////////////////////////////////////////////
		// pack custom table for TPE2 (default NSD,HSD or extended NSD)
		/////////////////////////////////////////////////////////////
		if(inst == 1 )
		{
			
			if(IS_NSD600_EXTENDED_TPE2((*pConfig)))
			{
				
				if(customTableFile->nrOfEntries > 0)
				{

					/////////////////////////////////
					//Get the right CustomTableEntry
					/////////////////////////////////
					      
					//Check NSD extended mode from TPE2
					lCustomModeId = ((pConfig->mode & INT_TPE2_MODE_EXT_MASK) >> GET_SET_LSB(INT_TPE2_MODE_EXT_MASK));

					customTableEntry = getTableEntryFromCustomTableFile(lCustomModeId,customTableFile);	
					if(customTableEntry.id > 0)
					{
										
						///////////////////////////////////////////////////////////////////////
						//Write packed CustomTable into Config for NSD instance TPE2
						///////////////////////////////////////////////////////////////////////
						unpackedCustomTable =  customTableEntry.customTable;
						nsdCustomTable_packEx(TRUE,&unpackedCustomTable,pConfig->u_NSD_AES.NSD[inst].customTable);
					}
	
				}
			}
			else
			{
					//Custom Table for NSD,HSD mode
					nsdCustomTable_pack(TRUE, defaultUnpackedTable, pConfig->u_NSD_AES.NSD[inst].customTable);
			}
		}

			
		///////////////////////////////////////////////////////////////////////////////////
		//
		// set default values that are identical for 
		// all commmands for NSD600,HSD and NSD600 Extended
		//
		//////////////////////////////////////////////////////////////////////////////////


		for(cmd=0; cmd <= MAX_NUMBER_LP_COMMANDS;cmd++) 
		{	
			
			if(IS_NSD600_EXTENDED((*pConfig)))
			{

				if(customTableFile->nrOfEntries > 0)
				{
            
					if((inst == 0) && IS_NSD600_EXTENDED_TPE1((*pConfig))) //TPE1 NSD extended	
					{
						//Check NSD Extended Mode inside TPE1
						lCustomModeId = ((pConfig->mode & INT_TPE1_MODE_EXT_MASK) >> GET_SET_LSB(INT_TPE1_MODE_EXT_MASK));
					
					}
					else if(IS_NSD600_EXTENDED_TPE2((*pConfig)) && (inst == 1)) //TPE2 NSD extended
					{
						//Check NSD Extended Mode inside TPE2
						lCustomModeId = ((pConfig->mode & INT_TPE2_MODE_EXT_MASK) >> GET_SET_LSB(INT_TPE2_MODE_EXT_MASK));
					
					}
				
				
					///////////////////////////////////////////////
					//Get the right CustomTableEntry for this mode
					///////////////////////////////////////////////
					customTableEntry = getTableEntryFromCustomTableFile(lCustomModeId,customTableFile);
					if(customTableEntry.id > 0)
					{
							 
						////////////////////////////////////////////////////////
						// Write defaults for defaultApplication into Config 
						////////////////////////////////////////////////////////
						pConfig->u_NSD_AES.NSD[inst].CommandApplication[cmd] 
									= customTableEntry.defaultApplication[cmd];
					}
				}

			}
			else			//NSD,HSD mode 
			{
				pConfig->u_NSD_AES.NSD[inst].CommandApplication[cmd]		= 0;
			}

		}
			
	}

	// Initialize the default parameters for the command application of all NSD600
	// commands of all NSD600 instances.
	InitDefaultsNSD600_CmdsApplication(pConfig);
}



/***************************************************************************************************
Function:		InitDefaultsNSD600_CmdsApplication(t_IntTeleprotConfig* pConfig)
Description:	Initializes the default parameters for the command application of all NSD600
				commands of all NSD600 instances.
Inputs:			t_IntTeleprotConfig* pConfig: Pointer to instance of struct t_IntTeleprotConfig
Outputs:		t_IntTeleprotConfig* pConfig: Pointer to instance of struct t_IntTeleprotConfig
Return:			None
Globals:		None
Important:		pConfig will be modified.
***************************************************************************************************/
void SEGMENT_EXT_CODE InitDefaultsNSD600_CmdsApplication(t_IntTeleprotConfig* pConfig)
{
	int inst;
	int cmd;

	for(inst = 0; inst < NUM_OF_NSD_INST; inst++)
	{
		for(cmd = 0; cmd <= MAX_NUMBER_LP_COMMANDS; cmd++) 
		{
			InitDefaultsNSD600_CmdApplication(pConfig, inst, cmd);
		}
	}
}



/***************************************************************************************************
Function:		InitDefaultsNSD600_CmdApplication(t_IntTeleprotConfig* pConfig)
Description:	Initializes the default parameters for the command application of a specific NSD600
				command of a specific NSD600 instance.
Inputs:			t_IntTeleprotConfig* pConfig: Pointer to instance of struct t_IntTeleprotConfig
				int iTPE_Inst: NSD600 TPE instance concerned
				int iCmd: Command concerned
Outputs:		t_IntTeleprotConfig* pConfig: Pointer to instance of struct t_IntTeleprotConfig
Return:			None
Globals:		None
Important:		pConfig will be modified.
***************************************************************************************************/
void SEGMENT_EXT_CODE InitDefaultsNSD600_CmdApplication(t_IntTeleprotConfig* pConfig, int iTPE_Inst, int iCmd)
{
	// Set defaults for different command applications.
	// 0 OFF
	// 1 Blocking
	// 2 Permissive
	// 3 Direct
	switch(pConfig->u_NSD_AES.NSD[iTPE_Inst].CommandApplication[iCmd])
	{
		case 1: // Blocking
		{
			pConfig->u_NSD_AES.NSD[iTPE_Inst].TxInpComDelay[iCmd]		   = DEFAULT_NSD_TX_INPUT_COM_DELAY_BLOCKING;
			pConfig->u_NSD_AES.NSD[iTPE_Inst].TxInpComDuration[iCmd]       = DEFAULT_NSD_TX_INPUT_COM_DURATION_BLOCKING;
			pConfig->u_NSD_AES.NSD[iTPE_Inst].TxInpComProlongation[iCmd]   = DEFAULT_NSD_TX_INPUT_COM_PROLONGATION_BLOCKING;
			pConfig->u_NSD_AES.NSD[iTPE_Inst].TxInpComMonit_Duration[iCmd] = DEFAULT_NSD_TX_INP_COM_MONIT_DURATION;

			
			pConfig->u_NSD_AES.NSD[iTPE_Inst].RxOutComDelay[iCmd]          = DEFAULT_NSD_RX_OUTPUT_COM_DELAY_BLOCKING;
			pConfig->u_NSD_AES.NSD[iTPE_Inst].RxOutComDuration[iCmd]       = DEFAULT_NSD_RX_OUT_DURATION_IF_OFF;	
			pConfig->u_NSD_AES.NSD[iTPE_Inst].RxOutComProlongation[iCmd]   = DEFAULT_NSD_RX_OUTPUT_PROLONGATION_BLOCKING;
			pConfig->u_NSD_AES.NSD[iTPE_Inst].RxOutComMonit_Duration[iCmd] = DEFAULT_NSD_RX_OUT_COM_MONIT_DURATION_OFF;	// @NS@: Originally DEFAULT_NSD_RX_OUT_COM_MONIT_DURATION;	
	
			break;
		}
		
		case 2: // Permissive
		{
			pConfig->u_NSD_AES.NSD[iTPE_Inst].TxInpComDelay[iCmd]		   = DEFAULT_NSD_TX_INPUT_COM_DELAY_PERMISSIVE;
			pConfig->u_NSD_AES.NSD[iTPE_Inst].TxInpComDuration[iCmd]       = DEFAULT_NSD_TX_INPUT_COM_DURATION_PERMISSIVE;
			pConfig->u_NSD_AES.NSD[iTPE_Inst].TxInpComProlongation[iCmd]   = DEFAULT_NSD_TX_INPUT_COM_PROLONGATION_PERMISSIVE;
			pConfig->u_NSD_AES.NSD[iTPE_Inst].TxInpComMonit_Duration[iCmd] = DEFAULT_NSD_TX_INP_COM_MONIT_DURATION;

			
			pConfig->u_NSD_AES.NSD[iTPE_Inst].RxOutComDelay[iCmd]          = DEFAULT_NSD_RX_OUTPUT_COM_DELAY_PERMISSIVE;
			pConfig->u_NSD_AES.NSD[iTPE_Inst].RxOutComDuration[iCmd]       = DEFAULT_NSD_RX_OUT_DURATION_IF_OFF;	
			pConfig->u_NSD_AES.NSD[iTPE_Inst].RxOutComProlongation[iCmd]   = DEFAULT_NSD_RX_OUTPUT_PROLONGATION_PERMISSIVE;
			pConfig->u_NSD_AES.NSD[iTPE_Inst].RxOutComMonit_Duration[iCmd] = DEFAULT_NSD_RX_OUT_COM_MONIT_DURATION_OFF;	// @NS@: Originally DEFAULT_NSD_RX_OUT_COM_MONIT_DURATION;	
			
			break;
		}
		
		case 3: // Direct
		{
			pConfig->u_NSD_AES.NSD[iTPE_Inst].TxInpComDelay[iCmd]		   = DEFAULT_NSD_TX_INPUT_COM_DELAY_DIRECT;
			pConfig->u_NSD_AES.NSD[iTPE_Inst].TxInpComDuration[iCmd]       = DEFAULT_NSD_TX_INPUT_COM_DURATION_DIRECT;
			pConfig->u_NSD_AES.NSD[iTPE_Inst].TxInpComProlongation[iCmd]   = DEFAULT_NSD_TX_INPUT_COM_PROLONGATION_DIRECT;
			pConfig->u_NSD_AES.NSD[iTPE_Inst].TxInpComMonit_Duration[iCmd] = DEFAULT_NSD_TX_INP_COM_MONIT_DURATION;
		

		
			pConfig->u_NSD_AES.NSD[iTPE_Inst].RxOutComDelay[iCmd]          = DEFAULT_NSD_RX_OUTPUT_COM_DELAY_DIRECT;
			pConfig->u_NSD_AES.NSD[iTPE_Inst].RxOutComDuration[iCmd]       = DEFAULT_NSD_RX_OUT_DURATION_IF_OFF;	
			pConfig->u_NSD_AES.NSD[iTPE_Inst].RxOutComProlongation[iCmd]   = DEFAULT_NSD_RX_OUTPUT_PROLONGATION_DIRECT;
			pConfig->u_NSD_AES.NSD[iTPE_Inst].RxOutComMonit_Duration[iCmd] = DEFAULT_NSD_RX_OUT_COM_MONIT_DURATION_OFF;	// @NS@: Originally DEFAULT_NSD_RX_OUT_COM_MONIT_DURATION;	

			break;
		}
		
		case 0:	// Off
		default:
		{
			pConfig->u_NSD_AES.NSD[iTPE_Inst].TxInpComDelay[iCmd]		   = DEFAULT_NSD_TX_INPUT_COM_DELAY_OFF;
			pConfig->u_NSD_AES.NSD[iTPE_Inst].TxInpComDuration[iCmd]       = DEFAULT_NSD_TX_INPUT_COM_DURATION_OFF;
			pConfig->u_NSD_AES.NSD[iTPE_Inst].TxInpComProlongation[iCmd]   = DEFAULT_NSD_TX_INPUT_COM_PROLONGATION_OFF;
			pConfig->u_NSD_AES.NSD[iTPE_Inst].TxInpComMonit_Duration[iCmd] = DEFAULT_NSD_TX_INP_COM_MONIT_DURATION;

			pConfig->u_NSD_AES.NSD[iTPE_Inst].RxOutComDelay[iCmd]          = DEFAULT_NSD_RX_OUTPUT_COM_DELAY_OFF;			
			pConfig->u_NSD_AES.NSD[iTPE_Inst].RxOutComDuration[iCmd]       = DEFAULT_NSD_RX_OUT_DURATION_IF_OFF;		// @NS@: Originally DEFAULT_NSD_RX_OUT_DURATION_IF_ON;	
			pConfig->u_NSD_AES.NSD[iTPE_Inst].RxOutComProlongation[iCmd]   = DEFAULT_NSD_RX_OUTPUT_PROLONGATION_OFF;
			pConfig->u_NSD_AES.NSD[iTPE_Inst].RxOutComMonit_Duration[iCmd] = DEFAULT_NSD_RX_OUT_COM_MONIT_DURATION_OFF;	// @NS@: Originally DEFAULT_NSD_RX_OUT_COM_MONIT_DURATION;	
	
			break;
		}
	}
}

		

/***************************************************************************************************
Function:		InitDefaultConfigAES600(t_IntTeleprotConfig* pConfig)
Description:	Initializes the AES600 part of the P4LT configuration to default values
Inputs:			None
Outputs:		t_IntTeleprotConfig* pConfig	ptr to a t_IntTeleprotConfig conifg data struct
Return:			None
Globals:		
Important:		pConfig will be modified
***************************************************************************************************/
void SEGMENT_EXT_CODE InitDefaultConfigAES600(t_IntTeleprotConfig* pConfig)
{
	int cmd;
	BOOL bIncreasedDep = FALSE;
	BOOL bT01 = FALSE;

	// Defaults for mode
	pConfig->u_NSD_AES.AES.AES600_Mode = 0; // Accumulative long
	pConfig->u_NSD_AES.AES.AES600_Mode |= AES600_INIT;

	// Determine performance settings
	if(!(pConfig->u_NSD_AES.AES.AES600_Mode & AES600_SEC_DEP_CMD))
	{
		bIncreasedDep = TRUE;
	}

	if(!(pConfig->u_NSD_AES.AES.AES600_Mode & AES600_T0))
	{
		bT01 = TRUE;
	}

	// set default values that are not command specific
	// AES.StartInputActivTime is not used in HMI600 Rel.1
	pConfig->u_NSD_AES.AES.StartInputActivTime		  = DEFAULT_NSD_START_INPUT_ACTIVE_TIME; //1
	pConfig->u_NSD_AES.AES.UnblockingPickupTime		  = DEFAULT_NSD_UNBLOCKING_PICKUP_TIME;//20
	pConfig->u_NSD_AES.AES.UnblockingPulsDuration	  = DEFAULT_NSD_UNBLOCKING_PULS_DURATION; //200
	pConfig->u_NSD_AES.AES.UnblockingThreshold		  = DEFAULT_NSD_UNBLOCKING_THRESHOLD; //-14 dB
	pConfig->u_NSD_AES.AES.HoldTime_Tx_Rx_Ack		  = 1; 

	pConfig->u_NSD_AES.AES.TimeBetweenCL_Test		  = DEFAULT_NSD_CYCLIC_LOOP_TEST_TIME;//6

	pConfig->u_NSD_AES.AES.Guard_Alarm_Threshold	  = (float)0.2511886431509580;// -6dB//3.98107170553497; //6dB //3.98107170553497

	pConfig->u_NSD_AES.AES.OutputsAtRxAlarm			  = OUTPUT_AT_RX_ALARM_UNAFFECTED;//0 (unaffected)
	pConfig->u_NSD_AES.AES.OutputsAtRxAlarmHoldTime	  = DEFAULT_AES_OUT_FAILURE_HOLD; //1
	pConfig->u_NSD_AES.AES.OutputsAtRxAlarmPickupTime = DEFAULT_AES_OUT_FAILURE_PICKUP_TIME; //10

	pConfig->u_NSD_AES.AES.BoostLevel				  = (float)1.99526231496887960;	// corresponds to 6dB
																					// @NS@	Set to -1.0 if AES600_MODE_BOOST_RATIO is set within AES600_INIT;
																					//		Must be negative because of the config checker.
	pConfig->u_NSD_AES.AES.maxBoostLevelinDB		  = (float)0.0;
	pConfig->u_NSD_AES.AES.SNR_Alarm_Threshold		  = (float)1.0;

	pConfig->u_NSD_AES.AES.TANG_DelayOnOff = 0;
	SET_BITS(TRUE, pConfig->u_NSD_AES.AES.TANG_DelayOnOff, (DEFAULT_TANG_SET_TIME << GET_SET_LSB(TANG_DELAY_ON_MASK)));
	SET_BITS(TRUE, pConfig->u_NSD_AES.AES.TANG_DelayOnOff, (DEFAULT_TANG_RESET_TIME << GET_SET_LSB(TANG_DELAY_OFF_MASK)));

	pConfig->u_NSD_AES.AES.RxGuardOnOffDelay = 0;
	SET_BITS(TRUE, pConfig->u_NSD_AES.AES.RxGuardOnOffDelay, (DEFAULT_RX_GUARD_SIGNALING_EXTRA_OFF_DELAY << GET_SET_LSB(RX_GUARD_OFF_DELAY_MASK)));
	SET_BITS(TRUE, pConfig->u_NSD_AES.AES.RxGuardOnOffDelay, (DEFAULT_RX_GUARD_SIGNALING_EXTRA_ON_DELAY << GET_SET_LSB(RX_GUARD_ON_DELAY_MASK)));

	///////////////////////////////////////////////////////////
	// Set default values that are identical for all commmands.
	///////////////////////////////////////////////////////////

	//////////////////////////////
	// 1. Line protection commands
	//////////////////////////////
	for(cmd = 0; cmd < (MAX_NUMBER_AES_LP_COMMANDS - 1); cmd++)
	{
		// Inputs		
		if(pConfig->u_NSD_AES.AES.AES600_Mode & AES600_LP_MODE)	// Signaling
		{
			pConfig->u_NSD_AES.AES.TxInpComDelay[cmd] = DEFAULT_AES_LP_CMD_TX_INP_COM_DELAY_SIGNALING;
		}

		else	// Stepping
		{
			pConfig->u_NSD_AES.AES.TxInpComDelay[cmd] = DEFAULT_AES_LP_CMD_TX_INP_COM_DELAY_STEPPING;
		}		
		
		pConfig->u_NSD_AES.AES.TxInpComProlongation[cmd]   = DEFAULT_AES_LP_CMD_TX_INP_COM_PROL;
		pConfig->u_NSD_AES.AES.TxInpComMonit_Duration[cmd] = DEFAULT_AES_LP_CMD_TX_INP_COM_MONIT_DUR;

		// Outputs
		if(bT01 && bIncreasedDep)	// T01 and increased dependability
		{
			pConfig->u_NSD_AES.AES.RxOutComProlongation[cmd] = DEFAULT_AES_LP_CMD_RX_OUT_COM_PROL_T01_ID;
		}

		else if(bT01 && !bIncreasedDep)	// T01 and increase security
		{
			pConfig->u_NSD_AES.AES.RxOutComProlongation[cmd] = DEFAULT_AES_LP_CMD_RX_OUT_COM_PROL_T01_IS;
		}
			
		else if(!bT01 && bIncreasedDep)	// T02 and increased dependability
		{
			pConfig->u_NSD_AES.AES.RxOutComProlongation[cmd] = DEFAULT_AES_LP_CMD_RX_OUT_COM_PROL_T02_ID;
		}
	
		else if(!bT01 && !bIncreasedDep)	// T02 and increased security
		{
			pConfig->u_NSD_AES.AES.RxOutComProlongation[cmd] = DEFAULT_AES_LP_CMD_RX_OUT_COM_PROL_T02_IS;
		}

		// invalid case <tbd> log error	
		else	
		{

		}
		
		pConfig->u_NSD_AES.AES.AES600_Mode |= AES600_RXOUTDURPROL_A;
		pConfig->u_NSD_AES.AES.RxOutComDuration[cmd] = DEFAULT_AES_LP_CMD_RX_OUT_COM_DUR;
	}

	////////////////////////
	// 2. Persistent command
	////////////////////////
	// Inputs
	pConfig->u_NSD_AES.AES.TxInpComDelay[(MAX_NUMBER_AES_LP_COMMANDS - 1)]			= DEFAULT_AES_P_CMD_TX_INP_COM_DELAY;
	pConfig->u_NSD_AES.AES.TxInpComProlongation[(MAX_NUMBER_AES_LP_COMMANDS - 1)]	= DEFAULT_AES_P_CMD_TX_INP_COM_PROL;
	pConfig->u_NSD_AES.AES.TxInpComMonit_Duration[(MAX_NUMBER_AES_LP_COMMANDS - 1)]	= DEFAULT_AES_P_CMD_TX_INP_COM_MONIT_DUR;

	// Outputs
	pConfig->u_NSD_AES.AES.RxOutComProlongation[(MAX_NUMBER_AES_LP_COMMANDS - 1)]   = DEFAULT_AES_P_CMD_RX_OUT_COM_PROL;
	pConfig->u_NSD_AES.AES.RxOutComDuration[(MAX_NUMBER_AES_LP_COMMANDS - 1)]		= DEFAULT_AES_P_CMD_RX_OUT_COM_DUR;
	
	//////////////////
	// 3. AES commands
	//////////////////
	for(cmd=0; cmd < MAX_NUMBER_AES_AES_COMMANDS; cmd++)
	{
		///////////////
		// clear values
		///////////////
				
		// Inputs
		SET_BITS(FALSE, pConfig->u_NSD_AES.AES.TxInpRxOutComParametersAES[cmd], AES600_TXINPDELAY_MASK);
		
		// Outputs
		SET_BITS(FALSE, pConfig->u_NSD_AES.AES.RxOutComParametersAES[cmd], AES600_RXOUTPROLONG_MASK);
		SET_BITS(FALSE, pConfig->u_NSD_AES.AES.RxOutComParametersAES[cmd], AES600_RXOUTDURATION_MASK);

		/////////////////////
		// set default values
		/////////////////////

		// Inputs
		SET_BITS(TRUE, pConfig->u_NSD_AES.AES.TxInpRxOutComParametersAES[cmd], (DEFAULT_AES_AES_CMD_TX_INP_COM_DELAY << GET_SET_LSB(AES600_TXINPDELAY_MASK)));
		
		// Outputs		
		if(bT01 && bIncreasedDep)	// T01 and increased dependability
		{
			SET_BITS(TRUE, pConfig->u_NSD_AES.AES.RxOutComParametersAES[cmd], (DEFAULT_AES_AES_CMD_RX_OUT_COM_PROL_T01_ID << GET_SET_LSB(AES600_RXOUTPROLONG_MASK)));
		}
		
		else if(bT01 && !bIncreasedDep)	// T01 and increase security
		{
			SET_BITS(TRUE, pConfig->u_NSD_AES.AES.RxOutComParametersAES[cmd], (DEFAULT_AES_AES_CMD_RX_OUT_COM_PROL_T01_IS << GET_SET_LSB(AES600_RXOUTPROLONG_MASK)));
		}
			
		else if(!bT01 && bIncreasedDep)	// T02 and increased dependability
		{
			SET_BITS(TRUE, pConfig->u_NSD_AES.AES.RxOutComParametersAES[cmd], (DEFAULT_AES_AES_CMD_RX_OUT_COM_PROL_T02_ID << GET_SET_LSB(AES600_RXOUTPROLONG_MASK)));
		}
		
		else if(!bT01 && !bIncreasedDep)	// T02 and increased security
		{
			SET_BITS(TRUE, pConfig->u_NSD_AES.AES.RxOutComParametersAES[cmd], (DEFAULT_AES_AES_CMD_RX_OUT_COM_PROL_T02_IS << GET_SET_LSB(AES600_RXOUTPROLONG_MASK)));
		}

		// invalid case <tbd> log error	
		else	
		{

		}
		
		SET_BITS(TRUE, pConfig->u_NSD_AES.AES.RxOutComParametersAES[cmd], (DEFAULT_AES_AES_CMD_RX_OUT_COM_DUR << GET_SET_LSB(AES600_RXOUTDURATION_MASK)));     // Rx output duration
	}
	
//////////////////////// REMOVE LATER TV 070305////////////////////////////////////////////////////////////
/*
	pConfig->u_NSD_AES.AES.AES_Mode					= 0;
	pConfig->u_NSD_AES.AES.T0						= 0; //default is TO1
	pConfig->u_NSD_AES.AES.LP_Mode					= 0; //default is Mode3 (State mode, persistent - off)
	pConfig->u_NSD_AES.AES.Sec_Dep_Commands			= 1; //Incrased Security 
	pConfig->u_NSD_AES.AES.ResetRxAlarm				= 0; //Reset SNR and level alarm: automatically
	pConfig->u_NSD_AES.AES.NSD_ETL_Guard				= DEFAULT_NSD_ETL_GUARD; //0, ETL guard is default
	pConfig->u_NSD_AES.AES.CyclicLoopTestOn_Off		= DEFAULT_NSD_CYCLIC_LOOP_TEST_ON; //Off
	pConfig->u_NSD_AES.AES.PersistentCommand_On_Off	= 0; //Persistent off

	//New fields
	pConfig->u_NSD_AES.AES.RxOutDurProlong_GroupA = 0; //Rx Output Prolongation fields are enabled
	pConfig->u_NSD_AES.AES.RxOutDurProlong_GroupB = 1; //Rx output duration
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
}



/***************************************************************************************************
Function:		void InitDefaultConfigP4LTSystem(t_P4LT_Config* pP4LTConfig)
Description:	Initializes the System part of the P4LT configuration 
				to default values
Inputs:			None
Outputs:		pP4LTConfig* pConfig		ptr to a pP4LTConfig data struct that will be initialized
Return:			None
Globals:		
Important:		pConfig will be modified
***************************************************************************************************/
void SEGMENT_EXT_CODE InitDefaultConfigP4LTSystem(t_P4LT_Config* pP4LTConfig)
{
	int	i;

	t_SystemConfig*	pConfig	= &(pP4LTConfig->system);

	// Initialize member "t_SystemConfig system" of pConfig
	pConfig->equipmentInfo.address					  =	DEFAULT_EQUIPMENT_ADDRESS;
	pConfig->equipmentInfo.configurationCounter		  = DEFAULT_MODIFICATION_COUNTER;

	pConfig->HMIBaudrate = DEFAULT_HMI_BAUDRATE;

	for(i=0;i<sizeof(pConfig->equipmentInfo.identification);i++) {
		pConfig->equipmentInfo.identification[i]		  = 0;
	}

	pConfig->G4Ax_BoardType								= G4AX_BORADTYPE_G4AI;
	pConfig->equipmentType								= DEFAULT_EQUIPMENT_TYPE;

	pConfig->serviceData.type							= DEFAULT_SERVICES_TYPE;
	pConfig->serviceData.rfPEP							= DEFAULT_SERVICES_RF_PEP; 
	pConfig->serviceData.telephonyTeleprotMode[0]		= 0;
	pConfig->serviceData.telephonyTeleprotMode[1]		= 0;
	pConfig->serviceData.telephonyTeleprotMode[2]		= 0;
	pConfig->serviceData.speechBandwidth[0]				= -1; // none
	pConfig->serviceData.speechBandwidth[1]				= -1; // none
	pConfig->serviceData.speechBandwidth[2]				= -1; // none
	pConfig->serviceData.numberOfTeleoperationPorts[0]	= 0;
	pConfig->serviceData.numberOfTeleoperationPorts[1]	= 0;
	pConfig->serviceData.numberOfTeleoperationPorts[2]	= 0;
	pConfig->serviceData.numberOfITS_Inputs[0]			= 0;
	pConfig->serviceData.numberOfITS_SolidOutputs[0]	= 0;
	pConfig->serviceData.numberOfITS_RelayOutputs[0]	= 0;
	pConfig->serviceData.numberOfITS_Inputs[1]			= 0;
	pConfig->serviceData.numberOfITS_SolidOutputs[1]	= 0;
	pConfig->serviceData.numberOfITS_RelayOutputs[1]	= 0;
	pConfig->serviceData.TelephonySlotID[0]				= -1;
	pConfig->serviceData.TelephonySlotID[1]				= -1;
	pConfig->serviceData.TelephonySlotID[2]				= -1;
	pConfig->serviceData.LANSettings                    = LAN_SERVICES_OFF;
	
	pConfig->RF.txFrequency			= DEFAULT_TX_NOMINAL_FREQUENCY; 
	pConfig->RF.rxFrequency			= DEFAULT_RX_NOMINAL_FREQUENCY;
	pConfig->RF.PEP_Value			= DEFAULT_PEP_VALUE;	
	pConfig->RF.sumSValuesInv		= (float)DEFAULT_HF_SUMME_S_WERTE_INVERS;	
	pConfig->RF.channelMode			= DEFAULT_RF_CHANNEL_MODE;	
	pConfig->RF.txLevelAdjustment	= (float)DEFAULT_HF_TX_PEGELKORREKTUR;	
	//pConfig->RF.RF_Bandwidth		= DEFAULT_RF_BANDWIDTH;

	//pConfig->operationMode[0]	= INITIALIZE_NEW_CONFIGURATION; // Equipment changes automatically into STARTE_NORMALBETRIEB after initialization;
	//pConfig->operationMode[1]	= INITIALIZE_NEW_CONFIGURATION; // Equipment changes automatically into STARTE_NORMALBETRIEB after initialization;
	//pConfig->operationMode[2]	= INITIALIZE_NEW_CONFIGURATION; // Equipment changes automatically into STARTE_NORMALBETRIEB after initialization;

	pConfig->configLinkAlarmRelay 				= 0x000a0064; // delay 100 * 10ms and holdtime 1s	
	pConfig->configHWAlarmRelay 				= 0x000a0064; // delay 100 * 10ms and holdtime 1s	
	pConfig->R1BC_Config[0].setupAndHoldTime	= 0x000a0064; // delay 100 * 10ms and holdtime 1s
	pConfig->R1BC_Config[1].setupAndHoldTime	= 0x000a0064; // delay 100 * 10ms and holdtime 1s
	pConfig->R1BC_Config[2].setupAndHoldTime	= 0x000a0064; // delay 100 * 10ms and holdtime 1s
	
	pConfig->systemSettings		|= SYSTEM_SETTINGS_ENABLE_RX_FILTER_MONITORING;
	pConfig->systemSettings		|= SYSTEM_SETTINGS_ENABLE_EOC;
	pConfig->systemSettings		|= SYSTEM_SETTINGS_ENABLE_AFC;
	pConfig->systemSettings		&= ~SYSTEM_SETTINGS_ENABLE_IRIG_B;
	pConfig->systemSettings		|= SYSTEM_SETTINGS_LINE_IMP_75_OHM;

	pConfig->hwSettings.powerSupplyVoltage |= DEFAULT_POWER_SUPPLY_VOLTAGE & PS_VOLTAGE_MASK;
	pConfig->hwSettings.powerSupplyVoltage &= ~PS_REDUNDANT_MODULE_PLUGGED;

	pConfig->hwSettings.hybridAndP4RX_Info |= P4RX_PLUGGED;

	pConfig->hwSettings.R1BCMode |= DEFAULT_R1BC_MODE_RELAY1;
	pConfig->hwSettings.R1BCMode |= DEFAULT_R1BC_MODE_RELAY2;
	pConfig->hwSettings.R1BCMode |= DEFAULT_R1BC_MODE_RELAY3;
	pConfig->hwSettings.R1BCMode |= DEFAULT_R1BC_MODE_RELAY4;
	pConfig->hwSettings.R1BCMode |= DEFAULT_R1BC_MODE_RELAY5;
	pConfig->hwSettings.R1BCMode |= DEFAULT_R1BC_MODE_RELAY6;
	pConfig->hwSettings.R1BCMode |= DEFAULT_R1BC_MODE_RELAY7;
	pConfig->hwSettings.R1BCMode |= DEFAULT_R1BC_MODE_RELAY8;

	pConfig->hwSettings.txFilterBandwidthAndCoilEndStop |= DEFAULT_TX_COIL1_POS;
	pConfig->hwSettings.txFilterBandwidthAndCoilEndStop |= DEFAULT_TX_COIL2_POS;
	pConfig->hwSettings.txFilterBandwidthAndCoilEndStop |= DEFAULT_TX_FILTER_SEL;
	pConfig->hwSettings.rxFilterBandwidth				|= DEFAULT_RX_FILTER_SEL;

}



/***************************************************************************************************
Function:		void InitDefaultConfigP4LTAplc(t_APLC_Config* pConfig)
Description:	Initializes the APLC part of the P4LT configuration 
				to default values
Inputs:			None
Outputs:		pP4LTConfig* pConfig		ptr to a pP4LTConfig data struct that will be initialized
Return:			None
Globals:		
Important:		pConfig will be modified
***************************************************************************************************/
void SEGMENT_EXT_CODE InitDefaultConfigP4LTAplc(t_P4LT_Config* pP4LTConfig)
{
	t_APLC_Config*	pConfig	= &(pP4LTConfig->APLC);

	// Initialize member "t_APLC_Config APLC" of pConfig
	pConfig->pilot[0].frequency				  =	DEFAULT_PILOT_FREQUENCY_40;                                                 
	pConfig->pilot[1].frequency 			  = DEFAULT_PILOT_FREQUENCY_00;
	pConfig->pilot[2].frequency 			  = DEFAULT_PILOT_FREQUENCY_00;	

	pConfig->pilot[0].snrAlarmThreshold_dB 	  = DEFAULT_SNR_ALARM_THRESHOLD_IN_DB;
	pConfig->pilot[1].snrAlarmThreshold_dB 	  = DEFAULT_SNR_ALARM_THRESHOLD_IN_DB;
	pConfig->pilot[2].snrAlarmThreshold_dB 	  = DEFAULT_SNR_ALARM_THRESHOLD_IN_DB;
	// use defines inst. of calculation pConfig->pilot[0].snrAlarmThresholdLin 	  = (float)(pow((double)10.,(double)pConfig->pilot[0].snrAlarmThreshold_dB/10));
	pConfig->pilot[0].snrAlarmThresholdLin 	  = (float)DEFAULT_SNR_ALARM_THRESHOLD_LIN;
	pConfig->pilot[1].snrAlarmThresholdLin 	  = (float)DEFAULT_SNR_ALARM_THRESHOLD_LIN;	
	pConfig->pilot[2].snrAlarmThresholdLin 	  = (float)DEFAULT_SNR_ALARM_THRESHOLD_LIN;	

	pConfig->pilot[0].levelAlarmThreshold_dB  = DEFAULT_LEVEL_ALARM_TRESHOLD_IN_DB;
	pConfig->pilot[1].levelAlarmThreshold_dB  = DEFAULT_LEVEL_ALARM_TRESHOLD_IN_DB;
	pConfig->pilot[2].levelAlarmThreshold_dB  = DEFAULT_LEVEL_ALARM_TRESHOLD_IN_DB;
	// use defines inst. of calculation pConfig->pilot[0].levelAlarmThresholdLin  = (float)(pow((double)10.,(double)pConfig->pilot[0].levelAlarmThreshold_dB/10))/2; // da nur i oder q Kanal ausgewertet wird 
	pConfig->pilot[0].levelAlarmThresholdLin  = (float)DEFAULT_LEVEL_ALARM_TRESHOLD_LIN; 
	pConfig->pilot[1].levelAlarmThresholdLin  = (float)DEFAULT_LEVEL_ALARM_TRESHOLD_LIN; 
	pConfig->pilot[2].levelAlarmThresholdLin  = (float)DEFAULT_LEVEL_ALARM_TRESHOLD_LIN;
    
	//use defines inst. of calculation pConfig->pilot[0].pilotLevelLowThreshold  = (float)(pow( 10.0,(double)((float)DEFAULT_PILOT_LEVEL_ALARM_TRESHOLD_IN_DB/20.0F)));
 	pConfig->pilot[0].pilotLevelLowThreshold  = (float)DEFAULT_PILOT_LEVEL_ALARM_TRESHOLD_LIN;
    pConfig->pilot[1].pilotLevelLowThreshold  = (float)DEFAULT_PILOT_LEVEL_ALARM_TRESHOLD_LIN;
	pConfig->pilot[2].pilotLevelLowThreshold  = (float)DEFAULT_PILOT_LEVEL_ALARM_TRESHOLD_LIN;

	pConfig->equalizer[0].equalizerSettings	= 0; // Standard equalizer
	pConfig->equalizer[1].equalizerSettings	= 0; // Standard equalizer
	pConfig->equalizer[2].equalizerSettings	= 0; // Standard equalizer
		
	/*
	pConfig->testTone[0].frequency	= DEFAULT_TESTTONE_FREQUENCY_IN_HZ;
	pConfig->testTone[1].frequency	= DEFAULT_TESTTONE_FREQUENCY_IN_HZ;
	pConfig->testTone[2].frequency	= DEFAULT_TESTTONE_FREQUENCY_IN_HZ;	

	pConfig->testTone[0].level_dB	= DEFAULT_TESTTONE_LEVEL_IN_DB;
	pConfig->testTone[1].level_dB	= DEFAULT_TESTTONE_LEVEL_IN_DB;
	pConfig->testTone[2].level_dB	= DEFAULT_TESTTONE_LEVEL_IN_DB;	

	pConfig->testTone[0].levelLin	= (float)DEFAULT_TESTTONE_LEVEL_LIN;
	pConfig->testTone[1].levelLin	= (float)DEFAULT_TESTTONE_LEVEL_LIN;
	pConfig->testTone[2].levelLin	= (float)DEFAULT_TESTTONE_LEVEL_LIN;
	*/

	pConfig->rxAFLevelAdjust[0]		= (float)1; // 0dB == amplification 1
	pConfig->rxAFLevelAdjust[1]		= (float)1; // 0dB == amplification 1;
	pConfig->rxAFLevelAdjust[2]		= (float)1; // 0dB == amplification 1;

	pConfig->channelMode			= DEFAULT_APLC_CHANNEL_MODE;
	
	// Allpass filter for group delay equalization is set to disabled as default
	// for all APLC channels with ETL600 Rel. 3.01.x.
	pConfig->apFilterFlags			= 0;
	pConfig->apFilterFlags			&= (DEFAULT_AP_FILTER_CH1 & DEFAULT_AP_FILTER_CH2 & DEFAULT_AP_FILTER_CH3);	

}



/***************************************************************************************************
Function:		void InitDefaultConfigP4LTOfdm(t_OFDM_Config* pConfig)
Description:	Initializes the APLC part of the P4LT configuration 
				to default values
Inputs:			None
Outputs:		pP4LTConfig* pConfig		ptr to a pP4LTConfig data struct that will be initialized
Return:			None
Globals:		
Important:		pConfig will be modified
***************************************************************************************************/
void SEGMENT_EXT_CODE InitDefaultConfigP4LTOfdm(t_P4LT_Config* pP4LTConfig)
{
	t_OFDM_Config*	pConfig	= &(pP4LTConfig->OFDM);

	// initialize member "t_OFDM_Config OFDM" of pConfig
	pConfig->channelBW		= DEFAULT_OFDM_CHANNEL_BW;
	pConfig->mode			= DEFAULT_OFDM_MODE;
	pConfig->weight			= DEFAULT_OFDM_WEIGHT;
	pConfig->dataRate[0]	= DEFAULT_OFDM_DATARATE_0;
	pConfig->dataRate[1]	= DEFAULT_OFDM_DATARATE_1;
	pConfig->dataRate[2]	= DEFAULT_OFDM_DATARATE_2;
	pConfig->dataRate[3]	= DEFAULT_OFDM_DATARATE_3;
	pConfig->dataRate[4]	= DEFAULT_OFDM_DATARATE_4;

}



/***************************************************************************************************
Function:		void InitDefaultConfigP4LTNsk(t_NSK_Config* pConfig)
Description:	Initializes the NSK part of the P4LT configuration 
				to default values
Inputs:			None
Outputs:		pP4LTConfig* pConfig		ptr to a pP4LTConfig data struct that will be initialized
Return:			None
Globals:		
Important:		pConfig will be modified
***************************************************************************************************/
void SEGMENT_EXT_CODE InitDefaultConfigP4LTNsk(t_P4LT_Config* pP4LTConfig)
{
	int i;

	t_NSK_Config*	pConfig	= &(pP4LTConfig->NSK);

	// initialize member "t_NSK_Config NSK" of pConfig
	for(i=0; i < N_NSK_MODEMS; i++)
	{
		pConfig->modem[i].centerFrequency	= DEFAULT_NSK600_RX_CFREQ + (DEFAULT_NSK600_TX_CFREQ << 16);			
		pConfig->modem[i].mode				= DEFAULT_MODEM_MODE;	
		pConfig->modem[i].bw_txDataDelay	= DEFAULT_NSK600_TX_DATA_DELAY + (DEFAULT_NSK600_BW << 16 );
	}

}



/***************************************************************************************************
Function:		void InitDefaultConfigP4LTMux(t_P4LT_Config* pConfig)
Description:	Initializes the NSK part of the P4LT configuration 
				to default values
Inputs:			None
Outputs:		pP4LTConfig* pConfig		ptr to a pP4LTConfig data struct that will be initialized
Return:			None
Globals:		
Important:		pConfig will be modified
***************************************************************************************************/
void SEGMENT_EXT_CODE InitDefaultConfigP4LTMux(t_P4LT_Config* pP4LTConfig)
{
	int i;
	int iPort;
	int iCh;
	int iDataRate;

	t_Mux_Cfg*	pConfig	= &(pP4LTConfig->muxCfg);

	// Initialize MUX config
	// Port interface extension module not available
	pConfig->control &= (~MUX_CFG_CONTROL_PORTEXT_ENABLE);

	// Mux and V.24 port sharing device not available
	pConfig->control &= (~MUX_CFG_CONTROL_MUX_ENABLE);
	pConfig->control &= (~MUX_CFG_CONTROL_PSD_ENABLE);

	// Initialize V.24 ports
	for(i = 0; i < NUMBER_OF_V24_PORTS_P4LU; i++)
	{
		pConfig->ports.V24[i].datarate	= DEFAULT_NSK600_BITRATE;
		pConfig->ports.V24[i].control   = DEFAULT_V24PORT_MODE;

	}
	
	// Initialize V.11 ports.	
	for(iPort = 0; iPort < NUMBER_OF_V11_PORTS_P4LU; iPort++)
	{
		// Initialize V.11 ports for each profile.
		for(iDataRate=0; iDataRate < N_DATA_RATES; iDataRate++)
		{		
			// Set data rate of V.11 port to 0.
			pConfig->rate[iDataRate].V11_Rate[iPort] = 0;
		}
	}

	// Initialize O4CV channels
	for(iCh = 0; iCh < N_O4CV_CHANNELS; iCh++)
	{
		pConfig->voiceChannel[iCh] = 0; // channel off, all data rates = 0;
	}
		
}


/***************************************************************************************************
Function:		void InitDefaultConfigO4CV(t_P4LT_Config* pConfig)
Description:	Initializes the O4CV part of the P4LT configuration 
				to default values
Inputs:			None
Outputs:		pP4LTConfig* pConfig		ptr to a pP4LTConfig data struct that will be initialized
Return:			None
Globals:		
Important:		pConfig will be modified
***************************************************************************************************/
void SEGMENT_EXT_CODE InitDefaultConfigO4CV(t_P4LT_Config* pP4LTConfig)
{
	pP4LTConfig->O4CVCfg.numbersOfBoardsAndChs = 0;
	pP4LTConfig->O4CVCfg.numbersOfAFInterfaces = 0;
	pP4LTConfig->O4CVCfg.settings = 0;

	//Default DataRate
	SET_BITS(TRUE, pP4LTConfig->O4CVCfg.settings, (CH_VOICE_DR_5300 << GET_SET_LSB(O4CV_VOICE_DATA_RATE_MASK)));
    //Service Phone Off
	SET_BITS(TRUE, pP4LTConfig->O4CVCfg.settings, (O4CV_SERVICE_PHONE_OFF << GET_SET_LSB(O4CV_SERVICE_PHONE_MODE_MASK)));
   

}

/***************************************************************************************************
Function:		void InitDefaultConfigO4CV(t_P4LT_Config* pConfig)
Description:	Initializes the O4CV part of the P4LT configuration 
				to default values
Inputs:			None
Outputs:		pP4LTConfig* pConfig		ptr to a pP4LTConfig data struct that will be initialized
Return:			None
Globals:		
Important:		pConfig will be modified
***************************************************************************************************/
void SEGMENT_EXT_CODE InitDefaultConfigP4LTNskEqualizer(t_P4LT_Config* pP4LTConfig)
{

	int i;

	t_NSK_EqualizerCfg*	pConfig	= &(pP4LTConfig->NSKEqualizerCfg);

	// initialize member "t_NSK_EqualizerCfg NSKEqualizerCfg" of pConfig
	for(i=0; i < N_NSK_MODEMS; i++)
	{
		pConfig->equalizer[i].mode = DEFAULT_NSK_EQUALIZER_MODE;
		pConfig->equalizer[i].settings = DEFAULT_NSK_EQUALIZER_SETTINGS;
		pConfig->equalizer[i].operatingRange = DEFAULT_NSK_EQUALIZER_OPERATING_RANGE;
		pConfig->equalizer[i].reservedL1 = 0;
	}

}

/***************************************************************************************************
Function   : void InitDefaultConfigR1LA(t_R1LA_Config* pR1LA_Config)
Description: This function initializes the default configuration. This is used in the HMI if the "New"
			 command is pressed and on the R1LA if the active or temporary configuration could not be read.
Inputs	   :
Outputs	   :
Return	   :
Globals	   :
Important  : This function utilizes data types which are not 32 bits long (Example: unsigned char or short int).
			 Do not use this function if your HW does not support this!

Remark     : Line commented out not needed because the entire strcuture is zeroed-out at the beginning
***************************************************************************************************/
void InitDefaultConfigR1LA(t_R1LA_Config* pR1LA_Config)
{
	int ix;

	memset( (void*) pR1LA_Config, 0, sizeof(t_R1LA_Config) );

	// Defaults for t_R1LA_Config
	//pR1LA_Config->configVersion = 0;
	pR1LA_Config->serviceFlags  = R1LA_SERVICES_OFF;

	// Defaults for t_R1LA_Common_Config
	pR1LA_Config->commonConfig.ipAddress      = 0xC0A80002; // 192.168.0.2
	pR1LA_Config->commonConfig.defaultGateway = 0xC0A80001; // 192.168.0.1
	pR1LA_Config->commonConfig.subnetMask     = 16;         // 255.255.0.0
	//pR1LA_Config->commonConfig.ETLaddress     = 0;
	pR1LA_Config->commonConfig.serialBaudRate = DEFAULT_HMI_BAUDRATE;

	// Defaults for t_R1LA_BridgingIpRouting_Config
	//memset( (void*) pR1LA_Config->bridgingIpRoutingConfig.destIpAddresses, 0, R1LA_MAX_NOF_IP_ROUTES*sizeof(unsigned long) );
	//memset( (void*) pR1LA_Config->bridgingIpRoutingConfig.nextGateways,    0, R1LA_MAX_NOF_IP_ROUTES*sizeof(unsigned long) );
	//memset( (void*) pR1LA_Config->bridgingIpRoutingConfig.destSubnetMasks, 0, R1LA_MAX_NOF_IP_ROUTES*sizeof(unsigned char) );
	//pR1LA_Config->bridgingIpRoutingConfig.isMaster = 0;
	pR1LA_Config->bridgingIpRoutingConfig.mode     = R1LA_LANCONN_ROUTER_FORWARD_ALL_MODE;
	pR1LA_Config->bridgingIpRoutingConfig.options  = DEFAULT_R1LA_LANCONN_OPT_HC;

	// Defaults for t_R1LA_Snmp_Config
	//memset( (void*) pR1LA_Config->snmpConfig.alarmPollingMngmtStationIps, 0, R1LA_MAX_NOF_MNGMT_STATIONS*sizeof(unsigned long) );
	//memset( (void*) pR1LA_Config->snmpConfig.alarmPollingList, 0, R1LA_MAX_NOF_ALARM_POLLING_DEVICES*sizeof(unsigned short int) );
	
	for (ix = 0; ix < R1LA_MAX_NOF_MNGMT_STATIONS; ix++)
	{
		pR1LA_Config->snmpConfigEx.alarmPollingMngmtStationPorts[ix] = SNMP_TRAP_DESTINATION_PORT_DEFAULT;
	}

	R1LA_ALARM_POLLING_SET_MODE(pR1LA_Config->snmpConfig.alarmPollingInterval,     R1LA_ALARM_POLLING_MODE_EVERY_X_MINUTES);
	R1LA_ALARM_POLLING_SET_MINUTE(pR1LA_Config->snmpConfig.alarmPollingInterval,   0);
	R1LA_ALARM_POLLING_SET_HOUR(pR1LA_Config->snmpConfig.alarmPollingInterval,	   0);
	R1LA_ALARM_POLLING_SET_INTERVAL(pR1LA_Config->snmpConfig.alarmPollingInterval, 5);

	// Defaults for t_R1LA_HmiOverLan_Config
	pR1LA_Config->hmiOverLanConfig.tcpPort = 5000;
	//memset( (void*) pR1LA_Config->hmiOverLanConfig.subnetMasks, 0, R1LA_MAX_NOF_ALLOWED_CLIENTS*sizeof(unsigned char) );
	//memset( (void*) pR1LA_Config->hmiOverLanConfig.ipAddresses, 0, R1LA_MAX_NOF_ALLOWED_CLIENTS*sizeof(unsigned long) );
	pR1LA_Config->hmiOverLanConfig.featureFlags = R1LA_HMI_OVER_LAN_FEATURES_OFF;
}
/***************************************************************************************************
Function   :t_NSDExtCustomTableFileEntry SEGMENT_EXT_CODE getTableEntryFromCustomTableFile(
																	unsigned long id,
																	t_NSDExtCustomTableFile* tblFile)
Description: This function figures out which TabelEntry belongs to an 
			 id inside a given TableFileStruct
Inputs	   : unsigned long id:					the TableEntry id to find  
Inputs	   : t_NSDExtCustomTableFile* tblFile : a pointer to the TableFileStruct where the Tableentry id
												will be searched in
Outputs	   :						
Return	   : t_NSDExtCustomTableFileEntry tblResultEntry :  TableFileEntry with the input id
															
Globals	   :
Important  :if no tblEntry is found a tableEntry with id=0 will be given back to the caller  
			used inside the NSD600 Extended Configuration
***************************************************************************************************/
SEGMENT_EXT_CODE t_NSDExtCustomTableFileEntry getTableEntryFromCustomTableFile(unsigned long id,t_NSDExtCustomTableFile* tblFile)
{

	t_NSDExtCustomTableFileEntry tblResultEntry; //Result
	unsigned long i; //Schleifenlaufvariable

	//default for the result TableEntry, if nothing was found
	tblResultEntry.id = 0;

	if(tblFile != NULL)
	{

		//Loop over all filestruct entries
		for(i=0;i< tblFile->nrOfEntries;i++)
		{
			t_NSDExtCustomTableFileEntry tblTempEntry = tblFile->entries[i];
        
			//Compare each id with the id to find
			if(tblTempEntry.id == id)
			{
				tblResultEntry = tblTempEntry;
    			break;  
			}
		}
	}
	return tblResultEntry;  
}
