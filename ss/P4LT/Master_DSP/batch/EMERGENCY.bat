REM -------------------------------------------------------------------------
REM $Workfile: EMERGENCY.bat $
REM Part of      : ETL600
REM Language     : Windows batch file
REM Created by   : M. Buhl
REM Remarks      :  
REM Purpose      : 
REM Copyright    : ABB Switzerland Ltd., 2004
REM -------------------------------------------------------------------------
REM
REM -------------------------------------------------------------------------
REM $Log: /Source/P4LT/Master_DSP/batch/EMERGENCY.bat $
rem 
rem 7     1.11.05 16:23 Chchlee
rem batch files changed / -DDEBUG introduced
rem 
rem 6     3.10.05 20:26 Chmibuh
rem 
rem 5     3.10.05 19:05 Chmibuh
rem EMERGENCY_EPROMMED.doj added
rem 
rem 4     9.06.05 19:42 Chmibuh
rem 
rem 3     10.05.05 12:54 Chmibuh
rem 
rem 2     20.01.05 16:04 Chmibuh
rem EMERGENCY_Configuration.doj added
rem 
rem 1     19.01.05 15:47 Chchlee
rem 
rem 3     11.12.04 19:00 Chmibuh
rem 
rem 2     6.12.04 18:35 Chmibuh
rem 
rem 1     2.12.04 16:16 Chmibuh
rem initial
rem 
rem 

REM -------------------------------------------------------------------------


echo on

set WORK_DIR=D:\ADSP\$P4LT
if defined P4LT_BUILD_PATH set WORK_DIR=%P4LT_BUILD_PATH%

:BEGIN
set DSP_ID=Master_DSP

IF "%1" == "-debug"   (SET output=%WORK_DIR%\%DSP_ID%\debug&SET C_OPT=-g -DDEBUG&mkdir %WORK_DIR%\%DSP_ID%\debug)
IF "%1" == "-release" (SET output=%WORK_DIR%\%DSP_ID%\release&SET C_OPT=&mkdir %WORK_DIR%\%DSP_ID%\release)


SET H_PATH=%WORK_DIR%\%DSP_ID%\H
SET C_PATH=%WORK_DIR%\%DSP_ID%\C
SET ASM_PATH=%WORK_DIR%\%DSP_ID%\ASM


echo on

:COMP_C
cc21k  %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\EMERGENCY_Main.doj								%C_PATH%\EMERGENCY_Main.c
cc21k -D EMERGENCY_FW %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\EMERGENCY_Init_Master_DSP.doj		%C_PATH%\Init_Master_DSP.c
cc21k -D EMERGENCY_FW %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\EMERGENCY_bridge.doj				%C_PATH%\bridge.c
cc21k -D EMERGENCY_FW %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\EMERGENCY_msgHandler.doj			%C_PATH%\msgHandler.c
cc21k -D EMERGENCY_FW %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\EMERGENCY_SPI_functions.doj		%C_PATH%\SPI_functions.c
cc21k -D EMERGENCY_FW %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\EMERGENCY_DIAG.doj				%C_PATH%\DIAG.c
cc21k -D EMERGENCY_FW %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\EMERGENCY_EPROMMED.doj            %C_PATH%\EPROMMED.c
cc21k -D EMERGENCY_FW %C_OPT% -21161 %DBG% -I %H_PATH% -c -o %output%\EMERGENCY_Configuration.doj       %C_PATH%\Configuration.c


echo off

:LINK
linker.exe @EMERGENCY.lnk -e -T %WORK_DIR%\Master_DSP\architecture\Master_DSP-21161.ldf -DADSP-21161 -L %output% -o %WORK_DIR%\Master_DSP\EMERGENCY.dxe -Map %WORK_DIR%\Master_DSP\EMERGENCY.xml -warnonce


:LOADER
elfloader.exe %WORK_DIR%\Master_DSP\EMERGENCY.dxe -bPROM -fHEX -ExternalMemoryWidth 48 -l %WORK_DIR%\DSP_Boot_Loader\EMERGENCY_SDRAM\161_promSDRAM_EMERGENCY.dxe -o %WORK_DIR%\Master_DSP\P4LT_EMERGENCY.ldr -proc ADSP-21161 -v -p0x000000


:DUMMY BUILD
REM build (dummy build) a multiprocessor fw to find out the size of the emergency FW. 
REM If the startadress of the second application is less than 0x00FFFF then the
REM emergency FW is less than 64 KByte (1 FLASH sector)
REM The tool GenerateLoaderFiles checks the startadress of the second application!

elfloader.exe -id1exe=%WORK_DIR%\Master_DSP\EMERGENCY.dxe -id2exe=%WORK_DIR%\Master_DSP\EMERGENCY.dxe -p0x000000 -bPROM -fHEX -ExternalMemoryWidth 48 -l %WORK_DIR%\DSP_Boot_Loader\EMERGENCY_SDRAM\161_promSDRAM_EMERGENCY.dxe -o %WORK_DIR%\Master_DSP\dummy_build.ldr -v  -p0x000000 -proc ADSP-21161
%WORK_DIR%\Tools\GenerateLoaderFiles\GenerateLoaderFiles.exe %WORK_DIR%\Master_DSP\dummy_build.ldr


echo .
echo .
echo P4LT_EMERGENCY.ldr   Emergency FW  0x00000 - 0x00FFFF
echo p4lt.ldr for HMI                   0x10000 - 0x39FFFF	

 


:END