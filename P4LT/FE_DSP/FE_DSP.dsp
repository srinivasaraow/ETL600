# Microsoft Developer Studio Project File - Name="FE_DSP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=FE_DSP - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FE_DSP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FE_DSP.mak" CFG="FE_DSP - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FE_DSP - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "FE_DSP - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Source/P4LT/FE_DSP", EAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FE_DSP - Win32 Release"

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

!ELSEIF  "$(CFG)" == "FE_DSP - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
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

# Name "FE_DSP - Win32 Release"
# Name "FE_DSP - Win32 Debug"
# Begin Group "C"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\C\AD6620_reg_io.c
# End Source File
# Begin Source File

SOURCE=.\C\afc.c
# End Source File
# Begin Source File

SOURCE=.\C\apFilter.c
# End Source File
# Begin Source File

SOURCE=.\C\BiIIR_Filter.c
# End Source File
# Begin Source File

SOURCE=.\C\crc.c
# End Source File
# Begin Source File

SOURCE=.\C\DIAG.C
# End Source File
# Begin Source File

SOURCE=.\C\equalizerFirFilter.c
# End Source File
# Begin Source File

SOURCE=.\C\FE_DSP_Alarm.c
# End Source File
# Begin Source File

SOURCE=.\C\FE_DSP_Main.c
# End Source File
# Begin Source File

SOURCE=.\C\freqShift.c
# End Source File
# Begin Source File

SOURCE=.\C\idleCounter.c
# End Source File
# Begin Source File

SOURCE=.\C\Init_FE_DSP.c
# End Source File
# Begin Source File

SOURCE=.\C\intDec.c
# End Source File
# Begin Source File

SOURCE=.\C\MemoryCheck.c
# End Source File
# Begin Source File

SOURCE=.\C\oscillator.c
# End Source File
# Begin Source File

SOURCE=.\C\PilotMod.c
# End Source File
# Begin Source File

SOURCE=.\C\sampling.c
# End Source File
# Begin Source File

SOURCE=.\C\SignalGen.c
# End Source File
# Begin Source File

SOURCE=.\C\SSB.c
# End Source File
# Begin Source File

SOURCE=.\C\SupervisionUC_Bus_FE_DSP.c
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

SOURCE=.\C\testPlcLink.c
# End Source File
# Begin Source File

SOURCE=.\C\TestTone.c
# End Source File
# Begin Source File

SOURCE=.\C\Timer.c
# End Source File
# Begin Source File

SOURCE=.\C\txSignalProcessingOfdm.c
# End Source File
# Begin Source File

SOURCE=.\C\upDownConvTdmBus.c
# End Source File
# End Group
# Begin Group "H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\H\ad6620.h
# End Source File
# Begin Source File

SOURCE=.\H\ad6620_filter.h
# End Source File
# Begin Source File

SOURCE=.\H\AD6620_Init.h
# End Source File
# Begin Source File

SOURCE=.\H\ad6620_RCF.inc
# End Source File
# Begin Source File

SOURCE=.\H\AD6620_reg_io.h
# End Source File
# Begin Source File

SOURCE=.\H\AD6620_utils.h
# End Source File
# Begin Source File

SOURCE=.\H\afc.h
# End Source File
# Begin Source File

SOURCE=.\H\apFilter.h
# End Source File
# Begin Source File

SOURCE=.\H\apFilterCoef.h
# End Source File
# Begin Source File

SOURCE=.\H\APLC_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\ASM_PROT.H
# End Source File
# Begin Source File

SOURCE=.\H\BASETYPE.H
# End Source File
# Begin Source File

SOURCE=.\H\BiIIR_Filter.h
# End Source File
# Begin Source File

SOURCE=.\H\commissioning.h
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

SOURCE=.\H\EQ_CALCU.H
# End Source File
# Begin Source File

SOURCE=.\H\EQ_COEFF.H
# End Source File
# Begin Source File

SOURCE=.\H\equalizerFirFilter.h
# End Source File
# Begin Source File

SOURCE=.\H\equalizerTypes.h
# End Source File
# Begin Source File

SOURCE=.\H\FE_DSP_Alarm.h
# End Source File
# Begin Source File

SOURCE=.\H\FE_DSP_Const.h
# End Source File
# Begin Source File

SOURCE=.\H\FE_DSP_Main.h
# End Source File
# Begin Source File

SOURCE=.\H\FE_DSP_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\freqShift.h
# End Source File
# Begin Source File

SOURCE=.\H\idleCounter.h
# End Source File
# Begin Source File

SOURCE=.\H\Init_FE_DSP.h
# End Source File
# Begin Source File

SOURCE=.\H\intDec.h
# End Source File
# Begin Source File

SOURCE=.\H\intDecCoef.h
# End Source File
# Begin Source File

SOURCE=.\H\MagicNumbersForMemoryCheck.h
# End Source File
# Begin Source File

SOURCE=.\H\MemoryCheck.h
# End Source File
# Begin Source File

SOURCE=.\H\msg_P4LQ.h
# End Source File
# Begin Source File

SOURCE=.\H\NSD_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\NSK_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\ofdmFilterCoef.h
# End Source File
# Begin Source File

SOURCE=.\H\oscillator.h
# End Source File
# Begin Source File

SOURCE=.\H\P4LT_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\PilotMod.h
# End Source File
# Begin Source File

SOURCE=.\H\PMFilter.h
# End Source File
# Begin Source File

SOURCE=.\H\R1LA_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\RF_Types.h
# End Source File
# Begin Source File

SOURCE=.\H\sampling.h
# End Source File
# Begin Source File

SOURCE=.\H\SignalGen.h
# End Source File
# Begin Source File

SOURCE=.\H\speeddsp.h
# End Source File
# Begin Source File

SOURCE=.\H\SSB.h
# End Source File
# Begin Source File

SOURCE=.\H\SSB_Coef.h
# End Source File
# Begin Source File

SOURCE=.\H\SupervisionUC_bus_FE_DSP.h
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

SOURCE=.\H\testPlcLink.h
# End Source File
# Begin Source File

SOURCE=.\H\TestTone.h
# End Source File
# Begin Source File

SOURCE=.\H\Timer.h
# End Source File
# Begin Source File

SOURCE=.\H\TTFILTER.H
# End Source File
# Begin Source File

SOURCE=.\H\txSignalProcessingOfdm.h
# End Source File
# Begin Source File

SOURCE=.\H\upDownConvTdmBus.h
# End Source File
# End Group
# Begin Group "ASM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ASM\AD6620_utils.asm
# End Source File
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

SOURCE=.\ASM\CDBIQUAD.ASM
# End Source File
# Begin Source File

SOURCE=.\ASM\Interrupt_routines_FE_DSP.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\Interrupt_table_FE_DSP.asm
# End Source File
# Begin Source File

SOURCE=.\ASM\PDEMOD.ASM
# End Source File
# Begin Source File

SOURCE=.\ASM\PMOD.ASM
# End Source File
# Begin Source File

SOURCE=.\ASM\PN_GENER.ASM
# End Source File
# Begin Source File

SOURCE=.\ASM\TTONE.ASM
# End Source File
# End Group
# Begin Group "architecture"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\architecture\FE_DSP-21161.ldf"
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

SOURCE=.\batch\FE_DSP.bat
# End Source File
# Begin Source File

SOURCE=.\batch\FE_DSP.lnk
# End Source File
# End Group
# End Target
# End Project
