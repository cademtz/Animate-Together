@echo off
set /p SUBDIR=Deploy in subdir: 
cd %SUBDIR%
echo %cd%
pause
robocopy "C:\Program Files (x86)\Windows Kits\10\Redist\ucrt\DLLs\x86" "%cd%"