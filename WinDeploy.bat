@echo off
rem ===== Deploy settings =====
set QT_DIR=C:\Qt
set QT_VER=5.12.0
set QT_CFG_32=msvc2017
set QT_CFG_64=msvc2017_64
set QT_OPT=--no-system-d3d-compiler --no-opengl-sw

set VC_REDIST_32=C:\Program Files (x86)\Windows Kits\10\Redist\ucrt\DLLs\x86
set VC_REDIST_64=C:\Program Files (x86)\Windows Kits\10\Redist\ucrt\DLLs\x64

set OUT_FILE=Animate Together.exe

rem ===== Initial settings =====
set QT_CFG=%QT_CFG_32%
set VC_REDIST=%VC_REDIST_32%

rem ===== Input =====
set /p SUBDIR=Deploy in subdir: 
set /p CFG=Build config (x86 or x64): 

if "%CFG%" neq "x86" if "%CFG%" neq "x64" goto bad_cfg
if "%CFG%" neq "x64" goto deploy
rem x64 was chosen:
set VC_REDIST=%VC_REDIST_64%
set QT_CFG=%QT_CFG_64%

rem ===== Deploy =====
:deploy

robocopy "%VC_REDIST%" "%cd%\%SUBDIR%"
copy "LICENSE" "%cd%\%SUBDIR%\License.txt"

%QT_DIR%\%QT_VER%\%QT_CFG%\bin\windeployqt.exe %QT_OPT% "%cd%\%SUBDIR%\%OUT_FILE%"

pause
exit

rem ===== Error =====
:bad_cfg
echo "Bad configuration "%CFG%", choose "x64" or "x86"
pause
exit