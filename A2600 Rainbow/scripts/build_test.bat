@echo off
set CC65=E:\Software\cc65
set CC65BIN=%CC65%\bin

set STELLA=E:\Software\Stella\Stella.exe

set CC65CFG=%CC65%\cfg\atari2600.cfg
set CC65INC=%CC65%\asminc\atari2600.inc
set CC65LIB=%CC65%\lib\atari2600.lib

set DIR=build
set FILE=test

if not exist %DIR% mkdir %DIR%
DEL /Q %DIR%\%FILE%.*

%CC65BIN%\ca65.exe -t atari2600 -o %DIR%\%FILE%.o src\%FILE%.s
if errorlevel 1 exit /b 1

%CC65BIN%\ld65.exe -C %CC65CFG% -m %DIR%\%FILE%.map -Ln %DIR%\%FILE%.labels -vm %DIR%\%FILE%.o --lib %CC65LIB% -o %DIR%\%FILE%.bin
if errorlevel 1 exit /b 1

%STELLA% -format ntsc %DIR%\%FILE%.bin
