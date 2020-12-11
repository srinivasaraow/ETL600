/*-------------------------------------------------------------------------*
 * $Workfile: upDownConvTdmBus.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Functions to read and write the up/down-converter TDM bus
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/UC_DSP/C/upDownConvTdmBus.c $ 
 * 
 * 2     10.02.05 16:09 Chbrbus
 * Reduced upconverter TDM bus DMA buffer - changed interrupt routine
 * 
 * 1     19.01.05 15:31 Chchlee
 * 
 * 5     4.01.05 11:41 Chhamaa
 * 
 * 4     26.10.04 18:58 Maag01
 * 
 * 3     7.10.04 15:43 Maag01
 * 
 * 2     7.10.04 14:28 Maag01
 * 
 * 1     22.07.04 18:27 Maag01
 * Initial Version
 * 
 *
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include <stdlib.h>

#include "basetype.h"
#include "upDownConvTdmBus.h"
#include "TDM_Init.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		upDownConvTdmBus_FE_read()
  Description:	TDM bus read function for FE DSP
  Inputs:		*rxBufferPtr: pointer to receive buffer
  Outputs:		vSamples: sample array
  Return:		None
  Globals:		None
  Important:	-

*******************************************************************************/

void upDownConvTdmBus_FE_read(long vSamples[], volatile long *rxBufferPtr)
{
	long i;

	for (i = 0; i < 12; i++) {

		vSamples[i] = (*(rxBufferPtr));				// real part
		vSamples[i + 12] = (*(rxBufferPtr + 1));	// imaginary part
		rxBufferPtr += 8;
	};
}


/*****************************************************************************
  Function:		upDownConvTdmBus_FE_write()
  Description:	TDM bus write function for FE DSP
  Inputs:		*txBufferPtr: pointer to send buffer
				vSamples: sample array 
  Outputs:		None
  Return:		None
  Globals:		None
  Important:	-

*******************************************************************************/

void upDownConvTdmBus_FE_write(complex_float vSamples[], volatile long *txBufferPtr)
{
	long i;

	for (i = 0; i < 12; i++) {

		(*((float*)txBufferPtr + 1)) = vSamples[i].re;		// time slot 1
		(*((float*)txBufferPtr + 2)) = vSamples[i].im;		// time slot 2
		txBufferPtr += 8;
	};
}


/*****************************************************************************
  Function:		upDownConvTdmBus_UC_read()
  Description:	TDM bus read function for UC DSP
  Inputs:		*rxBufferPtr: pointer to receive buffer
				vSamples: sample array 
  Outputs:		None
  Return:		None
  Globals:		None
  Important:	-

*******************************************************************************/

// CAUTION * CAUTION * CAUTION * CAUTION * CAUTION * CAUTION * CAUTION * CAUTION * CAUTION * CAUTION * CAUTION *
//
// Concept of acessing SPORT02 Rx Buffer pointer has changed: NEW CONCEPT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Because we don't use SPORT02 Tx for anything on UC, no change  with DMA2
//
//
//
//	SPORT02 runs with 6*10.666 kHz
//  UC DSP is running with 10.666 kHz - 6 SPORT02 Interrups will occur during a main cycle of UC DSP
//  For optimal low delay only 12 DMA buffers are required. But we have to update the DMA buffer pointers
//  6 times during a Main() cycle !!!!!!!
//  There will be tough timing requirements for data exchange.
//
//	TRICK: We use a buffer pointer array which has the start adress of every DMA buffer.
//	Acessing data can be done by incrementing the adress of the interrupt routine returns  
//
//
//	------------------------------ 10.666 kHz cycle
//			|TCB #0      GP=0  | -> DMA Rx Buffer #0
//			--------------------
//			|TCB #1      GP=1  | -> DMA Rx Buffer #1			N = 6
//			--------------------
//			|TCB #2      GP=2  | -> DMA Rx Buffer #2
//		N	--------------------
//			|TCB #3      GP=3  | -> DMA Rx Buffer #3
//			--------------------
//			|TCB #4      GP=4  | -> DMA Rx Buffer #4
//			--------------------
//			|TCB #5      GP=5  | -> DMA Rx Buffer #5
//	------------------------------ 10.666 kHz cycle
//			|TCB #6      GP=6  | -> DMA Rx Buffer #6
//			--------------------
//			|TCB #7      GP=7  | -> DMA Rx Buffer #7
//			--------------------
//			|TCB #8      GP=8  | -> DMA Rx Buffer #8
//		N	--------------------
//			|TCB #9      GP=9  | -> DMA Rx Buffer #9
//			--------------------
//			|TCB #10     GP=10 | -> DMA Rx Buffer #10
//			--------------------
//			|TCB #11     GP=11 | -> DMA Rx Buffer #11
//	------------------------------ 10.666 kHz cycle
//							|
//							|
//					Is used in Interrupt function (is checked if range 0...11 is ok)
//							|
//							------
//								  |		SPORT02_RxBufferLookupTable, is initialized in TDM_Init.c
//			----------------------------------------------------------
//								|   |   Adress Position 0 Buffer #10 |
//					|			--------------------------------------
//					|			|   |   Adress Position 0 Buffer #9  |
//					|			--------------------------------------
//					|			|   |   Adress Position 0 Buffer #8  |
//					|			--------------------------------------
//					|			|   |   Adress Position 0 Buffer #7  |
//					|			--------------------------------------
//					|			|   |   Adress Position 0 Buffer #6  |
//					|			--------------------------------------
//					|			| 11|   Adress Position 0 Buffer #5  |
//					|			--------------------------------------
//					|			| 10|   Adress Position 0 Buffer #4  |
//					|			--------------------------------------
//					|			|  9|   Adress Position 0 Buffer #3  |
//					|			--------------------------------------
//				(3 * N) - 1		|  8|   Adress Position 0 Buffer #2  |  ---------------------->
//					|			--------------------------------------	Interrupt routine returns 
//					|			|  7|   Adress Position 0 Buffer #1  |	in _externalTDM_Rx_Ptr the 
//					|			--------------------------------------	Adress to the SPORT02_RxBufferLookupTable 
//					|			|  6|   Adress Position 0 Buffer #0  |	containing the adress of the oldest	
//					|			--------------------------------------	DMA Rx Buffer
//					|			|  5|   Adress Position 0 Buffer #11 |	
//					|			--------------------------------------	acessing the 6 previous received frames is	 
//					|			|  4|   Adress Position 0 Buffer #10 |	done by saving the returned externalTDM_Rx_Ptr
//					|			--------------------------------------  and then using this temporary adress by getting
//					|			|  3|   Adress Position 0 Buffer #9  |  the values out of the buffers by	
//					|			--------------------------------------  *(TempAdress)     : oldest DMA Rx buffer Adress Position 0
//					|			|  2|   Adress Position 0 Buffer #8  |  *(TempAdress + 1) :			.			.
//					|			--------------------------------------  *(TempAdress + 2) :			.
//					|			|  1|   Adress Position 0 Buffer #7  |  *(TempAdress + 3) :			.
//					|			--------------------------------------  *(TempAdress + 4) :			.
//								|  0|   Adress Position 0 Buffer #6  |  *(TempAdress + 5) : newest DMA Rx Buffer Adress Position 0
//			---------------------------------------------------------- 
//	
//		CAUTION TIMING !! CAUTION TIMING !! CAUTION TIMING !! CAUTION TIMING !! CAUTION TIMING !! CAUTION TIMING !!!!!!!!!!!!!!!!!!!!!!!!!
//		Because this externalTDM_Rx_Ptr changes during every SPORT02 Rx Interrupt, getting the values out of  
//		the DMA buffers has to be VERY QUICK and always at the beginning of the Main case after the while (.....).
//		ALL NESESSARY VALUES has to be copied out of the  DMA buffer BEFORE a NEW SPORT02 INTERRUPT occurs !!!!!!!!!!!!!
//
//      /END CAUTION

void upDownConvTdmBus_UC_read(complex_float vSamples[], long *RxDMA_BufferArrayPtr)
{
	long i;
	long TempPtr;

	// Check if offset in SPORT02 Rx Buffer Pointer Array is within range

	for (i = 0; i < 6; i++) {
		TempPtr = *(RxDMA_BufferArrayPtr + i);
		vSamples[i].re = *((float*) (TempPtr + 2));
		vSamples[i].im = *((float*) (TempPtr + 3));
	};
}


/*****************************************************************************
  Function:		checkFloatVal()
  Description:	Checks arrays of floating point values. An invalid value
				(0xFFFFFFFF) is set to 0
  Inputs:		val[]: floating point array
				nVal: number of values
  Outputs:		None
  Return:		None
  Globals:		None
  Important:	-

*******************************************************************************/
/*
void checkFloatVal(float val[], long nVal)
{
	long i;


	for(i = 0; i < nVal; i++) {

		if(((unsigned long)val[i]) == 0xFFFFFFFF) {

			val[i] = 0.0;
		};
	};
}
*/

