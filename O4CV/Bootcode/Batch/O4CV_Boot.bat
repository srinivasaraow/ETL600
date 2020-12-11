echo off
rem /*-------------------------------------------------------------------------*
rem  * $Workfile    : O4CV_Boot.bat  $
rem  * Part of      : ETL600 / O4CV
rem  * Language     : DOS-COMMAND INTERPRETER
rem  * Created by   : A. Notter
rem  * Remarks      :  
rem  * Purpose      : Build Loader-File
rem  * Copyright    : ABB Switzerland Ltd., 2007
rem  *-------------------------------------------------------------------------*/
rem /*-------------------------------------------------------------------------*
rem  * $Log: /branches/Source R4/O4CV/Bootcode/Batch/O4CV_Boot.bat $
rem 
rem 7     23.06.10 15:57 Chhamaa
rem DSP revision set to 0.4
rem 
rem 4     07-10-05 16:25 Chdocac
rem 
rem 3     07-10-05 16:17 Chdocac
rem 
rem 2     07-09-27 15:32 Chalnot
rem Output path changed.
rem 
rem 1     07-09-27 14:08 Chalnot
rem Initial version.
rem  *-------------------------------------------------------------------------*/

:EXECUTABLES
if defined ADSP_PATH (set ASM="%ADSP_PATH%\easmblkfn")	else (set ASM=easmblkfn)
if defined ADSP_PATH (set LNK="%ADSP_PATH%\ccblkfn")	else (set LNK=ccblkfn)
if defined ADSP_PATH (set LDR="%ADSP_PATH%\elfloader")	else (set LDR=elfloader)

:PATHS
set WORK_DIR=c:\temp\R3\O4CV
if defined O4CV_BUILD_PATH set WORK_DIR=%O4CV_BUILD_PATH%

set ASM_DIR=%WORK_DIR%\BootCode\ASM
set LINKER_DIR=%WORK_DIR%\BootCode\Linker

:CONFIGURATIONS
if "%1" == "-debug"		(set OUT_DIR=%WORK_DIR%\BootCode\Debug&			mkdir %WORK_DIR%\BootCode\Debug)
if "%1" == "-release"		(set OUT_DIR=%WORK_DIR%\BootCode\Release&		mkdir %WORK_DIR%\BootCode\Release)

:OPTIONS
if "%1" == "-debug"		set ASM_OPT=-proc ADSP-BF538 -g -D RDY_LED_ON -si-revision 0.4
if "%1" == "-release"		set ASM_OPT=-proc ADSP-BF538		      -si-revision 0.4

if "%1" == "-debug"		set LNK_OPT=-T %LINKER_DIR%\bootCode.ldf -L %OUT_DIR% -add-debug-libpaths -flags-link -od,%OUT_DIR% -proc ADSP-BF538 -si-revision 0.4 -flags-link -MM
if "%1" == "-release"		set LNK_OPT=-T %LINKER_DIR%\bootCode.ldf -L %OUT_DIR%			  -flags-link -od,%OUT_DIR% -proc ADSP-BF538 -si-revision 0.4 -flags-link -MM

set LDR_OPT=-b Flash -f HEX -Width 16 -si-revision 0.4 -proc ADSP-BF538 -MM

:DELETE
del %OUT_DIR%\*.doj
del %OUT_DIR%\*.dxe
del %OUT_DIR%\*.ldr

echo on

:COMP_ASM
%ASM%	%ASM_DIR%\bootCode.asm	%ASM_OPT%	-o %OUT_DIR%\bootCode.doj

:LINK
%LNK%   %OUT_DIR%\bootCode.doj  %LNK_OPT%	-o %OUT_DIR%\bootCode.dxe 

:LOADER
%LDR%	-init %OUT_DIR%\bootCode.dxe %LDR_OPT%	-o %OUT_DIR%\bootCode.ldr	

:COUNTER
concat_ldr.exe	%OUT_DIR%\bootCode.ldr	bootTries.ldr	%OUT_DIR%\bootCode_counter.ldr

cd /D %WORK_DIR%
