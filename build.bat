@echo off
REM Build script for the C++ / raylib snake game.
REM Uses the w64devkit g++ and raylib that are installed under C:\raylib.

setlocal
set RAYLIB=C:\raylib\w64devkit
set GPP=%RAYLIB%\bin\g++.exe
REM Put w64devkit's own tools (as, ld) first so the 64-bit toolchain is used.
set PATH=%RAYLIB%\bin;%PATH%

REM Do NOT add -L"%RAYLIB%\lib": that dir holds UCRT runtime libs that clash
REM with this msvcrt g++ and crash at startup. Let g++ use its default (sysroot) libs.
"%GPP%" snake.cpp -o snake.exe ^
    -O2 -std=c++17 ^
    -I"%RAYLIB%\include" ^
    -lraylib -lopengl32 -lgdi32 -lwinmm

if %errorlevel%==0 (
    echo Build succeeded. Run snake.exe from this folder.
) else (
    echo Build failed. Make sure raylib is installed and on the include/lib path.
)
endlocal
