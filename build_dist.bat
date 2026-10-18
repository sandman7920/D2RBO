@echo off
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

if not exist "%VSWHERE%" (
    echo vswhere.exe not found.
    exit /b 1
)

for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
    set "VARSALL=%%i\VC\Auxiliary\Build\vcvarsall.bat"
)

if not exist "%VARSALL%" (
    echo vcvarsall.bat not found.
    exit /b 1
)

call "%VARSALL%" x64

cmake -S . -B build\dist -G "Ninja Multi-Config"
cmake --build build\dist --config Release --target package
rd /s /q "build\dist"
rd /s /q "dist\_CPack_Packages"
pause
