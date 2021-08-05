/******************************************************************************
 *
 * PROJECT     : ETL600  
 * FILE        : $Workfile: TCM_defs.h $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : TCM codec main parameter definitions
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * 
 * 
 * Initial version
 *       08.09.2003 vgp
 * Independent TX/RX directions
 *       13.01.2004 vgp
 * Add BER tester
 *       23.01.04 vgp
 * Delta bits generation support
 *      12.05.2004 vgp
 *****************************************************************************/

#ifndef TCM_defs_h
#define TCM_defs_h

#define ALIGN2 (asm(".align 2;");)
#define ALIGN4 (asm(".align 4;");)

#define CRC_LEN   5   // number of bits in CRC
#define TERM_BITS 4   // number of bits for trellis 3/4 code termination

#define QAM_ORDER_MAX_TX     14           // 16384QAM
#define LEN_2D_MAX_TX       512           // 2D signals block length must be even (!!!! Don't change that value!) 
#define LEN_4D_MAX_TX       (LEN_2D_MAX_TX/2) // 4D signals block length

#define QAM_ORDER_MAX_RX    QAM_ORDER_MAX_TX           // 16384QAM
#define LEN_2D_MAX_RX       LEN_2D_MAX_TX           // 2D signals block length must be even (!!!! Don't change that value!) 
#define LEN_4D_MAX_RX       (LEN_2D_MAX_RX/2) // 4D signals block length


#define SCRAMBLE_LEN_SYMB    64           // Length  of scramble/descramble data if OFDM symbols
#define PCM_INIT_VALUE       0xFFFFFFFF
#define BER_INIT_VALUE       0xFFFFFFFF 
#define RX_DELAY_SYMB        0 // Group delay in RX data path in OFDM symbols
#define INF_PCM_SEED         0x57253437
#define INF_DUMMY_SEED       0x23452312
#define INF_MQAM_SEED        0x12345432
#define INF_DELTA_SEED       0x86328459

#endif
