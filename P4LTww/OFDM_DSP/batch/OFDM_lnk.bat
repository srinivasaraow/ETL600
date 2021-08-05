REM -------------------------------------------------------------------------
REM $Workfile: OFDM_lnk.bat $
REM Part of      : ETL600
REM Language     : Windows batch file
REM Created by   : Hans-Peter Rytz
REM Remarks      :  
REM Purpose      : 
REM Copyright    : ABB Switzerland Ltd., 2004
REM -------------------------------------------------------------------------
REM
REM -------------------------------------------------------------------------
REM $Log: /branches/Source HSD600/P4LT/OFDM_DSP/batch/OFDM_lnk.bat $
rem 
rem 1     5.04.06 15:33 Chhamaa
rem Initial version
rem 
rem 2     17.03.06 14:41 Chhamaa
REM -------------------------------------------------------------------------

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

REM ============   OFDM functions
rem easm21k.exe -21161  -o %output%\ofdm_dsp_hdr.doj "%ASM_PATH%\ofdm_dsp_hdr.asm"
REM ============   OFDM functions
cc21k  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Modem_Init_basic.doj "%C_PATH%\Modem_Init_basic.c"
rem cc21k  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Modulation_Init_QAM4.doj "%C_PATH%\Modulation_Init_QAM4.c"
rem cc21k  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\Preamble_processing_TX.doj "%C_PATH%\Preamble_processing_TX.c"
cc21k  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_SFC_RX.doj "%C_PATH%\OFDM_SFC_RX.c"
cc21k  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_SFC_TX.doj "%C_PATH%\OFDM_SFC_TX.c"

cc21k  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Modem.doj "%C_PATH%\OFDM_Modem.c"
rem cc21k  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\verify_modem_config.doj "%C_PATH%\verify_modem_config.c"
REM cc21k  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_LFM_Processing_RX.doj "%C_PATH%\OFDM_LFM_Processing_RX.c"
REM cc21k  %OFDM_OPT% %C_OPT% -21161  -c -o %output%\OFDM_Optim1.doj "%C_PATH%\OFDM_Optim1.c"




REM == Software ID 10702061 ->0_21l7
echo off

:LINK
cd %output%\
cc21k @%WORK_DIR%\OFDM_DSP\batch\OFDM_DSP.lnk -T "%WORK_DIR%\OFDM_DSP\architecture\OFDM_P4LT.ldf" -proc ADSP-21161 -L %output% -o "%WORK_DIR%\OFDM_DSP\OFDM_DSP_V0_21l7.dxe" -map "%WORK_DIR%\OFDM_DSP\OFDM_DSP_V0_21l7.xml" -flags-link


:PATCH CRC into DXE file
REM add CRC Patch here!!!!!!!!!!!!!!!!!!!!!!!!!


:LOADER
IF EXIST "%WORK_DIR%\OFDM_DSP\*.dxe" elfloader.exe "%WORK_DIR%\OFDM_DSP\OFDM_DSP_V0_21l7.dxe" -bSPI -fASCII -HostWidth 32 -ExternalMemoryWidth 48 -l "%WORK_DIR%\DSP_Boot_Loader\SPI_Boot_SDRAM\161_spi32_SDRAM.dxe" -o "%WORK_DIR%\OFDM_DSP\OFDM_SlaveSPI32.ldr" -proc ADSP-21161 -v

:CONVERT LDR add comma separator to *.ldr
IF EXIST "%WORK_DIR%\OFDM_DSP\*.ldr" %WORK_DIR%\Tools\ConvertDSP_Loader\ConvertLDR_ETL600.exe "%WORK_DIR%\OFDM_DSP\OFDM_SlaveSPI32.ldr" "%WORK_DIR%\OFDM_DSP\OFDM_SlaveSPI32include.ldr"
IF EXIST "%WORK_DIR%\OFDM_DSP\*include.ldr" copy "%WORK_DIR%\OFDM_DSP\OFDM_SlaveSPI32include.ldr" "%WORK_DIR%\Master_DSP\C\OFDM_SlaveSPI32include.ldr"

IF NOT EXIST "%WORK_DIR%\OFDM_DSP\*.dxe" echo "!!!!!!!!!!!!!!!! Compiler or linker Error -> scroll up !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"

:END
