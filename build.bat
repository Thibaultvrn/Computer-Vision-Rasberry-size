@echo off
REM Script de compilation pour Windows avec Visual Studio
REM Adapter les chemins selon votre installation OpenCV

SET OPENCV_DIR=C:\opencv\build
SET OPENCV_VERSION=481

REM Vérifier que OpenCV est installé
IF NOT EXIST "%OPENCV_DIR%" (
    echo ERREUR: OpenCV n'est pas installé dans %OPENCV_DIR%
    echo Veuillez installer OpenCV ou modifier OPENCV_DIR dans ce script
    pause
    exit /b 1
)

echo Compilation de Algorith.C avec OpenCV...

cl /I"%OPENCV_DIR%\include" Algorith.C ^
   /link /LIBPATH:"%OPENCV_DIR%\x64\vc16\lib" ^
   opencv_world%OPENCV_VERSION%.lib

IF %ERRORLEVEL% EQU 0 (
    echo.
    echo === COMPILATION REUSSIE ===
    echo Executable: algorith.exe
    echo.
    echo Usage: algorith.exe image.jpg [scale_mm_per_pixel]
    echo Exemple: algorith.exe raspberry.jpg 0.5
) ELSE (
    echo.
    echo === ERREUR DE COMPILATION ===
    echo Verifiez:
    echo  1. Visual Studio est installe avec les outils C/C++
    echo  2. Vous etes dans "Developer Command Prompt for VS"
    echo  3. OpenCV est installe dans %OPENCV_DIR%
    echo  4. La version OpenCV correspond (actuellement: %OPENCV_VERSION%)
)

pause
