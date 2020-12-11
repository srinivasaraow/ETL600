# Microsoft Developer Studio Project File - Name="OFDM_DSP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=OFDM_DSP - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OFDM_DSP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OFDM_DSP.mak" CFG="OFDM_DSP - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OFDM_DSP - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "OFDM_DSP - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Source/P4LT/OFDM_DSP", LBAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OFDM_DSP - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "NDEBUG"
# ADD RSC /l 0x807 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "OFDM_DSP - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "_DEBUG"
# ADD RSC /l 0x807 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "OFDM_DSP - Win32 Release"
# Name "OFDM_DSP - Win32 Debug"
# Begin Group "C"

# PROP Default_Filter ""
# Begin Group "QAM4_FEC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\C\QAM4_FEC\crc32.C
# End Source File
# Begin Source File

SOURCE=.\C\QAM4_FEC\Decode_CHD.C
# End Source File
# Begin Source File

SOURCE=.\C\QAM4_FEC\Encode_CHD.C
# End Source File
# End Group
# Begin Group "RS_FEC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\C\RS_FEC\rs_decoder.c
# End Source File
# Begin Source File

SOURCE=.\C\RS_FEC\rs_decoder_init.c
# End Source File
# Begin Source File

SOURCE=.\C\RS_FEC\rs_decoder_init_cb.c
# End Source File
# Begin Source File

SOURCE=.\C\RS_FEC\rs_encoder.c
# End Source File
# Begin Source File

SOURCE=.\C\RS_FEC\rs_encoder_init.c
# End Source File
# Begin Source File

SOURCE=.\C\RS_FEC\rs_encoder_init_cb.c
# End Source File
# Begin Source File

SOURCE=.\C\RS_FEC\rs_nk_decoder.c
# End Source File
# Begin Source File

SOURCE=.\C\RS_FEC\rs_nk_encoder.c
# End Source File
# End Group
# Begin Group "TCM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\C\TCM\tcm_ber_tester.c
# End Source File
# Begin Source File

SOURCE=.\C\TCM\tcm_ber_tester_init.c
# End Source File
# Begin Source File

SOURCE=.\C\TCM\TCM_bitcalc.c
# End Source File
# Begin Source File

SOURCE=.\C\TCM\TCM_Decoder.c
# End Source File
# Begin Source File

SOURCE=.\C\TCM\TCM_Delta_Gen_Init.c
# End Source File
# Begin Source File

SOURCE=.\C\TCM\TCM_Dummy_Gen_Init.c
# End Source File
# Begin Source File

SOURCE=.\C\TCM\TCM_Encoder.c
# End Source File
# Begin Source File

SOURCE=.\C\TCM\TCM_Encoder_REF.c
# End Source File
# Begin Source File

SOURCE=.\C\TCM\TCM_Gen_Delta_Bits.c
# End Source File
# Begin Source File

SOURCE=.\C\TCM\TCM_Gen_Dummy_Data.c
# End Source File
# Begin Source File

SOURCE=.\C\TCM\TCM_Gen_MQAM_Train_Data_RX.c
# End Source File
# Begin Source File

SOURCE=.\C\TCM\TCM_Gen_MQAM_Train_Data_TX.c
# End Source File
# Begin Source File

SOURCE=.\C\TCM\TCM_Gen_PCM_Data.c
# End Source File
# Begin Source File

SOURCE=.\C\TCM\TCM_Init_4QAM_Codec.c
# End Source File
# Begin Source File

SOURCE=.\C\TCM\TCM_Init_Decoder.c
# End Source File
# Begin Source File

SOURCE=.\C\TCM\TCM_Init_Encoder.c
# End Source File
# Begin Source File

SOURCE=.\C\TCM\TCM_Init_MQAM_Decoder.c
# End Source File
# Begin Source File

SOURCE=.\C\TCM\TCM_Init_MQAM_Encoder.c
# End Source File
# Begin Source File

SOURCE=.\C\TCM\TCM_MQAM_Train_Gen_Init.c
# End Source File
# Begin Source File

SOURCE=.\C\TCM\TCM_PCM_Gen_Init.c
# End Source File
# Begin Source File

SOURCE=.\C\TCM\TCM_Shaping_Decoder.c
# End Source File
# End Group
# Begin Group "TDM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\C\TDM\clear_alarm.c
# End Source File
# Begin Source File

SOURCE=.\C\TDM\get_configuration.c
# End Source File
# Begin Source File

SOURCE=.\C\TDM\get_message.c
# End Source File
# Begin Source File

SOURCE=.\C\TDM\init_control_services.c
# End Source File
# Begin Source File

SOURCE=.\C\TDM\Master_Message_Handler.c
# End Source File
# Begin Source File

SOURCE=.\C\TDM\ofdm_tdm_adjust.c
# End Source File
# Begin Source File

SOURCE=.\C\TDM\ofdm_tdm_init.c
# End Source File
# Begin Source File

SOURCE=.\C\TDM\ofdm_tdm_stop.c
# End Source File
# Begin Source File

SOURCE=.\C\TDM\send_message.c
# End Source File
# Begin Source File

SOURCE=.\C\TDM\set_alarm.c
# End Source File
# End Group
# Begin Group "V11"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\C\V11\Adjust_V11_RX_Buffer.c
# End Source File
# Begin Source File

SOURCE=.\C\V11\Adjust_V11_TX_Buffer.c
# End Source File
# Begin Source File

SOURCE=.\C\V11\Get_V11_Divider.c
# End Source File
# Begin Source File

SOURCE=.\C\V11\V11_Init.c
# End Source File
# End Group
# Begin Group "DCTCXO No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\C\DCTCXO\ds4000_convert.c
# End Source File
# Begin Source File

SOURCE=.\C\DCTCXO\ds4000_io.c
# End Source File
# End Group
# Begin Group "int_dec_filters No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\C\INT_DEC_FILTERS\dec2x12_filt_c_init.c
# End Source File
# Begin Source File

SOURCE=.\C\INT_DEC_FILTERS\dec4x24_filt_c_init.c
# End Source File
# Begin Source File

SOURCE=.\C\INT_DEC_FILTERS\Int2x12_filt_c_init.c
# End Source File
# Begin Source File

SOURCE=.\C\INT_DEC_FILTERS\Int3x24_filt_c_init.c
# End Source File
# Begin Source File

SOURCE=.\C\INT_DEC_FILTERS\Int4x24_filt_c_Init.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\C\Descrambler.c
# End Source File
# Begin Source File

SOURCE=.\C\Descrambler_Init.c
# End Source File
# Begin Source File

SOURCE=.\C\Error_handler.c
# End Source File
# Begin Source File

SOURCE=.\C\Exp_Gen.c
# End Source File
# Begin Source File

SOURCE=.\C\FFT.c
# End Source File
# Begin Source File

SOURCE=.\C\Fr_Tracking.c
# End Source File
# Begin Source File

SOURCE=.\C\Get_Stuffing_Flag_RX.c
# End Source File
# Begin Source File

SOURCE=.\C\Get_Stuffing_Flag_TX.c
# End Source File
# Begin Source File

SOURCE=.\C\IFFT.c
# End Source File
# Begin Source File

SOURCE=.\C\Modem_Init_basic.c
# End Source File
# Begin Source File

SOURCE=.\C\Modulation_Init_QAM4.c
# End Source File
# Begin Source File

SOURCE=.\C\Modulation_Init_QAM_M_v2.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_2Tone_Correlation.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_AWGN_NB_Estimation.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_Channel_Simulator.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_Channel_Simulator_Init.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_Detection_AGC.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_Engine_RX.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_Engine_TX.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_Fall_BackForward.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_Filter_RX.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_Filter_RX_Watchdog.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_Filter_TX.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_Freq_Estim_RX.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_FreqCtrl_RX.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_LFM_Processing_RX.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_Modem.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_NB_Detection.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_NB_Interpolation.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_Noise_Estimation.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_opt_decide_rate.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_opt_decide_stuffing.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_Optim1.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_Optim2.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_Optim3.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_QAM4_Data_Preparation.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_QAM4_Detection.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_Receiver.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_Restart.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_SFC_RX.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_SFC_TX.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_Spectrum_RX.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_Tracking_Engine_RX.c
# End Source File
# Begin Source File

SOURCE=.\C\OFDM_Transmitter.c
# End Source File
# Begin Source File

SOURCE=.\C\Pilots_Preprocessing.c
# End Source File
# Begin Source File

SOURCE=.\C\Preamble_processing_RX.c
# End Source File
# Begin Source File

SOURCE=.\C\Preamble_processing_TX.c
# End Source File
# Begin Source File

SOURCE=.\C\Preamble_scheduler_RX.c
# End Source File
# Begin Source File

SOURCE=.\C\Preamble_scheduler_TX.c
# End Source File
# Begin Source File

SOURCE=.\C\QAM_processing_RX.c
# End Source File
# Begin Source File

SOURCE=.\C\QAM_processing_TX.c
# End Source File
# Begin Source File

SOURCE=.\C\QAM_scheduler_RX.c
# End Source File
# Begin Source File

SOURCE=.\C\QAM_scheduler_TX.c
# End Source File
# Begin Source File

SOURCE=.\C\Scrambler.c
# End Source File
# Begin Source File

SOURCE=.\C\Scrambler_Init.c
# End Source File
# Begin Source File

SOURCE=.\C\Service_Bits.c
# End Source File
# Begin Source File

SOURCE=.\C\Sim_AWGN.c
# End Source File
# Begin Source File

SOURCE=.\C\Sim_Frequency_Offset.c
# End Source File
# Begin Source File

SOURCE=.\C\Sim_NB.c
# End Source File
# Begin Source File

SOURCE=.\C\Sim_NL_Lin_Dist.c
# End Source File
# Begin Source File

SOURCE=.\C\Verify_Modem_Config.c
# End Source File
# Begin Source File

SOURCE=.\C\Wait_for_Config.c
# End Source File
# End Group
# Begin Group "H"

# PROP Default_Filter ""
# Begin Group "FFT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\H\FFT\fft_c_dpd.h
# End Source File
# Begin Source File

SOURCE=.\H\FFT\fft_c_dpd_defs.h
# End Source File
# Begin Source File

SOURCE=.\H\FFT\twid2048.dat
# End Source File
# End Group
# Begin Group "P4LT_TDM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\H\P4LT_TDM\p4lt_tdm_defs.h
# End Source File
# Begin Source File

SOURCE=.\H\P4LT_TDM\tdm_bus_io.h
# End Source File
# End Group
# Begin Group "QAM4_FEC No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\H\QAM4_FEC\C3212.h
# End Source File
# Begin Source File

SOURCE=.\H\QAM4_FEC\channel_data.h
# End Source File
# End Group
# Begin Group "RS_FEC No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\H\RS_FEC\RS6355.h
# End Source File
# Begin Source File

SOURCE=.\H\RS_FEC\RS6355_DEFS.h
# End Source File
# Begin Source File

SOURCE=.\H\RS_FEC\rs_decoder.h
# End Source File
# Begin Source File

SOURCE=.\H\RS_FEC\RS_DEFS.h
# End Source File
# Begin Source File

SOURCE=.\H\RS_FEC\rs_encoder.h
# End Source File
# Begin Source File

SOURCE=.\H\RS_FEC\RS_externs.h
# End Source File
# Begin Source File

SOURCE=.\H\RS_FEC\RS_globals.h
# End Source File
# Begin Source File

SOURCE=.\H\RS_FEC\rs_nk_decoder.h
# End Source File
# Begin Source File

SOURCE=.\H\RS_FEC\rs_nk_encoder.h
# End Source File
# End Group
# Begin Group "TCM No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\H\TCM\TCM_Codec.h
# End Source File
# Begin Source File

SOURCE=.\H\TCM\TCM_Codec_externs.h
# End Source File
# Begin Source File

SOURCE=.\H\TCM\TCM_Codec_globals.h
# End Source File
# Begin Source File

SOURCE=.\H\TCM\TCM_defs.h
# End Source File
# End Group
# Begin Group "TDM No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\H\TDM\ofdm_tdm_externs.h
# End Source File
# Begin Source File

SOURCE=.\H\TDM\ofdm_tdm_globals.h
# End Source File
# Begin Source File

SOURCE=.\H\TDM\ofdm_tdm_io.h
# End Source File
# End Group
# Begin Group "UTIL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\H\UTIL\ADSP_Init.h
# End Source File
# Begin Source File

SOURCE=.\H\UTIL\gamma.h
# End Source File
# Begin Source File

SOURCE=.\H\UTIL\ma.h
# End Source File
# Begin Source File

SOURCE=.\H\UTIL\sig_glob.h
# End Source File
# Begin Source File

SOURCE=.\H\UTIL\speeddsp.h
# End Source File
# Begin Source File

SOURCE=.\H\UTIL\timer.h
# End Source File
# Begin Source File

SOURCE=.\H\UTIL\util.h
# End Source File
# End Group
# Begin Group "V11 No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\H\V11\V11_defs.h
# End Source File
# Begin Source File

SOURCE=.\H\V11\V11_externs.h
# End Source File
# Begin Source File

SOURCE=.\H\V11\V11_globals.h
# End Source File
# End Group
# Begin Group "DCTCXO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\H\DCTCXO\ds4000.h
# End Source File
# Begin Source File

SOURCE=.\H\DCTCXO\ds4000_defs.h
# End Source File
# Begin Source File

SOURCE=.\H\DCTCXO\ds4000_externs.h
# End Source File
# Begin Source File

SOURCE=.\H\DCTCXO\ds4000_globals.h
# End Source File
# End Group
# Begin Group "TX_RX_FILTERS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\H\TX_RX_FILTERS\h20_128.dat
# End Source File
# Begin Source File

SOURCE=.\H\TX_RX_FILTERS\h20_128_16.dat
# End Source File
# Begin Source File

SOURCE=.\H\TX_RX_FILTERS\h20_128_8.dat
# End Source File
# Begin Source File

SOURCE=.\H\TX_RX_FILTERS\h20_256.dat
# End Source File
# Begin Source File

SOURCE=.\H\TX_RX_FILTERS\h20_512.dat
# End Source File
# Begin Source File

SOURCE=.\H\TX_RX_FILTERS\h20_64.dat
# End Source File
# Begin Source File

SOURCE=.\H\TX_RX_FILTERS\h28_128.dat
# End Source File
# Begin Source File

SOURCE=.\H\TX_RX_FILTERS\h28_128_16.dat
# End Source File
# Begin Source File

SOURCE=.\H\TX_RX_FILTERS\h28_128_8.dat
# End Source File
# Begin Source File

SOURCE=.\H\TX_RX_FILTERS\h28_256.dat
# End Source File
# Begin Source File

SOURCE=.\H\TX_RX_FILTERS\h28_512.dat
# End Source File
# Begin Source File

SOURCE=.\H\TX_RX_FILTERS\h28_64.dat
# End Source File
# Begin Source File

SOURCE=.\H\TX_RX_FILTERS\h32_128.dat
# End Source File
# Begin Source File

SOURCE=.\H\TX_RX_FILTERS\h32_128_16.dat
# End Source File
# Begin Source File

SOURCE=.\H\TX_RX_FILTERS\h32_128_8.dat
# End Source File
# Begin Source File

SOURCE=.\H\TX_RX_FILTERS\h32_256.dat
# End Source File
# Begin Source File

SOURCE=.\H\TX_RX_FILTERS\h32_512.dat
# End Source File
# Begin Source File

SOURCE=.\H\TX_RX_FILTERS\h32_64.dat
# End Source File
# Begin Source File

SOURCE=.\H\TX_RX_FILTERS\h4r_128_16.dat
# End Source File
# Begin Source File

SOURCE=.\H\TX_RX_FILTERS\h4r_128_8.dat
# End Source File
# Begin Source File

SOURCE=.\H\TX_RX_FILTERS\TXRX_FILT_externs.h
# End Source File
# Begin Source File

SOURCE=.\H\TX_RX_FILTERS\TXRX_FILT_globals.h
# End Source File
# End Group
# Begin Group "INT_DEC_FILTERS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\H\INT_DEC_FILTERS\H2x12_0.dat
# End Source File
# Begin Source File

SOURCE=.\H\INT_DEC_FILTERS\H2x12_1.dat
# End Source File
# Begin Source File

SOURCE=.\H\INT_DEC_FILTERS\H3x24_0.dat
# End Source File
# Begin Source File

SOURCE=.\H\INT_DEC_FILTERS\H3x24_1.dat
# End Source File
# Begin Source File

SOURCE=.\H\INT_DEC_FILTERS\H3x24_2.dat
# End Source File
# Begin Source File

SOURCE=.\H\INT_DEC_FILTERS\H4x24_0.dat
# End Source File
# Begin Source File

SOURCE=.\H\INT_DEC_FILTERS\H4x24_1.dat
# End Source File
# Begin Source File

SOURCE=.\H\INT_DEC_FILTERS\H4x24_2.dat
# End Source File
# Begin Source File

SOURCE=.\H\INT_DEC_FILTERS\H4x24_3.dat
# End Source File
# Begin Source File

SOURCE=.\H\INT_DEC_FILTERS\INTDEC_defs.h
# End Source File
# Begin Source File

SOURCE=.\H\INT_DEC_FILTERS\INTDEC_externs.h
# End Source File
# Begin Source File

SOURCE=.\H\INT_DEC_FILTERS\INTDEC_filt_c.h
# End Source File
# Begin Source File

SOURCE=.\H\INT_DEC_FILTERS\INTDEC_globals.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\H\BASETYPE.H
# End Source File
# Begin Source File

SOURCE=.\H\board.H
# End Source File
# Begin Source File

SOURCE=.\H\Channel_Random_Data.h
# End Source File
# Begin Source File

SOURCE=.\H\CONSTANT.H
# End Source File
# Begin Source File

SOURCE=.\H\Init_OFDM_DSP.H
# End Source File
# Begin Source File

SOURCE=.\H\List_of_data_rates.h
# End Source File
# Begin Source File

SOURCE=.\H\OFDM_CFG_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\OFDM_Defs.h
# End Source File
# Begin Source File

SOURCE=.\H\OFDM_DSP_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\OFDM_externs.h
# End Source File
# Begin Source File

SOURCE=.\H\OFDM_FPGA.h
# End Source File
# Begin Source File

SOURCE=.\H\OFDM_globals.h
# End Source File
# Begin Source File

SOURCE=.\H\OFDM_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\Prefix.h
# End Source File
# Begin Source File

SOURCE=.\H\Suffix.h
# End Source File
# Begin Source File

SOURCE=.\H\tdm_int_cfg.h
# End Source File
# Begin Source File

SOURCE=.\H\tdm_int_types.h
# End Source File
# Begin Source File

SOURCE=.\H\TDM_Service.H
# End Source File
# End Group
# Begin Group "ASM"

# PROP Default_Filter ""
# Begin Group "FFT No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ASM\FFT\fft_c_dpd.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\FFT\ifft_c_dpd.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\FFT\twid2048.dat
# End Source File
# Begin Source File

SOURCE=.\ASM\FFT\twid_c_dpd.asm
# End Source File
# End Group
# Begin Group "P4LT_TDM No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ASM\P4LT_TDM\tdm_bus_io.asm
# End Source File
# End Group
# Begin Group "QAM4_FEC No. 2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ASM\QAM4_FEC\Decoder_3212.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\QAM4_FEC\Encoder_3212.asm
# End Source File
# End Group
# Begin Group "RS_FEC No. 2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ASM\RS_FEC\FFT63_8.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\RS_FEC\IFFT63.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\RS_FEC\RS6355_Decoder.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\RS_FEC\RS6355_Encoder.asm
# End Source File
# End Group
# Begin Group "TCM No. 2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ASM\TCM\TCM_CRC5_Decoder.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\TCM\TCM_CRC5_Encoder.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\TCM\TCM_Decoder_Extract_Gr_Cd_Bits.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\TCM\TCM_Decoder_Remodulate.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\TCM\TCM_Decoder_Vcopy_SPbits_DD.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\TCM\TCM_Decoder_Vget_SPbits_DD.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\TCM\TCM_Gray_Encoder.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\TCM\TCM_Shaping_Encoder.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\TCM\TCM_Trellis_34_Encoder.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\TCM\TCM_Trellis_34_ViterbiDecoder.asm
# End Source File
# End Group
# Begin Group "TDM No. 2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ASM\TDM\ofdm_sync_to_tdm.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\TDM\ofdm_tdm_rec_isr.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\TDM\ofdm_tdm_trm_isr.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\TDM\vcopy_in_buf.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\TDM\vcopy_out_buf.asm
# End Source File
# End Group
# Begin Group "UTIL No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ASM\UTIL\ADSP_Init.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\DRS7_random.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\Init_OFDM_DSP.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\Pack_1Bit_To_32BitWord_DD.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\svdivz_f_DD.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\timer.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\Unpack_32Bit_To_1BitWord_DD.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vabs_uc_DDP.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vadd_f_EEE.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vconjg_c_DD.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vcopy_EE.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vcopy_pe.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vcopy_s_ED.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vdiv_f_EEE.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vdivz_c_DDD.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vdivz_f_DDD.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vdivz_f_DDP.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vdivz_f_EEE.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vfix_c_DD.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vfloat_cs_DD.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vfloat_DE.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vfsmulvadd_c_DDD.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vgamma_off_dd.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vgamma_on_dd.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vgauss_f_D.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vkalman_c_DDD.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vkalman_f_DDD.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vmag2_c_DD.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vmag2_c_DE.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vmag2_uc_DDP.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vmax_f_E.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vmax_i_E.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vmin_f_E.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vmin_i_E.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vmul_c_EEE.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vmul_f_EEE.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vones_i_E.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vset_f_E.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vset_sf_E.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vsmul_f_EE.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vsorti_f_DDD.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vssver.scc
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vsubmul_c_DDPD.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vsum_f_E.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vsum_i_E.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vsummag2_c_E.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vsummag2_uc_DP.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vwin_c_DDD.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\UTIL\vxor_i_EEE.asm
# End Source File
# End Group
# Begin Group "V11 No. 2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ASM\V11\Copy_from_V11_Buffer.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\V11\Copy_to_V11_Buffer.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\V11\DSP2_SPORT02_V11.asm
# End Source File
# End Group
# Begin Source File

SOURCE=.\ASM\Extract_QAM.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\Interrupt_routines_OFDM_DSP.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\Interrupt_table_21161.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\ofdm_dsp_hdr.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\Spectrum_QAM.asm
# End Source File
# End Group
# Begin Group "architecture"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\architecture\OFDM_P4LT.ldf
# End Source File
# End Group
# Begin Group "batch"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\batch\OFDM.bat
# End Source File
# Begin Source File

SOURCE=.\batch\OFDM_DSP.lnk
# End Source File
# Begin Source File

SOURCE=.\batch\OFDM_lnk.bat
# End Source File
# End Group
# End Target
# End Project
