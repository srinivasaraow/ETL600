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

#include <string.h>

typedef signed long int		QDWORD;
typedef signed short int	SWORD;
typedef unsigned long int	UDWORD;

#define Q_MAX			(32767)
#define Q_MIN			(-32767)     
#define QD_MAX			(QDWORD)(0x3FFFFFFF)        /* 1073741823 */
#define QD_MIN			(QDWORD)(-1073741823)		/* 0xC000 0000 */
#define QQMULQD(a,b)	((QDWORD)((QWORD)(a) * (QWORD)(b)))
#define QQMULQR15(a,b)	((QWORD)((QQMULQD(a,b) + 0x4000) >> 15))
#define CP_ROUND(x,n)	(((x)+(1<<(n-1))) >> (n))
#define QD_SAT(d)  		{   \
    					if      ((d)>QD_MAX)    (d)=QD_MAX;  \
						else if ((d)<QD_MIN)    (d)=QD_MIN; }
#define QROUND_SAT(d)  	{ \
    					(d) = ((d)+0x4000)>>15;     \
    					if      ((d)>Q_MAX) (d)=Q_MAX;\
    					else if ((d)<Q_MIN) (d)=Q_MIN;  }

#include "cpext.h"

extern const SWORD DSP_tCOS_TABLE[];
extern const QWORD DSP_tCOS_OFFSET[];
extern void SinCos_Lookup_Fine(UWORD *Phase, QWORD *qSin, QWORD *qCos);

/****** Goertzel algorithm ******** 
coef = cos(2*pi*tone1_freq/Fs);
for i=1:N,
	w = 2*coef*wm1 - wm2 + tone1(i);
	wm2 = wm1;
	wm1 = w;
end
yn_square(N) = wm1^2 + wm2^2 - 2*coef*wm2*wm1;

coef: Q15 (QDWORD)
w, wm1, wm2: Q15 (QDWORD)
tone: Q15 (QDWORD)
YnS: Q0 (QDWORD)
TotalEgy: Q15 (QDWORD)
***********************************/

#if RM_DC_ENABLE
#define GAMMA   4
/****** High Pass Filter *******/
static QWORD Hpf(HpfStruct *pHpf, QWORD sin)
{
    QDWORD dtemp;
    dtemp = (QDWORD)sin - (QDWORD)pHpf->qData0;
    dtemp = (dtemp<<15) - (dtemp << (15 - (GAMMA+1))) + pHpf->qdOut - (pHpf->qdOut >> (GAMMA));
    QD_SAT(dtemp);
    pHpf->qdOut  = dtemp;
    pHpf->qData0 = sin;

	QROUND_SAT(dtemp);
    return  (QWORD)dtemp;
}
/****** Remove DC offset *******/
static void Remove_DC(CpDetStruc *pCp, QWORD *x, QWORD *y, QWORD Len)
{
	QWORD i;

	for (i=0; i<Len; i++)
		y[i] = Hpf(&(pCp->hps), x[i]);
}
#endif

void CpDet_Init(CpDetStruc *pCpDet, CP_Def_Type *pCpSrc)
{
	CpDetStruc *pCp = pCpDet;
	QWORD i;
	UWORD phase, freq;
	QWORD sin, cos;
	UDWORD f, ph;

	memset(pCp, 0, sizeof(CpDetStruc));
	pCp->pCpToneDef = pCpSrc;
	
	for (i=0; i<MAX_CP_TONE_NUM; i++)
	{
		pCp->pToneHis[i] = &(pCp->ToneHis[i][0]);
	}
	for (i=0; i<pCpSrc->num_freqs; i++)
	{
		freq = pCpSrc->freq[i];
		f = (UDWORD)freq; // maximum 4000Hz, 12-bit
		ph = (UDWORD)(f*8389); // (65536/8000)Q10:8389
		phase = (UWORD)CP_ROUND(ph,10);
		SinCos_Lookup_Fine(&phase, &sin, &cos);
		pCp->Coef[i] = cos;
	}
}

/******************* Cp_Det() ***********************
This function is a 10ms routine. It uses Goertzel algorithm 
to calculate energy at each defined frequency.
****************************************************/
UBYTE Cp_Det(CpDetStruc *pCpDet, QWORD *pPcm, UBYTE *pToneIdx)
{
	CpDetStruc *pCp = pCpDet;
	QDWORD w, w1, w2, w1t;
	QWORD i, j, k;
	QDWORD coef, tone, eng, TEgyThr;
	QDWORD YnS[MAX_FREQ_NUM]; // |y(n)|^2
	CP_Tone_Type *pTone;
	UBYTE bTone, cnt, playingCnt, bFlag[MAX_CP_TONE_NUM], PlayingFlag[MAX_CP_TONE_NUM];
	UBYTE *pH, *pT;
	UWORD cntp, offset;

	for (i=0; i<pCp->pCpToneDef->num_freqs; i++) // calculate for each frequency
	{
		w1 = pCp->wm1[i];
		w2 = pCp->wm2[i];
		coef = (QDWORD)(pCp->Coef[i]);
		coef = CP_ROUND(coef,1); // Q13 2*coef>>2

		for (j=0; j<CP_SAMPLE_PERFRAME; j++)
		{
			tone = (QDWORD)pPcm[j];
			w1t = w1;
			w1 = CP_ROUND(w1,8); // Q7
			tone = tone; // Q15
			w = ((CP_ROUND((coef*w1),5)) - w2 + tone); // Q15
			w2 = w1t;
			w1 = w;
		}
		pCp->wm1[i] = w1;
		pCp->wm2[i] = w2;
	}

	for (i=0; i<CP_SAMPLE_PERFRAME; i++) // calculate total energy
	{
		tone = (QDWORD)pPcm[i]; // Q15
		eng = tone * tone; // Q30
		eng = CP_ROUND(eng,15); // Q15
		pCp->TotalEgy += eng;
	}

	pCp->FrameCnt ++;

	*pToneIdx = NOTONE_DETECTED;

	if (pCp->FrameCnt >= BASIC_FRAMEUNIT) // check for every frame.
	{
		pCp->FrameCnt = 0;
		offset = (UWORD)(BASIC_FRAMEUNIT * 10);
		/* compute the YnS for each frequency first */
		for (i=0; i<pCp->pCpToneDef->num_freqs; i++)
		{
			w = pCp->wm1[i];
			w1 = pCp->wm2[i];
			w = CP_ROUND(w,9); // Q6
			w1 = CP_ROUND(w1,9); // Q6
			YnS[i] = ((CP_ROUND((w*w),2)) + (CP_ROUND((w1*w1),2))); // Q10
			w2 = w*w1; // Q12
			w2 = CP_ROUND(w2,13); // Q-1
			coef = (QDWORD)(pCp->Coef[i]); // Q15
			coef = CP_ROUND(coef,2); // Q12 2*coef>>3
			YnS[i] -= (CP_ROUND((w2*coef),1)); // Q10
			YnS[i] = CP_ROUND(YnS[i],10); // Q0
			PRINTJ2("[Idx:%d] Energy: %d\n", i, YnS[i]);

			/* reset wm1 and wm2 */
			pCp->wm1[i] = 0;
			pCp->wm2[i] = 0;
		}
		eng = pCp->TotalEgy;
		/* reset total energy */
		pCp->TotalEgy = 0;

		TEgyThr = CP_ROUND(eng,9) + CP_ROUND(eng,11) + CP_ROUND(eng,12);
		PRINTJ1("***** Total Energy: %d\n", TEgyThr);

		for (i=0; i<pCp->pCpToneDef->num_tones; i++)
		{
			pTone = &(pCp->pCpToneDef->cp_tone[i]);
			if (pTone->freq2_ind == 0xFF) // single tone
			{
				if (YnS[pTone->freq1_ind] > TONE_THRESHOLD 
#if TOTALEGY_THR_ENABLE
					&& YnS[pTone->freq1_ind] > TEgyThr
#endif
					)
					bTone = 1;
				else
					bTone = 0;
			}
			else // dual tone
			{
				if ((YnS[pTone->freq1_ind]<<1) > TONE_THRESHOLD && (YnS[pTone->freq2_ind]<<1) > TONE_THRESHOLD 
#if TOTALEGY_THR_ENABLE
					&& (YnS[pTone->freq1_ind]<<1) > TEgyThr && (YnS[pTone->freq2_ind]<<1) > TEgyThr
#endif
					)
					bTone = 1;
				else
					bTone = 0;
			}

			if (bTone)
				PlayingFlag[i] = 1;
			else
				PlayingFlag[i] = 0;

			/* put flag in history buffer */
			pH = pCp->pToneHis[i];
			pT = pCp->pToneHis[i] + HISTORY_BUF_LEN;
			*pH = bTone;
			*pT = bTone;
			(pCp->pToneHis[i])++;
			if (pCp->pToneHis[i] >= &(pCp->ToneHis[i][0]) + HISTORY_BUF_LEN)
				pCp->pToneHis[i] = &(pCp->ToneHis[i][0]);
			pH = pCp->pToneHis[i]; // so far the history is stored in pH[HISTORY_BUF_LEN].
								// pH[HISTORY_BUF_LEN-1] is the most recent one.

			/* make decision */
			bFlag[i] = 0;
			if (pH[HISTORY_BUF_LEN-1]) // current flag is on
			{
				if (pTone->t_off1 == 0) // continuous tone, t_on1 CANNOT equal to 0.
				{					
					cnt = 0;
					j=HISTORY_BUF_LEN-1;
					while (pH[j] && j>=0)
					{
						cnt++;
						j--;
					}
					cntp = (UWORD)((UWORD)cnt*offset);
					if (cntp < pTone->t_on1+(offset) && cntp > pTone->t_on1-(offset))
					{
						bFlag[i] = 1;
						/* clear history buffer, keep the last one (*(pH-1) and *(pH-1+HISTORY_BUF_LEN)) */
						pT = &(pCp->ToneHis[i][0]); // history buffer head
						while (pT < pH-1)
							*pT++ = 0;
						for (j=0; j<HISTORY_BUF_LEN-1; j++)
							pH[j] = 0;
						pT = pH + HISTORY_BUF_LEN;
						while (pT <= &(pCp->ToneHis[i][(HISTORY_BUF_LEN<<1)-1])) // to history buffer tail
							*pT++ = 0;
					}
				}
				else if (pTone->t_on2 == 0 || pTone->t_off2 == 0) // 1 cadence
				{
					j = HISTORY_BUF_LEN-1;
					while (pH[j] && j>=0)
						j--;
					cnt = 0;
					while (!pH[j] && j>=0) // find off cadence
					{
						cnt++;
						j--;
					}
					cntp = (UWORD)((UWORD)cnt*offset);
					if (cntp < pTone->t_off1+(offset<<1) && cntp > pTone->t_off1-(offset<<1)) 
					{
						cnt = 0;
						while (pH[j] && j>=0) // find on cadence
						{
							cnt++;
							j--;
						}
						cntp = (UWORD)((UWORD)cnt*offset);
						if (cntp < pTone->t_on1+(offset<<1) && cntp > pTone->t_on1-(offset<<1))
						{
							bFlag[i] = 1;
							/* clear history buffer, keep the last one (*(pH-1) and *(pH-1+HISTORY_BUF_LEN)) */
							pT = &(pCp->ToneHis[i][0]); // history buffer head
							while (pT < pH-1)
								*pT++ = 0;
							for (j=0; j<HISTORY_BUF_LEN-1; j++)
								pH[j] = 0;
							pT = pH + HISTORY_BUF_LEN;
							while (pT <= &(pCp->ToneHis[i][(HISTORY_BUF_LEN<<1)-1])) // to history buffer tail
								*pT++ = 0;
						}
					}
				}
				else // 2 cadence
				{
					j = HISTORY_BUF_LEN-1;
					while (pH[j] && j>=0)
						j--;
					cnt = 0;
					while (!pH[j] && j>=0) // find off cadence 2
					{
						cnt++;
						j--;
					}
					cntp = (UWORD)((UWORD)cnt*offset);
					if (cntp < pTone->t_off2+(offset<<1) && cntp > pTone->t_off2-(offset<<1)) 
					{
						cnt = 0;
						while (pH[j] && j>=0) // find on cadence 2
						{
							cnt++;
							j--;
						}
						cntp = (UWORD)((UWORD)cnt*offset);
						if (cntp < pTone->t_on2+(offset<<1) && cntp > pTone->t_on2-(offset<<1))
						{
							cnt = 0;
							while (!pH[j] && j>=0) // find off cadence 1
							{
								cnt++;
								j--;
							}
							cntp = (UWORD)((UWORD)cnt*offset);
							if (cntp < pTone->t_off1+(offset<<1) && cntp > pTone->t_off1-(offset<<1))
							{
								cnt = 0;
								while (pH[j] && j>=0) // find on cadence 1
								{
									cnt++;
									j--;
								}
								cntp = (UWORD)((UWORD)cnt*offset);
								if (cntp < pTone->t_on1+(offset<<1) && cntp > pTone->t_on1-(offset<<1))
								{
									bFlag[i] = 1;
									/* clear history buffer, keep the last one (*(pH-1) and *(pH-1+HISTORY_BUF_LEN)) */
									pT = &(pCp->ToneHis[i][0]); // history buffer head
									while (pT < pH-1)
										*pT++ = 0;
									for (j=0; j<HISTORY_BUF_LEN-1; j++)
										pH[j] = 0;
									pT = pH + HISTORY_BUF_LEN;
									while (pT <= &(pCp->ToneHis[i][(HISTORY_BUF_LEN<<1)-1])) // to history buffer tail
										*pT++ = 0;
								}
							}							
						}
					}
				}
			} // current flag is on
		}// for every tone

		cnt = 0;
		playingCnt = 0;
		for (i=0; i<pCp->pCpToneDef->num_tones; i++)
		{
			if (bFlag[i])
			{
				cnt++;
				j = i;
			}
			if (PlayingFlag[i])
			{
				playingCnt++;
				k = i;
			}
		}

		if (playingCnt >= 1)
			*pToneIdx = (UBYTE)k;

		if (cnt > 1) // more than one tone detected in one frame.
			return MORETONE_DETECTED;
		else if (cnt == 1)
			return (UBYTE)j; // valid tone detection
	} // every frame

	return NOTONE_DETECTED;
}
/******* Cp_30MS_Det() 30ms routine. *******/
UBYTE Cp_30MS_Det(CpDetStruc *pCpDet, QWORD *pPcm, UBYTE *pToneIdx)
{
	CpDetStruc *pCp = pCpDet;
	UBYTE rc, tc, tIdx;

#if RM_DC_ENABLE
	QWORD yPcm[240];

	Remove_DC(pCp, pPcm, yPcm, 240);
	pPcm = yPcm;
#endif

	rc = NOTONE_DETECTED;
	*pToneIdx = NOTONE_DETECTED;

	/* first call */
	tc = Cp_Det(pCp, pPcm, &tIdx);
	if (tc != NOTONE_DETECTED)
		rc = tc;
	if (tIdx != NOTONE_DETECTED)
		*pToneIdx = tIdx;
	pPcm += CP_SAMPLE_PERFRAME;

	/* second call */
	tc = Cp_Det(pCp, pPcm, &tIdx);
	if (tc != NOTONE_DETECTED)
		rc = tc;
	if (tIdx != NOTONE_DETECTED)
		*pToneIdx = tIdx;
	pPcm += CP_SAMPLE_PERFRAME;
	
	/* final call */
	tc = Cp_Det(pCp, pPcm, &tIdx);
	if (tc != NOTONE_DETECTED)
		rc = tc;
	if (tIdx != NOTONE_DETECTED)
		*pToneIdx = tIdx;

	return rc;
}

void CpGen_Init(CpGenStruc *pCpGen, CP_Def_Type *pCpSrc)
{
	CpGenStruc *pCp = pCpGen;
	QWORD i;
	UWORD freq;
	UDWORD f, ph;

	memset(pCp, 0, sizeof(CpGenStruc));
	pCp->pCpToneDef = pCpSrc;

	pCp->tIdx = NOTONE_DETECTED;

	/* calculate phase for each frequency */
	for (i=0; i<pCpSrc->num_freqs; i++)
	{
		freq = pCpSrc->freq[i];
		f = (UDWORD)freq; // maximum 4000Hz, 12-bit
		ph = (UDWORD)(f*8389); // (65536/8000)Q10:8389
		pCp->dphase[i] = (UWORD)CP_ROUND(ph,10);
	}
}

void Cp_30MS_Gen(CpGenStruc *pCpGen, UBYTE ToneIdx, QWORD *pPcmOut)
{
	CpGenStruc *pCp = pCpGen;
	UWORD ph1, ph2, dphase1, dphase2;
	UBYTE freqIdx1, freqIdx2;
	QWORD i;
	QWORD qTone1, qTone2;

	if (ToneIdx == pCp->tIdx) // repeating
	{
		ph1 = pCp->phase1;
		ph2 = pCp->phase2;
	}
	else // generating new one
	{
		ph1 = 49152; // begin from 3/2 PI. 2^16*3/2/2=49152. phase (UWORD) [0 2^16-1] is [0 2PI];
		ph2 = 49152;
		pCp->tIdx = ToneIdx;
	}

	if (ToneIdx == NOTONE_DETECTED || ToneIdx == MORETONE_DETECTED) // generate silence
	{
		memset(pPcmOut, 0, sizeof(QWORD)*CP_SAMPLE_PERFRAME*BASIC_FRAMEUNIT);
	}
	else // generate tone
	{
		/* get delta phase first */
		freqIdx1 = pCp->pCpToneDef->cp_tone[ToneIdx].freq1_ind;
		freqIdx2 = pCp->pCpToneDef->cp_tone[ToneIdx].freq2_ind;
		dphase1 = pCp->dphase[freqIdx1];
		if (freqIdx2 != 0xFF) // dual-tone
			dphase2 = pCp->dphase[freqIdx2];

		/* generate PCM */
		if (freqIdx2 == 0xFF) // single-tone
		{
			for (i=0; i<CP_SAMPLE_PERFRAME*BASIC_FRAMEUNIT; i++)
			{
				qTone1 = DSP_tCOS_TABLE[ph1>>8] + (QWORD)(QQMULQD(DSP_tCOS_OFFSET[ph1>>8], ph1&0xff)>>8);
				pPcmOut[i] = QQMULQR15(qTone1, 11627); // -9dB Q15:round(10^(-9/20)*2^15)
				ph1 += dphase1;
			}
		}
		else // dual-tone
		{
			for (i=0; i<CP_SAMPLE_PERFRAME*BASIC_FRAMEUNIT; i++)
			{
				qTone1 = DSP_tCOS_TABLE[ph1>>8] + (QWORD)(QQMULQD(DSP_tCOS_OFFSET[ph1>>8], ph1&0xff)>>8);
				pPcmOut[i] = QQMULQR15(qTone1, 6538); // -14dB Q15:round(10^(-14/20)*2^15)
				qTone2 = DSP_tCOS_TABLE[ph2>>8] + (QWORD)(QQMULQD(DSP_tCOS_OFFSET[ph2>>8], ph2&0xff)>>8);
				pPcmOut[i] += QQMULQR15(qTone2, 6538); // -14dB Q15:round(10^(-14/20)*2^15)
				ph1 += dphase1;
				ph2 += dphase2;
			}
		}
	}

	/* update phase */
	pCp->phase1 = ph1;
	pCp->phase2 = ph2;
}

