/*-------------------------------------------------------------------------*
 * $Workfile	: msg_O4CV.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Roger Biffiger (RBi)
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/msg_O4CV.h $
 * 
 * 4     07-09-12 15:00 Chalnot
 * Emergency firmware download messages added.
 * 
 * 3     18.04.07 14:32 Chrobif
 * Some message types added and some modified.
 * 
 * 2     2.04.07 11:03 Chrobif
 * New line added at the end of file to avoid compiler warning.
 * 
 * 1     29.03.07 16:24 Chrobif
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef MSG_O4CV_H
#define MSG_O4CV_H

/* INCLUDE FILES **************************************************************/
/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

// Messages which the O4CV should ignore
#define MSG_O4CV_MESSAGE_TYPE_NONE				0x0000

// Messages for developer functions
// Start at 0x5000

// Messages for client functions
#define MSG_O4CV_DOWNLOAD_CONFIG_DATA			0x5100
#define MSG_O4CV_STORE_CONFIG_DATA_TO_FLASH		0x5101
#define MSG_O4CV_UPLOAD_CONFIG_DATA				0x5102
#define MSG_O4CV_UPLOAD_STATUS_DATA				0x5103
#define MSG_O4CV_FW_UPDATE						0x5104
#define MSG_O4CV_ERASE_FLASH					0x5105
#define MSG_O4CV_PROG_FLASH_IN_BACKGROUND		0x5106
#define MSG_O4CV_END_OF_PROG_FLASH				0x5107
#define	MSG_O4CV_GET_EPROM_ACTUAL_STATUS		0x5108
#define MSG_O4CV_EFW_ERASE_FLASH				0x5109
#define MSG_O4CV_EFW_PROG_FLASH_IN_BACKGROUND	0x510A

/* PUBLIC TYPE DECLARATIONS ***************************************************/
/* PUBLIC MACRO DEFINITIONS ***************************************************/
/* PUBLIC FUNCTIONS ***********************************************************/

#endif
