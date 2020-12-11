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

#ifndef _CPSTRUCT_H_
#define _CPSTRUCT_H_

#include "cpdef.h"
#include "GaoTF.h"

typedef struct {
    QDWORD qdOut;
    QWORD qData0;
} HpfStruct;

typedef struct {
	CP_Def_Type	*pCpToneDef;
	QDWORD		wm1[MAX_FREQ_NUM]; // z^-1
	QDWORD		wm2[MAX_FREQ_NUM]; // z^-2
	QDWORD		TotalEgy;
	UBYTE		*pToneHis[MAX_CP_TONE_NUM];
	HpfStruct	hps;
	QWORD		FrameCnt;	
	QWORD		Coef[MAX_FREQ_NUM];
	UBYTE		ToneHis[MAX_CP_TONE_NUM][HISTORY_BUF_LEN<<1]; // double history buffer	
} CpDetStruc;

typedef struct {
	CP_Def_Type	*pCpToneDef;
	UWORD		dphase[MAX_FREQ_NUM];
	UWORD		phase1;
	UWORD		phase2;
	UBYTE		tIdx;
} CpGenStruc;

#endif
