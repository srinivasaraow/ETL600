REM -------------------------------------------------------------------------
REM $Workfile: FE_DSP.bat $
REM Part of      : ETL600
REM Language     : Windows batch file
REM Created by   : Hans-Peter Rytz
REM Remarks      :  
REM Purpose      : 
REM Copyright    : ABB Switzerland Ltd., 2004
REM -------------------------------------------------------------------------
REM
REM -------------------------------------------------------------------------
REM $Log: /branches/Source O4CV/P4LT/FE_DSP/batch/FE_DSP.bat $
rem 
rem 15    07-04-13 8:30 Chchlee
rem now using environment variables for assembler, compiler and linker
rem 
rem 11    5.04.06 14:16 Chhamaa
rem txSignalProcessingOfdm.c added
rem 
rem 10    14.03.06 16:12 Chbrbus
rem Added SupervisionUC_Bus_FE_DSP.c for compilation
rem 
rem 9     7.02.06 11:53 Chhamaa
rem 
rem 8     19.01.06 16:42 Chmibuh
rem CRC Patcher argument (-p -> -s) & Error handling changed
rem 
rem 7     1.11.05 16:23 Chchlee
rem batch files changed / -DDEBUG introduced
rem 
rem 6     17.05.05 11:49 Chbrbus
rem float_Check.asm removed (old stuff)
rem 
rem 5     12.05.05 18:33 Chmibuh
rem crc.doj added
rem -e switch re-enabled!!!
rem 
rem 4     9.05.05 11:53 Chbrbus
rem MemoryCheck and Checksum patcher in batch file added - CAUTION: I had
rem to remove the -e switch
rem 
rem 3     12.04.05 15:50 Chhamaa
rem 
rem 2     16.02.05 18:07 Chhamaa
rem 
rem 1     19.01.05 15:31 Chchlee
rem 
rem 33    6.01.05 10:13 Chhamaa
rem 
rem 32    4.01.05 11:34 Chhamaa
rem 
rem 31    8.12.04 18:15 Chhamaa
rem afc.c added
rem 
rem 30    29.11.04 11:52 Maag01
rem 
rem 29    15.11.04 15:54 Maag01
rem 
rem 28    5.11.04 18:33 Maag01
rem 
rem 27    4.11.04 19:27 Maag01
rem 
rem 26    27.10.04 17:53 Maag01
rem 
rem 25    8.10.04 17:21 Maag01
rem 
rem 24    7.10.04 17:59 Maag01
rem 
rem 23    29.09.04 14:08 Unp00632
rem 
rem 22    15.09.04 18:42 Maag01
rem 
rem 21    2.09.04 10:54 Unp00632
rem Added %WORK_FILE%
rem 
rem 20    27.08.04 15:23 Buhl01
rem 
rem 19    27.08.04 15:07 Buhl01
rem start loader only IF EXIST *.dxe; start convert tool only IF EXIST
rem *.ldr
rem 
rem 18    11.08.04 17:06 Leeb02
REM -------------------------------------------------------------------------

rem set executable path
if defined ADSP_PATH (set EASM21K_EXE="%ADSP_PATH%\easm21k") else (set EASM21K_EXE=easm21k)
if defined ADSP_PATH (set CC21K_EXE="%ADSP_PATH%\cc21k") else (set CC21K_EXE=cc21k)
if defined ADSP_PATH (set LINKER_EXE="%ADSP_PATH%\linker") else (set LINKER_EXE=linker)
if defined ADSP_PATH (set ELFLOADER_EXE="%ADSP_PATH%\elfloader") else (set ELFLOADER_EXE=elfloader)

echo on

set WORK_DIR=D:\ADSP\$P4LT
if defined P4LT_BUILD_PATH set WORK_DIR=%P4LT_BUILD_PATH%

set DSP_ID=FE_DSP

:BEGIN
IF "%1" == "-debug"   (SET output=%WORK_DIR%\%DSP_ID%\debug&SET C_OPT=-g -DDEBUG&mkdir %WORK_DIR%\%DSP_ID%\debug)
IF "%1" == "-release" (SET output=%WORK_DIR%\%DSP_ID%\release&SET C_OPT=&mkdir %WORK_DIR%\%DSP_ID%\release)

SET H_PATH=%WORK_DIR%\%DSP_ID%\H
SET C_PATH=%WORK_DIR%\%DSP_ID%\C
SET ASM_PATH=%WORK_DIR%\%DSP_ID%\ASM

:DELETE
del %output%\*.doj

del %WORK_DIR%\%DSP_ID%\*.dxe            
del %WORK_DIR%\%DSP_ID%\*.ldr


:COMP_ASM
%EASM21K_EXE% -21161  -o %output%\Interrupt_table_FE_DSP.doj "%ASM_PATH%\Interrupt_table_FE_DSP.asm"
%EASM21K_EXE% -21161 -I "%H_PATH%" -o %output%\AD6620_utils.doj "%ASM_PATH%\AD6620_utils.asm"
%EASM21K_EXE% -21161  -o %output%\ArrayFloatCheck.doj "%ASM_PATH%\ArrayFloatCheck.asm"
%EASM21K_EXE% -21161  -o %output%\benchmark.doj "%ASM_PATH%\benchmark.asm"
%EASM21K_EXE% -21161  -o %output%\BiIIR.doj "%ASM_PATH%\BiIIR.asm"
%EASM21K_EXE% -21161  -o %output%\CDBIQUAD.doj "%ASM_PATH%\CDBIQUAD.asm"
%EASM21K_EXE% -21161  -o %output%\Interrupt_routines_FE_DSP.doj "%ASM_PATH%\Interrupt_routines_FE_DSP.asm"
%EASM21K_EXE% -21161 -I "%H_PATH%" -o %output%\PDEMOD.doj "%ASM_PATH%\PDEMOD.asm"
%EASM21K_EXE% -21161 -I "%H_PATH%" -o %output%\PMOD.doj "%ASM_PATH%\PMOD.asm"
%EASM21K_EXE% -21161 -I "%H_PATH%" -o %output%\PN_GENER.doj "%ASM_PATH%\PN_GENER.asm"
%EASM21K_EXE% -21161 -I "%H_PATH%" -o %output%\TTONE.doj "%ASM_PATH%\TTONE.asm"


:COMP_C
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\AD6620_reg_io.doj "%C_PATH%\AD6620_reg_io.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\afc.doj "%C_PATH%\afc.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\apFilter.doj "%C_PATH%\apFilter.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\BiIIR_Filter.doj "%C_PATH%\BiIIR_Filter.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\diag.doj "%C_PATH%\diag.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\equalizerFirFilter.doj "%C_PATH%\equalizerFirFilter.c
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\FE_DSP_Alarm.doj "%C_PATH%\FE_DSP_Alarm.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\FE_DSP_Main.doj "%C_PATH%\FE_DSP_Main.c"
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -I "%H_PATH%" -c -o %output%\freqShift.doj "%C_PATH%\freqShift.c"
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -I "%H_PATH%" -c -o %output%\idleCounter.doj "%C_PATH%\idleCounter.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\Init_FE_DSP.doj "%C_PATH%\Init_FE_DSP.c"
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -I "%H_PATH%" -c -o %output%\intDec.doj "%C_PATH%\intDec.c"
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -I "%H_PATH%" -c -o %output%\oscillator.doj "%C_PATH%\oscillator.c"
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -I "%H_PATH%" -c -o %output%\PilotMod.doj "%C_PATH%\PilotMod.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\sampling.doj "%C_PATH%\sampling.c"
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -I "%H_PATH%" -c -o %output%\SignalGen.doj "%C_PATH%\SignalGen.c"
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -I "%H_PATH%" -c -o %output%\SSB.doj "%C_PATH%\SSB.c"
%CC21K_EXE%  %C_OPT% -21161 -DFE_DSP -I "%H_PATH%" -c -o %output%\TDM_Init.doj "%C_PATH%\TDM_Init.c"
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -21161 -I "%H_PATH%" -c -o %output%\testTone.doj "%C_PATH%\testTone.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\Timer.doj "%C_PATH%\Timer.c"
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -I "%H_PATH%" -c -o %output%\tdmEocData.doj "%C_PATH%\tdmEocData.c"
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -I "%H_PATH%" -c -o %output%\TDM_Service.doj "%C_PATH%\TDM_Service.C"
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -I "%H_PATH%" -c -o %output%\testPlcLink.doj "%C_PATH%\testPlcLink.c"
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -I "%H_PATH%" -c -o %output%\txSignalProcessingOfdm.doj "%C_PATH%\txSignalProcessingOfdm.c"
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -I "%H_PATH%" -c -o %output%\upDownConvTdmBus.doj "%C_PATH%\upDownConvTdmBus.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\MemoryCheck.doj "%C_PATH%\MemoryCheck.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\crc.doj "%C_PATH%\crc.c"
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -I "%H_PATH%" -c -o %output%\SupervisionUC_Bus_FE_DSP.doj "%C_PATH%\SupervisionUC_Bus_FE_DSP.c"


echo off

:LINK
%LINKER_EXE% @FE_DSP.lnk -e -T "%WORK_DIR%\FE_DSP\architecture\FE_DSP-21161.ldf" -DADSP-21161 -L %output% -o "%WORK_DIR%\FE_DSP\FE_DSP_V0_00.dxe" -Map "%WORK_DIR%\FE_DSP\FE_DSP_V0_00.xml" -warnonce

IF NOT EXIST "%WORK_DIR%\FE_DSP\*.dxe" echo "!!!!!!!!!!!!!!!! Compiler or linker Error -> scroll up !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
IF NOT EXIST "%WORK_DIR%\FE_DSP\*.dxe" goto end

:PATCH CRC into DXE file. Argument -s for any Slave DSP
CRC_ChecksumPatcherDSP.exe -s %WORK_DIR%\FE_DSP\FE_DSP_V0_00.dxe %WORK_DIR%\FE_DSP\architecture\FE_DSP-21161.ldf

REM if CRC_ChecksumPatcherDSP returns 0 continue with Loader
REM if CRC_ChecksumPatcherDSP returns > 0 print error message
if %errorlevel% LSS 1 goto LOADER 
if %errorlevel% GTR 0 goto PATCH_ERROR


:LOADER
IF EXIST "%WORK_DIR%\FE_DSP\*.dxe" %ELFLOADER_EXE% "%WORK_DIR%\FE_DSP\FE_DSP_V0_00.dxe" -bSPI -fASCII -HostWidth 32 -ExternalMemoryWidth 48 -l "%WORK_DIR%\DSP_Boot_Loader\SPI_Boot_SDRAM\161_spi32_SDRAM.dxe" -o "%WORK_DIR%\FE_DSP\AFE_SlaveSPI32.ldr" -proc ADSP-21161 -v

:CONVERT LDR add comma separator to *.ldr
IF EXIST "%WORK_DIR%\FE_DSP\*.ldr"  %WORK_DIR%\Tools\ConvertDSP_Loader\ConvertLDR_ETL600.exe "%WORK_DIR%\FE_DSP\AFE_SlaveSPI32.ldr" "%WORK_DIR%\FE_DSP\AFE_SlaveSPI32include.ldr"
IF EXIST "%WORK_DIR%\FE_DSP\*include.ldr"  copy "%WORK_DIR%\FE_DSP\AFE_SlaveSPI32include.ldr" "%WORK_DIR%\Master_DSP\C\AFE_SlaveSPI32include.ldr"

goto END

:PATCH_ERROR
echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
echo !!!!!!!!! No *.ldr generated. Error with CRC Patcher  !!!!!!!!!!!!

:END
