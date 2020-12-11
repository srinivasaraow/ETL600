/******************************************************************************
 *
 * PROJECT     : ETL600  
 * FILE        : $Workfile: vdecode3212_3x32bw.C $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Concatinated (32,12,10) decoder of input vector of 32-bit unsigned integers
 *               Length of input vector must contain 8 
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/QAM4_FEC/vdecode3212_3x32bw.C $
 * 
 * 1     19.01.05 15:41 Chchlee
 * 
 * 1     22.12.04 9:39 Chchlee
 * 
 * Initial version
 *       15.12.2004 vgp
 *****************************************************************************/
/*===================================*/
void  segment ( "seg_ext_pmco" ) pack_8x12bw_to_3x32bw(unsigned int *trio_out, unsigned int *octet_in)
//void  pack_8x12bw_to_3x32bw(unsigned int *trio_out, unsigned int *octet_in)
{
	unsigned int rest;
	
	trio_out[0] = (octet_in[0] << 20) | (octet_in[1]<<8) | (octet_in[6]>>4);
	trio_out[1] = (octet_in[2] << 20) | (octet_in[3]<<8) | ((octet_in[6]&0xf)<<4) | (octet_in[7]>>8) ;
	trio_out[2] = (octet_in[4] << 20) | (octet_in[5]<<8) | (octet_in[7]&0xff);
}
/*===================================*/
int  segment ( "seg_ext_pmco" ) vdecode3212_3x32bw(unsigned int *out, unsigned int *in, int n)
//int  vdecode3212_3x32bw(unsigned int *out, unsigned int *in, int n)
{
	int           n8w, i, j, status;
	unsigned int  doctet[8], dw;
	void          pack_8x12bw_to_3x32bw(unsigned int *trio_out, unsigned int *octet_in);
	int           Decoder_3212(unsigned int *, unsigned int);
	
	if ( (n<8)|| (n&0x7 != 0) ) return -1;
	n8w = n>>3;   // number of octets
	
	status = 0;
	for(i=0; i<n8w; i++) {
		for (j=0; j<8; j++) status |= Decoder_3212(&doctet[j],in[8*i+j]);
		pack_8x12bw_to_3x32bw(&out[3*i], doctet);
	}
	return status;
}
/*===================================*/
