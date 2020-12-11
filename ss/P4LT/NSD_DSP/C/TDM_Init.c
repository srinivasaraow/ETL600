/*-------------------------------------------------------------------------*
 * $Workfile: TDM_Init.c $
 * Part of      : ETL600 / System communication
 * Language     : C
 * Created by   : Bruno Busslinger, PTUKT2  -NOT a PhD-
 * Remarks      :
 * Purpose      :
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/Master_DSP/C/TDM_Init.c $
 * 
 * 11    29.09.05 9:06 Chbrbus
 * Small changes in InitSPORT13 and InitSPORT02 - now RestartSPORT02() is
 * working without a core hang.  
 * 
 * 10    5.07.05 9:44 Chharyt
 * 
 * 9     4.07.05 10:25 Chharyt
 * 
 * 8     13.05.05 9:02 Chchlee
 * 
 * 7     12.05.05 21:34 Chchlee
 * global variables removed
 * 
 * 6     16.02.05 11:06 Chbrbus
 * Activated SPORT02 (External TDM Bus) supervision same  as in ETL500
 * 
 * 5     14.02.05 14:27 Chbrbus
 * Interrupt routine cleaned, preventing read and write in non DMA buffer
 * segments anytime
 * 
 * 4     11.02.05 14:44 Chbrbus
 * Added DMA buffer boundary check and SIMD mode off during interrupt
 * 
 * 3     10.02.05 16:29 Chbrbus
 * Changed to SPORT02_RxBufferLookupTable
 * 
 * 2     10.02.05 16:16 Chbrbus
 * Reduced upconverter TDM bus DMA buffer - changed interrupt routine
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 54    14.01.05 10:21 Chharyt
 * Code cleaned. External memory used if possible.
 * 
 * 53    13.01.05 16:39 Chharyt
 * Garbage removed
 * 
 * 52    13.01.05 14:35 Chharyt
 * identical with version 50 except some comments
 * 
 * 51    12.01.05 16:03 Chharyt
 * 
 * 50    7.01.05 16:30 Chharyt
 * Special GP-Reg handling for NSD_DSP removed
 * 
 * 49    18.11.04 7:48 Chharyt
 * Unused SPORT stopp functions marked as text
 * 
 * 48    15.11.04 18:11 Leeb02
 * tdmInit_SetDSPId removed
 * 
 * 47    27.09.04 11:51 Rytz01
 * Special GP-Reg handling for NSD_DSP(DSP_ID = 6) on Tx side with dynamic
 * allocation like NumberOfChannels from Config
 * 
 * 46    24.09.04 14:24 Leeb02
 * 
 * 45    24.09.04 13:41 Rytz01
 * SPORT02_16BIT_PACKING_ENABLED considered
 * 
 * 44    24.09.04 9:39 Rytz01
 * NSD DSP spez. handling of SPORT02 Bufferpointer in GP Reg.
 * Open Point: handling of number of channels in 16 bit packing mode if
 * they are odd instead of even!!!
 * 
 * 43    10.09.04 8:49 Plueer01
 * In function "tdmInit_OverrideTxChannelMask02_0()" -> ASSERT removed!
 * 
 * 42    10.09.04 7:36 Leeb02
 * preparing for removal of compiler switches
 * 
 * 41    9.09.04 16:54 Leeb02
 * 
 * 40    9.09.04 15:50 Leeb02
 * 
 * 39    9.09.04 15:13 Leeb02
 * DSP_ID removed
 * 
 * 38    9.09.04 8:26 Leeb02
 * preparing for removal of compiler switches
 * 
 * 37    2.09.04 14:01 Rytz01
 * 
 * 36    1.09.04 15:35 Rytz01
 * 
 * 35    1.09.04 14:20 Rytz01
 * 
 * 34    1.09.04 14:04 Rytz01
 * p_SPORT02_DMA_RX_Buffer_Order,p_SPORT02_DMA_TX_Buffer_Order,p_SPORT13_D
 * MA_RX_Buffer_Order,p_SPORT13_DMA_TX_Buffer_Order removed
 * 
 * 33    31.08.04 15:19 Rytz01
 * SEGMENT_EXT_CODE labels inserted but code is still internal
 * 
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES *************************************************************/
#include "basetype.h"
#include "TDM_Init.h"
#include <def21161.h>

#include "diag.h"

asm("#include <def21161.h>");

/* LOCAL TYPE DECLARATIONS ***************************************************/
/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define TCB_BLOCK_LENGTH				8			// # elements in a TCB block
#define SPORT_PCI_SET					0x40000		// enables Chaining DMA in a TCB at SPORT
#define SPORT_PCI_CLEAR					0x3ffff


/* LOCAL VARIABLE DECLARATIONS ***********************************************/


// don't move any of these variables. Only God the almighty knows why it doesn't work anymore otherwise (CL)
const unsigned long rx_channel_mask13[4] = RX_CHANNEL_MASK13;
const unsigned long tx_channel_mask13[4] = TX_CHANNEL_MASK13;

const unsigned long SPORT02_RegisterSPCLT0 = SPORT02_REGISTER_SPCLT0;
const unsigned long SPORT02_RegisterSPCLT2 = SPORT02_REGISTER_SPCLT2;


// Define variables for TCB and RX/TX buffer SPORT02 and SPORT13
static unsigned long SEGMENT_DMA SPORT02_RX_tcb[NUMBER_OF_SPORT02_BUFFERS][TCB_BLOCK_LENGTH];
static unsigned long SEGMENT_DMA SPORT02_TX_tcb[NUMBER_OF_SPORT02_BUFFERS][TCB_BLOCK_LENGTH];
static unsigned long SEGMENT_DMA SPORT13_RX_tcb[NUMBER_OF_SPORT13_BUFFERS][TCB_BLOCK_LENGTH];
static unsigned long SEGMENT_DMA SPORT13_TX_tcb[NUMBER_OF_SPORT13_BUFFERS][TCB_BLOCK_LENGTH];

static unsigned long SEGMENT_DMA SPORT02_RX_Buffer[NUMBER_OF_SPORT02_BUFFERS][MAX_WORDS_PER_DMA_AT_SPORT02_RX];
static unsigned long SEGMENT_DMA SPORT02_TX_Buffer[NUMBER_OF_SPORT02_BUFFERS][MAX_WORDS_PER_DMA_AT_SPORT02_TX];
static unsigned long SEGMENT_DMA SPORT13_RX_Buffer[NUMBER_OF_SPORT13_BUFFERS][MAX_WORDS_PER_DMA_AT_SPORT13_RX];
static unsigned long SEGMENT_DMA SPORT13_TX_Buffer[NUMBER_OF_SPORT13_BUFFERS][MAX_WORDS_PER_DMA_AT_SPORT13_TX];
// variables for interrupt routines preventing TDM routines writing in non DMA buffer areas
// Is used in DMA routine
const long *SPORT02_FirstDMA_RxBufferAdress = (long*) &SPORT02_RX_Buffer[0][0];
const long *SPORT02_LastDMA_RxBufferAdress = (long*) &SPORT02_RX_Buffer[NUMBER_OF_SPORT02_BUFFERS - 1][0];
const long *SPORT02_FirstDMA_TxBufferAdress = (long*) &SPORT02_TX_Buffer[0][0];
const long *SPORT02_LastDMA_TxBufferAdress = (long*) &SPORT02_TX_Buffer[NUMBER_OF_SPORT02_BUFFERS - 1][0];

const long *SPORT13_FirstDMA_RxBufferAdress = (long*) &SPORT13_RX_Buffer[0][0];
const long *SPORT13_LastDMA_RxBufferAdress = (long*) &SPORT13_RX_Buffer[NUMBER_OF_SPORT13_BUFFERS - 1][0];
const long *SPORT13_FirstDMA_TxBufferAdress = (long*) &SPORT13_TX_Buffer[0][0];
const long *SPORT13_LastDMA_TxBufferAdress = (long*) &SPORT13_TX_Buffer[NUMBER_OF_SPORT13_BUFFERS - 1][0];

#ifdef UC_DSP
long SEGMENT_DMA SPORT02_RxBufferLookupTable[(3 * (NUMBER_OF_SPORT02_BUFFERS/2)) - 1];
const long SEGMENT_DMA SPORT02_RxBufferSize = NUMBER_OF_SPORT02_BUFFERS;

#endif		/* End of Special GP-Reg handling of UC_DSP */

//extern volatile long *internalTDM_Rx_Ptr[2];
//extern volatile long *internalTDM_Tx_Ptr[2];
extern volatile long IR_BlockCounter;

static unsigned long NumberOfChannels = 0;


static const unsigned long rx_channel_mask02[4] = RX_CHANNEL_MASK02;
static       unsigned long tx_channel_mask02[4] = TX_CHANNEL_MASK02; // must be modifiable for function "tdmInit_OverrideTxChannelMask02_0"



/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL FUNCTION DEFINITIONS ************************************************/

static void CreateTCB_Block (unsigned long *tcbPtr, unsigned long *bufferPtr, unsigned long NumberOfWordsPerDMA, unsigned long NumberOfBuffers);
static void ClearDMA_Buffer(unsigned long *DMA_BufferPtr, unsigned long ArrayLength);
static long GetNumberOfChannels(const unsigned long *given_channel_mask);

/**************************************************************************************************************/
/* W A R N I N G  --  W A R N I N G  --  W A R N I N G  -- W A R N I N G  -- W A R N I N G  -- W A R N I N G  */ 
/* W A R N I N G  --  W A R N I N G  --  W A R N I N G  -- W A R N I N G  -- W A R N I N G  -- W A R N I N G  */ 
/* W A R N I N G  --  W A R N I N G  --  W A R N I N G  -- W A R N I N G  -- W A R N I N G  -- W A R N I N G  */ 
/*																											  */
/* If you like to outsource the following internal functions you will fail!!! I promise it. HPR				  */
/* I made a lot of tests to get some of the function out but without success!								  */
/*																											  */
/* W A R N I N G  --  W A R N I N G  --  W A R N I N G  -- W A R N I N G  -- W A R N I N G  -- W A R N I N G  */ 
/* W A R N I N G  --  W A R N I N G  --  W A R N I N G  -- W A R N I N G  -- W A R N I N G  -- W A R N I N G  */ 
/* W A R N I N G  --  W A R N I N G  --  W A R N I N G  -- W A R N I N G  -- W A R N I N G  -- W A R N I N G  */ 
/**************************************************************************************************************/


// override the default settings of the least significant tx channel mask of SPORT02
void SEGMENT_EXT_CODE tdmInit_OverrideTxChannelMask02_0 (unsigned long channelMask)
{
	tx_channel_mask02[0] = channelMask;
}

void Start_SPORT02()
{
	long ValueFromRxSPORT;

	asm ("bit set imask SP0I | SP2I;");
	// Empty SPORT receive buffer -> keeping order in DMA buffer
	ValueFromRxSPORT = *(long*)RX0A;
 	*(long*)SP02MCTL |= (((NUMBER_OF_TIMESLOTS_SPORT02 - 1)<<5) | MULTICHANNEL_FRAME_DELAY02 | MCE);
	asm("bit set mode1 IRPTEN;");
}

void Start_SPORT13()
{
	long ValueFromRxSPORT;

	asm ("bit set imask SP1I | SP3I;");
	// Empty SPORT receive buffer -> keeping order in DMA buffer
	ValueFromRxSPORT = *(long*)RX1A;
 	*(long*)SP13MCTL |= (((NUMBER_OF_TIMESLOTS_SPORT13 - 1)<<5) | MULTICHANNEL_FRAME_DELAY13 | MCE);

	asm("bit set mode1 IRPTEN;");
}

// The following 3 functions are necessary to synchronize the order in the DMA Tx buffers
// They must be called a while after the SPORTs are getting the clock signals.
void StartChainedDMA_SPORT02Tx(void)
{
	// Not needed but usefull to know for sync of TX interrupts or DMA-Buffer mismatch
	/*	#ifdef MASTER_DSP
			*(long*)TX2A = 0;
	//		*(long*)TX2A = 0;
		#endif
	*/
	// Start chained DMA2 (TX) 
	*(long*)CP2A = (( (unsigned long) SPORT02_TX_tcb) + TCB_BLOCK_LENGTH - 1) & SPORT_PCI_CLEAR;
}
void StartChainedDMA_SPORT13Tx(void)
{
	// This is the trick for synchronizing DMA buffer order Tx
	*(long*)TX3A = 0;
	// Start chained DMA3 (TX) 
	*(long*)CP3A = (( (unsigned long) SPORT13_TX_tcb) + TCB_BLOCK_LENGTH - 1) & SPORT_PCI_CLEAR;
}
void StartChainedDMA_SPORT13Tx_Double(void)
{
	// When using with 5.333 kHz cycle, trick is no longer nessessary
	// Start chained DMA3 (TX) 
	*(long*)CP3A = (( (unsigned long) SPORT13_TX_tcb) + TCB_BLOCK_LENGTH - 1) & SPORT_PCI_CLEAR;
}

long SEGMENT_EXT_CODE GetNumberOfChannels(const unsigned long *given_channel_mask)
{
	unsigned long bit_count, word_count;
	unsigned long channel_sum = 0;
	
	const unsigned long *tmpPtr = given_channel_mask;

	for(word_count=0; word_count < NR_OF_BYTES_IN_LONG; word_count++)
	{
		for(bit_count=0; bit_count < NR_OF_BITS_IN_LONG; bit_count++)
		{
			if(*tmpPtr & (BIT0 << bit_count))
			{
				channel_sum++;
			}
		}
		tmpPtr++;
	}
	return channel_sum;
}

static void CreateTCB_Block (unsigned long *tcbPtr,unsigned long *bufferPtr,\
					  unsigned long NumberOfWordsPerDMA, unsigned long NumberOfBuffers)
{
	unsigned long *tmpPtr;
	unsigned long LoopCounter = 0;
	tmpPtr = tcbPtr;

	for(LoopCounter = 0; LoopCounter < NumberOfBuffers; LoopCounter++)
	{
		*tmpPtr++ = 0x00000000;			// not used Reg
		*tmpPtr++ = 0x00000000;			// not used Reg
		*tmpPtr++ = 0x00000000;			// not used Reg
		*tmpPtr++ = ((unsigned long)bufferPtr - NumberOfWordsPerDMA);	//GP-Reg of TCB, contains adress of next TDM Buffer
		*tmpPtr = ((unsigned long) tmpPtr	+ 11) & SPORT_PCI_CLEAR;	//CP-Reg (chainpointer) of this TCB points to II-Reg of next TCB;
		*tmpPtr++;
		*tmpPtr++ = NumberOfWordsPerDMA;								//C-Reg of TCB contains used number of words per DMA
		*tmpPtr++ = 1;													//IM-Reg of TCB contains the imcrement
		*tmpPtr++ = (unsigned long)bufferPtr;							//II-Reg of TCB contains memory adress of the TDM buffer
		if(LoopCounter < (NumberOfBuffers - 1))
			bufferPtr += NumberOfWordsPerDMA;		//bufferPtr points to next DMA_Window
	}
	//set CP-Reg (chainpointer) of last TCB to II-Reg of first TCB
	tmpPtr = tmpPtr - 4;
	*tmpPtr = ((long)tcbPtr + 7) & SPORT_PCI_CLEAR; // Startadress + 7

	//set GP-Reg of first TCB with address of last RealTimeBus Buffer;
	tcbPtr = tcbPtr + 3;		// 3 not used Reg
	*tcbPtr = (long) bufferPtr;
}

static void SEGMENT_EXT_CODE ClearDMA_Buffer(unsigned long *DMA_BufferPtr, unsigned long ArrayLength)
{
	unsigned long *TempPtr;
	unsigned long LocalIndex = 0;

	TempPtr = DMA_BufferPtr;
	LocalIndex = 0;
	for (LocalIndex = 0; LocalIndex < ArrayLength; LocalIndex++)
	{
		*TempPtr++ = 0x00000000;
	}
}

void SPORT02_DMA_init(void)
{
	unsigned long LocalIndex = 0;
	unsigned long Index = 0;
	unsigned long *tmpPtr = 0;

	// Initialize TCB for SPORT0 RX
	NumberOfChannels = GetNumberOfChannels(&rx_channel_mask02[0]);
	if (SPORT02_16BIT_PACKING_ENABLED) NumberOfChannels = NumberOfChannels / 2;
	CreateTCB_Block (&SPORT02_RX_tcb[0][0], &SPORT02_RX_Buffer[0][0], NumberOfChannels, NUMBER_OF_SPORT02_BUFFERS);

	#ifdef FE_DSP
		// Special GP-Reg handling for FE_DSP(DSP_ID = 5) to get the last/actual pointer to the 
		// DMA-buffer block direct from Interrupt routine. We work with 2 Blocks of Buffers. SPORT02 makes 12 interrupts
		// while SPORT13 makes only 2 in a main loop.
		// 12 Interrupts/DMA's are "grouped" to a block. But in the GP-Reg are the pointers to the buffers (2*12=24) 
		// stored. Now they will be changed to two blocks!
		//
		// Same for UC_DSP(DSP_ID = 2) but only on RX side!!! and with other defines for Buffer lenght, ...
		tmpPtr = (&SPORT02_RX_tcb[0][0]+3); //Points to the GP-Reg
		for(LocalIndex = 0; LocalIndex < NUMBER_OF_SPORT02_BUFFERS; LocalIndex++)
		{	
			if(LocalIndex < (NUMBER_OF_SPORT02_BUFFERS/2))	*tmpPtr = (unsigned long)(&SPORT02_RX_Buffer[0][0] + ((NUMBER_OF_SPORT02_BUFFERS/2)*MAX_WORDS_PER_DMA_AT_SPORT02_RX));
			else *tmpPtr = (unsigned long)&SPORT02_RX_Buffer[0][0];
			//if((LocalIndex < (2*(NUMBER_OF_SPORT02_BUFFERS/3))) && (LocalIndex >= (NUMBER_OF_SPORT02_BUFFERS/3)))	*tmpPtr = (unsigned long)&SPORT02_RX_Buffer[0][0];
			//if((LocalIndex < NUMBER_OF_SPORT02_BUFFERS) && (LocalIndex >= (2*(NUMBER_OF_SPORT02_BUFFERS/3))))	*tmpPtr = (unsigned long)(&SPORT02_RX_Buffer[0][0] + ((NUMBER_OF_SPORT02_BUFFERS/3)*MAX_WORDS_PER_DMA_AT_SPORT02_RX));
			tmpPtr = tmpPtr + TCB_BLOCK_LENGTH;	//8=lenght of tcb
		}
	#endif	// End of Special GP-Reg handling for FE_DSP
	#ifdef UC_DSP
		// Special GP-Reg handling for FE_DSP(DSP_ID = 5) to get the last/actual pointer to the 
		// DMA-buffer block direct from Interrupt routine. We work with 3 Blocks of Buffers. SPORT02 makes 12 interrupts
		// while SPORT13 makes only 2 in a main loop.
		// 12 Interrupts/DMA's are "grouped" to a block. But in the GP-Reg are the pointers to the buffers (3*12=36) 
		// stored. Now they will be changed to three blocks!
		//
		// Same for UC_DSP(DSP_ID = 2) but only on RX side!!! and with other defines for Buffer lenght, ...
		tmpPtr = (&SPORT02_RX_tcb[0][0]+3); //Points to the GP-Reg
		// With access lookup table
		for(LocalIndex = 0; LocalIndex < NUMBER_OF_SPORT02_BUFFERS; LocalIndex++)
		{
			*tmpPtr = LocalIndex;
			tmpPtr = tmpPtr + TCB_BLOCK_LENGTH;	//8=lenght of tcb
		}
		tmpPtr = (unsigned long*) &SPORT02_RxBufferLookupTable[0];
		for(LocalIndex = 0; LocalIndex < ((3 * (NUMBER_OF_SPORT02_BUFFERS/2)) - 1); LocalIndex++)
		{
			if (LocalIndex < (NUMBER_OF_SPORT02_BUFFERS/2)) *tmpPtr = (unsigned long) &SPORT02_RX_Buffer[LocalIndex + 6][0];
			else *tmpPtr = (unsigned long) &SPORT02_RX_Buffer[LocalIndex - 6][0];
			tmpPtr = tmpPtr + 1;
		}

//		for(LocalIndex = 0; LocalIndex < NUMBER_OF_SPORT02_BUFFERS; LocalIndex++)
//		{	
//			if(LocalIndex < (NUMBER_OF_SPORT02_BUFFERS/3))	*tmpPtr = (unsigned long)(&SPORT02_RX_Buffer[0][0] + ((2*(NUMBER_OF_SPORT02_BUFFERS/3))*MAX_WORDS_PER_DMA_AT_SPORT02_RX));
//			if((LocalIndex < (2*(NUMBER_OF_SPORT02_BUFFERS/3))) && (LocalIndex >= (NUMBER_OF_SPORT02_BUFFERS/3)))	*tmpPtr = (unsigned long)&SPORT02_RX_Buffer[0][0];
//			if((LocalIndex < NUMBER_OF_SPORT02_BUFFERS) && (LocalIndex >= (2*(NUMBER_OF_SPORT02_BUFFERS/3))))	*tmpPtr = (unsigned long)(&SPORT02_RX_Buffer[0][0] + ((NUMBER_OF_SPORT02_BUFFERS/3)*MAX_WORDS_PER_DMA_AT_SPORT02_RX));
//			tmpPtr = tmpPtr + TCB_BLOCK_LENGTH;	//8=lenght of tcb
//		}
	#endif	// End of Special GP-Reg handling for UC_DSP

	
	*(long*)II0A = ((unsigned long) SPORT02_RX_Buffer);
	*(long*)IM0A = 1;									// Increment
	*(long*)C0A = NumberOfChannels;						// Number of Rx words
	// Enable interrupt after a complete transfer of a RealTimeBus frame TX, RX has the same timing
	for(LocalIndex=0;LocalIndex<NUMBER_OF_SPORT02_BUFFERS;LocalIndex++)
	{
		SPORT02_RX_tcb[LocalIndex][4] |= SPORT_PCI_SET;
	}
	// Initialize TCB for SPORT2 TX
	NumberOfChannels = GetNumberOfChannels(&tx_channel_mask02[0]);
	if(((NumberOfChannels%2) ==1) && SPORT02_16BIT_PACKING_ENABLED)
	{
		NumberOfChannels++;
	}

	if (SPORT02_16BIT_PACKING_ENABLED) NumberOfChannels = NumberOfChannels / 2;
	CreateTCB_Block (&SPORT02_TX_tcb[0][0], &SPORT02_TX_Buffer[0][0], NumberOfChannels, NUMBER_OF_SPORT02_BUFFERS);

	#ifdef FE_DSP
	// Special GP-Reg handling for FE_DSP(DSP_ID = 5) to get the last/actual pointer to the 
		// DMA-buffer block direct from Interrupt routine. We work with 3 Blocks of Buffers. SPORT02 makes 12 interrupts
		// while SPORT13 makes only 2 in a main loop.
		// 12 Interrupts/DMA's are "grouped" to a block. But in the GP-Reg are the pointers to the buffers (3*12=36) 
		// stored. Now they will be changed to three blocks!
		//
		// Same for UC_DSP(DSP_ID = 2) but only on RX side!!! and with other defines for Buffer lenght, ...

		tmpPtr = (&SPORT02_TX_tcb[0][0]+3); //Points to the GP-Reg
		for(LocalIndex = 0; LocalIndex < NUMBER_OF_SPORT02_BUFFERS; LocalIndex++)
		{	
			if(LocalIndex < (NUMBER_OF_SPORT02_BUFFERS/2))	*tmpPtr = (unsigned long)(&SPORT02_TX_Buffer[0][0] + ((NUMBER_OF_SPORT02_BUFFERS/2)*MAX_WORDS_PER_DMA_AT_SPORT02_TX));
			else *tmpPtr = (unsigned long)&SPORT02_TX_Buffer[0][0];
			//if((LocalIndex < (2*(NUMBER_OF_SPORT02_BUFFERS/3))) && (LocalIndex >= (NUMBER_OF_SPORT02_BUFFERS/3)))	*tmpPtr = (unsigned long)&SPORT02_TX_Buffer[0][0];
			//if((LocalIndex < NUMBER_OF_SPORT02_BUFFERS) && (LocalIndex >= (2*(NUMBER_OF_SPORT02_BUFFERS/3))))	*tmpPtr = (unsigned long)(&SPORT02_TX_Buffer[0][0] + (12*MAX_WORDS_PER_DMA_AT_SPORT02_TX));
			tmpPtr = tmpPtr + TCB_BLOCK_LENGTH;	//8=lenght of tcb
		}
	#endif		/* End of Special GP-Reg handling of FE_DSP */

	*(long*)II2A = ((unsigned long) SPORT02_TX_Buffer);
	*(long*)IM2A = 1;									// Increment
	*(long*)C2A = NumberOfChannels;						// Number of Tx words
	for(LocalIndex=0;LocalIndex<NUMBER_OF_SPORT02_BUFFERS;LocalIndex++)
	{
		SPORT02_TX_tcb[LocalIndex][4] |= SPORT_PCI_SET;
	}
	
	// Start chained DMA0 (RX)
	*(long*)CP0A = (( (unsigned long) SPORT02_RX_tcb) + TCB_BLOCK_LENGTH - 1) & SPORT_PCI_CLEAR;

	// Start chained DMA2 (TX) 
	// moved -> Order DMA buffer
}
	
void SPORT13_DMA_init(void)
{
	unsigned long LocalIndex = 0;

	// Initialize TCB for SPORT1 RX
	NumberOfChannels = GetNumberOfChannels(&rx_channel_mask13[0]);
	CreateTCB_Block (&SPORT13_RX_tcb[0][0], &SPORT13_RX_Buffer[0][0], NumberOfChannels, NUMBER_OF_SPORT13_BUFFERS);
	*(long*)II1A = ((unsigned long) SPORT13_RX_Buffer);
	*(long*)IM1A = 1;									// Increment
	*(long*)C1A = NumberOfChannels;						// Number of Rx words
	// Enable interrupt after a complete transfer of a RealTimeBus frame TX, RX has the same timing
	for(LocalIndex=0;LocalIndex<NUMBER_OF_SPORT13_BUFFERS;LocalIndex++)
	{
		SPORT13_RX_tcb[LocalIndex][4] |= SPORT_PCI_SET;
	}
	// Initialize TCB for SPORT3 TX
	NumberOfChannels = GetNumberOfChannels(&tx_channel_mask13[0]);
	CreateTCB_Block (&SPORT13_TX_tcb[0][0], &SPORT13_TX_Buffer[0][0], NumberOfChannels, NUMBER_OF_SPORT13_BUFFERS);
	*(long*)II3A = ((unsigned long) SPORT13_TX_Buffer);
	*(long*)IM3A = 1;									// Increment
	*(long*)C3A = NumberOfChannels;						// Number of Tx words
	for(LocalIndex=0;LocalIndex<NUMBER_OF_SPORT13_BUFFERS;LocalIndex++)
	{
		SPORT13_TX_tcb[LocalIndex][4] |= SPORT_PCI_SET;
	}
	
	// Start chained DMA1 (RX)
	*(long*)CP1A = (( (unsigned long) SPORT13_RX_tcb) + TCB_BLOCK_LENGTH - 1) & SPORT_PCI_CLEAR;

	// Start chained DMA3 (TX) 
	// moved -> Order DMA buffer
}

void InitSPORT13(void)
{
	asm ("bit clr imask SP1I | SP3I;");
	// clear all interrupts
	asm("bit clr mode1 IRPTEN;");
		
	// Stop DMA first and clear all registers
	*(long*)SPCTL1 = 0;			// clear SPORT1 serial port control register
	*(long*)SPCTL3 = 0;			// clear SPORT3 serial port control register
	*(long*)SP13MCTL = 0;		// clear SPORTs 1 & 3 Multichannel Control Register

	// CAUTION: Do not write into inactive registers - Manual page 10-7
	// This will cause a core hang and the reinitialisation of the SPORT won't work 
	//*(long*)TX1A = 0;			// prevent core hang - must not be touched
	//*(long*)TX1B = 0;			// prevent core hang - must not be touched
	*(long*)RX1A = 0;
	//*(long*)RX1B = 0;			// prevent core hang - must not be touched
	*(long*)DIV1 = 0;
	*(long*)CNT1 = 0;

	*(long*)TX3A = 0;			// clear all other non initialized SPORT3 registers
	//*(long*)TX3B = 0;			// prevent core hang - must not be touched
	//*(long*)RX3A = 0;			// prevent core hang - must not be touched
	//*(long*)RX3B = 0;			// prevent core hang - must not be touched
	*(long*)DIV3 = 0;
	*(long*)CNT3 = 0;

	*(long*)MR1CCS0 = 0;		// clear SPORT1 multichannel rx compand select, channels 31 - 0
	*(long*)MR1CCS1 = 0;		// clear SPORT1 multichannel rx compand select, channels 63 - 32
	*(long*)MR1CCS2 = 0;		// clear SPORT1 multichannel rx compand select, channels 95 - 64
	*(long*)MR1CCS3 = 0;		// clear SPORT1 multichannel rx compand select, channels 127 - 96
	*(long*)MR1CS0 = 0;			// clear SPORT1 multichannel rx select, channels 31 - 0
	*(long*)MR1CS1 = 0;			// clear SPORT1 multichannel rx select, channels 63 - 32
	*(long*)MR1CS2 = 0;			// clear SPORT1 multichannel rx select, channels 95 - 64
	*(long*)MR1CS3 = 0;			// clear SPORT1 multichannel rx select, channels 127 - 96

	*(long*)MT3CCS0 = 0;		// clear SPORT3 multichannel tx compand select, channels 31 - 0
	*(long*)MT3CCS1 = 0;		// clear SPORT3 multichannel tx compand select, channels 63 - 32
	*(long*)MT3CCS2 = 0;		// clear SPORT3 multichannel tx compand select, channels 95 - 64
	*(long*)MT3CCS3 = 0;		// clear SPORT3 multichannel tx compand select, channels 127 - 96
	*(long*)MT3CS0 = 0;			// clear SPORT3 multichannel tx select, channels 31 - 0
	*(long*)MT3CS1 = 0;			// clear SPORT3 multichannel tx select, channels 63 - 32
	*(long*)MT3CS2 = 0;			// clear SPORT3 multichannel tx select, channels 95 - 64
	*(long*)MT3CS3 = 0;			// clear SPORT3 multichannel tx select, channels 127 - 96

	ClearDMA_Buffer(&SPORT13_RX_Buffer[0][0], (MAX_WORDS_PER_DMA_AT_SPORT13_RX * NUMBER_OF_SPORT13_BUFFERS));
	ClearDMA_Buffer(&SPORT13_TX_Buffer[0][0], (MAX_WORDS_PER_DMA_AT_SPORT13_TX * NUMBER_OF_SPORT13_BUFFERS));

// Now start with Init of SPORT13
	// SPORT1 is int. TDM Rx
	//
	// GENERATE_FS1		mode of FS1 in multichannel mode
	// SDEN_A			SPORT TXnA/RXnA DMA enable primary A channel
	// SCHEN_A			SPORT TXnA/RXnA DMA chaining enable primary A channel
	// SLEN32			serial length 32
	*(long*)SPCTL1 = (GENERATE_FS1 | SDEN_A | SCHEN_A | SLEN32);
	// SPORT3 is ext.TDM Tx
	//
	// LTDV		SPORT3 Tx data valid ena in TDM mode - TDV2 is low aktiv
	// SDEN_A	SPORT TXnA/RXnA DMA enable primary A channel
	// SCHEN_A	SPORT TXnA/RXnA DMA chaining enable primary A channel
	// SLEN32	serial length 32 - was taken out because for internal TDM bus this is not required
	*(long*)SPCTL3 = (SDEN_A | SCHEN_A | SLEN32);

	// Common control register for SPORT13
	//
//	*(long*)SP13MCTL = (((NUMBER_OF_TIMESLOTS_SPORT13 - 1)<<5) | MULTICHANNEL_FRAME_DELAY13 | MCE);// MCE not now! does not work!!!!
	*(long*)SP13MCTL = (((NUMBER_OF_TIMESLOTS_SPORT13 - 1)<<5) | MULTICHANNEL_FRAME_DELAY13);

	// Clock and FS divider register
	*(long*)DIV1 = FS_DIVIDER_SPORT13;
	*(long*)DIV3 = FS_DIVIDER_SPORT13;

	// Channel select register
	//
	*(long*)MR1CCS0 = 0;					// clear SPORT1 multichannel rx compand select, channels 31 - 0
	*(long*)MR1CCS1 = 0;					// clear SPORT1 multichannel rx compand select, channels 63 - 32
	*(long*)MR1CCS2 = 0;					// clear SPORT1 multichannel rx compand select, channels 95 - 64
	*(long*)MR1CCS3 = 0;					// clear SPORT1 multichannel rx compand select, channels 127 - 96
	*(long*)MR1CS0 = rx_channel_mask13[0];	// set SPORT1 multichannel rx select, channels 31 - 0
	*(long*)MR1CS1 = rx_channel_mask13[1];	// set SPORT1 multichannel rx select, channels 63 - 32
	*(long*)MR1CS2 = rx_channel_mask13[2];	// set SPORT1 multichannel rx select, channels 95 - 64
	*(long*)MR1CS3 = rx_channel_mask13[3];	// set SPORT1 multichannel rx select, channels 127 - 96

	*(long*)MT3CCS0 = 0;					// clear SPORT3 multichannel tx compand select, channels 31 - 0
	*(long*)MT3CCS1 = 0;					// clear SPORT3 multichannel tx compand select, channels 63 - 32
	*(long*)MT3CCS2 = 0;					// clear SPORT3 multichannel tx compand select, channels 95 - 64
	*(long*)MT3CCS3 = 0;					// clear SPORT3 multichannel tx compand select, channels 127 - 96
	*(long*)MT3CS0 = tx_channel_mask13[0];	// set SPORT3 multichannel tx select, channels 31 - 0
	*(long*)MT3CS1 = tx_channel_mask13[1];	// set SPORT3 multichannel tx select, channels 63 - 32
	*(long*)MT3CS2 = tx_channel_mask13[2];	// set SPORT3 multichannel tx select, channels 95 - 64
	*(long*)MT3CS3 = tx_channel_mask13[3];	// set SPORT3 multichannel tx select, channels 127 - 96

	SPORT13_DMA_init();
}

void InitSPORT02(void)
{
	asm ("bit clr imask SP0I | SP2I;");
	// clear all interrupts
	asm("bit clr mode1 IRPTEN;");
		
	// Stop DMA first and clear all registers
	*(long*)SPCTL0 = 0;			// clear SPORT0 serial port control register
	*(long*)SPCTL2 = 0;			// clear SPORT2 serial port control register
	*(long*)SP02MCTL = 0;		// clear SPORTs 0 & 2 Multichannel Control Register

	// CAUTION: Do not write into inactive registers - Manual page 10-7
	// This will cause a core hang and the reinitialisation of the SPORT won't work 
	//*(long*)TX0A = 0;			// clear all other non initialized SPORT0 registers
	//*(long*)TX0B = 0;			// prevent core hang - must not be touched
	*(long*)RX0A = 0;
	//*(long*)RX0B = 0;			// prevent core hang - must not be touched
	*(long*)DIV0 = 0;
	*(long*)CNT0 = 0;

	*(long*)TX2A = 0;			// clear all other non initialized SPORT2 registers
	//*(long*)TX2B = 0;			// prevent core hang - must not be touched
	//*(long*)RX2A = 0;			// prevent core hang - must not be touched
	//*(long*)RX2B = 0;			// prevent core hang - must not be touched
	*(long*)DIV2 = 0;
	*(long*)CNT2 = 0;

	*(long*)MR0CCS0 = 0;		// clear SPORT0 multichannel rx compand select, channels 31 - 0
	*(long*)MR0CCS1 = 0;		// clear SPORT0 multichannel rx compand select, channels 63 - 32
	*(long*)MR0CCS2 = 0;		// clear SPORT0 multichannel rx compand select, channels 95 - 64
	*(long*)MR0CCS3 = 0;		// clear SPORT0 multichannel rx compand select, channels 127 - 96
	*(long*)MR0CS0 = 0;			// clear SPORT0 multichannel rx select, channels 31 - 0
	*(long*)MR0CS1 = 0;			// clear SPORT0 multichannel rx select, channels 63 - 32
	*(long*)MR0CS2 = 0;			// clear SPORT0 multichannel rx select, channels 95 - 64
	*(long*)MR0CS3 = 0;			// clear SPORT0 multichannel rx select, channels 127 - 96

	*(long*)MT2CCS0 = 0;		// clear SPORT2 multichannel tx compand select, channels 31 - 0
	*(long*)MT2CCS1 = 0;		// clear SPORT2 multichannel tx compand select, channels 63 - 32
	*(long*)MT2CCS2 = 0;		// clear SPORT2 multichannel tx compand select, channels 95 - 64
	*(long*)MT2CCS3 = 0;		// clear SPORT2 multichannel tx compand select, channels 127 - 96
	*(long*)MT2CS0 = 0;			// clear SPORT2 multichannel tx select, channels 31 - 0
	*(long*)MT2CS1 = 0;			// clear SPORT2 multichannel tx select, channels 63 - 32
	*(long*)MT2CS2 = 0;			// clear SPORT2 multichannel tx select, channels 95 - 64
	*(long*)MT2CS3 = 0;			// clear SPORT2 multichannel tx select, channels 127 - 96

	ClearDMA_Buffer(&SPORT02_RX_Buffer[0][0], (MAX_WORDS_PER_DMA_AT_SPORT02_RX * NUMBER_OF_SPORT02_BUFFERS));
	ClearDMA_Buffer(&SPORT02_TX_Buffer[0][0], (MAX_WORDS_PER_DMA_AT_SPORT02_TX * NUMBER_OF_SPORT02_BUFFERS));

// Now start with Init of SPORT02
	// SPORT0 is ext. TDM Rx
	//
	// GENERATE_FS0		mode of FS0 in multichannel mode
	// SDEN_A			SPORT TXnA/RXnA DMA enable primary A channel
	// SCHEN_A			SPORT TXnA/RXnA DMA chaining enable primary A channel
	// SLEN32			serial length 32
//	*(long*)SPCTL0 = (ICLK | GENERATE_FS0 | SDEN_A | SCHEN_A | SLEN32);
	*(long*)SPCTL0 = SPORT02_RegisterSPCLT0;
	// SPORT2 is ext. TDM Tx
	//
	// LTDV		SPORT2 Tx data valid ena in TDM mode - TDV2 is low aktiv
	// SDEN_A	SPORT TXnA/RXnA DMA enable primary A channel
	// SCHEN_A	SPORT TXnA/RXnA DMA chaining enable primary A channel
	// SLEN32	serial length 32
	*(long*)SPCTL2 = SPORT02_RegisterSPCLT2;

	// Common control register for SPORT02
	//
//	*(long*)SP02MCTL = (((NUMBER_OF_TIMESLOTS_SPORT02 - 1)<<5) | MULTICHANNEL_FRAME_DELAY13 | MCE); // MCE not now! does not work!!!!
	*(long*)SP02MCTL = (((NUMBER_OF_TIMESLOTS_SPORT02 - 1)<<5) | MULTICHANNEL_FRAME_DELAY13);

	// Clock and FS divider register
	*(long*)DIV0 = FS_DIVIDER_SPORT02;
	*(long*)DIV2 = FS_DIVIDER_SPORT02;

	// Channel select register
	//
	*(long*)MR0CCS0 = 0;					// clear SPORT0 multichannel rx compand select, channels 31 - 0
	*(long*)MR0CCS1 = 0;					// clear SPORT0 multichannel rx compand select, channels 63 - 32
	*(long*)MR0CCS2 = 0;					// clear SPORT0 multichannel rx compand select, channels 95 - 64
	*(long*)MR0CCS3 = 0;					// clear SPORT0 multichannel rx compand select, channels 127 - 96
	*(long*)MR0CS0 = rx_channel_mask02[0];	// set SPORT0 multichannel rx select, channels 31 - 0
	*(long*)MR0CS1 = rx_channel_mask02[1];	// set SPORT0 multichannel rx select, channels 63 - 32
	*(long*)MR0CS2 = rx_channel_mask02[2];	// set SPORT0 multichannel rx select, channels 95 - 64
	*(long*)MR0CS3 = rx_channel_mask02[3];	// clear SPORT0 multichannel rx select, channels 127 - 96

	*(long*)MT2CCS0 = 0;					// clear SPORT2 multichannel tx compand select, channels 31 - 0
	*(long*)MT2CCS1 = 0;					// clear SPORT2 multichannel tx compand select, channels 63 - 32
	*(long*)MT2CCS2 = 0;					// clear SPORT2 multichannel tx compand select, channels 95 - 64
	*(long*)MT2CCS3 = 0;					// clear SPORT2 multichannel tx compand select, channels 127 - 96
	*(long*)MT2CS0 = tx_channel_mask02[0];	// set SPORT2 multichannel tx select, channels 31 - 0
	*(long*)MT2CS1 = tx_channel_mask02[1];	// set SPORT2 multichannel tx select, channels 63 - 32
	*(long*)MT2CS2 = tx_channel_mask02[2];	// set SPORT2 multichannel tx select, channels 95 - 64
	*(long*)MT2CS3 = tx_channel_mask02[3];	// clear SPORT2 multichannel tx select, channels 127 - 96

	SPORT02_DMA_init();
}

void SEGMENT_EXT_CODE Init_InternalTDM_BufferPtr(unsigned long *RxDMA_BufferPtr, unsigned long *TxDMA_BufferPtr)
{
	RxDMA_BufferPtr = (unsigned long*) &SPORT13_RX_Buffer[0][0];
	TxDMA_BufferPtr = (unsigned long*) &SPORT13_TX_Buffer[0][0];
}

void SEGMENT_EXT_CODE Init_ExternalTDM_BufferPtr(unsigned long *RxDMA_BufferPtr, unsigned long *TxDMA_BufferPtr)
{
	RxDMA_BufferPtr = (unsigned long*) &SPORT02_RX_Buffer[0][0];
	TxDMA_BufferPtr = (unsigned long*) &SPORT02_TX_Buffer[0][0];
}

void Restart_SPORT02(void)
{
	long DelayCounter = 0;

	IR_BlockCounter = 1;
	while (IR_BlockCounter)
	{
	}
	InitSPORT02();
	Start_SPORT02();
	// After a delay, start the Tx DMAs (keep order in DMA buffer)
	for (DelayCounter = 0; DelayCounter < 900; DelayCounter++)
	{
	}
	StartChainedDMA_SPORT02Tx();
}


