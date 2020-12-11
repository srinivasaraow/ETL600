/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: vgauss_f_D.asm $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Generates vector with gaussian distributed elements
 *
 * REVISION HISTORY: Original function rangauss() from time to time 
 *                   may produce NAN in register f0, thus test for 
 *                   NAN is added.
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vgauss_f_D.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 * 
 * 
 * 1     08.09.2003  vgp
 * Initial version
 * 2     13.01.2004 vgp 
 * Modified to prevent dependecies from MODE1 register settings
 *****************************************************************************/

/******************************************************************************
 *
 * PROJECT     : SpeedDSP  
 * FILE        : $Workfile: vgauss_f_D.asm $
 * COPYRIGHT   : 1998 by LightSpray, Inc.
 * DESCRIPTION : Generates vector of gaussian distributed random numbers
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vgauss_f_D.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 * 
 * 1     22.06.04 14:25 Unp00611
 * 
 * 5     2/15/03 21:43 vgp
 * Updated comments
 * 
 * 3     3/17/01 10:43p Jeff
 * Updated comments
 * 
 * 2     9/27/00 8:49p Jeff
 * Added log_done() support
 * 
 * 1     7/27/00 10:04p Jeff
 * Initial version
 *****************************************************************************/

#include <asm_sprt.h>

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif

.global _vgauss_f_D;
.extern _rangauss;

//.global _vgauss_f_D.end;

//	.section/pm seg_pmco;
	.segment/pm   seg_ext_pmco;

/*****************************************************
*
* void vgauss_f_D( float X[], float mean, float stddev, int L );
*
* Operation : Generates vector of gaussian distributed numbers
*
*****************************************************/
_vgauss_f_D:
      leaf_entry;

      r0 = i3; puts = r0;
      puts = r13;
      puts = r14;
      puts = r15;
      puts = mode1;

      i3  = r4;
      bit clr MODE1 ALUSAT;
      r13 = r8;                    /* Mean */
      r14 = r12;                   /* Std dev */
      r15 = reads( 1 );            /* Length */
      r4 = r13;                    /* Initialize input parameters */
      r8 = r14;
lp1:
      ccall( _rangauss );          /* Translates to 5 instructions */
	  f0 = pass f0;                // test f0 for NAN
	  r4 = astatx;                 // read ASTATx
	  r8 = BIT_5;                  // load AI mask
      r4 = r4 and r8;              // test for AI bit set
	  if  ne jump lp1;             // if AI bit is set get next gaussian random data
      r15 = r15 - 1, dm( i3, m6 ) = f0;
      if ne jump lp1 (db);
       r4 = r13;                   /* Reset input parameters */
       r8 = r14;

      i3  = gets( 5 );
      r13 = gets( 4 );
      r14 = gets( 3 );
      r15 = gets( 2 );
      mode1 = gets( 1 );
      bit clr STKYx AOS | MOS;

      leaf_exit;
      
_vgauss_f_D.end:

.ENDSEG;
