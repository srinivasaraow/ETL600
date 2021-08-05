REM -------------------------------------------------------------------------
REM $Workfile: NSD.bat $
REM Part of      : ETL600
REM Language     : Windows batch file
REM Created by   : Hans-Peter Rytz
REM Remarks      :  
REM Purpose      : 
REM Copyright    : ABB Switzerland Ltd., 2004
REM -------------------------------------------------------------------------
REM
REM -------------------------------------------------------------------------
REM $Log: /branches/Source O4CV/P4LT/NSD_DSP/batch/NSD.bat $
rem 
rem 18    07-04-13 8:34 Chchlee
rem now using environment variables for assembler, compiler and linker
rem 
rem 17    07-04-13 8:32 Chchlee
rem 
rem 15    29.08.06 11:42 Chhamaa
rem AES_Detector.c added.
rem 
rem 14    10.08.06 18:08 Chhamaa
rem 
rem 10    19.01.06 16:42 Chmibuh
rem CRC Patcher argument (-p -> -s) & Error handling changed
rem 
rem 9     1.11.05 16:23 Chchlee
rem batch files changed / -DDEBUG introduced
rem 
rem 8     24.05.05 14:34 Chpeplu
rem optimizer for NSD_Detector.c changed to Ov100 
rem 
rem 7     11.05.05 20:13 Chmibuh
rem One_Wire_interface & Licencing removed
rem 
rem 6     9.05.05 13:20 Chbrbus
rem MemoryCheck and CRC Patcher initial version added, CAUTION: DO NOT USE
rem -e Switch !!!
rem 
rem 5     9.03.05 14:01 Chpeplu
rem New file NSD_CustomTableConverter.c added
rem 
rem 4     4.03.05 16:55 Chpeplu
rem nsdCustomTable.c added
rem 
rem 3     1.03.05 11:59 Chpeplu
rem NSD_Testsignal_Generator.c added
rem 
rem 2     27.01.05 13:32 Chharyt
rem mkdir %WORK_DIR%\NSD_DSP\debug added
rem 
rem 1     19.01.05 15:33 Chchlee
rem 
rem 27    13.01.05 15:20 Chpeplu
rem New file ArrayFloatCheck.asm added
rem 
rem 26    15.12.04 17:29 Chpeplu
rem New file NSD_Output_At_RxAlarm.c  added
rem 
rem 25    30.11.04 20:31 Buhl01
rem One_Wire_Interface.c & Licening.c added
rem 
rem 24    17.11.04 16:48 Plueer01
rem File added NSD_Monitor.c
rem 
rem 23    27.10.04 11:12 Plueer01
rem File LT_Func_NSD600.doj added
rem 
rem 22    13.10.04 15:04 Rytz01
rem Sorry Michael ich musste den ominösen -e switch wieder entfernen, er
rem ist wirklich nicht ohne!!! Nun hat sich gezeigt, dass der NSD_DSP nicht
rem mehr läuft, wenn dieser geschissene Switch aktiv ist!
rem 
rem 21    5.10.04 15:03 Buhl01
rem Linker -e Switch added (eliminate unused code) 
rem 
rem 20    29.09.04 14:08 Unp00632
rem 
rem 19    2.09.04 14:40 Unp00632
rem 
rem 18    27.08.04 15:24 Buhl01
rem start loader only IF EXIST *.dxe; start convert tool only IF EXIST
rem *.ldr
rem 
rem 17    23.08.04 19:00 Maag01
rem 
rem 16    11.08.04 17:06 Leeb02
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
set DSP_ID=NSD_DSP

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
%EASM21K_EXE% -21161  -o %output%\Interrupt_table_NSD_DSP.doj "%ASM_PATH%\Interrupt_table_NSD_DSP.asm"
%EASM21K_EXE% -21161  -o %output%\Interrupt_routines_NSD_DSP.doj "%ASM_PATH%\Interrupt_routines_NSD_DSP.asm"
%EASM21K_EXE% -21161  -o %output%\BIIIR.doj "%ASM_PATH%\BIIIR.asm"
%EASM21K_EXE% -21161  -o %output%\benchmark.doj "%ASM_PATH%\benchmark.asm"
%EASM21K_EXE% -21161  -o %output%\CRC32_calculation.doj "%ASM_PATH%\CRC32_calculation.asm"
%EASM21K_EXE% -21161  -o %output%\ArrayFloatCheck.doj "%ASM_PATH%\ArrayFloatCheck.asm"

:COMP_C
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -I "%H_PATH%" -c -o %output%\NSD_DSP_Main.doj "%C_PATH%\NSD_DSP_Main.c"
%CC21K_EXE%  %C_OPT% -21161 -D NSD_DSP -I "%H_PATH%" -c -o %output%\TDM_Init.doj "%C_PATH%\TDM_Init.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\idleCounter.doj "%C_PATH%\idleCounter.c"
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -I "%H_PATH%" -c -o %output%\NSD_Transmitter.doj "%C_PATH%\NSD_Transmitter.c"
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -I "%H_PATH%" -c -o %output%\NSD_Detector.doj "%C_PATH%\NSD_Detector.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\Timer.doj "%C_PATH%\Timer.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\Init_NSD_DSP.doj "%C_PATH%\Init_NSD_DSP.c"
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -I "%H_PATH%" -c -o %output%\Rx_Guard_NSD.doj "%C_PATH%\Rx_Guard_NSD.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\ExternalTDM_BusG4AI_HL.doj "%C_PATH%\ExternalTDM_BusG4AI_HL.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\TDM_Service.doj "%C_PATH%\TDM_Service.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\DIAG.doj "%C_PATH%\DIAG.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\LT_Func_NSD600.doj "%C_PATH%\LT_Func_NSD600.c"
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -I "%H_PATH%" -c -o %output%\NSD_Monitor.doj "%C_PATH%\NSD_Monitor.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\NSD_Output_At_RxAlarm.doj "%C_PATH%\NSD_Output_At_RxAlarm.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\NSD_Testsignal_Generator.doj "%C_PATH%\NSD_Testsignal_Generator.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\nsdCustomTable.doj "%C_PATH%\nsdCustomTable.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\NSD_CustomTableConverter.doj "%C_PATH%\NSD_CustomTableConverter.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\AES_Transmitter.doj "%C_PATH%\AES_Transmitter.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\AES_Detector.doj "%C_PATH%\AES_Detector.c"

%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\MemoryCheck.doj "%C_PATH%\MemoryCheck.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\crc.doj                        %C_PATH%\crc.c

echo off

:LINK
%LINKER_EXE% @NSD_DSP.lnk -T "%WORK_DIR%\NSD_DSP\architecture\NSD_DSP-21161.ldf" -DADSP-21161 -L %output% -o "%WORK_DIR%\NSD_DSP\NSD_DSP_V0_00.dxe" -Map "%WORK_DIR%\NSD_DSP\NSD_DSP_V0_00.xml" -warnonce

IF NOT EXIST "%WORK_DIR%\NSD_DSP\*.dxe" echo "!!!!!!!!!!!!!!!! Compiler or linker Error -> scroll up !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
IF NOT EXIST "%WORK_DIR%\NSD_DSP\*.dxe" goto END

:PATCH CRC into DXE file. Argument -s for any Slave DSP
CRC_ChecksumPatcherDSP.exe -s %WORK_DIR%\NSD_DSP\NSD_DSP_V0_00.dxe %WORK_DIR%\NSD_DSP\architecture\NSD_DSP-21161.ldf

REM if CRC_ChecksumPatcherDSP returns 0 continue with Loader
REM if CRC_ChecksumPatcherDSP returns > 0 print error message
if %errorlevel% LSS 1 goto LOADER 
if %errorlevel% GTR 0 goto PATCH_ERROR


:LOADER
IF EXIST "%WORK_DIR%\NSD_DSP\*.dxe" %ELFLOADER_EXE% "%WORK_DIR%\NSD_DSP\NSD_DSP_V0_00.dxe" -bSPI -fASCII -HostWidth 32 -ExternalMemoryWidth 48 -l "%WORK_DIR%\DSP_Boot_Loader\SPI_Boot_SDRAM\161_spi32_SDRAM.dxe" -o "%WORK_DIR%\NSD_DSP\NSD_SlaveSPI32.ldr" -proc ADSP-21161 -v

:CONVERT LDR add comma separator to *.ldr
IF EXIST "%WORK_DIR%\NSD_DSP\*.ldr" %WORK_DIR%\Tools\ConvertDSP_Loader\ConvertLDR_ETL600.exe "%WORK_DIR%\NSD_DSP\NSD_SlaveSPI32.ldr" "%WORK_DIR%\NSD_DSP\NSD_SlaveSPI32include.ldr"
IF EXIST "%WORK_DIR%\NSD_DSP\*include.ldr" copy "%WORK_DIR%\NSD_DSP\NSD_SlaveSPI32include.ldr" "%WORK_DIR%\Master_DSP\C\NSD_SlaveSPI32include.ldr"

goto END

:PATCH_ERROR
echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
echo !!!!!!!!! No *.ldr generated. Error with CRC Patcher  !!!!!!!!!!!!


:END
