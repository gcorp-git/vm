@echo off
if not exist ".\bin" mkdir ".\bin"
pushd ".\bin"
call g++ -std=c++17 -O3 "..\bc\main.cpp" -o "bc.exe" -luser32 -lgdi32
call ".\bc.exe"
popd