@echo off
echo "Debug build started..."
windres debug.rc mxt_debug_res.o
g++ -Wall -municode program.cpp uafl_api.cpp -o uafl-debug.exe mxt_debug_res.o
echo "Cleaning up files..."
if exist mxt_debug_res.o del /f mxt_debug_res.o
echo "Debug version is ready."
exit /b 0