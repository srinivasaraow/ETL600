REM -------------------------------------------------------------------------
REM $Workfile: OFDM.bat $
REM Part of      : ETL600
REM Language     : Windows batch file
REM Created by   : Hans-Peter Rytz
REM Remarks      :  
REM Purpose      : 
REM Copyright    : ABB Switzerland Ltd., 2004
REM -------------------------------------------------------------------------
REM
REM -------------------------------------------------------------------------
REM $Log: /branches/Source MAS/P4LT/OFDM_DSP/batch/OFDM.bat $
rem 
rem 34    29.08.08 15:29 Chhamaa
rem 
rem 30    07-04-13 8:37 Chchlee
rem now using environment variables for assembler, compiler and linker
rem 
rem 26    5.04.06 15:28 Chhamaa
rem 
rem 26    17.03.06 14:41 Chhamaa
rem 
rem 24    1.11.05 16:23 Chchlee
rem batch files changed / -DDEBUG introduced
rem 
rem 23    30.09.05 10:53 Chalgyg
rem OFDM_DSP_V0_11_ur (12709051)
rem 
rem 17    17.05.05 16:45 Chchlee
rem Echo off commented in again
rem 
rem 16    2.05.05 12:39 Ats
rem Exchange with TLC : OFDM V0_09S
rem 
rem 12    7.03.05 8:43 Ats
rem Exchange with TLC
rem 
rem 10    24.02.05 7:36 Chchlee
rem 
rem 8     7.02.05 14:34 Chalgyg
rem OFDM Modem Version 0.09C
rem 
rem 27    22.12.04 9:45 Chchlee
rem 
rem 26    2.12.04 17:48 Chhamaa
rem
rem 	    04.10.04 Langfeld
rem			replaced copy_to_V11_Buffer.c , copy_from_V11_Buffer.c by assembler versions
rem			output file OFDM_DSP_V0_03b
rem 
rem 24    30.09.04 9:18 Leeb02
rem 
rem 23    29.09.04 14:08 Unp00632
rem 
rem 22    29.09.04 11:33 Leeb02
rem 
rem 21    29.09.04 10:44 Leeb02
rem 
rem 18    2.09.04 14:59 Unp00632
rem Added %WORK_DIR%
rem 
rem 17    27.08.04 15:43 Buhl01
rem start loader only IF EXIST *.dxe; start convert tool only IF EXIST
rem *.ldr
rem 
rem 16    11.08.04 17:07 Leeb02
REM -------------------------------------------------------------------------

rem set executable path
if defined ADSP_PATH (set EASM21K_EXE="%ADSP_PATH%\easm21k") else (set EASM21K_EXE=easm21k)
if defined ADSP_PATH (set CC21K_EXE="%ADSP_PATH%\cc21k") else (set CC21K_EXE=cc21k)
if defined ADSP_PATH (set LINKER_EXE="%ADSP_PATH%\linker") else (set LINKER_EXE=linker)
if defined ADSP_PATH (set ELFLOADER_EXE="%ADSP_PATH%\elfloader") else (set ELFLOADER_EXE=elfloader)

echo on

set WORK_DIR=D:\ADSP\$P4LT
if defined P4LT_BUILD_PATH set WORK_DIR=%P4LT_BUILD_PATH%

:BEGIN
set DSP_ID=OFDM_DSP

IF "%1" == "-debug"   (SET output=%WORK_DIR%\%DSP_ID%\debug&SET C_OPT=-g -DDEBUG&mkdir %WORK_DIR%\%DSP_ID%\debug)
IF "%1" == "-release" (SET output=%WORK_DIR%\%DSP_ID%\release&SET C_OPT=&mkdir %WORK_DIR%\%DSP_ID%\release)

SET OFDM_OPT=-O1 -Ov100 -ipa -Oa -g -double-size-32 -flags-compiler --diag_warning,implicit_func_decl

SET H_PATH=%WORK_DIR%\%DSP_ID%\H
SET C_PATH=%WORK_DIR%\%DSP_ID%\C
SET ASM_PATH=%WORK_DIR%\%DSP_ID%\ASM

:DELETE
del %output%\*.doj

del %WORK_DIR%\%DSP_ID%\*.dxe            
del %WORK_DIR%\%DSP_ID%\*.ldr


:COMP_ASM
REM ============   FFT routings
%EASM21K_EXE% -21161  -o %output%\fft_c_dpd.doj "%ASM_PATH%\fft\fft_c_dpd.asm"
%EASM21K_EXE% -21161  -o %output%\ifft_c_dpd.doj "%ASM_PATH%\fft\ifft_c_dpd.asm"
%EASM21K_EXE% -21161  -o %output%\twid_c_dpd.doj "%ASM_PATH%\fft\twid_c_dpd.asm"
REM ============   TDM Bus I/O functions
%EASM21K_EXE% -21161  -o %output%\tdm_bus_io.doj "%ASM_PATH%\p4lt_tdm\tdm_bus_io.asm"
REM ============   QAM4 FEC functions
%EASM21K_EXE% -21161  -o %output%\Decoder_3212.doj "%ASM_PATH%\qam4_fec\Decoder_3212.asm"
%EASM21K_EXE% -21161  -o %output%\Encoder_3212.doj "%ASM_PATH%\qam4_fec\Encoder_3212.asm"
REM ============   Reed Solomon FEC functions
%EASM21K_EXE% -21161  -o %output%\FFT63_8.doj "%ASM_PATH%\rs_fec\FFT63_8.asm"
%EASM21K_EXE% -21161  -o %output%\IFFT63.doj "%ASM_PATH%\rs_fec\IFFT63.asm"
%EASM21K_EXE% -21161  -o %output%\RS6355_Decoder.doj "%ASM_PATH%\rs_fec\RS6355_Decoder.asm"
%EASM21K_EXE% -21161  -o %output%\RS6355_Encoder.doj "%ASM_PATH%\rs_fec\RS6355_Encoder.asm"
%EASM21K_EXE% -21161  -o %output%\Pack1BitTo6BitWord.doj "%ASM_PATH%\rs_fec\Pack1BitTo6BitWord.asm"
%EASM21K_EXE% -21161  -o %output%\Unpack6BitTo1BitWord.doj "%ASM_PATH%\rs_fec\Unpack6BitTo1BitWord.asm"
REM ============   Trellis coded modulation & demodulation (TCM) functions
%EASM21K_EXE% -21161  -o %output%\TCM_CRC5_Decoder.doj "%ASM_PATH%\tcm\TCM_CRC5_Decoder.asm"
%EASM21K_EXE% -21161  -o %output%\TCM_CRC5_Encoder.doj "%ASM_PATH%\tcm\TCM_CRC5_Encoder.asm"
%EASM21K_EXE% -21161  -o %output%\TCM_Decoder_Extract_Gr_Cd_Bits.doj "%ASM_PATH%\tcm\TCM_Decoder_Extract_Gr_Cd_Bits.asm"
%EASM21K_EXE% -21161  -o %output%\TCM_Decoder_Remodulate.doj "%ASM_PATH%\tcm\TCM_Decoder_Remodulate.asm"
%EASM21K_EXE% -21161  -o %output%\TCM_Decoder_Vcopy_SPbits_DD.doj "%ASM_PATH%\tcm\TCM_Decoder_Vcopy_SPbits_DD.asm"
%EASM21K_EXE% -21161  -o %output%\TCM_Decoder_Vget_SPbits_DD.doj "%ASM_PATH%\tcm\TCM_Decoder_Vget_SPbits_DD.asm"
%EASM21K_EXE% -21161  -o %output%\TCM_Gray_Encoder.doj "%ASM_PATH%\tcm\TCM_Gray_Encoder.asm"
%EASM21K_EXE% -21161  -o %output%\TCM_Shaping_Encoder.doj "%ASM_PATH%\tcm\TCM_Shaping_Encoder.asm"
%EASM21K_EXE% -21161  -o %output%\TCM_Trellis_34_Encoder.doj "%ASM_PATH%\tcm\TCM_Trellis_34_Encoder.asm"
%EASM21K_EXE% -21161  -o %output%\TCM_Trellis_34_ViterbiDecoder.doj "%ASM_PATH%\tcm\TCM_Trellis_34_ViterbiDecoder.asm"
REM ============   TDM Bus drivers
%EASM21K_EXE% -21161  -o %output%\ofdm_sync_to_tdm.doj "%ASM_PATH%\tdm\ofdm_sync_to_tdm.asm"
%EASM21K_EXE% -21161  -o %output%\ofdm_tdm_rec_isr.doj "%ASM_PATH%\tdm\ofdm_tdm_rec_isr.asm"
%EASM21K_EXE% -21161  -o %output%\ofdm_tdm_trm_isr.doj "%ASM_PATH%\tdm\ofdm_tdm_trm_isr.asm"
%EASM21K_EXE% -21161  -o %output%\vcopy_in_buf.doj "%ASM_PATH%\tdm\vcopy_in_buf.asm"
%EASM21K_EXE% -21161  -o %output%\vcopy_out_buf.doj "%ASM_PATH%\tdm\vcopy_out_buf.asm"


REM ============   OFDM functions
%EASM21K_EXE% -21161  -o %output%\ofdm_dsp_hdr.doj "%ASM_PATH%\ofdm_dsp_hdr.asm"
%EASM21K_EXE% -21161  -o %output%\extract_qam.doj "%ASM_PATH%\extract_qam.asm"
%EASM21K_EXE% -21161  -o %output%\spectrum_qam.doj "%ASM_PATH%\spectrum_qam.asm"
REM ============   Interpolation & decimation filters
%EASM21K_EXE% -21161  -o %output%\dec2x12_filt_c.doj "%ASM_PATH%\int_dec_filters\dec2x12_filt_c.asm"
%EASM21K_EXE% -21161  -o %output%\dec4x24_filt_c.doj "%ASM_PATH%\int_dec_filters\dec4x24_filt_c.asm"
%EASM21K_EXE% -21161  -o %output%\int2x12_filt_c.doj "%ASM_PATH%\int_dec_filters\int2x12_filt_c.asm"
%EASM21K_EXE% -21161  -o %output%\int3x24_filt_c.doj "%ASM_PATH%\int_dec_filters\int3x24_filt_c.asm"
%EASM21K_EXE% -21161  -o %output%\int4x24_filt_c.doj "%ASM_PATH%\int_dec_filters\int4x24_filt_c.asm"
%EASM21K_EXE% -21161  -o %output%\int5x25_filt_c.doj "%ASM_PATH%\int_dec_filters\int5x25_filt_c.asm"
REM ============   OFDM utility functions
%EASM21K_EXE% -21161  -o %output%\Init_OFDM_DSP.doj "%ASM_PATH%\util\Init_OFDM_DSP.asm"
%EASM21K_EXE% -21161  -o %output%\drs7_random.doj "%ASM_PATH%\util\drs7_random.asm"
%EASM21K_EXE% -21161  -o %output%\Pack_1Bit_To_32BitWord_DD.doj "%ASM_PATH%\util\Pack_1Bit_To_32BitWord_DD.asm"
%EASM21K_EXE% -21161  -o %output%\svdivz_f_DD.doj "%ASM_PATH%\util\svdivz_f_DD.asm"
%EASM21K_EXE% -21161  -o %output%\timer.doj "%ASM_PATH%\util\timer.asm"
%EASM21K_EXE% -21161  -o %output%\Unpack_32Bit_To_1BitWord_DD.doj "%ASM_PATH%\util\Unpack_32Bit_To_1BitWord_DD.asm"
%EASM21K_EXE% -21161  -o %output%\vadd_f_eee.doj "%ASM_PATH%\util\vadd_f_eee.asm"
%EASM21K_EXE% -21161  -o %output%\vconjg_c_dd.doj "%ASM_PATH%\util\vconjg_c_dd.asm"
%EASM21K_EXE% -21161  -o %output%\vcopy_ee.doj "%ASM_PATH%\util\vcopy_ee.asm"
%EASM21K_EXE% -21161  -o %output%\vcopy_pe.doj "%ASM_PATH%\util\vcopy_pe.asm"
%EASM21K_EXE% -21161  -o %output%\vdivz_c_ddd.doj "%ASM_PATH%\util\vdivz_c_ddd.asm"
%EASM21K_EXE% -21161  -o %output%\vdivz_f_ddd.doj "%ASM_PATH%\util\vdivz_f_ddd.asm"
%EASM21K_EXE% -21161  -o %output%\vdivz_f_ddp.doj "%ASM_PATH%\util\vdivz_f_ddp.asm"
%EASM21K_EXE% -21161  -o %output%\vdivz_f_eee.doj "%ASM_PATH%\util\vdivz_f_eee.asm"
%EASM21K_EXE% -21161  -o %output%\vfsmulvadd_c_ddd.doj "%ASM_PATH%\util\vfsmulvadd_c_ddd.asm"
%EASM21K_EXE% -21161  -o %output%\vgamma_off_dd.doj "%ASM_PATH%\util\vgamma_off_dd.asm"
%EASM21K_EXE% -21161  -o %output%\vgamma_on_dd.doj "%ASM_PATH%\util\vgamma_on_dd.asm"
%EASM21K_EXE% -21161  -o %output%\vgauss_f_d.doj "%ASM_PATH%\util\vgauss_f_d.asm"
%EASM21K_EXE% -21161  -o %output%\vkalman_c_ddd.doj "%ASM_PATH%\util\vkalman_c_ddd.asm"
%EASM21K_EXE% -21161  -o %output%\vkalman_f_ddd.doj "%ASM_PATH%\util\vkalman_f_ddd.asm"
%EASM21K_EXE% -21161  -o %output%\vmag2_c_dd.doj "%ASM_PATH%\util\vmag2_c_dd.asm"
%EASM21K_EXE% -21161  -o %output%\vmag2_uc_ddp.doj "%ASM_PATH%\util\vmag2_uc_ddp.asm"
%EASM21K_EXE% -21161  -o %output%\vmax_f_e.doj "%ASM_PATH%\util\vmax_f_e.asm"
%EASM21K_EXE% -21161  -o %output%\vmax_i_e.doj "%ASM_PATH%\util\vmax_i_e.asm"
%EASM21K_EXE% -21161  -o %output%\vmin_f_e.doj "%ASM_PATH%\util\vmin_f_e.asm"
%EASM21K_EXE% -21161  -o %output%\vmul_c_eee.doj "%ASM_PATH%\util\vmul_c_eee.asm"
%EASM21K_EXE% -21161  -o %output%\vones_i_e.doj "%ASM_PATH%\util\vones_i_e.asm"
%EASM21K_EXE% -21161  -o %output%\vset_f_e.doj "%ASM_PATH%\util\vset_f_e.asm"
%EASM21K_EXE% -21161  -o %output%\vsmul_f_ee.doj "%ASM_PATH%\util\vsmul_f_ee.asm"
%EASM21K_EXE% -21161  -o %output%\vsorti_f_ddd.doj "%ASM_PATH%\util\vsorti_f_ddd.asm"
%EASM21K_EXE% -21161  -o %output%\vsubmul_c_ddpd.doj "%ASM_PATH%\util\vsubmul_c_ddpd.asm"
%EASM21K_EXE% -21161  -o %output%\vsum_f_e.doj "%ASM_PATH%\util\vsum_f_e.asm"
%EASM21K_EXE% -21161  -o %output%\vsummag2_c_e.doj "%ASM_PATH%\util\vsummag2_c_e.asm"
%EASM21K_EXE% -21161  -o %output%\vsummag2_uc_dp.doj "%ASM_PATH%\util\vsummag2_uc_dp.asm"
%EASM21K_EXE% -21161  -o %output%\vwin_c_ddd.doj "%ASM_PATH%\util\vwin_c_ddd.asm"
%EASM21K_EXE% -21161  -o %output%\vxor_i_eee.doj "%ASM_PATH%\util\vxor_i_eee.asm"
%EASM21K_EXE% -21161  -o %output%\vcopy_s_ed.doj "%ASM_PATH%\util\vcopy_s_ed.asm"
%EASM21K_EXE% -21161  -o %output%\vsum_i_e.doj "%ASM_PATH%\util\vsum_i_e.asm"
REM ============   V11 interface
%EASM21K_EXE% -21161  -o %output%\DSP2_SPORT02_V11.doj "%ASM_PATH%\v11\DSP2_SPORT02_V11.asm"
%EASM21K_EXE% -21161  -o %output%\Copy_to_V11_Buffer.doj "%ASM_PATH%\v11\Copy_to_V11_Buffer.asm"
%EASM21K_EXE% -21161  -o %output%\Copy_from_V11_Buffer.doj "%ASM_PATH%\v11\Copy_from_V11_Buffer.asm"
REM ============   DCTXCO interface
%EASM21K_EXE% -21161  -o %output%\i2c_driver.doj "%ASM_PATH%\dctcxo\i2c_driver.asm"

:COMP_C
REM ============   QAM4 FEC functions
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\crc32.doj "%C_PATH%\qam4_fec\crc32.C"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Decode_CHD.doj "%C_PATH%\qam4_fec\Decode_CHD.C"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Encode_CHD.doj "%C_PATH%\qam4_fec\Encode_CHD.C"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\vencode3212_3x32bw.doj "%C_PATH%\qam4_fec\vencode3212_3x32bw.C"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\vdecode3212_3x32bw.doj "%C_PATH%\qam4_fec\vdecode3212_3x32bw.C"
REM ============   Reed Solomon FEC functions
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\rs_decoder.doj "%C_PATH%\rs_fec\rs_decoder.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\rs_decoder_init.doj "%C_PATH%\rs_fec\rs_decoder_init.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\rs_encoder.doj "%C_PATH%\rs_fec\rs_encoder.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\rs_encoder_init.doj "%C_PATH%\rs_fec\rs_encoder_init.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\rs_encoder_init_cb.doj "%C_PATH%\rs_fec\rs_encoder_init_cb.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\rs_decoder_init_cb.doj "%C_PATH%\rs_fec\rs_decoder_init_cb.c"
REM ============   Trellis coded modulation & demodulation (TCM) functions
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\tcm_ber_tester.doj "%C_PATH%\tcm\tcm_ber_tester.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\tcm_ber_tester_init.doj "%C_PATH%\tcm\tcm_ber_tester_init.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\TCM_bitcalc.doj "%C_PATH%\tcm\TCM_bitcalc.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\TCM_Decoder.doj "%C_PATH%\tcm\TCM_Decoder.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\TCM_Delta_Gen_Init.doj "%C_PATH%\tcm\TCM_Delta_Gen_Init.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\TCM_Dummy_Gen_Init.doj "%C_PATH%\tcm\TCM_Dummy_Gen_Init.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\TCM_Encoder.doj "%C_PATH%\tcm\TCM_Encoder.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\TCM_Encoder_Ref.doj "%C_PATH%\tcm\TCM_Encoder_Ref.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\TCM_Gen_Delta_Bits.doj "%C_PATH%\tcm\TCM_Gen_Delta_Bits.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\TCM_Gen_Dummy_Data.doj "%C_PATH%\tcm\TCM_Gen_Dummy_Data.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\TCM_Gen_MQAM_Train_Data_RX.doj "%C_PATH%\tcm\TCM_Gen_MQAM_Train_Data_RX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\TCM_Gen_MQAM_Train_Data_TX.doj "%C_PATH%\tcm\TCM_Gen_MQAM_Train_Data_TX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\TCM_Gen_PCM_Data.doj "%C_PATH%\tcm\TCM_Gen_PCM_Data.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\TCM_Init_4QAM_Codec.doj "%C_PATH%\tcm\TCM_Init_4QAM_Codec.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\TCM_Init_Decoder.doj "%C_PATH%\tcm\TCM_Init_Decoder.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\TCM_Init_Encoder.doj "%C_PATH%\tcm\TCM_Init_Encoder.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\TCM_Init_MQAM_Decoder.doj "%C_PATH%\tcm\TCM_Init_MQAM_Decoder.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\TCM_Init_MQAM_Encoder.doj "%C_PATH%\tcm\TCM_Init_MQAM_Encoder.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\TCM_MQAM_Train_Gen_Init.doj "%C_PATH%\tcm\TCM_MQAM_Train_Gen_Init.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\TCM_PCM_Gen_Init.doj "%C_PATH%\tcm\TCM_PCM_Gen_Init.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\TCM_Shaping_Decoder.doj "%C_PATH%\tcm\TCM_Shaping_Decoder.c"
REM ============   TDM Bus drivers
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\ofdm_tdm_init.doj "%C_PATH%\tdm\ofdm_tdm_init.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\ofdm_tdm_adjust.doj "%C_PATH%\tdm\ofdm_tdm_adjust.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\ofdm_tdm_stop.doj "%C_PATH%\tdm\ofdm_tdm_stop.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\set_alarm.doj "%C_PATH%\tdm\set_alarm.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\clear_alarm.doj "%C_PATH%\tdm\clear_alarm.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\get_message.doj "%C_PATH%\tdm\get_message.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\send_message.doj "%C_PATH%\tdm\send_message.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\get_configuration.doj "%C_PATH%\tdm\get_configuration.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Init_Control_Services.doj "%C_PATH%\tdm\Init_Control_Services.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Master_Message_Handler.doj "%C_PATH%\tdm\Master_Message_Handler.c"
	
REM ============   V11 interface	
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\V11_Init.doj "%C_PATH%\v11\V11_Init.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Get_V11_Divider.doj "%C_PATH%\v11\Get_V11_Divider.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Adjust_V11_RX_Buffer.doj "%C_PATH%\v11\Adjust_V11_RX_Buffer.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Adjust_V11_TX_Buffer.doj "%C_PATH%\v11\Adjust_V11_TX_Buffer.c"

REM ============   OFDM functions
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\verify_modem_config.doj "%C_PATH%\verify_modem_config.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\wait_for_config.doj "%C_PATH%\wait_for_config.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Descrambler.doj "%C_PATH%\Descrambler.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Descrambler_Init.doj "%C_PATH%\Descrambler_Init.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Error_handler.doj "%C_PATH%\Error_handler.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Exp_Gen.doj "%C_PATH%\Exp_Gen.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\FFT.doj "%C_PATH%\FFT.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Fr_Tracking.doj "%C_PATH%\Fr_Tracking.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Get_Stuffing_Flag_RX.doj "%C_PATH%\Get_Stuffing_Flag_RX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Get_Stuffing_Flag_TX.doj "%C_PATH%\Get_Stuffing_Flag_TX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\IFFT.doj "%C_PATH%\IFFT.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Modem_Init_basic.doj "%C_PATH%\Modem_Init_basic.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Modulation_Init_QAM_M_v2.doj "%C_PATH%\Modulation_Init_QAM_M_v2.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Modulation_Init_QAM4.doj "%C_PATH%\Modulation_Init_QAM4.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_2Tone_Correlation.doj "%C_PATH%\OFDM_2Tone_Correlation.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_AWGN_NB_Estimation.doj "%C_PATH%\OFDM_AWGN_NB_Estimation.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Channel_Simulator.doj "%C_PATH%\OFDM_Channel_Simulator.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Channel_Simulator_Init.doj "%C_PATH%\OFDM_Channel_Simulator_Init.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Detection_AGC.doj "%C_PATH%\OFDM_Detection_AGC.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Detection_Pilot_Mode.doj "%C_PATH%\OFDM_Detection_Pilot_Mode.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Engine_RX.doj "%C_PATH%\OFDM_Engine_RX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Engine_TX.doj "%C_PATH%\OFDM_Engine_TX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Filter_RX.doj "%C_PATH%\OFDM_Filter_RX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Filter_TX.doj "%C_PATH%\OFDM_Filter_TX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Filter_RX_Watchdog.doj "%C_PATH%\OFDM_Filter_RX_Watchdog.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Freq_LinReg_RX.doj "%C_PATH%\OFDM_Freq_LinReg_RX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_FreqCtrl_RX.doj "%C_PATH%\OFDM_FreqCtrl_RX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Freq_Estim_RX.doj "%C_PATH%\OFDM_Freq_Estim_RX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Fall_BackForward.doj "%C_PATH%\OFDM_Fall_BackForward.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_LFM_Processing_RX.doj "%C_PATH%\OFDM_LFM_Processing_RX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Modem.doj "%C_PATH%\OFDM_Modem.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_NB_Detection.doj "%C_PATH%\OFDM_NB_Detection.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_NB_Interpolation.doj "%C_PATH%\OFDM_NB_Interpolation.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Noise_Estimation.doj "%C_PATH%\OFDM_Noise_Estimation.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_opt_decide_rate.doj "%C_PATH%\OFDM_opt_decide_rate.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_opt_decide_stuffing.doj "%C_PATH%\OFDM_opt_decide_stuffing.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Optim1.doj "%C_PATH%\OFDM_Optim1.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Optim2.doj "%C_PATH%\OFDM_Optim2.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Optim3.doj "%C_PATH%\OFDM_Optim3.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Optim_NL.doj "%C_PATH%\OFDM_Optim_NL.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_QAM4_Data_Preparation.doj "%C_PATH%\OFDM_QAM4_Data_Preparation.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_QAM4_Detection.doj "%C_PATH%\OFDM_QAM4_Detection.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Receiver.doj "%C_PATH%\OFDM_Receiver.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Restart.doj "%C_PATH%\OFDM_Restart.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_SFC_RX.doj "%C_PATH%\OFDM_SFC_RX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_SFC_TX.doj "%C_PATH%\OFDM_SFC_TX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Spectrum_RX.doj "%C_PATH%\OFDM_Spectrum_RX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Tracking_Engine_RX.doj "%C_PATH%\OFDM_Tracking_Engine_RX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Transmitter.doj "%C_PATH%\OFDM_Transmitter.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Pilots_Preprocessing.doj "%C_PATH%\Pilots_Preprocessing.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Preamble_processing_RX.doj "%C_PATH%\Preamble_processing_RX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Preamble_processing_TX.doj "%C_PATH%\Preamble_processing_TX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Preamble_scheduler_RX.doj "%C_PATH%\Preamble_scheduler_RX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Preamble_scheduler_TX.doj "%C_PATH%\Preamble_scheduler_TX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\QAM_processing_RX.doj "%C_PATH%\QAM_processing_RX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\QAM_processing_TX.doj "%C_PATH%\QAM_processing_TX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\QAM_scheduler_RX.doj "%C_PATH%\QAM_scheduler_RX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\QAM_scheduler_TX.doj "%C_PATH%\QAM_scheduler_TX.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Scrambler.doj "%C_PATH%\Scrambler.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Scrambler_Init.doj "%C_PATH%\Scrambler_Init.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Service_Bits.doj "%C_PATH%\Service_Bits.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Sim_AWGN.doj "%C_PATH%\Sim_AWGN.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Sim_Frequency_Offset.doj "%C_PATH%\Sim_Frequency_Offset.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Sim_NB.doj "%C_PATH%\Sim_NB.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Sim_NL_Lin_Dist.doj "%C_PATH%\Sim_NL_Lin_Dist.c"
REM ============   Interpolation & decimation filters
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\dec2x12_filt_c_init.doj "%C_PATH%\int_dec_filters\dec2x12_filt_c_init.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\dec4x24_filt_c_init.doj "%C_PATH%\int_dec_filters\dec4x24_filt_c_init.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\int2x12_filt_c_init.doj "%C_PATH%\int_dec_filters\int2x12_filt_c_init.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\int3x24_filt_c_init.doj "%C_PATH%\int_dec_filters\int3x24_filt_c_init.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\int4x24_filt_c_init.doj "%C_PATH%\int_dec_filters\int4x24_filt_c_init.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\int5x25_filt_c_init.doj "%C_PATH%\int_dec_filters\int5x25_filt_c_init.c"
REM ============   DCTCXO drivers
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\ad5321_io.doj "%C_PATH%\dctcxo\ad5321_io.c"
%CC21K_EXE%  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\ad5321_convert.doj "%C_PATH%\dctcxo\ad5321_convert.c"

REM == Software ID 12008083 ->0_39K
echo off

:LINK
cd %output%\
%CC21K_EXE% @%WORK_DIR%\OFDM_DSP\batch\OFDM_DSP.lnk -T "%WORK_DIR%\OFDM_DSP\architecture\OFDM_P4LT.ldf" -proc ADSP-21161 -L %output% -o "%WORK_DIR%\OFDM_DSP\OFDM_DSP_V0_39K.dxe" -map "%WORK_DIR%\OFDM_DSP\OFDM_DSP_V0_39K.xml" -flags-link


:PATCH CRC into DXE file
REM add CRC Patch here!!!!!!!!!!!!!!!!!!!!!!!!!


:LOADER
IF EXIST "%WORK_DIR%\OFDM_DSP\*.dxe" %ELFLOADER_EXE% "%WORK_DIR%\OFDM_DSP\OFDM_DSP_V0_39K.dxe" -bSPI -fASCII -HostWidth 32 -ExternalMemoryWidth 48 -l "%WORK_DIR%\DSP_Boot_Loader\SPI_Boot_SDRAM\161_spi32_SDRAM.dxe" -o "%WORK_DIR%\OFDM_DSP\OFDM_SlaveSPI32.ldr" -proc ADSP-21161 -v

:CONVERT LDR add comma separator to *.ldr
IF EXIST "%WORK_DIR%\OFDM_DSP\*.ldr" %WORK_DIR%\Tools\ConvertDSP_Loader\ConvertLDR_ETL600.exe "%WORK_DIR%\OFDM_DSP\OFDM_SlaveSPI32.ldr" "%WORK_DIR%\OFDM_DSP\OFDM_SlaveSPI32include.ldr"
IF EXIST "%WORK_DIR%\OFDM_DSP\*include.ldr" copy "%WORK_DIR%\OFDM_DSP\OFDM_SlaveSPI32include.ldr" "%WORK_DIR%\Master_DSP\C\OFDM_SlaveSPI32include.ldr"

IF NOT EXIST "%WORK_DIR%\OFDM_DSP\*.dxe" echo "!!!!!!!!!!!!!!!! Compiler or linker Error -> scroll up !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"

:END
