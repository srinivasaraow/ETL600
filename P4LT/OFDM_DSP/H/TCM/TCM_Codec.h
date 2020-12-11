/******************************************************************************
 *
 * PROJECT     : ETL600  
 * FILE        : $Workfile: TCM_Codec.h $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : TCM codec parameter definitions
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * 
 * 
 * Initial version
 *       08.09.2003 vgp
 * Independent TX/RX directions
 *       13.01.2004 vgp
 * BER tester
 *       23.01.2004 vgp
 * Correction of PCM_BUFFER_LENGTH_TX & PCM_BUFFER_LENGTH_RX
 *       26.01.2004 vgp
 *****************************************************************************/

#ifndef  TCM_Codec_H
#define TCM_Codec_H

#include   "TCM_defs.h"

#define BIT_BUFFER_LENGTH_TX (3*LEN_4D_MAX_TX-TERM_BITS+2*LEN_2D_MAX_TX+2*LEN_2D_MAX_TX*(QAM_ORDER_MAX_TX/2-2)) // incl. CRC bits
#define BIT_BUFFER_LENGTH_RX BIT_BUFFER_LENGTH_TX // incl. CRC bits

#define BIT_BUFFER_MEMORY_LENGTH_TX ((BIT_BUFFER_LENGTH_TX+1)/2) 
#define BIT_BUFFER_MEMORY_LENGTH_RX BIT_BUFFER_MEMORY_LENGTH_TX

#define PCM_BUFFER_LENGTH_TX  ((BIT_BUFFER_LENGTH_TX-CRC_LEN+16)/32)
#define PCM_BUFFER_LENGTH_RX  ((BIT_BUFFER_LENGTH_RX-CRC_LEN+16)/32)
#define MAX_SCRAMBLE_TX       (SCRAMBLE_LEN_SYMB*PCM_BUFFER_LENGTH_TX) // length of scramble data
#define MAX_SCRAMBLE_RX       (SCRAMBLE_LEN_SYMB*PCM_BUFFER_LENGTH_RX) // length of scramble data

#endif
