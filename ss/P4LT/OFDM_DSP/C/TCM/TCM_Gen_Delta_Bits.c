/******************************************************************************
 *
 * PROJECT     : ETL600  
 * FILE        : $Workfile: TCM_Gen_Delta_Bits.c $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : Generates random bits to add on tx side
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source AES/P4LT/OFDM_DSP/C/TCM/TCM_Gen_Delta_Bits.c $
 * 
 * 5     6.09.06 16:58 Chchlee
 * Bug 257 
 * 
 * 4     21.08.06 15:23 Chchlee
 * Bug 257 fixed
 * 
 * 1     19.01.05 15:41 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 *
 * Initial version
 *       12.05.2004 vgp
 *****************************************************************************/
 
#include "../../h/tcm/TCM_Codec.h"
#include "../../h/tcm/TCM_Codec_externs.h"

//========================================

void segment ( "seg_ext_pmco" ) TCM_Gen_Delta_Bits(unsigned int *OutBit, int len)
{
	unsigned int Saved_Seed, rw, nw, nb, Mask;
	int          i;

	unsigned int DRS7_setseed(unsigned int);
	unsigned int DRS7_getseed();
	unsigned int DRS7_rand();

      if  ( len > 0 ) {

         // save seed 
         Saved_Seed = DRS7_getseed();

		 // get number of words to generate
		 nw = ((unsigned int)len) >> 5;
		 
		 // get number of bits to generate
         nb = ((unsigned int)len) & 0x1f;

         // load seed of previous block at the end of symbol
         DRS7_setseed(DeltaBits_Seed);

		 // generate 32*nw bits 
		 if ( nw != 0 ) {
			 for (i=0; i<nw; i++) {
				 Mask = 0x80000000;
                 // generate random word
                 rw = DRS7_rand();
				 // store random bits
				 while ( Mask != 0 ) {
					 if ( (Mask & rw) != 0 ) *OutBit++ = 1;
					 else *OutBit++ = 0;
					 Mask >>= 1;
				 }
			 }
		 }

		 if ( nb != 0 ) {
		     // generate rest nb bits 
             Mask = 1 << (nb-1);
             // generate random word
             rw = DRS7_rand();    
			 // store random bits
			 while ( Mask != 0 ) {
				 if ( (Mask & rw) != 0 ) *OutBit++ = 1;
				 else *OutBit++ = 0;
				 Mask >>= 1;
			 }  
		 }
         

         // save seed for DeltaBits
         DeltaBits_Seed =  DRS7_getseed();
    
         // restore seed
         DRS7_setseed(Saved_Seed);
      }

}

