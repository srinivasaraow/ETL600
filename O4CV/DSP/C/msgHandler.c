/*-------------------------------------------------------------------------*
 * $Workfile	: msgHandler.c  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Roger Biffiger (RBi)
 * Remarks      :  
 * Purpose      : The msgHandler module executes the messages from the HMI600
 *				  and sets up the responses.
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/msgHandler.c $
 * 
 * 7     08-01-10 10:40 Chrobif
 * Some comments modified.
 * 
 * 6     07-11-20 17:03 Chalnot
 * Licence code is initialized with LICENCE_DEF_O4CV_INVALID_LICENSE.
 * 
 * 5     07-10-05 11:22 Chrobif
 * Hardware version and HMI600 compatibility index modified.
 * 
 * 4     07-09-24 13:13 Chalnot
 * Em Fw with compiler switch EMERGENCY_FW implemented.
 * 
 * 3     17.09.07 13:51 Chhamaa
 * Function msgHandler_getNewCfgReceivedFlag() added.
 * 
 * 2     07-09-12 14:53 Chalnot
 * Emergency firmware download messages added.
 * 
 * 1     25.05.07 17:14 Chrobif
 * Initial version.
 *
 *-------------------------------------------------------------------------*/
 
/* INCLUDE FILES *************************************************************/

// C Run-Time Library
#include <string.h>
#include <assert.h>

// O4CV firmware
#include "msgHandler.h"
#include "ft12.h"
#include "msg_O4CV.h"
#include "O4CV_Types.h"
#include "Basetype_O4CV.h"
#include "O4CV_ConstDsp.h"
#include "epromHigh.h"
#include "epromMed.h"
#include "configuration.h"
#include "status.h"
#include "dll2ndStation.h"
#include "memoryCheck.h"
#include "licenceCheck_O4CV.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/
/* LOCAL TYPE DECLARATIONS ***************************************************/
/* LOCAL VARIABLE DECLARATIONS ***********************************************/
static t_EquipmentSWID_O4CV gs_equipmentSWIdentification;
static t_O4CVConfig gs_tempConfig;
static bool gs_newCfgReceived;

/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL FUNCTION DEFINITIONS ************************************************/
/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		void msgHandler_init()
  Description:	msgHandler_init() initializes the msgHandler module. 
  Inputs:
  Outputs:
  Return:       	
  Globals:
  Important:	This function must be called before any other function of this
  				module is called.
  Comments:	(1)	sizeof(O4CV_SW_IDENTIFICATION) = # of characters in
  				O4CV_SW_IDENTIFICATION + 1 (zero terminated)
*******************************************************************************/
void msgHandler_init()
{
    UINT8 fwVersion[ sizeof(O4CV_SW_IDENTIFICATION) ] = O4CV_SW_IDENTIFICATION; // => Comments (1)
    UINT8 eFwVersion[ sizeof(O4CV_EFW_SW_IDENTIFICATION) ] = O4CV_EFW_SW_IDENTIFICATION; 
	UINT32* pTemp;
	UINT32 temp;
	UINT16 i;

	memset( &gs_equipmentSWIdentification, 0, sizeof(gs_equipmentSWIdentification) );		
	
	////////////////////////////////////////////////////////////
	// Initialize the members of gs_equipmentSWIdentification //
	// which are determined at compilation time               //
	////////////////////////////////////////////////////////////
	
	// License code
	gs_equipmentSWIdentification.licenseCode = LICENCE_DEF_O4CV_INVALID_LICENSE;
	
#ifndef EMERGENCY_FW

	// Software identification
	pTemp = (UINT32*) &gs_equipmentSWIdentification.swIdentification;
	for (i=0; i<sizeof(O4CV_SW_IDENTIFICATION); i++) // => Comments (1)
	{
	    if ( i >= O4CV_SW_ID_LENGTH * NR_OF_BYTES_IN_LONG )
	    	break; // Avoid that we write beyond the swIdentification member
	    
	    temp = (UINT32) fwVersion[i];
	    switch (i%4)
	    {
	    case 0:
	    	if (i!=0) pTemp++;
	    	break;
	    case 1:
	    	temp = temp << 8;
	    	break;
	    case 2:
	    	temp = temp << 16;
	    	break;
	    case 3:
	    	temp = temp << 24;
	    	break;
	    }
	    *pTemp |= temp;
	}
	
	// MMI compatibility version
	gs_equipmentSWIdentification.mmiCompatibilityVersion = O4CV_MMI_COMPATIBILITY_VERSION;
	
	// HW version
	gs_equipmentSWIdentification.hwVersion = O4CV_HW_VERSION;
	
	// reset new configuration flag
	gs_newCfgReceived = false;
	
#else

	// Software identification
	pTemp = (UINT32*) &gs_equipmentSWIdentification.swIdentification;
	for (i=0; i<sizeof(O4CV_EFW_SW_IDENTIFICATION); i++) 
	{
	    if ( i >= O4CV_SW_ID_LENGTH * NR_OF_BYTES_IN_LONG )
	    	break; // Avoid that we write beyond the swIdentification member
	    
	    temp = (UINT32) eFwVersion[i];
	    switch (i%4)
	    {
	    case 0:
	    	if (i!=0) pTemp++;
	    	break;
	    case 1:
	    	temp = temp << 8;
	    	break;
	    case 2:
	    	temp = temp << 16;
	    	break;
	    case 3:
	    	temp = temp << 24;
	    	break;
	    }
	    *pTemp |= temp;
	}
	
	// MMI compatibility version
	gs_equipmentSWIdentification.mmiCompatibilityVersion = O4CV_EFW_MMI_COMPATIBILITY_VERSION;
	
	// Equipment state info
	gs_equipmentSWIdentification.equipmentStateInfo |= O4CV_EMERGENCY_FW_RUNNING;
	
	// HW version
	gs_equipmentSWIdentification.hwVersion = O4CV_EFW_HW_VERSION;
	
#endif	

}

/*****************************************************************************
  Function:		void msgHandler_setSWIDEqState(INT32 state)
  Description:	msgHandler_setSWIDEqState() is a setter to set the equipment
  				state in the t_EquipmentSWID_O4CV data structure.
  Inputs:		state: Equipment state as defined in O4CV_Types.h
  Outputs:
  Return:       	
  Globals:		gs_equipmentSWIdentification
  Important:                                      	
*******************************************************************************/
void msgHandler_setSWIDEqState(INT32 state)
{
    gs_equipmentSWIdentification.equipmentStateInfo = state;
}

/*****************************************************************************
  Function:		void msgHandler_setLicenseCode(INT32 licenseCode)
  Description:	msgHandler_setLicenseCode() is a setter to set the current license
  				code on iButton in the t_EquipmentSWID_O4CV data structure.
  Inputs:		licenseCode: current license code on iButton
  Outputs:
  Return:       	
  Globals:		gs_equipmentSWIdentification
  Important:                                      	
*******************************************************************************/
void msgHandler_setLicenseCode(INT32 licenseCode)
{
    gs_equipmentSWIdentification.licenseCode = licenseCode;
}

/*****************************************************************************
  Function:		void msgHandler_respondEqId(void)
  Description:	msgHandler_respondEqId() sets up the response to report the
  				equipment identification.
  Inputs:
  Outputs:
  Return:       	
  Globals:		gs_equipmentSWIdentification
  Important:                                      	
*******************************************************************************/
void msgHandler_respondEqId() 
{	
	dll2ndStation_setupTxUDPtrs( (UINT32*) &gs_equipmentSWIdentification,
								 sizeof(gs_equipmentSWIdentification) / NR_OF_BYTES_IN_LONG );
}

/*****************************************************************************
  Function:		bool msgHandler_getNewCfgReceivedFlag(void)
  Description:	Returns the state of the new configuration received flag.
  Inputs:
  Outputs:
  Return:		true if new configuration received, else false      	
  Globals:		gs_newCfgReceived
  Important:                                      	
*******************************************************************************/
bool msgHandler_getNewCfgReceivedFlag(void)
{
	if (gs_newCfgReceived)
	{
	    gs_newCfgReceived = false;
	    return(true);
	}
	else
	{
	    return(false);
	}   
}

/*****************************************************************************
  Function:		void msgHandler_executeMsg(UINT16 msgType, UINT32* PtrToRxUD)
  Description:	msgHandler_executeMsg() executes a message from the HMI600 
  				and sets up the response.
  Inputs:		msgType:	message type of the FT1.2 frame
				PtrToRxUD:	PtrToRxUD points to the first 32 bit word user data
							in the received FT1.2 frame
  Outputs:
  Return:       	
  Globals:		gs_tempConfig, gs_newCfgReceived
  Important:                                      	
*******************************************************************************/
void msgHandler_executeMsg(UINT16 msgType, UINT32* PtrToRxUD)
{
	static UINT32 messageNACK = 'F';
	static UINT32 messageACK  = 'O';
	static UINT32 messageEmpty    = 0;
	static UINT32 messageNotEmpty = 1;

	switch(msgType)
    {

#ifndef EMERGENCY_FW
                
		case MSG_O4CV_DOWNLOAD_CONFIG_DATA:
		{
		    //////////////////////////////////////////////////////////////////////////////////////
		    // If this message is received, a chunk of configuration data is written to the
		    // temporary configuration. If the last chunk was received, the configuration is written
		    // to the configuration module.
		    //////////////////////////////////////////////////////////////////////////////////////
			UINT16 chunkSizeInBytes, chunkOffsetInBytes;
			UINT8  *pSrc, *pDst;
			
			chunkSizeInBytes	= *PtrToRxUD++ * NR_OF_BYTES_IN_LONG;
			chunkOffsetInBytes 	= *PtrToRxUD++ * NR_OF_BYTES_IN_LONG;
		
			pSrc = (UINT8*) PtrToRxUD;
			pDst = (UINT8*) &gs_tempConfig + chunkOffsetInBytes;
		
			// Ensure that we do not write beyond config structure
			if ( (pDst + chunkSizeInBytes) > ( (UINT8*) &gs_tempConfig + sizeof(gs_tempConfig)) )
				chunkSizeInBytes = (UINT8*) &gs_tempConfig + sizeof(gs_tempConfig) - pDst;
		
			memcpy(pDst, pSrc, chunkSizeInBytes);
		
			dll2ndStation_setupTxUDPtrs( NULL, 0);
		
			// Write the received configuration to the configuration module if we have received the last chunk
			if ( (pDst + chunkSizeInBytes) == ( (UINT8*) &gs_tempConfig + sizeof(gs_tempConfig)) )
			{
				configuration_set(&gs_tempConfig);
				gs_newCfgReceived = true;
				status_setWarning(O4CV_WARNING_TEMP_CONFIGURATION_ACTIVE);
			}
		
			break;
		}
	
		case MSG_O4CV_STORE_CONFIG_DATA_TO_FLASH:
		{
		    //////////////////////////////////////////////////////////////////////////////////////
		    // If this message is received the configuration is retrieved from the
		    // configuration module and stored to eprom.
		    // Storing the configuration to eprom is slow. epromHigh_storeConfiguration()
		    // merely puts a command on the epromHigh command stack and returns. The actual store is
		    // done chunk by chunk on successive calls to epromMed_activate().
		    // The HMI600 send a series of MSG_O4CV_GET_EPROM_ACTUAL_STATUS messages to poll the
		    // completion of the process.
		    //////////////////////////////////////////////////////////////////////////////////////
			t_O4CVConfig *pConfig;
		
			pConfig = configuration_getPCurrentConfig();
			epromHigh_storeConfiguration( (UINT16*) pConfig );
		
			dll2ndStation_setupTxUDPtrs( NULL, 0);
		
			status_clearWarning(O4CV_WARNING_TEMP_CONFIGURATION_ACTIVE);
		
			break;
		}
	
		case MSG_O4CV_UPLOAD_CONFIG_DATA:
		{
		    //////////////////////////////////////////////////////////////////////////////////////
		    // If this message is received a chunk configuration data is retrieved from the
		    // configuration module and is send to the HMI600.
		    //////////////////////////////////////////////////////////////////////////////////////
			UINT32 chunkSizeIn32BitWords, chunkOffsetIn32BitWords;
			t_O4CVConfig *pConfig;

			chunkSizeIn32BitWords	= *PtrToRxUD++;
			chunkOffsetIn32BitWords	= *PtrToRxUD;

			pConfig = configuration_getPCurrentConfig();
	    
			dll2ndStation_setupTxUDPtrs( (UINT32*) pConfig + chunkOffsetIn32BitWords, chunkSizeIn32BitWords);
		
			break;
		}
		
		case MSG_O4CV_UPLOAD_STATUS_DATA:
		{
		    //////////////////////////////////////////////////////////////////////////////////////
		    // If this message is received a chunk status data is retrieved from the
		    // status module and is send to the HMI600.
		    //////////////////////////////////////////////////////////////////////////////////////
			UINT32 chunkSizeIn32BitWords, chunkOffsetIn32BitWords;
			t_O4CVStatus *pStatus;
		
			chunkSizeIn32BitWords	= *PtrToRxUD++;
			chunkOffsetIn32BitWords = *PtrToRxUD;

			pStatus = status_getPCurrentStatus();
		
			dll2ndStation_setupTxUDPtrs( (UINT32*) pStatus + chunkOffsetIn32BitWords, chunkSizeIn32BitWords);		
		
			break;
		}
		
		case MSG_O4CV_EFW_ERASE_FLASH:
		{
		    //////////////////////////////////////////////////////////////////////////////////////
		    // If this message is received, the emergency firmware in the eprom is erased.
		    // Erasing the firmware in the eprom is slow. epromHigh_eraseEFW()
		    // merely puts a series of commands on the epromHigh command stack and returns.
		    // The actual erase is done chunk by chunk on successive calls to epromMed_activate().
		    // The HMI600 send a series of MSG_O4CV_GET_EPROM_ACTUAL_STATUS messages to poll the
		    // completion of the process.
		    //////////////////////////////////////////////////////////////////////////////////////
			epromHigh_eraseEFW();
		
			dll2ndStation_setupTxUDPtrs(&messageACK, 1);
		
			break;	
		}
			
		case MSG_O4CV_EFW_PROG_FLASH_IN_BACKGROUND:
		{
		    //////////////////////////////////////////////////////////////////////////////////////
		    // If this message is received, a chunk of emergency firmware is written to eprom.
		    // Writing firmware to eprom is slow. epromHigh_programEFWData()
		    // merely puts a command on the epromHigh command stack and returns.
		    // The actual writing is done chunk by chunk on successive calls to epromMed_activate().
		    // The HMI600 send a series of MSG_O4CV_GET_EPROM_ACTUAL_STATUS messages to poll the
		    // completion of the process.
		    //////////////////////////////////////////////////////////////////////////////////////
			UINT32 chunkSizeInBytes, chunkOffsetInBytes;
			
			chunkSizeInBytes   = *PtrToRxUD++;
			chunkOffsetInBytes = *PtrToRxUD++;	    

			assert( ((UINT32) PtrToRxUD & BIT0) == 0 ); // epromHigh_programEFWData requires that PtrToRxUD points to
														// an even address.
		
			if ( epromHigh_programEFWData(chunkSizeInBytes, chunkOffsetInBytes, (UINT16*) PtrToRxUD) )
				dll2ndStation_setupTxUDPtrs(&messageACK,  1); // Note: response is not evaluated in HMI600
			else
				dll2ndStation_setupTxUDPtrs(&messageNACK, 1); // Note: response is not evaluated in HMI600
			
			break;	
		}
		
#endif	
			
		case MSG_O4CV_FW_UPDATE:
		{
		    //////////////////////////////////////////////////////////////////////////////////////
		    // If this message is received, it is just confirmed. The pupose of this message is
		    // to prepare the O4CV for a firmware download. At present no preparation is necessary.
		    //////////////////////////////////////////////////////////////////////////////////////
			dll2ndStation_setupTxUDPtrs( NULL, 0);
		
			break;
		}
		
		case MSG_O4CV_ERASE_FLASH:
		{
		    //////////////////////////////////////////////////////////////////////////////////////
		    // If this message is received, the firmware in the eprom is erased.
		    // Erasing the firmware in the eprom is slow. epromHigh_eraseFirmware()
		    // merely puts a series of commands on the epromHigh command stack and returns.
		    // The actual erase is done chunk by chunk on successive calls to epromMed_activate().
		    // The HMI600 send a series of MSG_O4CV_GET_EPROM_ACTUAL_STATUS messages to poll the
		    // completion of the process.
		    //////////////////////////////////////////////////////////////////////////////////////
			epromHigh_eraseFirmware();
		
			dll2ndStation_setupTxUDPtrs(&messageACK, 1);
		
			break;
		}
				
		case MSG_O4CV_PROG_FLASH_IN_BACKGROUND:
		{
		    //////////////////////////////////////////////////////////////////////////////////////
		    // If this message is received, a chunk of firmware is written to eprom.
		    // Writing firmware to eprom is slow. epromHigh_programData()
		    // merely puts a command on the epromHigh command stack and returns.
		    // The actual writing is done chunk by chunk on successive calls to epromMed_activate().
		    // The HMI600 send a series of MSG_O4CV_GET_EPROM_ACTUAL_STATUS messages to poll the
		    // completion of the process.
		    //////////////////////////////////////////////////////////////////////////////////////
			UINT32 chunkSizeInBytes, chunkOffsetInBytes;
			
			chunkSizeInBytes   = *PtrToRxUD++;
			chunkOffsetInBytes = *PtrToRxUD++;	    

			assert( ((UINT32) PtrToRxUD & BIT0) == 0 ); // epromHigh_programData requires that PtrToRxUD points to
														// an even address.
		
			if ( epromHigh_programData(chunkSizeInBytes, chunkOffsetInBytes, (UINT16*) PtrToRxUD) )
				dll2ndStation_setupTxUDPtrs(&messageACK,  1); // Note: response is not evaluated in HMI600
			else
				dll2ndStation_setupTxUDPtrs(&messageNACK, 1); // Note: response is not evaluated in HMI600
			
			break;
		}
			
		case MSG_O4CV_END_OF_PROG_FLASH:
		{
		    //////////////////////////////////////////////////////////////////////////////////////
		    // If this message is received, it is just confirmed. The purpose of this message is
		    // to indicate completion of the firmware download.
		    //////////////////////////////////////////////////////////////////////////////////////
			dll2ndStation_setupTxUDPtrs( NULL, 0);
		
			memoryCheck_resetBootTries();
			
			break;
		}
		
		case MSG_O4CV_GET_EPROM_ACTUAL_STATUS:
		{
		    //////////////////////////////////////////////////////////////////////////////////////
		    // If this message is received, the status of the command stack in epromHigh is reported.
		    // The status reported is either empty or not empty.
		    //////////////////////////////////////////////////////////////////////////////////////
			if ( epromMed_commandsInStack() )
				dll2ndStation_setupTxUDPtrs(&messageNotEmpty, 1);
			else
				dll2ndStation_setupTxUDPtrs(&messageEmpty,    1);
			break;
		}
		
		default:
			break;
    }
}

