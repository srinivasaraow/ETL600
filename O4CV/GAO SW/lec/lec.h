/******************************************************************************
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
#ifndef  _LEC_EXT_H_
#define  _LEC_EXT_H_

typedef signed short int  SWORD;
#define	  LEC_MEM_SIZE			(2688) // LEC structure 2688 bytes

#define ENABLE					(1)
#define DISABLE					(0)

#define LEC_CONTROL_NLP			(1)
#define LEC_CONTROL_CNG			(2)
#define LEC_CONTROL_LEC			(4)
#define LEC_CONTROL_ADAPTATION	(5)
#define LEC_CONTROL_HPF_SIN		(6)
#define LEC_CONTROL_HPF_RIN		(7)
#define LEC_GET_RESIDUE			(67)
#define LEC_IS_NLP_ENABLED		(68)
#define LEC_IS_CNG_ENABLED		(69)
#define LEC_IS_HPF_SIN_ENABLED	(71)
#define LEC_IS_HPF_RIN_ENABLED	(72)
#define LEC_GET_SIZE_IN_BYTES	(97)
#define LEC_GET_SIZE_IN_WORDS	(98)
#define LEC_SET_NLP_THRESHOLD	(129)
#define LEC_SET_CNG_LEVEL		(132)

#define LEC_CONTROL_ERROR		(-1)

SWORD LEC_init(SWORD *pLecData, SWORD *pPureDelayBuf, SWORD nPureDelayLen);
SWORD LEC_apply(SWORD *pLecData, SWORD Rin, SWORD Sin);
SWORD LEC_control(SWORD *pLecData, SWORD nMessage, SWORD nValue);

#endif
