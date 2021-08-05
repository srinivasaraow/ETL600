#ifndef OFDM_TDM_IO_H
#define OFDM_TDM_IO_H

#include <complex.h>

    void  OFDM_TDM_Init_Start();
    void  OFDM_TDM_Stop();
    void  OFDM_Sync_to_TDM();
    int vcopy_out_buf(complex_float *, int );
    int vcopy_in_buf(complex_float *, int );

#endif

	
