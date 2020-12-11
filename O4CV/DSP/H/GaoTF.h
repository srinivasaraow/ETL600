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

#ifndef _GAOTF_H_
#define _GAOTF_H_

/* data type definition for application */
#ifndef _COMMTYPE_H
typedef unsigned char		UBYTE;
typedef unsigned short int	UWORD;
typedef signed short int	QWORD;
#endif

#define TF_DATAMEM_SIZE		(14400) /* 14400 Bytes */
#define	PAYLOAD_MAX_SIZE	(24)
#define VAGC_PARAM_NUM		(3)
#define MAX_FREQ_NUM		(15)
#define MAX_CP_TONE_NUM		(15)

/* Return value */
#define SUCCESS				(0)
#define DTMF_GEN_ERROR		(1)
#define CP_TONEDEF_ERROR	(2)
#define LECMEM_ALLOC_ERR	(3)

/* TF Control Command Macros */
	/* for StateChange */
#define NO_CHANGE			(0)
#define TFCMD_CHANGE		(0x0001)
#define LEC_CHANGE			(0x0002)
#define VAGC_CHANGE			(0x0004)
#define CP_DET_CHANGE		(0x0008)
#define CP_GEN_CHANGE		(0x0010)
#define DTMF_DET_CHANGE		(0x0020)
#define DTMF_GEN_CHANGE		(0x0040)
#define CODEC_CHANGE		(0x0080)
	/* for TFCmd */
#define IDLE_CMD			(1)
#define VOICE_CMD			(2)
#define FAX_CMD				(3)
	/* for VAGCState */
#define VAGC_DEFAULT		(1)
#define VAGC_CONFIGURABLE	(2)
	/* for Codecstate */
#define SET_VOCODER_63		(1)
#define SET_VOCODER_53		(2)
	/* for all others */
#define CONTROL_ENABLE		(1)
#define CONTROL_DISABLE		(0)

/* TF Status Macros */
	/* for StateChange */
#define STATUS_NOCHANGE		(0)
#define TFSTATE_CHANGE		(0x0001)
#define CP_LOCAL_CHANGE		(0x0002)
#define CP_REMOTE_CHANGE	(0x0004)
#define DTMF_LOCAL_CHANGE	(0x0008)
#define DTMF_REMOTE_CHANGE	(0x0010)
#define FAXSTATE_CHANGE		(0x0020)
	/* for TFState */
#define IDLE_STATE			(1)
#define VOICE_STATE			(2)
#define FAX_STATE			(3)
	/* for FaxState */
#define FAX_STANDBY			(0)
#define FAX_START_STATE		(1)
#define FAX_CONTINUE_STATE	(2)
#define FAX_END_STATE		(3)
#define FAX_ERROR_STATE		(4)

/* For PayloadDataType */
#define PAYLOAD_NODATA		(0)
#define PAYLOAD_VOCODER		(1)
#define PAYLOAD_FAX			(2)
#define PAYLOAD_DTMF		(3)
#define PAYLOAD_CPTONE		(4)
#define PAYLOAD_BADVOICE	(5)
/*****************/

/* structures for CP tones */
typedef struct {
	UBYTE	freq1_ind;		/* freq.1 (index) of CP tone */
	UBYTE	freq2_ind;		/* freq.2 (index) of CP tone, 0xFF if single tone */
	UWORD	t_on1;			/* t_on 1 time of CP tone in [ms] */
	UWORD	t_off1;			/* t_off 1 time, 0 if continuous tone */
	UWORD	t_on2;			/* t_on 2 time, 0 if single on/off cadence */
	UWORD	t_off2;			/* t_off 2 time, 0 if single on/off cadence */
} CP_Tone_Type;

typedef struct {
	UBYTE	num_freqs;			/* number of frequencies, 0...15 */
	UBYTE	num_tones;			/* number of CP tones, 0...15 */
	UWORD	freq[MAX_FREQ_NUM];	/* frequency definitions in [Hz] */
	CP_Tone_Type cp_tone[MAX_CP_TONE_NUM]; /* CP tone definitions */
} CP_Def_Type;

/* structures for API */
typedef struct {
	UWORD	PayloadDataType; /* macros defined above */
	UBYTE	PayloadDataBuf[PAYLOAD_MAX_SIZE];
} TFPayloadStruc;

typedef struct {
	UWORD	StateChange;
	UBYTE	TFCmd;
	UBYTE	LECState;
	UBYTE	VAGCState;
	QWORD	VAGCParam[VAGC_PARAM_NUM];
	UBYTE	CPToneDetState;
	UBYTE	CPToneGenState;
	UBYTE	DTMFDetState;
	UBYTE	DTMFGenState;
	UBYTE	CodecState;
} TFControlStruc;

typedef struct {
	CP_Def_Type	CPDef;
	UBYTE		HPFilter;
	UBYTE		PostFilter;
	UBYTE		DelayBuf;
	UWORD		PureDelay;
	QWORD		*pPureDelayBuf;
} TFConfigStruc;

typedef struct {
	UWORD	StateChange;
	UBYTE	TFState;
	UBYTE	CPLocalDet;
	UBYTE	CPRemoteDet;
	UBYTE	DTMFLocalDet;
	UBYTE	DTMFRemoteDet;
	UBYTE	FaxState;
} TFStatusStruc;

/* API functions */
UWORD TF_Init   (UWORD *pTFDataMem);
UWORD TF_Encode (UWORD *pTFDataMem, QWORD *pPcmIn, TFPayloadStruc *pBitOut);
UWORD TF_Decode (UWORD *pTFDataMem, TFPayloadStruc *pBitIn, QWORD *pPcmOut);
UWORD TF_Control(UWORD *pTFDataMem, TFControlStruc *pControl);
UWORD TF_Config (UWORD *pTFDataMem, TFConfigStruc *pConfig);
UWORD TF_Status (UWORD *pTFDataMem, TFStatusStruc *pStatus);

#endif
