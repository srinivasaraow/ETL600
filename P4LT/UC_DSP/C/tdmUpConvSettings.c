/*-------------------------------------------------------------------------*
 * $Workfile: tdmUpConvSettings.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Functions to read and write up-converter settings from/to
				  internal TDM bus
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/Master_DSP/C/tdmUpConvSettings.c $ 
 * 
 * 1     16.02.05 14:39 Chhamaa
 * 
 * 1     11.02.05 18:36 Chhamaa
 * Initial version
 * 
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include "DIAG.H"
#include "tdmUpConvSettings.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define NUM_OF_RX_VALUES		10

#define MIN_RF_FREQUENCY		16000		// [Hz]
#define MAX_RF_FREQUENCY		1000000		// [Hz]	

#define CHECKSUM_INIT_VALUE		0x000000AA
#define MASK_BYTE0				0x000000FF
#define MASK_BYTE1				0x0000FF00
#define MASK_BYTE2				0x00FF0000
#define MASK_BYTE3				0xFF000000

#define SHIFT_R_BYTE1			8
#define SHIFT_R_BYTE2			16
#define SHIFT_R_BYTE3			24


/* LOCAL TYPE DECLARATIONS ***************************************************/

typedef enum {

	e_Waiting,
	e_Receiving,
	e_Testing,
	e_End
}
e_ReceiverStates;


typedef struct {

	t_UpConvSettings	currentSettings;
	t_UpConvSettings	newSettings;
	unsigned long		ulCurrentSettings;
	unsigned long		oldValue;
	e_ReceiverStates	state;
	long				rxCounter;

} t_Receiver;


/* LOCAL VARIABLES ***********************************************************/

t_UpConvSettings	g_toSend;
t_Receiver			g_receiver;


/* LOCAL FUNCTION DEFINITIONS ************************************************/

static void calcChecksum(t_UpConvSettings *upConvSettings);


/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		tdmUpConvSettings_initSender()	
  Description:	Initializes the function tdmUpConvSettings_send().
  Inputs:		frequency, channelMode
  Outputs:		
  Return:		
  Globals:		
  Important:	Call before using tdmUpConvSettings_send().	

*******************************************************************************/

void tdmUpConvSettings_initSender(long frequency, long channelMode)
{
	unsigned long temp;
	unsigned long checksum = CHECKSUM_INIT_VALUE;


	// initialize settings
	g_toSend.frequency = frequency;
	g_toSend.channelMode = channelMode;
	g_toSend.reserved = 0;

	//calculate checksum
	calcChecksum(&g_toSend);
}


/*****************************************************************************
  Function:		tdmUpConvSettings_send()	
  Description:	Sends the up-converter settings continuously to the UC DSP.
  Inputs:		Pointer to the corresponding TDM bus time slot.	
  Outputs:		
  Return:		
  Globals:		
  Important:	Call tdmUpConvSettings_initSender() before using this function.	

*******************************************************************************/

void tdmUpConvSettings_send(t_UpConvSettings *pTdmUpConvSettingsTx)
{

	pTdmUpConvSettingsTx->frequency = g_toSend.frequency;
	pTdmUpConvSettingsTx->channelMode = g_toSend.channelMode;
	pTdmUpConvSettingsTx->reserved = g_toSend.reserved;
	pTdmUpConvSettingsTx->checksum = g_toSend.checksum;

}


/*****************************************************************************
  Function:		tdmUpConvSettings_initReceiver()	
  Description:	Initializes the function tdmUpConvSettings_receive().
  Inputs:		
  Outputs:		
  Return:		
  Globals:		
  Important:	Call before using tdmUpConvSettings_receive().

*******************************************************************************/

void tdmUpConvSettings_initReceiver(long frequency, long channelMode)
{
	// initialize current settings
	g_receiver.currentSettings.frequency = frequency;
	g_receiver.currentSettings.channelMode = channelMode;
	g_receiver.currentSettings.reserved = 0;

	//calculate checksum
	calcChecksum(&g_receiver.currentSettings);

	g_receiver.ulCurrentSettings = *((unsigned long*)&g_receiver.currentSettings);

	// initialize receiver state machine
	g_receiver.state = e_Waiting;
	g_receiver.rxCounter = 0;
	g_receiver.oldValue = 0;
}


/*****************************************************************************
  Function:		tdmUpConvSettings_getNewSettingsPtr()
  Description:	Retruns the pointer to new up-converter settings.
  Inputs:		
  Outputs:				
  Return:		Pointer to t_UpConvSettings	
  Globals:		
  Important:	The new settings are only valid if the function
				tdmUpConvSettings_receive() returns true.

*******************************************************************************/

t_UpConvSettings *tdmUpConvSettings_getNewSettingsPtr(void)
{

	return(&g_receiver.newSettings);
}
 

/*****************************************************************************
  Function:		calcChecksum()	
  Description:	Calculates and appends the chechsum of t_UpConvSettings
  Inputs:		Pointer to upConvSettings
  Outputs:		
  Return:		
  Globals:		
  Important:	

*******************************************************************************/

static void calcChecksum(t_UpConvSettings *upConvSettings)
{
	unsigned long temp;
	unsigned long checksum = CHECKSUM_INIT_VALUE;


	temp = *((unsigned long*)upConvSettings);
	checksum += temp & MASK_BYTE0;
	checksum += (temp & MASK_BYTE1) >> SHIFT_R_BYTE1;
	checksum += (temp & MASK_BYTE2) >> SHIFT_R_BYTE2;

	upConvSettings->checksum = checksum;
}


/*****************************************************************************
  Function:		tdmUpConvSettings_receive()
  Description:	Receives and verifies new up-converter settings.
  Inputs:		Pointer to new TDM bus value
  Outputs:			
  Return:		rxFlag: True if new settings have been received.		
  Globals:		
  Important:	Call tdmUpConvSettings_initReceiver() before using this function.		

*******************************************************************************/

BOOL tdmUpConvSettings_receive(t_UpConvSettings *pTdmUpConvSettingsRx)
{
	BOOL rxFlag;
	register unsigned long newValue;
	register unsigned long temp;
	register unsigned long checksum;

	
	rxFlag = FALSE;
	newValue = *((unsigned long*)pTdmUpConvSettingsRx);

	switch(g_receiver.state) {

		case e_Waiting:
			
			if (newValue != g_receiver.ulCurrentSettings) {

				g_receiver.state = e_Receiving;
			};
			break;

		case e_Receiving:

			if (newValue == g_receiver.oldValue) {
				
				g_receiver.rxCounter++;

				if (g_receiver.rxCounter > NUM_OF_RX_VALUES) {
				
					*((unsigned long*)&g_receiver.newSettings) = newValue;
					g_receiver.rxCounter = 0;
					g_receiver.state = e_Testing;
				};
			}
			else {
				
				g_receiver.rxCounter = 0;
				g_receiver.state = e_Waiting;
			};

			break;

		case e_Testing:

			//calculate checksum
			temp = *((unsigned long*)&g_receiver.newSettings);
			checksum = CHECKSUM_INIT_VALUE;
			checksum += temp & MASK_BYTE0;
			checksum += (temp & MASK_BYTE1) >> SHIFT_R_BYTE1;
			checksum += (temp & MASK_BYTE2) >> SHIFT_R_BYTE2;

			if (g_receiver.newSettings.checksum == (checksum & MASK_BYTE0)) {

				g_receiver.state = e_End;
			}
			else {

				g_receiver.state = e_Waiting;
			};
			break;

		case e_End:		// this state has no exit, call tdmUpConvSettings_initReceiver() to reset

			rxFlag = TRUE;
			break;

		default: ASSERT(FALSE);

	};

	g_receiver.oldValue = newValue; 

	return(rxFlag);
}






