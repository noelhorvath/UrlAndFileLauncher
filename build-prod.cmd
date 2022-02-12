@echo off
echo Production build started...
windres prod.rc prod_res.o
g++ -Wall -O2 -municode -mwindows program.cpp uafl_api.cpp -o uafl.exe prod_res.o
echo Cleaning up files...
if exist prod_res.o del /f prod_res.o
echo Production version is ready.
exit /b 0