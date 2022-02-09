# FileLauncher
C++ Windows application for launching files. It was mainly created for my C# WinForms project called MiXTools to circumvent reverse engeneering Xiaomi's Mi OSD Utility software in order to change the dedicated assistant button's key press action to something useful for newer generation of Xiaomi laptops.

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
If the file has only one line the launch mode defaults to **open**. <br/>
To launch the specified file run the executable of FileLauncher.
  
## Compile
### Using terminal
  ***debug***:
  	<br/>
	<br/>
	&emsp;1.&ensp;`windres debug.rc debug_res.o`<br/>
	&emsp;2.&ensp;`g++ -Wall -o2 -municode -mwindows FileLauncher.cpp -o fl-debug.exe debug_res.o`<br/>
	<br/>
  ***release***: 
	<br/>
	<br/>
	&emsp;1.&ensp;`windres prod.rc prod_res.o`<br/>
	&emsp;2.&ensp;`g++ -Wall -o2 -municode -mwindows FileLauncher.cpp -o fl.exe prod_res.o`<br/>
	<br/>
  The flag mwindows is used for disabling console and municode is needed for Unicode (wWinMain) support.

### Using scripts  
  Run the .cmd files (***build-debug.cmd***, ***build-prod.cmd***, ***build.cmd***) to compile the source code.

## Log file
The app should log to a file called **fl_error.txt** in case of an error (ex: invalid arguments, file doesn't exist, ...).

### MiXTools version
MiXTools is a modified version of the FileLauncher. The main difference is that it uses MiXTools' AppData path for reading config file and outputs error log to the application's directory.
