@echo off
echo Building production...
windres prod.rc prod_res.o
g++ -Wall -o2 -municode -mwindows FileLauncher.cpp -o fl.exe prod_res.o
echo Production version is ready.