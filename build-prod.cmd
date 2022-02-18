@echo off
echo Production build started...
windres prod.rc mxt_prod_res.o
g++ -Wall -O2 -municode -mwindows -std=c++23 program.cpp uafl_api.cpp -o uafl.exe mxt_prod_res.o
echo Cleaning up files...
if exist mxt_prod_res.o del /f mxt_prod_res.o
echo Production version is ready.
exit /b 0