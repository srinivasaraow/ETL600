/*****************************************************************************
*             Copyright (c) 1995 - 2006 by GAO Research Inc.                 *
*                          All rights reserved.                              *
*                                                                            *
*  This program is a confidential and an unpublished work of GAO Research    *
*  Inc. that is protected under international and Canadian copyright laws.   *
*  This program is a trade secret and constitutes valuable property of GAO   *
*  Research Inc. All use, reproduction and/or disclosure in whole or in      *
*  part of this program is strictly prohibited unless expressly authorized   *
*  in writing by GAO Research Inc.                                           *
*****************************************************************************/

#include "GaoTF.h"
#include "packext.h"

#define MASK_EMPTY			0x00
#define PATN_EMPTY			0x00
#define MASK_DTMF			0x00
#define PATN_DTMF			0x01
#define MASK_CP				0x00
#define PATN_CP				0x02
#define MASK_FAX			0x0F
#define PATN_FAX			0x40
#define MASK_VOCODER		0x7F
#define PATN_VOCODER		0x80

#define PACKAGER(data, mask, pattern)		(((data)&(mask))|(pattern))
#define DEPACKAGER(data, mask)				((data)&(~(mask))) 

void TF_Packaging(TFPayloadStruc *pPayload)
{
	UWORD cd; /* call discriminator */
	UBYTE *pData;

	cd = pPayload->PayloadDataType;
	pData = pPayload->PayloadDataBuf;

	switch(cd)
	{
	case PAYLOAD_VOCODER:
		/* Do nothing. */
		break;
	case PAYLOAD_FAX:
		*pData = PACKAGER(*pData, MASK_FAX, PATN_FAX);
		break;
	case PAYLOAD_DTMF:
		*pData = PACKAGER(*pData, MASK_DTMF, PATN_DTMF);
		break;
	case PAYLOAD_CPTONE:
		*pData = PACKAGER(*pData, MASK_CP, PATN_CP);
		break;
	default: // empty
		*pData = PACKAGER(*pData, MASK_EMPTY, PATN_EMPTY);
	}
}

void TF_UnPackage(TFPayloadStruc *pPayload)
{
	UBYTE *pData;

	pData = pPayload->PayloadDataBuf;

	if (DEPACKAGER(*pData, MASK_FAX) == PATN_FAX) /* fax data */
	{
		pPayload->PayloadDataType = PAYLOAD_FAX;
	}
	else if (DEPACKAGER(*pData, MASK_DTMF) == PATN_DTMF) /* DTMF data */
	{
		pPayload->PayloadDataType = PAYLOAD_DTMF;
	}
	else if (DEPACKAGER(*pData, MASK_CP) == PATN_CP) /* CP data */
	{
		pPayload->PayloadDataType = PAYLOAD_CPTONE;
	}
	else if (DEPACKAGER(*pData, MASK_EMPTY) == MASK_EMPTY) /* empty */
	{
		pPayload->PayloadDataType = PAYLOAD_NODATA;
	}
	else if (DEPACKAGER(*pData, MASK_VOCODER) == MASK_VOCODER) /* empty */
	{
		pPayload->PayloadDataType = PAYLOAD_VOCODER;
	}
}
