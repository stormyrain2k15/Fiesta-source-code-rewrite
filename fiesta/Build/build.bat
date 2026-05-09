@echo off
REM ============================================================================
REM Local CI parity build. Replicates what .github/workflows/build.yml
REM does so you can validate the same compile path before pushing.
REM
REM Usage:
REM     Build\build.bat              -- Debug build, full solution
REM     Build\build.bat Release      -- Release build, full solution
REM     Build\build.bat Debug Zone   -- Debug build of just Zone.vcxproj
REM
REM Output goes to Build\Bin\<Cfg>\ and Build\Logs\msbuild-<Cfg>.log
REM ============================================================================
setlocal enabledelayedexpansion

set "CFG=%~1"
if "%CFG%"=="" set "CFG=Debug"
set "PROJ=%~2"
if "%PROJ%"=="" (
    set "TARGET=Build\Fiesta.sln"
) else (
    set "TARGET=Build\%PROJ%.vcxproj"
)

mkdir Build\Logs 2>nul

where msbuild >nul 2>nul
if errorlevel 1 (
    echo [ERROR] msbuild not on PATH. Open a "Developer Command Prompt for VS"
    echo or add MSBuild\Current\Bin to PATH.
    exit /b 2
)

echo Building %TARGET% ^(%CFG%^)
msbuild "%TARGET%" ^
    /m /v:minimal ^
    /p:Configuration=%CFG% ^
    /p:Platform=Win32 ^
    /flp:LogFile=Build\Logs\msbuild-%CFG%.log;Verbosity=normal ^
    /flp1:LogFile=Build\Logs\msbuild-%CFG%.errors.log;ErrorsOnly ^
    /flp2:LogFile=Build\Logs\msbuild-%CFG%.warnings.log;WarningsOnly

set "RC=%ERRORLEVEL%"
echo.
echo --- Build complete, exit code %RC% ---
echo Logs: Build\Logs\msbuild-%CFG%.{log,errors.log,warnings.log}
exit /b %RC%
