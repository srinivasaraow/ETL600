/******************************************************************************
 *
 * PROJECT     : ETL600  
 * FILE        : $Workfile: TCM_bitcalc.c $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Calculates number of TCM information bits out of QAM bit distribution table
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source HSD600/P4LT/OFDM_DSP/C/TCM/TCM_bitcalc.c $
 * 
 * 3     5.04.06 15:28 Chhamaa
 * 
 * 3     17.03.06 14:41 Chhamaa
 * 
 * 1     19.01.05 15:41 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 *
 * Initial version
 *       20.04.2004 vgp,PL
 * QPSK verification during Shaping_flag == ON energy calculation 
 *       09.05.2004 vgp
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#include "../../h/tcm/TCM_Codec.h"
#include "../../h/tcm/TCM_Codec_externs.h"
#include "../../h/OFDM_externs.h"
#include "../../h/ofdm_defs.h"

extern float Shaping_gain[];
//========================================

int segment ( "seg_ext_pmco" ) TCM_bitcalc(int *d2alloc, int Shaping_Flag, int *Bit_vec, float *C_Unsorted, int *Index, int M, int calc_Energy_Flag)
// // Input: 
// //        Bit_vec[0..M-1]  - array of integers, each element defines QAM order of modulation and belongs to set {2,4,6,8,10,12,14} 
// //        M                - number of active OFDM carriers
// //        Shaping_Flag     - modulation mode: 1 - shaping ON, 0 - shaping OFF
// // 		 C_Unsorted		  - reziprogue Coefficients of channel transfer function in physical order, used for calculation of power
// //		 Index			  - Index to convert sorted order in physical order: Index[i_sort]=physical_position
// //		 calc_Energy_Flag - if 1 then TCM_QAM_Energy is calculated
// 
// Output:
// //        d2alloc[0..M-1]  - array of integers, each element if greater than 0 defines number of Gray bits per complex coordinate, belongs to set {-1,0,1,2,3,4,5}
// //        TCM_Code_Bits    - global, number of TCM information bits to be encoded by Wey 3/4 trellis code
// //        TCM_Shaping_Bits - global, number of TCM information bits to be encoded by shaping code if shaping flag is ON, or
// //                           will be used to quadrant allocation
// //        TCM_Gray_Bits    - global, number of TCM information bits to be encoded by Gray code
// //        TCM_Num_Inf_Bits - global, overall number of TCM information bits
// //        TCM_Shaping_Flag - global, shaping flag, may be changed despite requested (Shaping_Flag) for flat QPSK modulation only
// //        TCM_QAM_Energy   - global, total energy of active OFDM carriers 
// //        
// // Returns               0 - on success, otherwise returns -1
{

	int i;
    int  QAM_Order;

	    
    int           n_gr_coord;  // = QAM_order/2-2, number of gray bits per coordinate if QAM order > 2
    int           n_QPSK; // number of QPSK carriers
    int           n_16QAM;
    int           n_64QAM;
    int           n_256QAM;
    int           n_1024QAM;
    int           n_4096QAM;
    int           n_16384QAM;


    float         a, b;


// check definitions and parameters
	if ( M < 4 ) return -1;         // M must be not less than 4
	if ( M & 1 ) return -1;         // M must be even
	if ( (Shaping_Flag & 0xfffffffe) != 0 ) return -1;  // exception: 

    TCM_Code_Bits = (3*M)/2 - CRC_LEN - TERM_BITS;   // number of information bits to be encoded by 3/4 trellis encoder 
	TCM_Gray_Bits = 0;                               // total number of gray bits per OFDM symbol
	n_QPSK     = 0;
	n_16QAM    = 0;
	n_64QAM    = 0;
	n_256QAM   = 0;
	n_1024QAM  = 0;
	n_4096QAM  = 0;
	n_16384QAM = 0;
	if (calc_Energy_Flag >0)
		TCM_QAM_Energy = 0.0f;              // OFDM symbol energy, DELTA == 1.0

    for (i=0; i<M; i++) {
	   QAM_Order = Bit_vec[i];
	   switch ( QAM_Order ) {
		  case 2:
			  n_QPSK++;
		      break;
		  case 4:
			  n_16QAM++;
		      break;
		  case 6:
			  n_64QAM++;
		      break;
		  case 8:
			  n_256QAM++;
		      break;
		  case 10:
			  n_1024QAM++;
		      break;
		  case 12:
			  n_4096QAM++;
		      break;
		  case 14:
			  n_16384QAM++;
		      break;
		  default:
		    TCM_Num_Inf_Bits = 0;
		    return -1;                 // excepition: QAM order is out of range
	   }

	   n_gr_coord = QAM_Order/2 - 2;
	   d2alloc[i] = n_gr_coord; 
	   if ( n_gr_coord > 0 ) TCM_Gray_Bits += 2*n_gr_coord;  
	   
	   // calculate power of carrier
	   if (calc_Energy_Flag >0)
	   	{
	   	a = (powf(2.0f,(float)QAM_Order)-1.0f)/6.0f;         // mean power of OFDM information carrier
       	if (Shaping_Flag != 0 && QAM_Order != 2 ) {
	       	  b = powf(10.0f,(-Shaping_gain[QAM_Order>>1]/10.0f)); // revise mean power if shaping is on
	       } else {
	       	  b = 1.0f;
	       }
		// update energy of OFDM symbol (considers Predistortion)
       	TCM_QAM_Energy += a*b*(C_Unsorted[Index[i]]*C_Unsorted[Index[i]]); 
	   	}
	}
	
	 if ( n_QPSK == M ) TCM_Shaping_Flag = 0;  
     else TCM_Shaping_Flag = Shaping_Flag;


    TCM_Shaping_Bits = M - n_QPSK;

    if ( TCM_Shaping_Flag == 0 ) TCM_Shaping_Bits *= 2;  // total number of shaping bits per OFDM symbol

    TCM_Num_Inf_Bits = TCM_Code_Bits + TCM_Gray_Bits + TCM_Shaping_Bits; // total number of information symbols per OFDM symbol	
    
	// Calculate borders of signal code construction 
	// 	Note: this will be used in optim-> bit-shifting 
      ngr16384	= n_16384QAM;				// first index of non-QAM16384 (sorted order)
      ngr4096	= ngr16384	+ n_4096QAM;	// first index of non-QAM4096  
      ngr1024	= ngr4096	+ n_1024QAM;
      ngr256 	= ngr1024	+ n_256QAM;
      ngr64  	= ngr256 	+ n_64QAM;
      ngr16  	= ngr64  	+ n_16QAM;
      ngr4   	= ngr16  	+ n_QPSK;
    
  return 0;
}

