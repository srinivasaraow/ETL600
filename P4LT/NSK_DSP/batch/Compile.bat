echo on

set WORK_DIR=D:\ADSP\$P4LT
if defined P4LT_BUILD_PATH set WORK_DIR=%P4LT_BUILD_PATH%

:COMP_C
cc21k  %C_OPT% -21161 -I "%WORK_DIR%\NSK_DSP\H" -c "%1"

del %WORK_DIR%\NSK_DSP\batch\*.doj
