/*-------------------------------------------------------------------------*
 * $Workfile: ad6620.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : 
 * Remarks      :  
 * Purpose      : Ad6620 address defines
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/FE_DSP/H/ad6620.h $ 
 * 
 * 1     19.01.05 15:31 Chchlee
 * 
 * 1     28.05.04 13:29 Maag01
 * Initial version
 * 
 *-------------------------------------------------------------------------*/

#ifndef     _AD6620_H	
#define     _AD6620_H	

// Ad6620 internal register addresses
#define     AD6620_MCR          0x300  //  Mode control register
#define     AD6620_NCO_CR       0x301  //  NCO control register
#define     AD6620_NCO_SYNC_CR  0x302  //  NCO sync control register
#define     AD6620_NCO_FREQ     0x303  //  NCO frequency register
#define     AD6620_NCO_PHASE    0x304  //  NCO phase offset register
#define     AD6620_CIC2_SC      0x305  //  CIC2 scale register
#define     AD6620_CIC2_DEC     0x306  //  CIC2 decimation register ( Mcic2-1 )
#define     AD6620_CIC5_SC      0x307  //  CIC5 scale register
#define     AD6620_CIC5_DEC     0x308  //  CIC5 decimation register (  Mcic5-1  )
#define     AD6620_RCF_SC       0x309  //  RCF scale register
#define     AD6620_RCF_DEC      0x30a  //  RCF decimation register (  Mrcf-1 )
#define     AD6620_RCF_OFF      0x30b  //  RCF address offset register
#define     AD6620_RCF_NTAPS    0x30c  //  RCF number of taps register ( Ntaps-1 )
#define     AD6620_RESERVED     0x30d  //  Reserved register (Should be written 0 !!!!)


#define     DOWN_CONV_BASE       0x08000000	// base address of downconverter

#define     AD6620_DR0  (DOWN_CONV_BASE + 0x000) // AD6620 DR0 Data register d[7:0]
#define     AD6620_DR1  (DOWN_CONV_BASE + 0x001) // AD6620 DR1 Data register d[15:8]
#define     AD6620_DR2  (DOWN_CONV_BASE + 0x002) // AD6620 DR2 Data register d[23:16]
#define     AD6620_DR3  (DOWN_CONV_BASE + 0x003) // AD6620 DR3 Data register d[31:24]
#define     AD6620_DR4  (DOWN_CONV_BASE + 0x004) // AD6620 DR4 Data register d[35:32]
#define     AD6620_RES  (DOWN_CONV_BASE + 0x005) // AD6620 Reserved
#define     AD6620_LAR  (DOWN_CONV_BASE + 0x006) // AD6620 Low Address Register  (LAR) A[7:0]
#define     AD6620_AMR  (DOWN_CONV_BASE + 0x007) // AD6620 Address Mode Register (AMR) 
                                                 // 1-0: A[9:8]
                                                 // 5-2: Reserved
												 //   6: Read Increment
                                                 //   7: Write Increment



#endif
