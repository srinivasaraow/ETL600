# Microsoft Developer Studio Project File - Name="UC_DSP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=UC_DSP - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "UC_DSP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "UC_DSP.mak" CFG="UC_DSP - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UC_DSP - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "UC_DSP - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Source/P4LT Prototype 2/UC_DSP", MBAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "UC_DSP - Win32 Release"

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

!ELSEIF  "$(CFG)" == "UC_DSP - Win32 Debug"

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

# Name "UC_DSP - Win32 Release"
# Name "UC_DSP - Win32 Debug"
# Begin Group "C"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\C\AD9777_Init.c
# End Source File
# Begin Source File

SOURCE=.\C\crc.c
# End Source File
# Begin Source File

SOURCE=.\C\DIAG.C
# End Source File
# Begin Source File

SOURCE=.\C\idleCounter.c
# End Source File
# Begin Source File

SOURCE=.\C\Init_UC_DSP.c
# End Source File
# Begin Source File

SOURCE=.\C\interpolation.c
# End Source File
# Begin Source File

SOURCE=.\C\MemoryCheck.c
# End Source File
# Begin Source File

SOURCE=.\C\oscillator.c
# End Source File
# Begin Source File

SOURCE=.\C\SupervisionUC_Bus_UC_DSP.c
# End Source File
# Begin Source File

SOURCE=.\C\TDM_Init.c
# End Source File
# Begin Source File

SOURCE=.\C\TDM_Service.C
# End Source File
# Begin Source File

SOURCE=.\C\tdmUpConvSettings.c
# End Source File
# Begin Source File

SOURCE=.\C\Timer.c
# End Source File
# Begin Source File

SOURCE=.\C\UC_DSP_Main.c
# End Source File
# Begin Source File

SOURCE=.\C\upDownConvTdmBus.c
# End Source File
# End Group
# Begin Group "H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\H\AD9777_dout.h
# End Source File
# Begin Source File

SOURCE=.\H\AD9777_Init.h
# End Source File
# Begin Source File

SOURCE=.\H\AD9777_utils.h
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

SOURCE=.\H\DIAG.H
# End Source File
# Begin Source File

SOURCE=.\H\H01.dat
# End Source File
# Begin Source File

SOURCE=.\H\H02.dat
# End Source File
# Begin Source File

SOURCE=.\H\H03.dat
# End Source File
# Begin Source File

SOURCE=.\H\H11.dat
# End Source File
# Begin Source File

SOURCE=.\H\H12.dat
# End Source File
# Begin Source File

SOURCE=.\H\H13.dat
# End Source File
# Begin Source File

SOURCE=.\H\H21.dat
# End Source File
# Begin Source File

SOURCE=.\H\H22.dat
# End Source File
# Begin Source File

SOURCE=.\H\H23.dat
# End Source File
# Begin Source File

SOURCE=.\H\H31.dat
# End Source File
# Begin Source File

SOURCE=.\H\H32.dat
# End Source File
# Begin Source File

SOURCE=.\H\H33.dat
# End Source File
# Begin Source File

SOURCE=.\H\h4x44_0.dat
# End Source File
# Begin Source File

SOURCE=.\H\h4x44_1.dat
# End Source File
# Begin Source File

SOURCE=.\H\h4x44_2.dat
# End Source File
# Begin Source File

SOURCE=.\H\h4x44_3.dat
# End Source File
# Begin Source File

SOURCE=.\H\idleCounter.h
# End Source File
# Begin Source File

SOURCE=.\H\Init_UC_DSP.h
# End Source File
# Begin Source File

SOURCE=.\H\interpolation.h
# End Source File
# Begin Source File

SOURCE=.\H\intFiltCoef.h
# End Source File
# Begin Source File

SOURCE=.\H\MagicNumbersForMemoryCheck.h
# End Source File
# Begin Source File

SOURCE=.\H\MemoryCheck.h
# End Source File
# Begin Source File

SOURCE=.\H\oscillator.h
# End Source File
# Begin Source File

SOURCE=.\H\RF_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\speeddsp.h
# End Source File
# Begin Source File

SOURCE=.\H\SupervisionUC_bus_UC_DSP.h
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

SOURCE=.\H\tdmUpConvSettings.h
# End Source File
# Begin Source File

SOURCE=.\H\Timer.h
# End Source File
# Begin Source File

SOURCE=.\H\UC_DSP_Const.h
# End Source File
# Begin Source File

SOURCE=.\H\UC_DSP_Main.h
# End Source File
# Begin Source File

SOURCE=.\H\UC_DSP_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\upDownConvTdmBus.h
# End Source File
# End Group
# Begin Group "ASM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ASM\AD9777_dout.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\AD9777_utils.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\ArrayFloatCheck.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\benchmark.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\Int4x44_filt_c.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\Interp4_filt_c_L16.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\Interp4_filt_c_L24.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\Interrupt_routines_UC_DSP.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\Interrupt_table_UC_DSP.asm
# End Source File
# End Group
# Begin Group "architecture"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\architecture\UC_DSP-21161.ldf"
# End Source File
# End Group
# Begin Group "batch"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\batch\asm.bat
# End Source File
# Begin Source File

SOURCE=.\batch\Compile.bat
# End Source File
# Begin Source File

SOURCE=.\batch\UC_DSP.bat
# End Source File
# Begin Source File

SOURCE=.\batch\UC_DSP.lnk
# End Source File
# End Group
# End Target
# End Project
