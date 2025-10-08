@echo off
REM compile.bat - Script de compilation simple

SET PATH=C:\opencv\build\x64\vc16\bin;%PATH%

echo === Compilation Algorith.C ===

REM Trouver Visual Studio
FOR /F "tokens=*" %%i IN ('"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath') DO SET VS_PATH=%%i

REM Initialiser environnement et compiler
call "%VS_PATH%\VC\Auxiliary\Build\vcvars64.bat" > nul 2>&1
cl /EHsc /TP /I"C:\opencv\build\include" Algorith.C /link /LIBPATH:"C:\opencv\build\x64\vc16\lib" opencv_world4120.lib

IF %ERRORLEVEL% EQU 0 (
    echo.
    echo === Compilation reussie ===
    echo Executable: Algorith.exe
    echo.
    echo Pour tester: run.bat
) ELSE (
    echo.
    echo === Erreur de compilation ===
)
