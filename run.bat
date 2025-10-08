@echo off
REM run.bat - Execute facilement le programme

REM Ajouter OpenCV au PATH
SET PATH=C:\opencv\build\x64\vc16\bin;%PATH%

REM Parametres par defaut
SET IMAGE=%1
SET SCALE=%2

IF "%IMAGE%"=="" SET IMAGE=Rasberry.jpg
IF "%SCALE%"=="" SET SCALE=0.023

echo === Detection de framboise ===
echo Image: %IMAGE%
echo Echelle: %SCALE% mm/px
echo.

Algorith.exe %IMAGE% %SCALE%
