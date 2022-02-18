@echo off
echo Debug build started...
windres debug.rc debug_res.o
g++ -Wall -municode -std=c++23 program.cpp uafl_api.cpp -o uafl-debug.exe debug_res.o
echo Cleaning up files...
if exist debug_res.o del /f debug_res.o
echo Debug version is ready.
exit /b 0