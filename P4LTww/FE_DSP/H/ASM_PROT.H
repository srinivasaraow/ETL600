/*-------------------------------------------------------------------------*
 * $Workfile: ASM_PROT.H $
 * Part of	: LCPLC
 * Language	: C
 * Created by	: 
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/FE_DSP/H/ASM_PROT.H $
 * 
 * 1     19.01.05 15:32 Chchlee
 * 
 * 1     10.03.04 12:14 Nohl01
 * Initial version
 * Based on MMI500 v. 3.20
 *-------------------------------------------------------------------------*/
#ifndef		_ASM_PROT
#define		_ASM_PROT
 
	void delay(long);
	// Extern equalizer assembler functions
	void Equalizer_FIR(float *InputArr, float *OutputArr);
    void PN_GeneratorSample(long *ShiftRegisterGenerator, float *BlockSampleValueArrPtr);
//   	void MesseETLChannel(EntzerrerStatusType *EntzerrerStatus, ChannelMeasurementStatusType *ChannelMeasurementStatus,
//											PNControllerStatusType *PNControllerStatus, float *RxAfRaw);                     
    // pilot demodulator (pmod.asm)
    void pilotDemodulator(PilotDemodDataType *pilotDemod, float *inSample, int blockLenth);
    
    // test tone generator (ttone.asm)
    void ttone(TestToneDataType *testTone, float *inSample, float *testToneSampleOut, int blockLength);     
    
    // Biquad IIR Filter
    
    void BiIIR(float *inSample, float *outSample, float pm *coef, float *state, long sections, long length);

    
#endif // _ASM_PROT
