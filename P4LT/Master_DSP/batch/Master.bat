echo off

REM -------------------------------------------------------------------------
REM $Workfile: Master.bat $
REM Part of      : ETL600
REM Language     : Windows batch file
REM Created by   : Hans Peter Rytz
REM Remarks      :  
REM Purpose      : 
REM Copyright    : ABB Switzerland Ltd., 2004
REM -------------------------------------------------------------------------
REM
REM -------------------------------------------------------------------------
REM $Log: /branches/Source O4CV/P4LT/Master_DSP/batch/Master.bat $
rem 
rem 22    07-04-13 8:31 Chchlee
rem now using environment variables for assembler, compiler and linker
rem 
rem 17    19.01.06 16:48 Chmibuh
rem CRC Patcher argument (-p -> -m) & Error handling changed
rem 
rem 16    1.11.05 16:23 Chchlee
rem batch files changed / -DDEBUG introduced
rem 
rem 15    8.06.05 18:54 Chchlee
rem 
rem 14    17.05.05 18:12 Chmibuh
rem %C_OPT%  for Interrupt_routines_Master_DSP.asm added
rem 
rem 13    12.05.05 22:02 Chchlee
rem New command line parameters
rem 
rem 12    9.05.05 11:50 Chbrbus
rem Changed "ProgramMemoryCheck" to "MemoryCheck" - global solution for all
rem DSP's
rem 
rem 11    6.05.05 15:42 Chbrbus
rem Checksum patcher for DSP checksums added
rem 
rem 10    10.04.05 13:37 Chmibuh
rem 
rem 9     23.02.05 14:36 Chmibuh
rem iButton added
rem 
rem 8     22.02.05 14:53 Chchlee
rem default.c added
rem 
rem 7     22.02.05 13:52 Chhamaa
rem eprom*.C changed to uppercase
rem 
rem 6     21.02.05 18:56 Chchlee
rem nsd custom table added
rem 
rem 5     16.02.05 15:58 Chhamaa
rem 
rem 4     16.02.05 13:52 Chhamaa
rem 
rem 3     27.01.05 13:54 Chharyt
rem mkdir %WORK_DIR%\Master_DSP\debug added
rem 
rem 2     20.01.05 16:04 Chmibuh
rem DCD7005.c no longer in use
rem 
rem 1     19.01.05 15:47 Chchlee
rem 
rem 61    18.01.05 17:14 Chpeplu
rem New file PilotController.c added
rem 
rem 60    12.01.05 11:37 Chbrbus
rem ProgramMemoryCheck added
rem 
rem 59    10.01.05 17:00 Chhamaa
rem 
rem 58    3.01.05 16:40 Chbrbus
rem ArrayFloatCheck.asm added
rem 
rem 57    15.12.04 10:26 Chchlee
rem 
rem 56    14.12.04 13:55 Chchlee
rem 
rem 55    13.12.04 9:17 Chchlee
rem eventRec and monitoring added
rem 
rem 54    15.11.04 7:55 Leeb02
rem status.c added
rem 
rem 53    12.11.04 13:30 Maag01
rem 
rem 52    11.11.04 10:26 Maag01
rem 
rem 51    9.11.04 17:53 Maag01
rem 

REM -------------------------------------------------------------------------

rem set executable path
if defined ADSP_PATH (set EASM21K_EXE="%ADSP_PATH%\easm21k") else (set EASM21K_EXE=easm21k)
if defined ADSP_PATH (set CC21K_EXE="%ADSP_PATH%\cc21k") else (set CC21K_EXE=cc21k)
if defined ADSP_PATH (set LINKER_EXE="%ADSP_PATH%\linker") else (set LINKER_EXE=linker)
if defined ADSP_PATH (set ELFLOADER_EXE="%ADSP_PATH%\elfloader") else (set ELFLOADER_EXE=elfloader)

set WORK_DIR=D:\ADSP\$P4LT
if defined P4LT_BUILD_PATH set WORK_DIR=%P4LT_BUILD_PATH%

set DSP_ID=Master_DSP

IF "%1" == "-debug"   (SET output=%WORK_DIR%\%DSP_ID%\debug&SET C_OPT=-g -DDEBUG&mkdir %WORK_DIR%\%DSP_ID%\debug)
IF "%1" == "-release" (SET output=%WORK_DIR%\%DSP_ID%\release&SET C_OPT=&mkdir %WORK_DIR%\%DSP_ID%\release)

rem check hardware version
IF "%2" == "-P2" (set C_OPT=-D P4LW_PTTY2 %C_OPT% & goto BEGIN)
IF "%2" == "-S0" (set C_OPT= %C_OPT%              & goto BEGIN)
echo !! Only prototype 2 and 0-series hardware is supported !!
pause
exit

:BEGIN

rem stop if not release 1
if not "%3" == "-R1" (echo !! Only Release 1 is supported !! & pause & exit)

IF /i "%4" == "-noSlaveBoot" SET BOOTPARAM=-D DO_NOT_BOOT_THE_DSPs

IF "%4" == "-assert" SET DBG=-D USE_ASSERT
IF "%5" == "-assert" SET DBG=-D USE_ASSERT

rem This is the Master DSP
set C_OPT=-D MASTER_DSP %C_OPT%


echo on

SET H_PATH=%WORK_DIR%\%DSP_ID%\H
SET C_PATH=%WORK_DIR%\%DSP_ID%\C
SET ASM_PATH=%WORK_DIR%\%DSP_ID%\ASM


:DELETE
del %output%\*.doj
echo off
del %WORK_DIR%\%DSP_ID%\*.dxe            
del %WORK_DIR%\%DSP_ID%\*.ldr

echo on

:COMP_ASM
%EASM21K_EXE% %C_OPT% -21161 -I %H_PATH% -o %output%\Interrupt_routines_Master_DSP.doj  %ASM_PATH%\Interrupt_routines_Master_DSP.asm
%EASM21K_EXE% -21161  -o %output%\Interrupt_table_Master_DSP.doj                        %ASM_PATH%\Interrupt_table_Master_DSP.asm
%EASM21K_EXE% -21161  -o %output%\ExternalTDM_BusLowLevel.doj                           %ASM_PATH%\ExternalTDM_BusLowLevel.asm
%EASM21K_EXE% -21161  -o %output%\benchmark.doj                                         %ASM_PATH%\benchmark.asm"
%EASM21K_EXE%  -21161  -o %output%\ArrayFloatCheck.doj									  %ASM_PATH%\ArrayFloatCheck.asm
%EASM21K_EXE% -21161  -o %output%\PN_GENER.doj										  %ASM_PATH%\PN_GENER.asm

:COMP_C
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\Master_DSP_Main.doj            %C_PATH%\Master_DSP_Main.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\TDM_Init.doj                   %C_PATH%\TDM_Init.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\idleCounter.doj                %C_PATH%\idleCounter.c

%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\Timer.doj                      %C_PATH%\Timer.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\ExternalTDM_BusBridge.doj      %C_PATH%\ExternalTDM_BusBridge.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\Init_Master_DSP.doj            %C_PATH%\Init_Master_DSP.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\DL_2nd.doj                     %C_PATH%\DL_2nd.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\DL_RS232.doj                   %C_PATH%\DL_RS232.c
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 %DBG% -I %H_PATH% -c -o %output%\DL_EOC.doj      %C_PATH%\DL_EOC.C
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\FT12.doj                       %C_PATH%\FT12.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\DIAG.doj                       %C_PATH%\DIAG.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\msgHandler.doj                 %C_PATH%\msgHandler.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\bridge.doj                     %C_PATH%\bridge.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\SPI_access.doj                 %C_PATH%\SPI_access.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\SPI_functions.doj              %C_PATH%\SPI_functions.c
%CC21K_EXE%  %C_OPT% -21161 -O1 -Ov100 -Oa %DBG% -I %H_PATH% -c -o %output%\rtc.doj	      %C_PATH%\rtc.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\Configuration.doj              %C_PATH%\Configuration.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\status.doj	                  %C_PATH%\status.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\TDM_Service.doj                %C_PATH%\TDM_Service.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\EPROMHI.doj                    %C_PATH%\EPROMHI.C
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\EPROMLOW.doj                   %C_PATH%\EPROMLOW.C
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\EPROMMED.doj                   %C_PATH%\EPROMMED.C
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\crc.doj                        %C_PATH%\crc.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\boost_manager.doj              %C_PATH%\boost_manager.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\AD9833_Generator.doj           %C_PATH%\AD9833_Generator.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\IrigB.doj				      %C_PATH%\IrigB.c
%CC21K_EXE%  %C_OPT% -O1 -Ov100 -Oa -21161 %DBG% -I %H_PATH% -c -o %output%\tdmEocData.doj  %C_PATH%\tdmEocData.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\edgeDetection.doj	          %C_PATH%\edgeDetection.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\Equalizer.doj				  %C_PATH%\Equalizer.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\EQ_CALCU.doj					  %C_PATH%\EQ_CALCU.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\tdmUpConvSettings.doj		  %C_PATH%\tdmUpConvSettings.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\eventRec.doj	                  %C_PATH%\EventRec.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\monitor.doj	                  %C_PATH%\Monitor.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\MemoryCheck.doj				  %C_PATH%\MemoryCheck.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\PilotController.doj			  %C_PATH%\PilotController.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\nsdCustomTable.doj             %C_PATH%\nsdCustomTable.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\default.doj                    %C_PATH%\DEFAULT.C
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\iButLow.doj	                  %C_PATH%\iButLow.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\iButMed.doj	                  %C_PATH%\iButMed.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\iButHigh.doj	                  %C_PATH%\iButHigh.c
%CC21K_EXE%  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\licenceCheck.doj	              %C_PATH%\licenceCheck.c

%CC21K_EXE%  %C_OPT% -21161 %DBG% %BOOTPARAM% -I %H_PATH% -c -o %output%\Booting.doj        %C_PATH%\Booting.c

echo off
	
:LINK
%LINKER_EXE% @Master_DSP.lnk -e -T %WORK_DIR%\Master_DSP\architecture\Master_DSP-21161.ldf -DADSP-21161 -L %output% -o %WORK_DIR%\Master_DSP\Master_DSP_V0_00.dxe -Map %WORK_DIR%\Master_DSP\Master_DSP_V0_00.xml -warnonce
IF NOT EXIST %WORK_DIR%\Master_DSP\*.dxe echo "!!!!!!!!!!!!!!!! Compiler or linker Error -> scroll up !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
IF NOT EXIST %WORK_DIR%\Master_DSP\*.dxe goto END


:CRC_PATCHER
REM PATCH CHECKSUMS into DSP code (*.dxe). Argument -m for Master DSP
CRC_ChecksumPatcherDSP.exe -m %WORK_DIR%\Master_DSP\Master_DSP_V0_00.dxe %WORK_DIR%\Master_DSP\architecture\Master_DSP-21161.ldf

REM if CRC_ChecksumPatcherDSP returns 0 continue with Loader
REM if CRC_ChecksumPatcherDSP returns > 0 print error message
if %errorlevel% LSS 1 goto LOADER 
if %errorlevel% GTR 0 goto PATCH_ERROR 

:LOADER
REM Start Loader only if -noSlaveBoot switch is not activated  
IF NOT "%4" == "-noSlaveBoot" %ELFLOADER_EXE% %WORK_DIR%\Master_DSP\Master_DSP_V0_00.dxe -bPROM -fHEX -ExternalMemoryWidth 48 -l %WORK_DIR%\DSP_Boot_Loader\FLASH_Boot_SDRAM\161_promSDRAM.dxe -o %WORK_DIR%\Master_DSP\Master_DSP_V0_00.ldr -proc ADSP-21161 -v -p0x000000
IF NOT "%4" == "-noSlaveBoot" copy %WORK_DIR%\Master_DSP\Master_DSP_V0_00.ldr %WORK_DIR%\Master_DSP\p4lt.ldr

IF /i  "%4" == "-noSlaveBoot" echo !!!!!!!!!! No bootable Version generated. Tool Switch "-noSlaveBoot" is activated !!!!!!!!!!!!!!
IF /i  "%4" == "-noSlaveBoot" goto END

IF NOT EXIST %WORK_DIR%\Master_DSP\*.ldr echo "!!!!!!!!!!!!!!!! No *.ldr generated. Error with Elfloader !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
goto END

:PATCH_ERROR
echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
echo !!!!!!!!! No *.ldr generated. Error with CRC Patcher  !!!!!!!!!!!!

:END
echo.
IF EXIST %WORK_DIR%\Master_DSP\*.ldr echo "SUCCESS -> scroll up to get Checksum for Code Authentication" 
echo.
