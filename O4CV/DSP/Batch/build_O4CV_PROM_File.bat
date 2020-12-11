echo off
rem /*-------------------------------------------------------------------------*
rem  * $Workfile    : build_O4CV_PROM_File.bat  $
rem  * Part of      : ETL600 / O4CV
rem  * Language     : DOS-COMMAND INTERPRETER
rem  * Created by   : A. Notter
rem  * Remarks      :  
rem  * Purpose      : Build Prom-File for programming the Flash
rem  * Copyright    : ABB Switzerland Ltd., 2007
rem  *-------------------------------------------------------------------------*/
rem /*-------------------------------------------------------------------------*
rem  * $Log: /branches/Source R4/O4CV/DSP/Batch/build_O4CV_PROM_File.bat $
rem 
rem 5     12-04-03 13:47 Chalnot
rem srec_cat.exe used instead of hex2bin.exe because the latter doesn't
rem work on Windows 7
rem 
rem 2     07-12-11 9:43 Chalnot
rem Application firmware offset set to 0x100000 (bank1) instead of 0x200000
rem (bank2).
rem 
rem 1     07-10-11 15:00 Chalnot
rem Initial version.
rem 
rem 
rem  *-------------------------------------------------------------------------*/

:This batch builds a prom-file in the binary format which can be programmed
:onto the flash. 
:
:The batch needs 3 loaderfiles, srec_cat.exe and checksum.exe which must 
:be in the same directory like the batch.
:
:The 3 loaderfiles must be named as follows:
:	Boot code: 	O4CV_Boot_*.ldr		(e.g. O4CV_Boot_101.ldr)
:	Emergency Fw: 	O4CV_EMERGENCY_*.ldr 	(e.g. O4CV_EMERGENCY_101.ldr)
:	Fw: 		O4CV_*.ldr 		(e.g. O4CV_103.ldr)
:
:The file has the same size like the flash (4MB), unused bytes are set to 0xFF.
:The checksum of the file is written in the console window.

echo on

:Delete old prom file and create temp dir
del O4CV_Flash.bin
rmdir /s /q temp
mkdir temp

:Make temp copy of loaderfiles
copy /v /b O4CV_Boot*.ldr .\temp\bootCode.ldr
move O4CV_Boot*.ldr .\temp\
copy /v /b O4CV_EMERGENCY*.ldr .\temp\emFw.ldr
move O4CV_EMERGENCY*.ldr .\temp
copy /v /b O4CV_*.ldr .\temp\fw.ldr
move .\temp\O4CV_Boot*.ldr .\
move .\temp\O4CV_EMERGENCY*.ldr .\ 

:Call Hex2Bin
srec_cat .\temp\bootCode.ldr -intel .\temp\emFw.ldr -intel -offset 0x10000 .\temp\fw.ldr -intel -offset 0x100000 -o .\temp\prom.ldr -intel
srec_cat .\temp\prom.ldr -intel -fill 0xFF 0 0x400000 -o .\temp\prom.bin -binary

:Copy prom file and delete temp dir
copy /v /b .\temp\prom.bin O4CV_Flash.bin
rmdir /s /q temp

:Call checksum
checksum O4CV_Flash.bin

pause


