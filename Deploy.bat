@echo off
set /p SUBDIR=Deploy in subdir: 
cd %SUBDIR%
echo %cd%
pause
robocopy "C:\Program Files (x86)\Windows Kits\10\Redist\ucrt\DLLs\x86" "%cd%"
C:\Qt\5.12.0\msvc2017\bin\windeployqt.exe
pause