/*-------------------------------------------------------------------------*
 * $Workfile: FPGA_Types.h $
 * Part of      : ETL600 / System
 * Language     : C
 * Created by   : Alexander Gygax, PTUKT2
 * Remarks      :  
 * Purpose      : FPGA type declarations
 * Copyright    : ABB Switzerland Ltd. 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/P4LT/NSK_DSP/H/FPGA_Types.h $
 * 
 * 10    10-10-20 18:49 Chhamaa
 * FPGALENGTH modified for new FPGA.
 * 
 * 3     18.05.05 11:41 Chalgyg
 * FPGA loading routine tries 3 times.
 * 
 * 2     20.01.05 13:31 Chalgyg
 * New FPGA length, because of Spartan-3
 * 
 * 5     19.01.05 15:10 Chalgyg
 * Subprint ID Flags added.
 * 
 * 4     17.01.05 16:00 Chalgyg
 * Cosmetics
 * 
 * 3     14.01.05 10:55 Chalgyg
 * Mask WR_BIT added.
 * 
 * 2     17.06.04 16:20 Gygax02
 * 
 * 1     3.06.04 15:40 Gygax02
 * 
 * 4     3.06.04 13:27 Gygax02
 * 
 * 3     18.05.04 11:56 Gygax02
 * 
 * 2     30.03.04 16:10 Gygax02
 * Initial version
 *-------------------------------------------------------------------------*/

#ifndef FPGA_TYPES_H
#define FPGA_TYPES_H

/* INCLUDE FILES ***********************************************************/
#include <def21161.h>

/* CONSTANT DECLARATIONS ***************************************************/
#define FPGA_BUFFER			0x0C000000

#define BYTEMASK			0x000000FF
#define STD_ABORT_COUNTER	0x01000000
#define END_COUNTER			0x00008000
#define NUMBER_OF_NOPS		0x00001000

#define FPGALENGTH			162962			//For XC3S1500-FG320-4
#define TRIES_TO_LOAD_FPGA	3

#define SUBPRINT_ID			0x0000000E		//Flag 05/06/07	Input
#define RESET				0x00000010		//Flag 08		Output
#define PROG				0x00000020		//Flag 09		Output
#define INIT				0x00000040		//Flag 10		Input
#define DONE				0x00000080		//Flag 11		Input

#define WR_BIT				0x00000080		//To write in an FPGA register the MSB must be '1'

#endif
/* End of File *************************************************************/
