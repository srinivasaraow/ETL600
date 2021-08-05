/*---------------------------------------------------------------------------*
 * $Workfile: AD9833_Generator.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : Hans Peter Rytz PTUKT
 * Remarks      :
 * Purpose      : Generates the frequency for DummyLoad P3LK 
 * Copyright    : ABB Switzerland Ltd., 2004
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 $Log: /branches/Source R4/P4LT/Master_DSP/C/AD9833_Generator.c $
 * 
 * 11    11-09-30 18:16 Chhamaa
 * initAD9833_DDS(): frequency calculation added for RF inverted mode.
 * 
 * 10    11-03-14 15:55 Chhamaa
 * Bug fixed: calculation of fNominalBandwidth 
 * 
 * 9     11-03-11 18:10 Chhamaa
 * Modifications for R4.
 * 
 * 4     22.05.06 13:30 Chchlee
 * Output now set to square wave (Issue 332)
 * 
 * 1     19.01.05 15:47 Chchlee
 * 
 * 6     21.10.04 13:11 Rytz01
 * DDS generates now sine waves. All is tested and garbage is removed.
 * 
 * 5     20.10.04 13:54 Rytz01
 * Output of DDS is now a square wave
 * 
 * 4     19.10.04 17:27 Rytz01
 * now with config data
 * 
 * 3     18.10.04 17:46 Buhl01
 * 
 * 2     20.09.04 17:00 Rytz01
 * 
 * 1     15.09.04 14:37 Rytz01
 * Initial version
 *---------------------------------------------------------------------------*/

/* INCLUDE FILES *************************************************************/
#include <def21161.h>
asm("#include <def21161.h>");
#include "basetype.h"
#include "constant.h"
#include "P4LT_Types.h"
#include "spi_dsp.h"
#include "SPI_functions.h"

/* LOCAL TYPE DECLARATIONS ***************************************************/

	
/* LOCAL CONSTANT DECLARATIONS ***********************************************/
#define SIZE_OF_DDS_DATA_BUFFER		6			// Data buffer size for SPI transfers. If more registers 
												// should be written increase this value.
#define SIZE_OF_DDS_START_DATA		6			// must be <= than SIZE_OF_DDS_DATA_BUFFER
#define SIZE_OF_DDS_STOP_DATA		1			// must be <= than SIZE_OF_DDS_DATA_BUFFER

#define DDS_CONTROL_REGISTER		0x00000000  // D15 0, D14 0
#define DDS_FREQ0_REGISTER			0x00004000  // D15 0, D14 1
#define DDS_FREQ1_REGISTER			0x00008000  // D15 1, D14 0
#define DDS_PHASE0_REGISTER			0x0000C000  // D15 1, D14 1, D13 0 
#define DDS_PHASE1_REGISTER			0x0000E000  // D15 1, D14 1, D13 1

// Control register Bits
#define TWO_WORDS_IN_ONE_SHOT		0x00002000
#define FREQ_REG_HIGH_WORD			0x00001000
#define FREQ_1_SELECT				0x00000800
#define PHASE_1_SELECT				0x00000400
#define DDS_RESET					0x00000100
#define DDS_SLEEP1					0x00000080
#define DDS_SLEEP12					0x00000040
#define DDS_OPBITEN					0x00000020
#define DDS_DIV2					0x00000008
#define DDS_MODE					0x00000002

// Masks
#define FREQ_MASK_LSB				0x00003FFF
#define FREQ_MASK_MSB				0x0FFFC000

// HW-defines for AD9833
#define CLK_FREQ					16384000	// [Hz] must be changed if input frequency is changed!!!!
#define FACTOR_2_HOCH_28			268435456	// calculation factor

// 0Hz defines
#define FREQUENCY_LSB_0HZ			0x00000000
#define FREQUENCY_MSB_0HZ			0x00000000

/* LOCAL VARIABLE DECLARATIONS ***********************************************/

static unsigned long freq_lsb = 0;
static unsigned long freq_msb = 0;
static unsigned long dataForDDSbuffer[SIZE_OF_DDS_DATA_BUFFER];
static unsigned long equipmentType;
static BOOL	isAD9833aktivated;

/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL FUNCTION DEFINITIONS ************************************************/

void SEGMENT_EXT_CODE initAD9833_DDS(t_P4LT_Config * config)
{
	unsigned long	sendInitWord;
	float			frequency_2txfc = 0;
	long			calculatedFreqency = 0;

	equipmentType = config->system.equipmentType;
	
	sendInitWord = DDS_CONTROL_REGISTER | DDS_SLEEP12 | DDS_SLEEP1 | DDS_RESET;
	sendSPIbyte(sendInitWord, CS_SPI_DDS, SPI_SETTINGS_FOR_DDS);

	// This block generate the freqency register values for the AD9833 DDS Generator 
	// dependence on the RxRf- TxRf-frequencies on init time.
	// For a frequency calculation tool for AD9833 see analog homepage:
	// http://www.analog.com/Analog_Root/static/techSupport/designTools/interactiveTools/ad98334/ad9833.html#instructions
	// The Dummy Load P3LK needs the double frequency of the difference between tx and rx
	// at the singal 2TXFC. The 2RXFC is not needed and is set to ground on P4LT.

	if (equipmentType == EQ_TYPE_ETL600_R3)
	{
		frequency_2txfc = ((config->system.RF.txFrequency - config->system.RF.rxFrequency)*2);	//[Hz]
		if(frequency_2txfc < 0)
		{
			frequency_2txfc *= -1;					// only positive values are allowed
		}
	}
	else	// release 4
	{
		float fNominalBandwidth;
		float fAplcBandWidth;
		long  lAplcBandWidth;
		
		GET_APLC_BANDWIDTH(config->APLC,lAplcBandWidth);

		fAplcBandWidth = (float)lAplcBandWidth;

		// get APLC + DPLC bandwidth (Nominal bandwidth)
		fNominalBandwidth = (float)(lAplcBandWidth + config->OFDM.channelBW);

		if (config->system.RF.channelMode == RF_CHANNEL_MODE_ERECT)
		{
			frequency_2txfc = ((config->system.RF.txFrequency + config->system.RF.rxFrequency + fNominalBandwidth)*2);	//[Hz]
		}
		else
		{
			frequency_2txfc = ((config->system.RF.txFrequency + config->system.RF.rxFrequency - fNominalBandwidth)*2);	//[Hz]
		}
	}

	calculatedFreqency = (long)((frequency_2txfc/CLK_FREQ)*FACTOR_2_HOCH_28);
	freq_lsb = (calculatedFreqency & FREQ_MASK_LSB);
	freq_msb = ((calculatedFreqency & FREQ_MASK_MSB)>>SB14);

	isAD9833aktivated = FALSE;
}

void SEGMENT_EXT_CODE ActivateAD9833(void)
{
	if(!isAD9833aktivated)
	{
		// set up data
		dataForDDSbuffer[0] = DDS_CONTROL_REGISTER | DDS_RESET;
		dataForDDSbuffer[1] = DDS_FREQ0_REGISTER | freq_lsb;		// add here with "|" the lower 14 Bit frequency value (LSB)
		dataForDDSbuffer[2] = DDS_CONTROL_REGISTER | DDS_RESET | FREQ_REG_HIGH_WORD; // switch to  Frequency High Word transfer
		dataForDDSbuffer[3] = DDS_FREQ0_REGISTER | freq_msb;		// add here with "|" the higher 14 Bit frequency 0 value (MSB)
		dataForDDSbuffer[4] = DDS_PHASE0_REGISTER;					// add here with "|" the 12 Bit phase 0 value 

		if (equipmentType == EQ_TYPE_ETL600_R3)
		{
			// control register setting for square wave
			dataForDDSbuffer[5] = DDS_CONTROL_REGISTER | DDS_OPBITEN | DDS_DIV2; // add here the other settings of the control register, e.g.MODE, DIV2, ... RESET must be 0 to start DDS
		}
		else	// release 4
		{
			// control register setting for sin wave
			dataForDDSbuffer[5] = DDS_CONTROL_REGISTER;					// add here the other settings of the control register, e.g.MODE, DIV2, ... RESET must be 0 to start DDS
		}

		isAD9833aktivated = requestSendDataToDDS((unsigned long*)&dataForDDSbuffer, SIZE_OF_DDS_START_DATA);
	}
}

void SEGMENT_EXT_CODE DeActivateAD9833(void)
{
	if(isAD9833aktivated)
	{
		// reset DDS if P3LK is plugged out
		dataForDDSbuffer[0] = DDS_CONTROL_REGISTER | DDS_RESET | DDS_SLEEP1 | DDS_SLEEP12;		// add here the other settings of the control register, e.g.MODE, DIV2, ... RESET must be 0 to start DDS

		isAD9833aktivated = !requestSendDataToDDS((unsigned long*)&dataForDDSbuffer, SIZE_OF_DDS_STOP_DATA);
	}
}






