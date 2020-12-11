# Microsoft Developer Studio Project File - Name="Master_DSP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Master_DSP - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Master_DSP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Master_DSP.mak" CFG="Master_DSP - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Master_DSP - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Master_DSP - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Source/P4LT Prototype 2/Master_DSP/Master_DSP", CPFAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Master_DSP - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "NDEBUG"
# ADD RSC /l 0x807 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Master_DSP - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "_DEBUG"
# ADD RSC /l 0x807 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Master_DSP - Win32 Release"
# Name "Master_DSP - Win32 Debug"
# Begin Group "C"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\C\AD9833_Generator.c
# End Source File
# Begin Source File

SOURCE=.\C\boost_manager.c
# End Source File
# Begin Source File

SOURCE=.\C\Booting.c
# End Source File
# Begin Source File

SOURCE=.\C\bridge.c
# End Source File
# Begin Source File

SOURCE=.\C\Configuration.c
# End Source File
# Begin Source File

SOURCE=.\C\crc.c
# End Source File
# Begin Source File

SOURCE=.\C\DEFAULT.C
# End Source File
# Begin Source File

SOURCE=.\C\DIAG.C
# End Source File
# Begin Source File

SOURCE=.\C\DL_2nd.c
# End Source File
# Begin Source File

SOURCE=.\C\DL_EOC.C
# End Source File
# Begin Source File

SOURCE=.\C\DL_RS232.C
# End Source File
# Begin Source File

SOURCE=.\C\edgeDetection.c
# End Source File
# Begin Source File

SOURCE=.\C\EMERGENCY_Main.c
# End Source File
# Begin Source File

SOURCE=.\C\EPROMHI.C
# End Source File
# Begin Source File

SOURCE=.\C\EPROMLOW.C
# End Source File
# Begin Source File

SOURCE=.\C\EPROMMED.C
# End Source File
# Begin Source File

SOURCE=.\C\EQ_CALCU.C
# End Source File
# Begin Source File

SOURCE=.\C\Equalizer.c
# End Source File
# Begin Source File

SOURCE=.\c\EventRec.c
# End Source File
# Begin Source File

SOURCE=.\C\ExternalTDM_BusBridge.c
# End Source File
# Begin Source File

SOURCE=.\C\FT12.c
# End Source File
# Begin Source File

SOURCE=.\C\iButHigh.c
# End Source File
# Begin Source File

SOURCE=.\C\iButLow.c
# End Source File
# Begin Source File

SOURCE=.\C\iButMed.c
# End Source File
# Begin Source File

SOURCE=.\C\idleCounter.c
# End Source File
# Begin Source File

SOURCE=.\C\Init_Master_DSP.c
# End Source File
# Begin Source File

SOURCE=.\C\IrigB.c
# End Source File
# Begin Source File

SOURCE=.\C\licenceCheck.c
# End Source File
# Begin Source File

SOURCE=.\C\Master_DSP_Main.c
# End Source File
# Begin Source File

SOURCE=.\C\MemoryCheck.c
# End Source File
# Begin Source File

SOURCE=.\c\Monitor.c
# End Source File
# Begin Source File

SOURCE=.\C\msgHandler.c
# End Source File
# Begin Source File

SOURCE=.\C\nsdCustomTable.c
# End Source File
# Begin Source File

SOURCE=.\C\PilotController.c
# End Source File
# Begin Source File

SOURCE=.\C\rtc.c
# End Source File
# Begin Source File

SOURCE=.\C\SPI_access.c
# End Source File
# Begin Source File

SOURCE=.\C\SPI_functions.c
# End Source File
# Begin Source File

SOURCE=.\c\Status.c
# End Source File
# Begin Source File

SOURCE=.\C\TDM_Init.c
# End Source File
# Begin Source File

SOURCE=.\C\TDM_Service.C
# End Source File
# Begin Source File

SOURCE=.\C\tdmEocData.c
# End Source File
# Begin Source File

SOURCE=.\C\tdmUpConvSettings.c
# End Source File
# Begin Source File

SOURCE=.\C\Timer.c
# End Source File
# End Group
# Begin Group "H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\H\AD9833_Generator.h
# End Source File
# Begin Source File

SOURCE=.\H\AES600.h
# End Source File
# Begin Source File

SOURCE=.\H\APLC_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\BASETYPE.H
# End Source File
# Begin Source File

SOURCE=.\H\boost_manager.h
# End Source File
# Begin Source File

SOURCE=.\H\Booting.h
# End Source File
# Begin Source File

SOURCE=.\H\bridge.h
# End Source File
# Begin Source File

SOURCE=.\H\ChannelSim_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\commissioning.h
# End Source File
# Begin Source File

SOURCE=.\H\Configuration.H
# End Source File
# Begin Source File

SOURCE=.\H\CONSTANT.H
# End Source File
# Begin Source File

SOURCE=.\H\crc.h
# End Source File
# Begin Source File

SOURCE=.\H\CRC16_table.H
# End Source File
# Begin Source File

SOURCE=.\H\DEFAULT.H
# End Source File
# Begin Source File

SOURCE=.\H\DIAG.H
# End Source File
# Begin Source File

SOURCE=.\H\DL_2nd.H
# End Source File
# Begin Source File

SOURCE=.\H\DL_EOC.H
# End Source File
# Begin Source File

SOURCE=.\H\DL_RS232.H
# End Source File
# Begin Source File

SOURCE=.\H\dsp_micro_common.h
# End Source File
# Begin Source File

SOURCE=.\H\edgeDetection.h
# End Source File
# Begin Source File

SOURCE=.\H\EPROMHI.H
# End Source File
# Begin Source File

SOURCE=.\H\EPROMLOW.H
# End Source File
# Begin Source File

SOURCE=.\H\EPROMMED.H
# End Source File
# Begin Source File

SOURCE=.\H\EQ_CALCU.H
# End Source File
# Begin Source File

SOURCE=.\H\Equalizer.h
# End Source File
# Begin Source File

SOURCE=.\H\equalizerFirFilter.h
# End Source File
# Begin Source File

SOURCE=.\H\equalizerTypes.h
# End Source File
# Begin Source File

SOURCE=.\H\Event_Defs.H
# End Source File
# Begin Source File

SOURCE=.\H\EventRec.h
# End Source File
# Begin Source File

SOURCE=.\H\ExternalTDM_BusBridge.h
# End Source File
# Begin Source File

SOURCE=.\H\ExternalTDM_BusLookupTables.h
# End Source File
# Begin Source File

SOURCE=.\H\FE_DSP_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\FT12.h
# End Source File
# Begin Source File

SOURCE=.\H\iButAlarm.h
# End Source File
# Begin Source File

SOURCE=.\H\iButHigh.h
# End Source File
# Begin Source File

SOURCE=.\H\iButLow.h
# End Source File
# Begin Source File

SOURCE=.\H\iButMed.h
# End Source File
# Begin Source File

SOURCE=.\H\iButPw.h
# End Source File
# Begin Source File

SOURCE=.\H\idleCounter.h
# End Source File
# Begin Source File

SOURCE=.\H\Init_Master_DSP.H
# End Source File
# Begin Source File

SOURCE=.\H\IO_HW_Buffer.h
# End Source File
# Begin Source File

SOURCE=.\H\IrigB.h
# End Source File
# Begin Source File

SOURCE=.\H\licenceCheck.h
# End Source File
# Begin Source File

SOURCE=.\H\MagicNumbersForMemoryCheck.h
# End Source File
# Begin Source File

SOURCE=.\H\Master_DSP_Main.h
# End Source File
# Begin Source File

SOURCE=.\H\masterDSP_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\MemoryCheck.h
# End Source File
# Begin Source File

SOURCE=.\H\MONITOR.H
# End Source File
# Begin Source File

SOURCE=.\H\msg_P4LQ.h
# End Source File
# Begin Source File

SOURCE=.\H\msgHandler.h
# End Source File
# Begin Source File

SOURCE=.\H\NSD_DSP_Types.h
# End Source File
# Begin Source File

SOURCE=..\NSD_DSP\H\NSD_TEST.H
# End Source File
# Begin Source File

SOURCE=.\H\NSD_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\nsdCustomTable.H
# End Source File
# Begin Source File

SOURCE=.\H\NSK_DSP_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\NSK_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\O4CV_Const.h
# End Source File
# Begin Source File

SOURCE=.\H\OFDM_DSP_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\OFDM_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\P4LT_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\Piggyback.h
# End Source File
# Begin Source File

SOURCE=.\H\PilotController.h
# End Source File
# Begin Source File

SOURCE=.\H\R1LA_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\RF_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\rtc.h
# End Source File
# Begin Source File

SOURCE=.\H\rtc_M41ST95.h
# End Source File
# Begin Source File

SOURCE=.\H\sampling.h
# End Source File
# Begin Source File

SOURCE=.\H\SerialFLASH.h
# End Source File
# Begin Source File

SOURCE=.\H\SPI_access.h
# End Source File
# Begin Source File

SOURCE=.\H\spi_dsp.h
# End Source File
# Begin Source File

SOURCE=.\H\SPI_functions.h
# End Source File
# Begin Source File

SOURCE=.\H\Status.h
# End Source File
# Begin Source File

SOURCE=.\H\tdm_ext_types.h
# End Source File
# Begin Source File

SOURCE=.\H\TDM_Init.h
# End Source File
# Begin Source File

SOURCE=.\H\tdm_int_cfg.h
# End Source File
# Begin Source File

SOURCE=.\H\tdm_int_types.h
# End Source File
# Begin Source File

SOURCE=.\H\TDM_Service.H
# End Source File
# Begin Source File

SOURCE=.\H\tdmEocData.h
# End Source File
# Begin Source File

SOURCE=.\H\tdmUpConvSettings.h
# End Source File
# Begin Source File

SOURCE=.\H\Timer.h
# End Source File
# Begin Source File

SOURCE=.\H\UC_DSP_Types.h
# End Source File
# End Group
# Begin Group "ASM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ASM\ArrayFloatCheck.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\ExternalTDM_BusLowLevel.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\Interrupt_routines_Master_DSP.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\Interrupt_table_Master_DSP.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\PN_GENER.ASM
# End Source File
# End Group
# Begin Group "architecture"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\architecture\Master_DSP-21161.ldf"
# End Source File
# End Group
# Begin Group "batch"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\batch\Compile.bat
# End Source File
# Begin Source File

SOURCE=.\batch\EMERGENCY.bat
# End Source File
# Begin Source File

SOURCE=.\batch\EMERGENCY.lnk
# End Source File
# Begin Source File

SOURCE=.\batch\Master.bat
# End Source File
# Begin Source File

SOURCE=.\batch\Master_DSP.lnk
# End Source File
# End Group
# End Target
# End Project
