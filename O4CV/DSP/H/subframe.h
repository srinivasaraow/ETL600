/*-------------------------------------------------------------------------*
 * $Workfile	: subframe.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      : 
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/subframe.h $
 * 
 * 4     26.07.07 9:51 Chhamaa
 * define BUFFER_SIZE_M_WIRE_SAMPLES added.
 * 
 * 3     07-07-16 15:15 Chalnot
 * mWireDataReceived added to mWireData.
 * 
 * 2     07-07-10 12:01 Chalnot
 * Changed mWireBufferServicePhone from array to one value.
 * 
 * 1     07-06-11 10:03 Chalnot
 * Initial version.
 *
 *
 *-------------------------------------------------------------------------*/
 
#ifndef SUBFRAME_H
#define SUBFRAME_H

/* INCLUDE FILES ***********************************************************/

#include "Basetype_O4CV.h"
#include "voiceData.h"
#include "GaoTF.h"

/* PUBLIC CONSTANT DECLARATIONS ********************************************/

#define BUFFER_SIZE_M_WIRE_SAMPLES	32

// Subframe headers
#define SUBFRAME_HEADER_VOICE_1		BIT6				//010
#define SUBFRAME_HEADER_VOICE_2		BIT7 | BIT5			//101
#define SUBFRAME_HEADER_VOICE_3		BIT7 | BIT5			//101

#define SUBFRAME_HEADER_FAX_1		BIT5				//001
#define SUBFRAME_HEADER_FAX_2		BIT7 | BIT6			//110
#define SUBFRAME_HEADER_FAX_3		BIT7 | BIT6			//110

#define SUBFRAME_HEADER_SIGNAL_1	BIT7 | BIT6 | BIT5	//111
#define SUBFRAME_HEADER_SIGNAL_2	0					//000
#define SUBFRAME_HEADER_SIGNAL_3	0					//000

/* PUBLIC TYPE DECLARATIONS ************************************************/

typedef struct
{ 
	UINT8 mWireBuffer[BUFFER_SIZE_M_WIRE_SAMPLES];
	UINT8 mWireBufferServicePhone;
	UINT16 freeze;
	bool insertMWireFrame;
	bool servicePhoneOn;
	bool mWireDataReceived;
} t_mWireData;

/* PUBLIC MACRO DEFINITIONS ************************************************/

/* PUBLIC FUNCTIONS ********************************************************/

void subframe_init(	UINT16 datarate);

void subframe_pack(	TFPayloadStruc *pTFPayloadIn, 
					t_mWireData *pmWireDataIn,
					t_Subframe *pSubframeOut);

void subframe_unpack(	t_RxFrameForDecoder *pRxFrame,	
						TFPayloadStruc *pTFPayloadOut, 
						t_mWireData *pmWireDataOut);

										
#endif
