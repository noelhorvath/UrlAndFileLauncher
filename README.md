# URLAndFileLauncher (UAFL)
C++ Windows application for launching\opening any valid URL or file. It was mainly created for my C# WinForms project called MiXTools to circumvent reverse engeneering Xiaomi's Mi OSD Utility software in order to change the dedicated assistant button's key press action to something useful for newer generation of Xiaomi laptops.

## Features
UAFL can open any valid URL or file. It can also open files in their extension associated editor and it has support for Unicode characters. Windows Drag-and-Drop works on the executable (because it a single argument for a path and performs an open launch mode).

## Arguments

UAFL accepts maximum of two arguments a string and another string and the order of the arguments are interchangeable.
It accepts arguments from the command line if any given otherwise it tries to read the first two lines of the config file if it exists.

### Launch modes 
  ( () == optional )
  
  * **-o**, **(-)open** : Opens the given URL or file. (default when there is no 2nd argument)
  * **-r**, **(-)runas** : Launches the given application (or through a shortcut) as an Administrator. (only works with executables (.exe) and shortcuts (.lnk))
  * **-e**, **(-)edit** : Launches an editor and opens the given file for editing. (works if the extension has an associated app)

## Usage
UAFL can be used with command line arguments or **uafl** (without an extension) config file.

### Command line arguments
  **uafl**: name of the executable 
  <br/>
  **url_or_path**: a valid URL or a file's path
  <br/>
  **m**: file/URL launch mode
  <br/>
  - `uafl "url_or_path" m`
  - `uafl "url_or_path" "m"`
  - `uafl "url_or_path" -m`
  - `uafl "url_or_path" "-m"`
  - `uafl m "url_or_path"`
  - `uafl "m" "url_or_path"`
  - `uafl -m "url_or_path"`
  - `uafl "-m" "url_or_path"`
  
  Using a single argument defaults to **open** launch mode
 
  - `uafl "url_or_path"`
  - `uafl url_or_path` (only works if it the path or URL doesn't contain any space character)
### Config file
Reads the first two lines of **uafl** (config file).<br/>
If the file has only one line the launch mode defaults to **open**. <br/>
To launch the specified file or url run the executable of FileLauncher.
  
## Compile
### Using terminal
  ***debug***:
  	<br/>
	<br/>
	&emsp;1.&ensp;`windres debug.rc debug_res.o`<br/>
	&emsp;2.&ensp;`g++ -Wall -municode -std=c++23 program.cpp uafl_api.cpp -o uafl-debug.exe debug_res.o`<br/>
	<br/>
  ***release***: 
	<br/>
	<br/>
	&emsp;1.&ensp;`windres prod.rc prod_res.o`<br/>
	&emsp;2.&ensp;`g++ -Wall -O2 -municode -mwindows -std=c++23 program.cpp uafl_api.cpp -o uafl.exe prod_res.o`<br/>
	<br/>
  The flag mwindows is used for disabling console and municode is needed for Unicode (wWinMain) support.

### Using scripts  
  Run the .cmd files (***build-debug.cmd***, ***build-prod.cmd***, ***build.cmd***) to compile the source code.

## Log file
The app should log to a file called **uafl_error.txt** in case of an error (ex: invalid arguments, the given arugment is neither a file nor a valid, etc.).

## MiXTools version
MiXTools is a modified version of the URLAndFileLauncher. The main difference is that it uses MiXTools' AppData path for reading the config file and creating an error log file.
