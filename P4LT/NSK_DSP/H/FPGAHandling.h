/*-------------------------------------------------------------------------*
 * $Workfile: FPGAHandling.h $
 * Part of      : ETL600 / System
 * Language     : C
 * Created by   : Alexander Gygax, PTUKT2
 * Remarks      :  
 * Purpose      : FPGA function declarations
 * Copyright    : ABB Switzerland Ltd. 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/NSK_DSP/H/FPGAHandling.h $
 * 
 * 4     28.06.05 18:46 Chmibuh
 * replace bool with BOOL
 * 
 * 3     16.02.05 10:40 Chalgyg
 * FPGA Interface changed to addressbus mode.
 * 
 * 2     20.01.05 13:44 Chalgyg
 * 
 * 10    19.01.05 15:11 Chalgyg
 * New function ReadSubprintID(void).
 * 
 * 9     17.01.05 16:05 Chalgyg
 * OFDM functions removed.
 * 
 * 8     14.01.05 10:56 Chalgyg
 * Return value of FPGAHandling_ConfigFPGA() is now bool.
 * 
 * 7     28.09.04 17:45 Maag01
 * 
 * 4     28.06.04 17:17 Unp00611
 * 
 * 1     22.06.04 14:37 Unp00611
 * 
 * 2     17.06.04 16:20 Gygax02
 * Some functions added
 * 
 * 1     3.06.04 15:40 Gygax02
 *-------------------------------------------------------------------------*/

#ifndef FPGAHANDLING_H
#define FPGAHANDLING_H

/* PUBLIC FUNCTIONS ********************************************************/
BOOL FPGAHandling_ConfigFPGA(void);
unsigned long ReadSubprintID(void);

#endif
/* End of File *************************************************************/
