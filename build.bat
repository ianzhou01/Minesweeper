@echo off
setlocal enabledelayedexpansion

REM Set MinGW path
set MINGW_DIR=C:\mingw64
set GPP="%MINGW_DIR%\bin\g++.exe"
set EXE_NAME=Minesweeper.exe

REM Check if MinGW exists
if not exist %GPP% (
    echo ERROR: MinGW not found in %MINGW_DIR%! Please check your installation.
    pause
    exit /b 1
)

REM Set SFML paths
set SFML_DIR=%CD%\dependencies\SFML-2.5.1
set SFML_LIB_DIR="%SFML_DIR%\lib"
set SFML_INCLUDE_DIR="%SFML_DIR%\include"

REM Verify SFML exists
if not exist "%SFML_INCLUDE_DIR%\SFML\Graphics.hpp" (
    echo ERROR: SFML headers not found in dependencies/SFML-2.5.1! Please check your extraction.
    pause
    exit /b 1
)

if not exist %SFML_LIB_DIR% (
    echo ERROR: SFML library directory not found! Expected at %SFML_LIB_DIR%
    pause
    exit /b 1
)

REM Check for necessary SFML DLLs
set SFML_DLLS=sfml-graphics-2.dll sfml-window-2.dll sfml-system-2.dll sfml-audio-2.dll
for %%d in (%SFML_DLLS%) do (
    if not exist "%SFML_DIR%\bin\%%d" (
        echo ERROR: Missing SFML DLL: %%d
        pause
        exit /b 1
    )
)

REM Compile program
echo Compiling Minesweeper...
%GPP% -std=c++11 -Wall -g ^
    -I%SFML_INCLUDE_DIR% ^
    -o %EXE_NAME% ^
    src/main.cpp src/Board.cpp src/Button.cpp src/Game.cpp src/Functions.cpp src/Timer.cpp src/MineCounter.cpp src/WelcomeWindow.cpp src/Leaderboard.cpp ^
    -L%SFML_LIB_DIR% -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

REM Check for compilation errors
if %errorlevel% neq 0 (
    echo ERROR: Compilation failed! Please check the error messages above.
    pause
    exit /b 1
)

REM Copy SFML DLLs to the output directory (if needed)
for %%d in (%SFML_DLLS%) do (
    copy "%SFML_DIR%\bin\%%d" "%CD%" >nul
)

echo Build successful!
echo Running Minesweeper...
start %EXE_NAME%

endlocal
