@echo off
echo Production build started...
windres prod.rc prod_res.o
g++ -Wall -O2 -municode -mwindows program.cpp uafl_api.cpp -o uafl.exe prod_res.o
echo Production version is ready.