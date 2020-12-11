/******************************************************************************
 *
 * PROJECT     : ETL600 COFDM modem  
 * FILE        : $Workfile: crc32.C $
 * COPYRIGHT   : 2004 TL Consulting GmbH.
 * DESCRIPTION : CRC32 encoder/decoder
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/QAM4_FEC/crc32.C $
 * 
 * 1     19.01.05 15:41 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * 1     1.04.04 vgp
 *****************************************************************************/
#include "../../h/ofdm_defs.h"
#include "../../h/ofdm_externs.h"
#include "../../h/qam4_fec/channel_data.h"

/*===================================*/
unsigned int segment ( "seg_ext_pmco" ) CRC32(unsigned int *Data, int n)
{
	int          i;
	unsigned int crc_word;
	
	crc_word = 0;
	for (i=0; i<n; i++) crc_word ^= *Data++;
	
	return crc_word;
}
/*===================================*/
void CRC32_Encoder(Channel_Data_type *Ch_Data, int n_k_tx)
{
	unsigned int crc_word, *ui_ptr;	
    unsigned int CRC32(unsigned int *Data, int n);
	
	ui_ptr = ((unsigned int *)Ch_Data) + 1;
	crc_word = CRC32(ui_ptr,n_k_tx+CHD_RES+CHD_32BW_TX-1);
	Ch_Data->CRC = crc_word;
	
}
/*===================================*/
int CRC32_Decoder(Channel_Data_type *Ch_Data, int n_k_rx)
{
	unsigned int crc_word, *ui_ptr;	
    unsigned int CRC32(unsigned int *Data, int n);

	ui_ptr = ((unsigned int *)Ch_Data) + 1;
	crc_word = CRC32(ui_ptr,n_k_rx+CHD_RES+CHD_32BW_RX-1);
    if ( Ch_Data->CRC == crc_word ) return 0;
	else return 1;
}
/*===================================*/
