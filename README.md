# FileLauncher
C++ Windows console application for launching files. It was mainly created for my C# WinForms project called MiXTools to circumvent reverse engeneering Xiaomi's Mi OSD Utility software in order to change the dedicated assistant button's key press action to something useful for newer generation of Xiaomi laptops.

## Features
FileLauncher can open any file or open file in it's extension associated editor.
It has Unicode support for Unicode paths. Windows Drag-and-Drop works on the executable (because it a single argument for a path and performs an open launch mode).

## Arguments

FileLauncher accepts maximum of two arguments a string and another string and the order of the arguments are interchangeable.
It accepts arguments from the command line if any given otherwise it tries to read the first two lines of the config file if it exists.

### Launch modes 
  ( () == optional )
  
  * **-o**, **(-)open** : Opens the file. (default when no 2nd argument is given to the app)
  * **-r**, **(-)runas** : Launches the application as an Administrator. (only works for apps)
  * **-e**, **(-)edit** : Launches an editor and opens the file for editing. (works if the extension has an associated app)

## Usage
FileLauncher can be used with command line arguments or **fl** (without an extension) config file.

### Command line arguments
  **fl**: name of the executable 
  <br/>
  **p**: path to the file
  <br/>
  **m**: launch mode
  <br/>
  - `fl "p" m`
  - `fl "p" -m`
  - `fl m "p"`
  - `fl -m "p"`
  
  Using a single argument defaults to **open** launch mode
 
  - `fl "p"`
  - `fl p` (only works if it the path doesn't contain any space characters)

### Config file
Reads the first two lines of **fl** (config file).<br/>
If the file has only one line the launch mode defaults to **open**.
  
## Compile

  ***debug***: `g++ -Wall -o2 -municode FileLauncher.cpp -o fl` <br/>
  ***release***: `g++ -Wall -o2 -mwindows -municode FileLauncher.cpp -o fl` <br/>
  
  The flag mwindows is used for disabling console and municode is needed for Unicode (wWinMain) support.
  
## Log file
The app should log to a file called **fl_error.txt** in case of an error (ex: invalid arguments, file doesn't exist, ...).
