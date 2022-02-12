@echo off
echo Debug build started...
windres debug.rc debug_res.o
g++ -Wall -municode program.cpp uafl_api.cpp -o uafl-debug.exe debug_res.o
echo Debug version is ready.