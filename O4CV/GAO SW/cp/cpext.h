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

#ifndef _CPEXT_H_
#define _CPEXT_H_

#include "cpstruct.h"

void  CpDet_Init(CpDetStruc *pCpDet, CP_Def_Type *pCpSrc);
UBYTE Cp_Det(CpDetStruc *pCpDet, QWORD *pPcm, UBYTE *pToneIdx);
UBYTE Cp_30MS_Det(CpDetStruc *pCpDet, QWORD *pPcm, UBYTE *pToneIdx);
void  CpGen_Init(CpGenStruc *pCpGen, CP_Def_Type *pCpSrc);
void  Cp_30MS_Gen(CpGenStruc *pCpGen, UBYTE ToneIdx, QWORD *pPcmOut);

#endif
