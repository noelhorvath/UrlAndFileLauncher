# URLAndFileLauncher (UAFL)
C++ Windows application for launching/opening any valid URL or file. It was mainly created for my C# WinForms project called MiXTools to circumvent reverse engeneering Xiaomi's Mi OSD Utility software in order to change the dedicated assistant button's key press action to something useful for newer generation of Xiaomi laptops.

## Features
* open files and valid URLs
* open files in their extension associated editor 
* UTF-8 support
* Windows Drag-and-Drop with files

## Arguments

UAFL accepts maximum of two arguments and the order of the arguments are interchangeable.
It accepts input from the command line if any was given otherwise it tries to read the first two lines of the config file if it exists.

### Launch modes 
  ( () == optional )
  
  * **-o**, **(-)open** : Opens the given URL or file. (default when there is only a single argument)
  * **-r**, **(-)runas** : Launches the given application (or through a shortcut) as an Administrator. (only works with executables (.exe) and shortcuts (.lnk))
  * **-e**, **(-)edit** : Launches an editor and opens the given file for editing. (works if the extension has an associated app)

## Usage
UAFL can be used with command line or **uafl** (without an extension) config file.

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
To launch the specified file or url run the executable of UrlAndFileLauncher.
  
## Compile

### Terminal
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

### Build scripts  
Run the .cmd files (***build-debug.cmd***, ***build-prod.cmd***, ***build.cmd***) to compile the source code.

## Log file
The app will output logs to **uafl_error.txt** in case of an error.

## MiXTools version
MiXTools (mxt) is a modified version of URLAndFileLauncher. The main difference is that it uses MiXTools specific file paths for input and (error) output.
