@echo off
echo Building mxt-production...
windres prod.rc mxt_prod_res.o
g++ -Wall -o2 -municode -mwindows FileLauncher.cpp -o fl.exe mxt_prod_res.o
echo Production version is ready.