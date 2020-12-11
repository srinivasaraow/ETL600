REM -------------------------------------------------------------------------
REM $Workfile: NSK.bat $
REM Part of      : ETL600
REM Language     : Windows batch file
REM Created by   : Hans-Peter Rytz
REM Remarks      :  
REM Purpose      : 
REM Copyright    : ABB Switzerland Ltd., 2004
REM -------------------------------------------------------------------------
REM
REM -------------------------------------------------------------------------
REM $Log: /branches/Source O4CV/P4LT/NSK_DSP/batch/NSK.bat $
rem 
rem 12    07-04-13 8:35 Chchlee
rem now using environment variables for assembler, compiler and linker
rem 
rem 7     19.01.06 16:42 Chmibuh
rem CRC Patcher argument (-p -> -s) & Error handling changed
rem 
rem 6     1.11.05 16:23 Chchlee
rem batch files changed / -DDEBUG introduced
rem 
rem 5     12.05.05 14:04 Chmibuh
rem crc.c added
rem 
rem 4     9.05.05 14:10 Chbrbus
rem Added DSP Checksum patcher
rem 
rem 3     17.02.05 14:08 Chtognu
rem NSK_FPGA with -O switch
rem 
rem 2     27.01.05 13:33 Chharyt
rem mkdir %WORK_DIR%\NSK_DSP\debug added
rem 
rem 1     19.01.05 15:35 Chchlee
rem 
rem 24    13.01.05 16:24 Chalgyg
rem DAPSK with -O switch
rem 
rem 23    16.11.04 13:38 Gnus01
rem 
rem 22    15.10.04 13:46 Gnus01
rem 
rem 21    15.10.04 9:43 Gnus01
rem 
rem 20    29.09.04 14:08 Unp00632
rem 
rem 19    23.09.04 9:32 Maag01
rem 
rem 18    22.09.04 18:08 Maag01
rem 
rem 17    2.09.04 14:44 Unp00632
rem Added %WORK_DIR%
rem 
rem 16    27.08.04 15:31 Buhl01
rem start loader only IF EXIST *.dxe; start convert tool only IF EXIST
rem *.ldr
rem 
rem 15    24.08.04 18:21 Maag01
rem 
rem 14    24.08.04 16:02 Gnus01
rem 
rem 13    11.08.04 17:07 Leeb02
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
set DSP_ID=NSK_DSP

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
%EASM21K_EXE% -21161  -o %output%\Interrupt_table_NSK_DSP.doj "%ASM_PATH%\Interrupt_table_NSK_DSP.asm"
%EASM21K_EXE% -21161  -o %output%\ArrayFloatCheck.doj "%ASM_PATH%\ArrayFloatCheck.asm"
%EASM21K_EXE% -21161  -o %output%\Interrupt_routines_NSK_DSP.doj "%ASM_PATH%\Interrupt_routines_NSK_DSP.asm"
%EASM21K_EXE% -21161  -o %output%\benchmark.doj "%ASM_PATH%\benchmark.asm"
%EASM21K_EXE% -21161  -o %output%\BIIIR.doj "%ASM_PATH%\BIIIR.asm"


:COMP_C
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\NSK_DSP_Main.doj "%C_PATH%\NSK_DSP_Main.c"
%CC21K_EXE%  %C_OPT% -21161 -D NSK_DSP -I "%H_PATH%" -c -o %output%\TDM_Init.doj "%C_PATH%\TDM_Init.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\idleCounter.doj "%C_PATH%\idleCounter.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\FPGAHandling.doj "%C_PATH%\FPGAHandling.c"
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -I "%H_PATH%" -c -o %output%\NSK_FPGA.doj "%C_PATH%\NSK_FPGA.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\Init_NSK_DSP.doj "%C_PATH%\Init_NSK_DSP.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\Timer.doj "%C_PATH%\Timer.c"
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -I "%H_PATH%" -c -o %output%\FSK.doj "%C_PATH%\FSK.c"
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -I "%H_PATH%" -c -o %output%\oscillator.doj "%C_PATH%\oscillator.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\DIAG.doj "%C_PATH%\DIAG.c"
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 -I "%H_PATH%" -c -o %output%\DAPSK.doj "%C_PATH%\DAPSK.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\TDM_Service.doj "%C_PATH%\TDM_Service.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\MemoryCheck.doj "%C_PATH%\MemoryCheck.c"
%CC21K_EXE%  %C_OPT% -21161 -I "%H_PATH%" -c -o %output%\crc.doj "%C_PATH%\crc.c"

rem !!!!!!!!!!!!!!!!!!!!!!!!!!!!  -O1 -Ov100 -Oa 
echo off

patchnsk -p %output%\FPGAHandling.doj

:LINK
%LINKER_EXE% @NSK_DSP.lnk -T "%WORK_DIR%\NSK_DSP\architecture\NSK_DSP-21161.ldf" -DADSP-21161 -L %output% -o "%WORK_DIR%\NSK_DSP\NSK_DSP_V0_00.dxe" -Map "%WORK_DIR%\NSK_DSP\NSK_DSP_V0_00.xml" -warnonce

IF NOT EXIST "%WORK_DIR%\NSK_DSP\*.dxe" echo "!!!!!!!!!!!!!!!! Compiler or linker Error -> scroll up !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
IF NOT EXIST "%WORK_DIR%\NSK_DSP\*.dxe" goto END

:PATCH CRC into DXE file. Argument -s for any Slave DSP
CRC_ChecksumPatcherDSP.exe -s %WORK_DIR%\NSK_DSP\NSK_DSP_V0_00.dxe %WORK_DIR%\NSK_DSP\architecture\NSK_DSP-21161.ldf

REM if CRC_ChecksumPatcherDSP returns 0 continue with Loader
REM if CRC_ChecksumPatcherDSP returns > 0 print error message
if %errorlevel% LSS 1 goto LOADER 
if %errorlevel% GTR 0 goto PATCH_ERROR


:LOADER
IF EXIST "%WORK_DIR%\NSK_DSP\*.dxe" %ELFLOADER_EXE% "%WORK_DIR%\NSK_DSP\NSK_DSP_V0_00.dxe" -bSPI -fASCII -HostWidth 32 -ExternalMemoryWidth 48 -l "%WORK_DIR%\DSP_Boot_Loader\SPI_Boot_SDRAM\161_spi32_SDRAM.dxe" -o "%WORK_DIR%\NSK_DSP\NSK_SlaveSPI32.ldr" -proc ADSP-21161 -v

:CONVERT LDR add comma separator to *.ldr
IF EXIST "%WORK_DIR%\NSK_DSP\*.ldr" %WORK_DIR%\Tools\ConvertDSP_Loader\ConvertLDR_ETL600.exe "%WORK_DIR%\NSK_DSP\NSK_SlaveSPI32.ldr" "%WORK_DIR%\NSK_DSP\NSK_SlaveSPI32include.ldr"
IF EXIST "%WORK_DIR%\NSK_DSP\*include.ldr" copy "%WORK_DIR%\NSK_DSP\NSK_SlaveSPI32include.ldr" "%WORK_DIR%\Master_DSP\C\NSK_SlaveSPI32include.ldr"
goto END

:PATCH_ERROR
echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
echo !!!!!!!!! No *.ldr generated. Error with CRC Patcher  !!!!!!!!!!!!


:END
