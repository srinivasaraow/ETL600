# Microsoft Developer Studio Project File - Name="NSD_DSP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=NSD_DSP - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NSD_DSP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NSD_DSP.mak" CFG="NSD_DSP - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NSD_DSP - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "NSD_DSP - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Source/P4LT/NSD_DSP", MAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NSD_DSP - Win32 Release"

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

!ELSEIF  "$(CFG)" == "NSD_DSP - Win32 Debug"

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

# Name "NSD_DSP - Win32 Release"
# Name "NSD_DSP - Win32 Debug"
# Begin Group "C"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\C\AES_Detector.c
# End Source File
# Begin Source File

SOURCE=.\C\AES_Transmitter.c
# End Source File
# Begin Source File

SOURCE=.\C\crc.c
# End Source File
# Begin Source File

SOURCE=.\C\DIAG.C
# End Source File
# Begin Source File

SOURCE=.\C\ExternalTDM_BusG4AI_HL.c
# End Source File
# Begin Source File

SOURCE=.\C\idleCounter.c
# End Source File
# Begin Source File

SOURCE=.\C\Init_NSD_DSP.c
# End Source File
# Begin Source File

SOURCE=.\C\LT_Func_NSD600.c
# End Source File
# Begin Source File

SOURCE=.\C\MemoryCheck.c
# End Source File
# Begin Source File

SOURCE=.\C\NSD_CustomTableConverter.c
# End Source File
# Begin Source File

SOURCE=.\C\NSD_Detector.c
# End Source File
# Begin Source File

SOURCE=.\C\NSD_DSP_Main.c
# End Source File
# Begin Source File

SOURCE=.\C\NSD_Monitor.c
# End Source File
# Begin Source File

SOURCE=.\C\NSD_Output_At_RxAlarm.c
# End Source File
# Begin Source File

SOURCE=.\C\NSD_Testsignal_Generator.c
# End Source File
# Begin Source File

SOURCE=.\C\NSD_Transmitter.c
# End Source File
# Begin Source File

SOURCE=.\C\nsdCustomTable.c
# End Source File
# Begin Source File

SOURCE=.\C\Rx_Guard_NSD.c
# End Source File
# Begin Source File

SOURCE=.\C\TDM_Init.c
# End Source File
# Begin Source File

SOURCE=.\C\TDM_Service.C
# End Source File
# Begin Source File

SOURCE=.\C\Timer.c
# End Source File
# End Group
# Begin Group "H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\H\AES600.h
# End Source File
# Begin Source File

SOURCE=.\H\AES_Detector.h
# End Source File
# Begin Source File

SOURCE=.\H\AES_LP_FilterCoef.h
# End Source File
# Begin Source File

SOURCE=.\H\AES_SineTable.h
# End Source File
# Begin Source File

SOURCE=.\H\AES_Transmitter.h
# End Source File
# Begin Source File

SOURCE=.\H\APLC_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\BASETYPE.H
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

SOURCE=.\H\CRC32_table.H
# End Source File
# Begin Source File

SOURCE=.\H\DIAG.H
# End Source File
# Begin Source File

SOURCE=.\H\Event_Defs.H
# End Source File
# Begin Source File

SOURCE=.\H\ExternalTDM_BusG4AI_HL.h
# End Source File
# Begin Source File

SOURCE=.\H\idleCounter.h
# End Source File
# Begin Source File

SOURCE=.\H\Init_NSD_DSP.h
# End Source File
# Begin Source File

SOURCE=.\H\MagicNumbersForMemoryCheck.h
# End Source File
# Begin Source File

SOURCE=.\H\MemoryCheck.h
# End Source File
# Begin Source File

SOURCE=.\H\NSD600_LT_Constants.h
# End Source File
# Begin Source File

SOURCE=.\H\NSD_CustomTableConverter.h
# End Source File
# Begin Source File

SOURCE=.\H\NSD_Detector.h
# End Source File
# Begin Source File

SOURCE=.\H\NSD_DSP_Main.h
# End Source File
# Begin Source File

SOURCE=.\H\NSD_DSP_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\NSD_Monitor.h
# End Source File
# Begin Source File

SOURCE=.\H\NSD_Output_At_RxAlarm.h
# End Source File
# Begin Source File

SOURCE=.\H\NSD_TEST.H
# End Source File
# Begin Source File

SOURCE=.\H\NSD_Transmitter.h
# End Source File
# Begin Source File

SOURCE=.\H\NSD_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\nsdCustomTable.H
# End Source File
# Begin Source File

SOURCE=.\H\P4LT_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\Rx_Guard_NSD.h
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

SOURCE=.\H\Timer.h
# End Source File
# End Group
# Begin Group "ASM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ASM\ArrayFloatCheck.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\benchmark.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\BIIIR.ASM
# End Source File
# Begin Source File

SOURCE=.\ASM\CRC32_calculation.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\Interrupt_routines_NSD_DSP.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\Interrupt_table_NSD_DSP.asm
# End Source File
# End Group
# Begin Group "architecture"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\architecture\NSD_DSP-21161.ldf"
# End Source File
# End Group
# Begin Group "batch"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\batch\Compile.bat
# End Source File
# Begin Source File

SOURCE=.\batch\NSD.bat
# End Source File
# Begin Source File

SOURCE=.\batch\NSD_DSP.lnk
# End Source File
# End Group
# End Target
# End Project
