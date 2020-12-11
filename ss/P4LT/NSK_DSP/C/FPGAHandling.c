/*-------------------------------------------------------------------------*
 * $Workfile: FPGAHandling.c $
 * Part of      : ETL600 / System
 * Language     : C
 * Created by   : Alexander Gygax, PTUKT2
 * Remarks      :  
 * Purpose      : FPGA functions
 * Copyright    : ABB Switzerland Ltd. 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/NSK_DSP/C/FPGAHandling.c $
 * 
 * 4     28.06.05 18:46 Chmibuh
 * replace bool with BOOL
 * 
 * 3     16.02.05 10:40 Chalgyg
 * FPGA Interface changed to addressbus mode.
 * 
 * 2     20.01.05 13:42 Chalgyg
 * 
 * 13    19.01.05 15:08 Chalgyg
 * New function ReadSubprintID(void).
 * 
 * 12    17.01.05 16:26 Chalgyg
 * Cosmetics
 * 
 * 11    14.01.05 10:54 Chalgyg
 * Alarm handling implemented.
 * 
 * 10    10.01.05 17:42 Chalgyg
 * Cosmetics
 * 
 * 9     22.07.04 16:05 Gygax02
 * 
 * 8     9.07.04 15:25 Gygax02
 * 
 * 7     9.07.04 13:34 Gnus01
 * 
 * 6     9.07.04 11:40 Gnus01
 * 
 * 5     9.07.04 11:12 Gnus01
 * 
 * 4     9.07.04 10:02 Gnus01
 * 
 * 3     21.06.04 11:40 Gygax02
 * Only common functions are included yet.
 * 
 * 2     17.06.04 16:19 Gygax02
 * Some functions added
 * 
 * 1     3.06.04 15:40 Gygax02
 * 
 * 3     3.06.04 13:26 Gygax02
 * FPGA is loadable
 * 
 * 2     30.03.04 16:10 Gygax02
 * Initial version
 *-------------------------------------------------------------------------*/

#include "FPGA_Types.h"
#include "BASETYPE.H"

asm("#include <def21161.h>");

// for the external program to patch the fpga code into the dxe file, it is nessessary to 
// keep a start pattern 0xaaaaaaaa, 0xbbbbbbbb, 0xcccccccc, 0xdddddddd, 0x12345678 at the beginning of the array.
// This will be overwritten by the patch programm - do not change this following line
segment("ext_fpga")	long FPGAData[FPGALENGTH+1] = {0xaaaaaaaa, 0xbbbbbbbb, 0xcccccccc, 0xdddddddd, 0x12345678};

BOOL FPGAHandling_ConfigFPGA(void)  //Does NOT run in external code!!!
{
	long i = 0;
	long abort = 0;
	long end_count = 0;
	long number_of_tries = 0;
	long tempreg;

	tempreg = (*(long*)IOFLAG) & (~PROG);
	*(long*)IOFLAG = tempreg;														// Tie PROG low to set DONE low.

	while (((*(long*)IOFLAG) & DONE) && (abort<STD_ABORT_COUNTER)){
		abort++;																	// No reaction from FPGA yet.
	}
	 
	if (abort>=STD_ABORT_COUNTER){
		// mark error
		tempreg = (*(long*)IOFLAG) | PROG;
		*(long*)IOFLAG = tempreg;													// Set PROG high, don't delay configuration.
	}

	abort = 0;

	while (!((*(long*)IOFLAG) & DONE) && (number_of_tries <= TRIES_TO_LOAD_FPGA)){	// Program until DONE goes high.
		tempreg = (*(long*)IOFLAG) & (~PROG);
		*(long*)IOFLAG = tempreg;													// Let PROG low.

		while (((*(long*)IOFLAG) & INIT) && (abort<STD_ABORT_COUNTER)){
			abort++;																// Wait until INIT goes low.
		}
		abort = 0;																	// FPGA is clearing memory now.

		tempreg = (*(long*)IOFLAG) | PROG;
		*(long*)IOFLAG = tempreg;													// Set PROG high, don't delay configuration.
		
		while ((!((*(long*)IOFLAG) & INIT)) && (abort<STD_ABORT_COUNTER)){
			abort++;																// Wait until INIT goes high, FPGA ready for loading.
		} 
		abort = 0;

		for (i=0; i<=(FPGALENGTH-1); ++i){
			*(long*)FPGA_BUFFER = BYTEMASK & (FPGAData[i] >> 24);
			*(long*)FPGA_BUFFER = BYTEMASK & (FPGAData[i] >> 16);
			*(long*)FPGA_BUFFER = BYTEMASK & (FPGAData[i] >> 8);
			*(long*)FPGA_BUFFER = BYTEMASK & FPGAData[i];
		}
		number_of_tries++;

		for (end_count=0; end_count<END_COUNTER; end_count++){
			asm("nop;");
		} 
	} 

	if (!((*(long*)IOFLAG) & INIT)){
		return(TRUE);												// Failure in FPGA configuration (wrong CRC)
	}
	
	if (number_of_tries <= TRIES_TO_LOAD_FPGA){
		tempreg = (*(long*)IOFLAG) & (~RESET);
		*(long*)IOFLAG = tempreg;									// Reset FPGA
		for (i=0; i<=NUMBER_OF_NOPS; i++){
			asm("nop;");
		}
		tempreg = (*(long*)IOFLAG) | RESET;
		*(long*)IOFLAG = tempreg;									// Release reset
		return(FALSE);												// FPGA loaded successfully
	}
	else {
		return(TRUE);												// FPGA not configurable
	}
}

unsigned long SEGMENT_EXT_CODE ReadSubprintID(void)
{
	unsigned long tempreg;

	tempreg = ((*(long*)IOFLAG) & SUBPRINT_ID) >> 1;
	return(tempreg);
}

/* End of File *************************************************************/
