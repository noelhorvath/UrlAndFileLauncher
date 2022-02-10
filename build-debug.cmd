@echo off
echo Building mxt-debug...
windres debug.rc mxt_debug_res.o
g++ -Wall -o2 -municode -mwindows FileLauncher.cpp -o fl-debug.exe mxt_debug_res.o
echo Debug version is ready.