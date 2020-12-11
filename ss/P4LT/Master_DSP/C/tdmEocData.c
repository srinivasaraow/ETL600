/*-------------------------------------------------------------------------*
 * $Workfile: tdmEocData.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Functions to transfer EOC data on internal TDM bus
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT/FE_DSP/C/tdmEocData.c $ 
 * 
 * 2     16.08.05 15:23 Chhamaa
 * tdmEocData_transfer() modified: data words in the write buffer are
 * transferd to the remote DSP without checking the full flag. The process
 * using the write buffer has to do it.
 * 
 * 1     19.01.05 15:31 Chchlee
 * 
 * 6     6.01.05 13:37 Chhamaa
 * 
 * 5     17.12.04 17:21 Chhamaa
 * 
 * 4     10.11.04 10:54 Maag01
 * 
 * 3     5.11.04 14:48 Maag01
 * 
 * 2     4.11.04 19:27 Maag01
 * 
 * 1     4.11.04 16:14 Maag01
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include "basetype.h"
#include "tdmEocData.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define N_EOC_CHANNELS		3		// number of EOC channels
#define EOC_CH1				0		// EOC ch1 index
#define EOC_CH2				1		// EOC ch2 index
#define EOC_CH3				2		// EOC ch3 index


/* LOCAL VARIABLES ***********************************************************/

EocDataBufferType *g_readBufPtr[N_EOC_CHANNELS];
EocDataBufferType *g_writeBufPtr[N_EOC_CHANNELS];


/* IMPLEMENTATION ************************************************************/

#pragma optimize_off

/*****************************************************************************
  Function:		tdmEocData_init()	
  Description:	Initializes EOC data transfer on internal TDM bus
  Inputs:		Arrays of read and write buffers with three elements
  Outputs:		None
  Return:		None
  Globals:		None
  Important:	Has to be called once before using tdmEocData_transfer()

*******************************************************************************/

void SEGMENT_EXT_CODE tdmEocData_init(EocDataBufferType readBuffer[], EocDataBufferType writeBuffer[])
{
	long i;

	
	for(i = 0; i < N_EOC_CHANNELS; i++) {
	
		// initialize g_readBufPtr
		g_readBufPtr[i] = &readBuffer[i];

		// initialize g_writeBufPtr
		g_writeBufPtr[i] = &writeBuffer[i];
	};
	
}


#pragma optimize_as_cmd_line

/*****************************************************************************
  Function:		tdmEocData_transfer()	
  Description:	Transfers data of three EOC channels between master and FE DSP
  Inputs:		Pointers to TDM bus send and receive frames
  Outputs:		None
  Return:		None
  Globals:		None
  Important:	Call tdmEocData_init() once before using

*******************************************************************************/

void tdmEocData_transfer(t_EOC_data *tdmEocDataReadPtr, t_EOC_data *tdmEocDataWritePtr)
{
	static long chId;
	static long chIndex;


	// read EOC data break from TDM bus (used only to indicate pilot failure to transmit data)
	g_writeBufPtr[EOC_CH1]->dataBreak = tdmEocDataReadPtr->dataBreakCh1;
	g_writeBufPtr[EOC_CH2]->dataBreak = tdmEocDataReadPtr->dataBreakCh2;
	g_writeBufPtr[EOC_CH3]->dataBreak = tdmEocDataReadPtr->dataBreakCh3;

	// read EOC data flag full from TDM bus
	g_writeBufPtr[EOC_CH1]->full = tdmEocDataReadPtr->bufferFullCh1;
	g_writeBufPtr[EOC_CH2]->full = tdmEocDataReadPtr->bufferFullCh2;
	g_writeBufPtr[EOC_CH3]->full = tdmEocDataReadPtr->bufferFullCh3;

	// read channel ID 
	chId = tdmEocDataReadPtr->channel_id;

	// check range of channel ID and if corrrect read data
	if ((chId > 0) && (chId <= N_EOC_CHANNELS)) {
		
		chIndex = chId - 1;
		g_readBufPtr[chIndex]->data = tdmEocDataReadPtr->data;
		g_readBufPtr[chIndex]->full = 1;	// flag is checked on writing to buffer on the remote DSP
	};


	// write buffer status data to TDM bus
	tdmEocDataWritePtr->bufferFullCh1 = g_readBufPtr[EOC_CH1]->full;
	tdmEocDataWritePtr->bufferFullCh2 = g_readBufPtr[EOC_CH2]->full;	
	tdmEocDataWritePtr->bufferFullCh3 = g_readBufPtr[EOC_CH3]->full;

	tdmEocDataWritePtr->dataBreakCh1 = g_readBufPtr[EOC_CH1]->dataBreak;
	tdmEocDataWritePtr->dataBreakCh2 = g_readBufPtr[EOC_CH2]->dataBreak;
	tdmEocDataWritePtr->dataBreakCh3 = g_readBufPtr[EOC_CH3]->dataBreak;

	// reset channel ID
	tdmEocDataWritePtr->channel_id = 0;
	
	// write one data word if available
	for(chIndex = 0; chIndex < N_EOC_CHANNELS; chIndex++) {

		if (g_writeBufPtr[chIndex]->data != 0) { // new data word available

			tdmEocDataWritePtr->channel_id = chIndex + 1;
			tdmEocDataWritePtr->data = g_writeBufPtr[chIndex]->data;
			g_writeBufPtr[chIndex]->data = 0;	// reset data word
			break;	// exit loop	
		}
	}
}





