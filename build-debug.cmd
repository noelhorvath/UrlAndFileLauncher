@echo off
echo Building debug...
windres debug.rc debug_res.o
g++ -Wall -o2 -municode FileLauncher.cpp -o fl-debug.exe debug_res.o
echo Debug version is ready.