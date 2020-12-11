echo off
rem /*-------------------------------------------------------------------------*
rem  * $Workfile    : O4CV_FW.bat  $
rem  * Part of      : ETL600 / O4CV
rem  * Language     : DOS-COMMAND INTERPRETER
rem  * Created by   : A. Notter
rem  * Remarks      :  
rem  * Purpose      : Build Loader-File
rem  * Copyright    : ABB Switzerland Ltd., 2007
rem  *-------------------------------------------------------------------------*/
rem /*-------------------------------------------------------------------------*
rem  * $Log: /branches/Source R4/O4CV/DSP/Batch/O4CV_FW.bat $
rem 
rem 9     23.06.10 15:21 Chhamaa
rem DSP revision set to 0.4
rem 
rem 6     07-10-05 16:01 Chdocac
rem 
rem 5     07-10-05 15:48 Chdocac
rem 
rem 4     07-10-05 15:40 Chdocac
rem 
rem 3     07-10-05 14:52 Chdocac
rem removed last 2 lines --> integrated in calling batch files
rem 
rem 05/10/2007 D. Cachin
rem 
rem 2     07-09-27 14:44 Chalnot
rem Output path changed.
rem 
rem 1     07-09-27 14:13 Chalnot
rem Initial version.
rem  *-------------------------------------------------------------------------*/

:EXECUTABLES
if defined ADSP_PATH (set ASM="%ADSP_PATH%\easmblkfn")	else (set ASM=easmblkfn)
if defined ADSP_PATH (set CC="%ADSP_PATH%\ccblkfn")	else (set CC=ccblkfn)
if defined ADSP_PATH (set LNK="%ADSP_PATH%\ccblkfn")	else (set LNK=ccblkfn)
if defined ADSP_PATH (set LDR="%ADSP_PATH%\elfloader")	else (set LDR=elfloader)

:PATHS
set WORK_DIR=c:\temp\R3\O4CV
if defined O4CV_BUILD_PATH set WORK_DIR=%O4CV_BUILD_PATH%

set H_DIR=%WORK_DIR%\DSP\H
set C_DIR=%WORK_DIR%\DSP\C
set ASM_DIR=%WORK_DIR%\DSP\ASM
set LIB_DIR=%WORK_DIR%\DSP\Lib
set LINKER_DIR=%WORK_DIR%\DSP\Linker

:CONFIGURATIONS
if "%1" == "-debug"		(set OUT_DIR=%WORK_DIR%\DSP\Debug&		mkdir %WORK_DIR%\DSP\Debug)
if "%1" == "-release"		(set OUT_DIR=%WORK_DIR%\DSP\Release&		mkdir %WORK_DIR%\DSP\Release)
if "%1" == "-emfwdebug"		(set OUT_DIR=%WORK_DIR%\DSP\EmFwDebug&		mkdir %WORK_DIR%\DSP\EmFwDebug)
if "%1" == "-emfwrelease"	(set OUT_DIR=%WORK_DIR%\DSP\EmFwRelease&	mkdir %WORK_DIR%\DSP\EmFwRelease)

:OPTIONS
if "%1" == "-debug"		set ASM_OPT=-proc ADSP-BF538 -g -si-revision 0.4
if "%1" == "-release"		set ASM_OPT=-proc ADSP-BF538    -si-revision 0.4
if "%1" == "-emfwdebug"		set ASM_OPT=-proc ADSP-BF538 -g -si-revision 0.4
if "%1" == "-emfwrelease"	set ASM_OPT=-proc ADSP-BF538    -si-revision 0.4

if "%1" == "-debug"		set CC_OPT=-I %H_DIR% -file-attr ProjectName=O4CV -g -structs-do-not-overlap -no-multiline					 -double-size-32 -decls-strong -warn-protos -si-revision 0.4 -proc ADSP-BF538 
if "%1" == "-release"		set CC_OPT=-I %H_DIR% -file-attr ProjectName=O4CV    -structs-do-not-overlap -no-multiline -D NDEBUG -D WATCHDOG -D MEMORYCHECK  -double-size-32 -decls-strong -warn-protos -si-revision 0.4 -proc ADSP-BF538 
if "%1" == "-emfwdebug"		set CC_OPT=-I %H_DIR% -file-attr ProjectName=O4CV -g -structs-do-not-overlap -no-multiline -D EMERGENCY_FW			 -double-size-32 -decls-strong -warn-protos -si-revision 0.4 -proc ADSP-BF538 
if "%1" == "-emfwrelease"	set CC_OPT=-I %H_DIR% -file-attr ProjectName=O4CV    -structs-do-not-overlap -no-multiline -D NDEBUG -D WATCHDOG -D EMERGENCY_FW -double-size-32 -decls-strong -warn-protos -si-revision 0.4 -proc ADSP-BF538 
if "%2" == "-ptty"		set CC_OPT=-D PROTOTYP_HW_NO_PULSE_CODE_DIALING_EXTENSION %CC_OPT%

if "%1" == "-debug"		set LNK_OPT=-L %LIB_DIR% -T %LINKER_DIR%\O4CV.ldf -map %OUT_DIR%\O4CV.map.xml -L %OUT_DIR% -flags-link -MDUSER_CRT="%OUT_DIR%\O4CV_basiccrt.doj",-MDUSE_INSTRUCTION_CACHE,-MDUSE_DATA_B_CACHE,-MDUSE_FILEIO,-MDUSER_CPLBTAB="%OUT_DIR%\O4CV_cplbtab.doj" -add-debug-libpaths -flags-link -MDUSE_CACHE -flags-link -od,%OUT_DIR% -proc ADSP-BF538 -si-revision 0.4 -flags-link -MM
if "%1" == "-release"		set LNK_OPT=-L %LIB_DIR% -T %LINKER_DIR%\O4CV.ldf -map %OUT_DIR%\O4CV.map.xml -L %OUT_DIR% -flags-link -MDUSER_CRT="%OUT_DIR%\O4CV_basiccrt.doj",-MDUSE_INSTRUCTION_CACHE,-MDUSE_DATA_B_CACHE,-MDUSE_FILEIO,-MDUSER_CPLBTAB="%OUT_DIR%\O4CV_cplbtab.doj" -flags-link -e	     -flags-link -MDUSE_CACHE -flags-link -od,%OUT_DIR% -proc ADSP-BF538 -si-revision 0.4 -flags-link -MM
if "%1" == "-emfwdebug"		set LNK_OPT=-L %LIB_DIR% -T %LINKER_DIR%\O4CV.ldf -map %OUT_DIR%\O4CV.map.xml -L %OUT_DIR% -flags-link -MDUSER_CRT="%OUT_DIR%\O4CV_basiccrt.doj",-MDUSE_INSTRUCTION_CACHE,-MDUSE_DATA_B_CACHE,-MDUSE_FILEIO,-MDUSER_CPLBTAB="%OUT_DIR%\O4CV_cplbtab.doj" -add-debug-libpaths -flags-link -MDUSE_CACHE -flags-link -od,%OUT_DIR% -proc ADSP-BF538 -si-revision 0.4 -flags-link -MM
if "%1" == "-emfwrelease"	set LNK_OPT=-L %LIB_DIR% -T %LINKER_DIR%\O4CV.ldf -map %OUT_DIR%\O4CV.map.xml -L %OUT_DIR% -flags-link -MDUSER_CRT="%OUT_DIR%\O4CV_basiccrt.doj",-MDUSE_INSTRUCTION_CACHE,-MDUSE_DATA_B_CACHE,-MDUSE_FILEIO,-MDUSER_CPLBTAB="%OUT_DIR%\O4CV_cplbtab.doj" -flags-link -e	     -flags-link -MDUSE_CACHE -flags-link -od,%OUT_DIR% -proc ADSP-BF538 -si-revision 0.4 -flags-link -MM

set LDR_OPT=-b Flash -f HEX -Width 16 -si-revision 0.4 -proc ADSP-BF538

:DELETE
del %OUT_DIR%\*.doj
del %OUT_DIR%\*.dxe
del %OUT_DIR%\*.ldr

echo on

:COMP_ASM
%ASM%  %ASM_DIR%\iir_df1_05.asm   %ASM_OPT% -o %OUT_DIR%\iir_df1_05.doj
%ASM%  %ASM_DIR%\O4CV_basiccrt.s  %ASM_OPT% -o %OUT_DIR%\O4CV_basiccrt.doj

:COMP_C
%CC%  -c %C_DIR%\app.c		  %CC_OPT%  -o %OUT_DIR%\app.doj
%CC%  -c %C_DIR%\appControl.c     %CC_OPT%  -o %OUT_DIR%\appControl.doj
%CC%  -c %C_DIR%\binInOut.c       %CC_OPT%  -o %OUT_DIR%\binInOut.doj
%CC%  -c %C_DIR%\configuration.c  %CC_OPT%  -o %OUT_DIR%\configuration.doj
%CC%  -c %C_DIR%\crc.c            %CC_OPT%  -o %OUT_DIR%\crc.doj
%CC%  -c %C_DIR%\defaultO4CV.c    %CC_OPT%  -o %OUT_DIR%\defaultO4CV.doj
%CC%  -c %C_DIR%\dll2ndStation.c  %CC_OPT%  -o %OUT_DIR%\dll2ndStation.doj
%CC%  -c %C_DIR%\DuSlic.c         %CC_OPT%  -o %OUT_DIR%\DuSlic.doj
%CC%  -c %C_DIR%\epromHigh.c      %CC_OPT%  -o %OUT_DIR%\epromHigh.doj
%CC%  -c %C_DIR%\epromLow.c       %CC_OPT%  -o %OUT_DIR%\epromLow.doj
%CC%  -c %C_DIR%\epromMed.c       %CC_OPT%  -o %OUT_DIR%\epromMed.doj
%CC%  -c %C_DIR%\extTdmBus.c      %CC_OPT%  -o %OUT_DIR%\extTdmBus.doj
%CC%  -c %C_DIR%\ft12.c           %CC_OPT%  -o %OUT_DIR%\ft12.doj
%CC%  -c %C_DIR%\gain.c           %CC_OPT%  -o %OUT_DIR%\gain.doj
%CC%  -c %C_DIR%\iButHigh.c       %CC_OPT%  -o %OUT_DIR%\iButHigh.doj
%CC%  -c %C_DIR%\iButLow.c        %CC_OPT%  -o %OUT_DIR%\iButLow.doj
%CC%  -c %C_DIR%\iButMed.c        %CC_OPT%  -o %OUT_DIR%\iButMed.doj
%CC%  -c %C_DIR%\Initialize.c     %CC_OPT%  -o %OUT_DIR%\Initialize.doj
%CC%  -c %C_DIR%\intDec.c         %CC_OPT%  -o %OUT_DIR%\intDec.doj
%CC%  -c %C_DIR%\ISR.c            %CC_OPT%  -o %OUT_DIR%\ISR.doj
%CC%  -c %C_DIR%\licenceCheck.c   %CC_OPT%  -o %OUT_DIR%\licenceCheck.doj
%CC%  -c %C_DIR%\main.c           %CC_OPT%  -o %OUT_DIR%\main.doj
%CC%  -c %C_DIR%\memoryCheck.c    %CC_OPT%  -o %OUT_DIR%\memoryCheck.doj
%CC%  -c %C_DIR%\msgHandler.c     %CC_OPT%  -o %OUT_DIR%\msgHandler.doj
%CC%  -c %C_DIR%\O4CV_cplbtab.c   %CC_OPT%  -o %OUT_DIR%\O4CV_cplbtab.doj
%CC%  -c %C_DIR%\O4CV_heaptab.c   %CC_OPT%  -o %OUT_DIR%\O4CV_heaptab.doj
%CC%  -c %C_DIR%\speechFilter.c   %CC_OPT%  -o %OUT_DIR%\speechFilter.doj
%CC%  -c %C_DIR%\SPI.c            %CC_OPT%  -o %OUT_DIR%\SPI.doj
%CC%  -c %C_DIR%\sport0.c         %CC_OPT%  -o %OUT_DIR%\sport0.doj
%CC%  -c %C_DIR%\sport1.c         %CC_OPT%  -o %OUT_DIR%\sport1.doj
%CC%  -c %C_DIR%\sport2.c         %CC_OPT%  -o %OUT_DIR%\sport2.doj
%CC%  -c %C_DIR%\status.c         %CC_OPT%  -o %OUT_DIR%\status.doj
%CC%  -c %C_DIR%\subframe.c       %CC_OPT%  -o %OUT_DIR%\subframe.doj
%CC%  -c %C_DIR%\timer.c          %CC_OPT%  -o %OUT_DIR%\timer.doj
%CC%  -c %C_DIR%\voiceData.c      %CC_OPT%  -o %OUT_DIR%\voiceData.doj
%CC%  -c %C_DIR%\watchdog.c       %CC_OPT%  -o %OUT_DIR%\watchdog.doj

:LINK
if "%1" == "-debug"		%LNK%   @O4CV.link      %LNK_OPT%	-o %OUT_DIR%\applic.dxe 
if "%1" == "-release"		%LNK%   @O4CV.link      %LNK_OPT%	-o %OUT_DIR%\applic.dxe 
if "%1" == "-emfwdebug"		%LNK%   @O4CV.link      %LNK_OPT%	-o %OUT_DIR%\em_fw.dxe 
if "%1" == "-emfwrelease"	%LNK%   @O4CV.link	%LNK_OPT%	-o %OUT_DIR%\em_fw.dxe      

:LOADER
if "%1" == "-debug"		%LDR%	%OUT_DIR%\applic.dxe	%LDR_OPT%	-o %OUT_DIR%\applic.ldr	
if "%1" == "-release"		%LDR%	%OUT_DIR%\applic.dxe	%LDR_OPT%	-o %OUT_DIR%\applic.ldr	
if "%1" == "-emfwdebug"		%LDR%	%OUT_DIR%\em_fw.dxe	%LDR_OPT%	-o %OUT_DIR%\em_fw.ldr
if "%1" == "-emfwrelease"	%LDR%	%OUT_DIR%\em_fw.dxe	%LDR_OPT%	-o %OUT_DIR%\em_fw.ldr

:CRC
cd /D "%OUT_DIR%"
if "%1" == "-release"		..\Batch\ChecksumPatcher.exe	applic.dxe	applic.ldr	
cd /D %WORK_DIR%
		      
