/******************************************************************************
 *
 * PROJECT     : ETL600  
 * FILE        : $Workfile: vencode3212_3x32bw.C $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Concatinated (32,12,10) encoder of input vector of 32-bit unsigned integers
 *               Length of input vector must contain 3 
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/QAM4_FEC/vencode3212_3x32bw.C $
 * 
 * 1     19.01.05 15:41 Chchlee
 * 
 * 1     22.12.04 9:39 Chchlee
 * 
 * Initial version
 *       15.12.2004 vgp
 *****************************************************************************/
/*===================================*/
void  segment ( "seg_ext_pmco" ) pack_3x32bw_to_8x12bw(unsigned int *octet_out, unsigned int *trio_in)
//void  pack_3x32bw_to_8x12bw(unsigned int *octet_out, unsigned int *trio_in)
{
	
	octet_out[0] = trio_in[0]>>20;
	octet_out[1] = (trio_in[0]>>8) & 0xfff;
	octet_out[2] = trio_in[1]>>20;
	octet_out[3] = (trio_in[1]>>8) & 0xfff;
	octet_out[4] = trio_in[2]>>20;
	octet_out[5] = (trio_in[2]>>8) & 0xfff;
	octet_out[6] = (trio_in[0]<<4 | (trio_in[1]>>4 & 0xf)) & 0xfff;
	octet_out[7] = (trio_in[1]<<8 | (trio_in[2] & 0xff) ) & 0xfff;		
}
/*===================================*/
int  segment ( "seg_ext_pmco" ) vencode3212_3x32bw(unsigned int *out, unsigned int *in, int n)
//int  vencode3212_3x32bw(unsigned int *out, unsigned int *in, int n)
{
	int            i, j, n3w;
	unsigned  int  octet[8];
    void           pack_3x32bw_to_8x12bw(unsigned int *octet_out, unsigned int *trio_in);
    unsigned int   Encoder_3212(unsigned int);	

	n3w = n/3;  // number of triplets
	if ( (n<3) || (n-3*n3w != 0) ) return -1;
	
	for(i=0; i<n3w; i++) {
		pack_3x32bw_to_8x12bw(octet,&in[3*i]);
		for (j=0; j<8; j++) *out++ = Encoder_3212(octet[j]);
	}
		
	return 0;
}
