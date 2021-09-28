@echo off
if not exist ".\bin" mkdir ".\bin"
pushd ".\bin"
call g++ -std=c++17 -O3 "..\src\main.cpp" -o "vm.exe" -luser32 -lgdi32
popd