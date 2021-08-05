REM -------------------------------------------------------------------------
REM $Workfile: UC_DSP.bat $
REM Part of      : ETL600
REM Language     : Windows batch file
REM Created by   : Hans-Peter Rytz
REM Remarks      :  
REM Purpose      : 
REM Copyright    : ABB Switzerland Ltd., 2004
REM -------------------------------------------------------------------------
REM
REM -------------------------------------------------------------------------
REM $Log: /branches/Source O4CV/P4LT/UC_DSP/batch/UC_DSP.bat $
rem 
rem 16    07-04-13 8:38 Chchlee
rem now using environment variables for assembler, compiler and linker
rem 
rem 12    4.04.06 18:33 Chmibuh
rem SupervisionUC_Bus_UC_DSP.c added for compilation. Function tested.
rem 
rem 10    19.01.06 16:42 Chmibuh
rem CRC Patcher argument (-p -> -s) & Error handling changed
rem 
rem 9     1.11.05 16:23 Chchlee
rem batch files changed / -DDEBUG introduced
rem 
rem 8     24.06.05 11:24 Chmibuh
rem Disable optimizer for UC_DSP_Main.c
rem 
rem 7     22.06.05 14:38 Chhamaa
rem Optimizer activated for UC_DSP_Main.c and tdmUpConvSettings.c 
rem 
rem 6     12.05.05 18:31 Chmibuh
rem crc.doj added
rem 
rem 5     9.05.05 16:53 Chbrbus
rem MemoryCheck function added - memory check itself has still to be
rem implemented
rem 
rem 4     9.05.05 14:35 Chbrbus
rem DSP CRC Patcher added
rem 
rem 3     16.02.05 16:51 Chhamaa
rem 
rem 2     27.01.05 13:34 Chharyt
rem mkdir %WORK_DIR%\UC_DSP\debug added
rem 
rem 1     20.01.05 8:32 Chchlee
rem 
rem 34    4.01.05 14:07 Chhamaa
rem 
rem 33    7.10.04 15:43 Maag01
rem 
rem 32    5.10.04 18:25 Maag01
rem Do not use -e switch!
rem 
rem 31    5.10.04 15:03 Buhl01
rem Linker -e Switch added (eliminate unused code) 
rem 
rem 30    29.09.04 14:08 Unp00632
rem 
rem 29    2.09.04 14:55 Unp00632
rem Added %WORK_DIR%
rem 
rem 28    31.08.04 7:46 Leeb02
rem -D NO_EXT_RAM moved to C_OPT
rem 
rem 27    30.08.04 16:27 Rytz01
rem 
rem 26    27.08.04 15:47 Buhl01
rem start loader only IF EXIST *.dxe; start convert tool only IF EXIST
rem *.ldr
rem 
rem 25    12.08.04 13:37 Maag01
rem 
rem 24    11.08.04 18:23 Maag01
rem 
rem 23    11.08.04 17:07 Leeb02
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
set DSP_ID=UC_DSP

IF "%1" == "-debug"   (SET output=%WORK_DIR%\%DSP_ID%\debug&SET C_OPT=-g -D NO_EXT_RAM -DDEBUG&mkdir %WORK_DIR%\%DSP_ID%\debug)
IF "%1" == "-release" (SET output=%WORK_DIR%\%DSP_ID%\release&SET C_OPT= -D NO_EXT_RAM&mkdir %WORK_DIR%\%DSP_ID%\release)

IF "%1" == "-debug" SET output=%WORK_DIR%\UC_DSP\debug
IF "%1" == "-debug" SET C_OPT=-g -D NO_EXT_RAM
IF "%1" == "-release" SET output=%WORK_DIR%\UC_DSP\release
IF "%1" == "-release" SET C_OPT= -D NO_EXT_RAM

SET H_PATH=%WORK_DIR%\UC_DSP\H
SET C_PATH=%WORK_DIR%\UC_DSP\C
SET ASM_PATH=%WORK_DIR%\UC_DSP\ASM

SET H_PATH=%WORK_DIR%\%DSP_ID%\H
SET C_PATH=%WORK_DIR%\%DSP_ID%\C
SET ASM_PATH=%WORK_DIR%\%DSP_ID%\ASM

:DELETE
del %output%\*.doj

del %WORK_DIR%\%DSP_ID%\*.dxe            
del %WORK_DIR%\%DSP_ID%\*.ldr


:COMP_ASM
%EASM21K_EXE% -21161  -o %output%\AD9777_dout.doj "%ASM_PATH%\AD9777_dout.asm"
%EASM21K_EXE% -21161  -o %output%\AD9777_utils.doj "%ASM_PATH%\AD9777_utils.asm"
%EASM21K_EXE% -21161  -o %output%\ArrayFloatCheck.doj "%ASM_PATH%\ArrayFloatCheck.asm"
%EASM21K_EXE% -21161  -o %output%\benchmark.doj "%ASM_PATH%\benchmark.asm"
%EASM21K_EXE% -21161  -o %output%\Interrupt_routines_UC_DSP.doj "%ASM_PATH%\Interrupt_routines_UC_DSP.asm"
%EASM21K_EXE% -21161  -o %output%\Interrupt_table_UC_DSP.doj "%ASM_PATH%\Interrupt_table_UC_DSP.asm"
%EASM21K_EXE% -21161  -o %output%\Interp4_filt_c_L16.doj "%ASM_PATH%\Interp4_filt_c_L16.asm"
%EASM21K_EXE% -21161  -o %output%\Interp4_filt_c_L24.doj "%ASM_PATH%\Interp4_filt_c_L24.asm"
%EASM21K_EXE% -21161  -o %output%\Int4x44_filt_c.doj "%ASM_PATH%\Int4x44_filt_c.asm"

:COMP_C
%CC21K_EXE% %C_OPT%         	    -21161 -I "%H_PATH%" -c -o %output%\AD9777_Init.doj "%C_PATH%\AD9777_Init.c"
%CC21K_EXE% %C_OPT%                 -21161 -I "%H_PATH%" -c -o %output%\diag.doj "%C_PATH%\diag.c"
%CC21K_EXE% %C_OPT% -O1 -Ov100 -Oa  -21161 -I "%H_PATH%" -c -o %output%\idleCounter.doj "%C_PATH%\idleCounter.c"
%CC21K_EXE% %C_OPT%                 -21161 -I "%H_PATH%" -c -o %output%\Init_UC_DSP.doj "%C_PATH%\Init_UC_DSP.c"
%CC21K_EXE% %C_OPT%                 -21161 -I "%H_PATH%" -c -o %output%\interpolation.doj "%C_PATH%\interpolation.c"
%CC21K_EXE% %C_OPT% -O1 -Ov100 -Oa  -21161 -I "%H_PATH%" -c -o %output%\oscillator.doj "%C_PATH%\oscillator.c"
%CC21K_EXE% %C_OPT% -D UC_DSP       -21161 -I "%H_PATH%" -c -o %output%\TDM_Init.doj "%C_PATH%\TDM_Init.c"
%CC21K_EXE% %C_OPT% -O1 -Ov100 -Oa  -21161 -I "%H_PATH%" -c -o %output%\tdmUpConvSettings.doj "%C_PATH%\tdmUpConvSettings.c"
REM %CC21K_EXE% %C_OPT% -O1 -Ov100 -Oa  -21161 -I "%H_PATH%" -c -o %output%\UC_DSP_Main.doj "%C_PATH%\UC_DSP_Main.c"
REM Do not enable optimizer for UC_DSP_Main.c !!! Hidden problems will occure, e.g. no CRC error will be transmitted by TDM bus!
%CC21K_EXE% %C_OPT%				  -21161 -I "%H_PATH%" -c -o %output%\UC_DSP_Main.doj "%C_PATH%\UC_DSP_Main.c"
%CC21K_EXE% %C_OPT%                 -21161 -I "%H_PATH%" -c -o %output%\Timer.doj "%C_PATH%\Timer.c"
%CC21K_EXE% %C_OPT%                 -21161 -I "%H_PATH%" -c -o %output%\TDM_Service.doj "%C_PATH%\TDM_Service.C"
%CC21K_EXE% %C_OPT% -O1 -Ov100 -Oa  -21161 -I "%H_PATH%" -c -o %output%\upDownConvTdmBus.doj "%C_PATH%\upDownConvTdmBus.C"
%CC21K_EXE% %C_OPT%                 -21161 -I "%H_PATH%" -c -o %output%\MemoryCheck.doj "%C_PATH%\MemoryCheck.C"
%CC21K_EXE% %C_OPT%                 -21161 -I "%H_PATH%" -c -o %output%\crc.doj "%C_PATH%\crc.C"
%CC21K_EXE% %C_OPT%                 -21161 -I "%H_PATH%" -c -o %output%\SupervisionUC_Bus_UC_DSP.doj "%C_PATH%\SupervisionUC_Bus_UC_DSP.C"

echo off

:LINK
%LINKER_EXE% @UC_DSP.lnk -T "%WORK_DIR%\UC_DSP\architecture\UC_DSP-21161.ldf" -DADSP-21161 -L %output% -o "%WORK_DIR%\UC_DSP\UC_DSP_V0_00.dxe" -Map "%WORK_DIR%\UC_DSP\UC_DSP_V0_00.xml" -warnonce

IF NOT EXIST "%WORK_DIR%\UC_DSP\*.dxe" echo "!!!!!!!!!!!!!!!! Compiler or linker Error -> scroll up !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
IF NOT EXIST "%WORK_DIR%\UC_DSP\*.dxe" goto END

:PATCH CRC into DXE file. Argument -s for any Slave DSP
CRC_ChecksumPatcherDSP.exe -s %WORK_DIR%\UC_DSP\UC_DSP_V0_00.dxe %WORK_DIR%\UC_DSP\architecture\UC_DSP-21161.ldf

REM if CRC_ChecksumPatcherDSP returns 0 continue with Loader
REM if CRC_ChecksumPatcherDSP returns > 0 print error message
if %errorlevel% LSS 1 goto LOADER 
if %errorlevel% GTR 0 goto PATCH_ERROR


:LOADER
IF EXIST "%WORK_DIR%\UC_DSP\*.dxe" %ELFLOADER_EXE% "%WORK_DIR%\UC_DSP\UC_DSP_V0_00.dxe" -bSPI -fASCII -HostWidth 32 -ExternalMemoryWidth 48 -l "%WORK_DIR%\DSP_Boot_Loader\SPI_Boot\161_spi32.dxe" -o "%WORK_DIR%\UC_DSP\UC_SlaveSPI32.ldr" -proc ADSP-21161 -v

:CONVERT LDR add comma separator to *.ldr
IF EXIST "%WORK_DIR%\UC_DSP\*.ldr" %WORK_DIR%\Tools\ConvertDSP_Loader\ConvertLDR_ETL600.exe "%WORK_DIR%\UC_DSP\UC_SlaveSPI32.ldr" "%WORK_DIR%\UC_DSP\UC_SlaveSPI32include.ldr"
IF EXIST "%WORK_DIR%\UC_DSP\*include.ldr" copy "%WORK_DIR%\UC_DSP\UC_SlaveSPI32include.ldr" "%WORK_DIR%\Master_DSP\C\UC_SlaveSPI32include.ldr"

goto END

:PATCH_ERROR
echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
echo !!!!!!!!! No *.ldr generated. Error with CRC Patcher  !!!!!!!!!!!!


:END
