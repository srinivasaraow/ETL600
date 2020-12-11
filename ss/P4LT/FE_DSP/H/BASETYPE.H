/*-------------------------------------------------------------------------*
 * $Workfile: BASETYPE.H $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Christian Leeb
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source O4CV/O4CV/H/BASETYPE.H $ 
 * 
 * 52    5.04.07 8:46 Chrobif
 * O4CV compiler specifics added.
 * 
 * 50    25.08.06 11:23 Chsanoh
 * Classes
 * - CAES600GroupB
 * - CAES600Interface
 * - CAES600Interface0
 * - CAES600Mode
 * - CAES600System
 * - CAES600Unblock
 * revised.
 * 
 * 46    6.11.05 15:21 Chchlee
 * Rolled back
 * 
 * 45    6.11.05 15:14 Chchlee
 * Chris' new version
 * 
 * 44    19.08.05 16:21 Chhamaa
 * Defines SEGMENT_DM_DATA2, SEGMENT_PM_DATA2 added
 * 
 * 43    30.06.05 8:08 Chchlee
 * cleaned up
 * 
 * 42    14.06.05 17:30 Chsanoh
 * 
 * 41    21.03.05 18:12 Chchlee
 * memory.h replaced by string.h
 * 
 * 40    21.03.05 17:47 Chchlee
 * #include "memory.h" added
 * 
 * 39    8.03.05 7:35 Chchlee
 * MEMSET now mapped to memset()
 * 
 * 38    25.02.05 17:21 Chhesch5
 * 
 * 37    28.01.05 15:43 Chhamaa
 * #define SEGMENT_EXT_CODE  for VISUALC modified
 * 
 * 36    27.01.05 13:44 Chhamaa
 * #define SEGMENT_EXT_CODE  for VISUALC added
 * 
 * 35    3.01.05 10:39 Chchlee
 * __min(a,b) added
 * 
 * 34    30.12.04 15:29 Chchlee
 * SET_BITS macro added
 * 
 * 33    1.11.04 18:01 Maag01
 * MEMCPY() calls memcpy()
 * 
 * 32    29.09.04 11:08 Maag01
 * 
 * 30    15.09.04 11:37 Rytz01
 * #define SB14	14  inserted
 * 
 * 29    9.09.04 15:18 Buhl01
 * MASK_5LSB - MASK_7LSB added
 * 
 * 28    1.09.04 13:34 Rytz01
 * #define SEGMENT_DMA inserted
 * 
 * 27    25.08.04 18:20 Leeb02
 * MEMSET added
 * 
 * 26    18.08.04 14:22 Leeb02
 * Bug in MEMCPY fixed
 * 
 * 25    13.08.04 9:02 Leeb02
 * compiler switch "NO_EXT_RAM" added
 * 
 * 24    12.08.04 13:24 Leeb02
 * MEMCPY added
 * 
 * 23    11.08.04 14:58 Maag01
 * 
 * 21    2.08.04 18:54 Buhl01
 * SEGMENT_EXT_SLAVE_CODE added
 * 
 * 20    2.08.04 16:37 Leeb02
 * 
 * 19    2.08.04 9:10 Leeb02
 * NR_OF_BITS_IN_BYTE added
 * 
 * 18    25.07.04 13:35 Leeb02
 * NR_OF_BYTE_IN_LONG replaced by NR_OF_BYTES_IN_LONG
 * 
  *-------------------------------------------------------------------------*/

#ifndef BASETYPE_H
#define BASETYPE_H

/* INCLUDE FILES **************************************************************/

#include <string.h>

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

// begin config struct specifics
// !!!!!!!!!!!!!! do not checkin without this define commented out !!!!!!!!!!!
// #define		NEW_CONFIG_STRUCT_V020
// end config struct specifics


// begin of compiler specifics
#ifdef VISUALC
	#define SEGMENT_EXT_CODE 
	#define SEGMENT_EXT_DATA
#else
	#ifdef O4CV
		#define SEGMENT_EXT_CODE 
		#define SEGMENT_EXT_DATA
	#else
		#ifdef NO_EXT_RAM
			#define SEGMENT_EXT_CODE 
			#define SEGMENT_EXT_DATA 
		#else
			#define SEGMENT_EXT_CODE	segment ("ext_code")
			#define SEGMENT_EXT_DATA	segment ("ext_dmda")
		#endif
			#define SEGMENT_EXT_SLAVE_CODE	segment ("ext_slaveCode")
			#define SEGMENT_DMA				segment ("seg_dma")
		
			#define SEGMENT_DM_DATA2	segment ("seg_dmData")
			#define SEGMENT_PM_DATA2	segment ("seg_pmData")
	#endif
#endif
// end of compiler specifics


// define NULL, TRUE, FALSE if not already done
#ifndef NULL
  #define NULL (void*)0
#endif

#ifndef TRUE
  #define TRUE (1==1)
#endif

#ifndef FALSE
  #define FALSE (!TRUE)
#endif

#define NR_OF_BYTES_IN_LONG		4  
#define NR_OF_BITS_IN_LONG		32
#define NR_OF_BITS_IN_BYTE		8

#define PI				          3.14159265359
#define TWO_PI				      6.28318530718


#define P4LT_ZERO    0x00000000
#define P4LT_ONES    0xffffffff

// ShiftBy N defines:
#define SB2		2    
#define SB4		4    
#define SB8		8    
#define SB12	12
#define SB14	14    
#define SB16	16    
#define SB24	24 


#define BIT0    (1<< 0)
#define BIT1    (1<< 1)
#define BIT2    (1<< 2)
#define BIT3    (1<< 3)
#define BIT4    (1<< 4)
#define BIT5    (1<< 5)
#define BIT6    (1<< 6)
#define BIT7    (1<< 7)
#define BIT8    (1<< 8)
#define BIT9    (1<< 9)
#define BIT10   (1<<10)
#define BIT11   (1<<11)
#define BIT12   (1<<12)
#define BIT13   (1<<13)
#define BIT14   (1<<14)
#define BIT15   (1<<15)
#define BIT16   (1<<16)
#define BIT17   (1<<17)
#define BIT18   (1<<18)
#define BIT19   (1<<19)
#define BIT20   (1<<20)
#define BIT21   (1<<21)
#define BIT22   (1<<22)
#define BIT23   (1<<23)
#define BIT24   (1<<24)
#define BIT25   (1<<25)
#define BIT26   (1<<26)
#define BIT27   (1<<27)
#define BIT28   (1<<28)
#define BIT29   (1<<29)
#define BIT30   (1<<30)
#define BIT31   0x80000000	//(1<<31)

#define MASK_4LSB						(BIT0 |BIT1 |BIT2 |BIT3)
#define MASK_5LSB						(BIT0 |BIT1 |BIT2 |BIT3| BIT4)
#define MASK_6LSB						(BIT0 |BIT1 |BIT2 |BIT3| BIT4 |BIT5)
#define MASK_7LSB						(BIT0 |BIT1 |BIT2 |BIT3| BIT4 |BIT5 |BIT6)
#define MASK_8LSB						(BIT0 |BIT1 |BIT2 |BIT3| BIT4 |BIT5 |BIT6 |BIT7)
#define MASK_16LSB						(BIT0 |BIT1 |BIT2 |BIT3| BIT4 |BIT5 |BIT6 |BIT7 \
											|BIT8 |BIT9 |BIT10 |BIT11| BIT12 |BIT13 |BIT14 |BIT15)
#define MASK_16MSB						0xffff0000
	
#define MASK_24LSB						0x00ffffff 
#define MASK_24MSB						0xffffff00

#define BYTE1	(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7)
#define BYTE2	(BIT8 | BIT9 | BIT10 | BIT11 | BIT12 | BIT13 | BIT14 | BIT15)
#define BYTE3	(BIT16 | BIT17 | BIT18 | BIT19 | BIT20 | BIT21 | BIT22 | BIT23)	
#define BYTE4	(BIT24 | BIT25 | BIT26 | BIT27 | BIT28 | BIT29 | BIT30 | BIT31)

#ifndef BOOL

	#ifndef _WINDEF_
		
		#define BOOL unsigned long
		
	#endif

#endif
	


/* PUBLIC TYPE DECLARATIONS ***************************************************/


/* PUBLIC MACRO DEFINITIONS ***************************************************/

// Normally, part of stdlib.h. Not the case for analog devices.
#define __min(a,b)  (((a) < (b)) ? (a) : (b))


#define SET_BITS(condition, data, mask)\
	if (condition) (data) |= (mask);\
	else           (data) &= ~(mask);

#define GET_SET_LSB(ulValue)\
( \
	(ulValue & BIT0)  ?  0 : \
	(ulValue & BIT1)  ?  1 : \
	(ulValue & BIT2)  ?  2 : \
	(ulValue & BIT3)  ?  3 : \
	(ulValue & BIT4)  ?  4 : \
	(ulValue & BIT5)  ?  5 : \
	(ulValue & BIT6)  ?  6 : \
	(ulValue & BIT7)  ?  7 : \
	(ulValue & BIT8)  ?  8 : \
	(ulValue & BIT9)  ?  9 : \
	(ulValue & BIT10) ? 10 : \
	(ulValue & BIT11) ? 11 : \
	(ulValue & BIT12) ? 12 : \
	(ulValue & BIT13) ? 13 : \
	(ulValue & BIT14) ? 14 : \
	(ulValue & BIT15) ? 15 : \
	(ulValue & BIT16) ? 16 : \
	(ulValue & BIT17) ? 17 : \
	(ulValue & BIT18) ? 18 : \
	(ulValue & BIT19) ? 19 : \
	(ulValue & BIT20) ? 20 : \
	(ulValue & BIT21) ? 21 : \
	(ulValue & BIT22) ? 22 : \
	(ulValue & BIT23) ? 23 : \
	(ulValue & BIT24) ? 24 : \
	(ulValue & BIT25) ? 25 : \
	(ulValue & BIT26) ? 26 : \
	(ulValue & BIT27) ? 27 : \
	(ulValue & BIT28) ? 28 : \
	(ulValue & BIT29) ? 29 : \
	(ulValue & BIT30) ? 30 : \
	(ulValue & BIT31) ? 31 : \
						 0   \
)

#define MEMCPY(to , from, n_longs)		memcpy(to, from, n_longs)
#define MEMSET(to , value, n_longs)     memset(to, value, n_longs)


/* PUBLIC FUNCTIONS ***********************************************************/



#endif
