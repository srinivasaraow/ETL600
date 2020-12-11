# Microsoft Developer Studio Project File - Name="NSK_DSP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=NSK_DSP - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NSK_DSP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NSK_DSP.mak" CFG="NSK_DSP - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NSK_DSP - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "NSK_DSP - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Source/P4LT/NSK_DSP", KBAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NSK_DSP - Win32 Release"

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

!ELSEIF  "$(CFG)" == "NSK_DSP - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "NSK_DSP___Win32_Debug"
# PROP BASE Intermediate_Dir "NSK_DSP___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "NSK_DSP___Win32_Debug"
# PROP Intermediate_Dir "NSK_DSP___Win32_Debug"
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

# Name "NSK_DSP - Win32 Release"
# Name "NSK_DSP - Win32 Debug"
# Begin Group "C"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\C\crc.c
# End Source File
# Begin Source File

SOURCE=.\C\DAPSK.c
# End Source File
# Begin Source File

SOURCE=.\C\DIAG.C
# End Source File
# Begin Source File

SOURCE=.\C\FPGAHandling.c
# End Source File
# Begin Source File

SOURCE=.\C\FSK.c
# End Source File
# Begin Source File

SOURCE=.\C\idleCounter.c
# End Source File
# Begin Source File

SOURCE=.\C\Init_NSK_DSP.c
# End Source File
# Begin Source File

SOURCE=.\C\MemoryCheck.c
# End Source File
# Begin Source File

SOURCE=.\C\NSK_DSP_Main.c
# End Source File
# Begin Source File

SOURCE=.\C\NSK_FPGA.c
# End Source File
# Begin Source File

SOURCE=.\C\oscillator.c
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

SOURCE=.\H\ADPSK_FilterCoef.h
# End Source File
# Begin Source File

SOURCE=.\H\BASETYPE.H
# End Source File
# Begin Source File

SOURCE=.\H\CONSTANT.H
# End Source File
# Begin Source File

SOURCE=.\H\CRC16_table.H
# End Source File
# Begin Source File

SOURCE=.\H\DAPSK.h
# End Source File
# Begin Source File

SOURCE=.\H\DAPSK_Data.h
# End Source File
# Begin Source File

SOURCE=.\H\DIAG.H
# End Source File
# Begin Source File

SOURCE=.\H\FPGA_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\FPGAHandling.h
# End Source File
# Begin Source File

SOURCE=.\H\FSK.h
# End Source File
# Begin Source File

SOURCE=.\H\FSK_FilterCoef.h
# End Source File
# Begin Source File

SOURCE=.\H\idleCounter.h
# End Source File
# Begin Source File

SOURCE=.\H\Init_NSK_DSP.H
# End Source File
# Begin Source File

SOURCE=.\H\MagicNumbersForMemoryCheck.h
# End Source File
# Begin Source File

SOURCE=.\H\MemoryCheck.h
# End Source File
# Begin Source File

SOURCE=.\H\NSK_DSP_Constant.h
# End Source File
# Begin Source File

SOURCE=.\H\NSK_DSP_Main.h
# End Source File
# Begin Source File

SOURCE=..\NSK_DSP\H\NSK_DSP_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\NSK_FPGA.h
# End Source File
# Begin Source File

SOURCE=.\H\NSK_FPGA_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\NSK_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\O4CV_Const.h
# End Source File
# Begin Source File

SOURCE=.\H\OFDM_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\oscillator.h
# End Source File
# Begin Source File

SOURCE=.\H\Piggyback.h
# End Source File
# Begin Source File

SOURCE=.\H\speeddsp.h
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

SOURCE=.\ASM\Interrupt_routines_NSK_DSP.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\Interrupt_table_NSK_DSP.asm
# End Source File
# End Group
# Begin Group "architecture"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\architecture\NSK_DSP-21161.ldf"
# End Source File
# End Group
# Begin Group "batch"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\batch\Compile.bat
# End Source File
# Begin Source File

SOURCE=.\batch\NSK.bat
# End Source File
# Begin Source File

SOURCE=.\batch\NSK_DSP.lnk
# End Source File
# End Group
# End Target
# End Project
