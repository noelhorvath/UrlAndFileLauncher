/*
* Author: Noel Horváth
*/
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <codecvt>
#include <memory>

using namespace std;

/**
*
* Preffered compile commands:
*
*		debug:
* 
*			windres debug.rc debug_res.o
*			g++ -Wall -o2 -municode -mwindows FileLauncher.cpp -o fl-debug.exe debug_res.o
* 
*		production:
*			
*			windres prod.rc prod_res.o
*			g++ -Wall -o2 -municode -mwindows FileLauncher.cpp -o fl.exe prod_res.o
*	
* 
* or use the .cmd files for compiling.
*
*/

const int ARGS_COUNT = 2; // number of max possible arguments that needs to be processed
const int NUM_OF_FILE_ARGS = 1; // number of file args used
const int READ_BUFFER_SIZE = 512; // maybe support for max file char length?
const wchar_t QMARK = 34; // value of '"'
const wchar_t SPACE = 32; // value of ' '
const string CONFIG_FILE = "fl"; // config file
const string LOG_FILE = "fl_error.txt"; // log file
const int FROM_FILE = 0;
const int FROM_ARGS = 1;
// error messagees
const string SYS_OUT_OF_RESOURCES = "System is out of memory or resources!";
const string FILE_NOT_FOUND = "File was not found!";
const string PATH_NOT_FOUND = "Path was not found!";
const string SE_ACCESSDENIED = "File has been deleted or access is denied!";
const string SE_OOM = "There was not enough memory to complete the operation!";
const string BAD_FORMAT = "Invalid executable file!";
const string SE_SHARE = "Sharing violatin occured!";
const string SE_ASSOCINCOMPLETE = "File name association is incomplete or invalid!";
const string SE_DDETIMEOUT = "DDE transaction could not be completed, bacause the request timed out!";
const string SE_DDEFAIL = "DDE transaction failed!";
const string SE_DDEBUSY = "DDE transaction could not be completed, because other DDE transactions were being processed!";
const string SE_NOASSOC = "There is no application associated with the given file name extension!";
const string SE_DLLNOTFOUND = "DDL was not found!";
const string UNKNOWN_ERROR = "Unknown error!";
const string INVALID_FILE_LAUNCH_MODE = "Invalid file launch mode!";
const string FILE_NOT_EXISTS = "File doesn't exist!";
const string INVALID_ARGS = "Invalid arguments!";
const string COULD_NOT_OPEN_CONFIG_FILE = "Couldn't open config file!";
const string NO_CONFIG_FILE_FOUND = "Config file named \"fl\" doesn't exist in the executable's folder";
const string FILE_LAUNCH_MODE_ARG_ERROR = 
	"Error: Invalid file launch mode argument!\n"
	"Available modes:\n"
	"	-o, (-)open: Opens the file. (default when no 2nd argument)\n"
	"	-r, (-)runas: Launches the application as an Administrator.\n"
	"	-e, (-)edit: Launches an editor and opens the file for editing.\n";
const string ARG_ERROR = 
	"Error: Invalid arguments!\n"
	"Correct usages:\n\n"
	"	fl \"path_to_file\" file_launch_mode\n"
	"	fl file_launch_mode \"path_to_file\"\n"
	"\n"
	"	Using only a single argument defaults to open!\n\n"
	"	fl \"path_to_file\"\n"
	"	fl path_to_file\n";

// sets the correct access value for each argument
int order_args(unique_ptr<wstring[]> &args, int &argFile, int &argMode, string logFile);
// checks wether the given mode is valid
int is_mode_valid(wstring &mode);
// executes shellexecute on the file with the given mode
int open_file(const wchar_t* file, const wchar_t* mode, const wchar_t* params, int showCmd, string logFile);
// writes errorText to file and also prints it
// if write is successful returns true else false
int log_error(string file, string errorText);
// launch file from args or file
int launch_file(int from, const wchar_t *cmdLine, const int argsLength);
// returns the length of a wchar_t array
int wstr_length(wchar_t *str);
// returns the length of a const wchar_t array
int wstr_length(const wchar_t *str);

// win app entry point
// compile with -mwindows to completely hide terminal
// -municode needed to compile wWinMain
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {
	// terminate non-null terminated cmd arguments
	lpCmdLine += '\0';
	const int argsLength = wstr_length(lpCmdLine);
	if (argsLength == 0) {
		return launch_file(FROM_FILE, nullptr, argsLength);
	} else {
		return launch_file(FROM_ARGS, lpCmdLine, argsLength);
	}
	return 0;
}

int launch_file(int from, const wchar_t *cmdLine, const int argsLength) {
	if (from == FROM_ARGS) {
		//
		// cmdLine args to array + arg(s) validation
		//
		unique_ptr<wstring[]> args(new wstring[ARGS_COUNT]);
		int argsCount = 0;
		int suspectedFile = -1; // allow argument interchangeability
		int qmarkCount = 0;
		bool inQmarks = false;
		for (int i = 0; i < argsLength; i++) {
			if (cmdLine[i] == QMARK) {
				inQmarks = !inQmarks;
				suspectedFile = argsCount;
				if (++qmarkCount > NUM_OF_FILE_ARGS * 2) {				// allow only a max number of quotationmarks
					return log_error(LOG_FILE, ARG_ERROR) ? 1 : 36;
				}
			} else {
				if ((cmdLine[i] == SPACE && inQmarks) || cmdLine[i] != SPACE) {
					args[argsCount] += cmdLine[i];
				} else {
					if (i > 0 && i < argsLength - 1) {
						if (cmdLine[i - 1] == QMARK || cmdLine[i + 1] == QMARK) {
							if (++argsCount > ARGS_COUNT - 1) {
								return log_error(LOG_FILE, ARG_ERROR) ? 1 : 36;
							}
						} else {
							if (qmarkCount == 0) {
								if (++argsCount > ARGS_COUNT - 1) {
									return log_error(LOG_FILE, ARG_ERROR) ? 1 : 36;
								}
							}
						}
					}
				}
			}
		}

		// prevent odd number of quotationmarks, beacuse path can't contain any quotationmark anyway...
		if (qmarkCount % 2 == 1) {
			return log_error(LOG_FILE, ARG_ERROR) ? 1 : 36;
		}

		// check arg count
		if (++argsCount == 1) {
			suspectedFile = 0;
			if (filesystem::exists(args[suspectedFile])) {
				return open_file(args[0].c_str(), L"open", NULL, SW_SHOW, LOG_FILE);
			}
			else {
				return log_error(LOG_FILE, FILE_NOT_EXISTS) ? 9 : 36;
			}
		} else if (argsCount > ARGS_COUNT) {
			return log_error(LOG_FILE, ARG_ERROR) ? 1 : 36;
		}

		cout << argsCount << endl;

		// prevent the usage of non-quotationmarked file and quotationmarked mode ex: lr file "-o"
		if (qmarkCount != 0 && !filesystem::exists(args[suspectedFile])) {
			return log_error(LOG_FILE, ARG_ERROR) ? 1 : 36;
		}
		//
		// check args order
		//
		int argFile = -1;
		int argMode = -1;
		if (order_args(args, argFile, argMode, LOG_FILE) != 1) {
			return log_error(LOG_FILE, INVALID_ARGS) ? 13 : 36;
		}

		if (!is_mode_valid(args[argMode])) {
			return log_error(LOG_FILE, ARG_ERROR) ? 1 : 36;
		}

		// run the file
		return open_file(args[argFile].c_str(), args[argMode].c_str(), NULL, SW_SHOW, LOG_FILE);
	} else if (from == FROM_FILE) {
		filesystem::path configFile = filesystem::current_path();
		configFile += "\\";
		configFile += CONFIG_FILE;
		//
		// check if config file exists
		//
		if (!filesystem::exists(configFile)) {
			cout << "Missing: " << configFile << endl;
			return log_error(LOG_FILE, NO_CONFIG_FILE_FOUND) ? 34 : 36;
		}
		//
		// read args from config file
		//
		unique_ptr<wstring[]> args(new wstring[ARGS_COUNT]);
		int numOfArgs = 0;
		wifstream wif;
		wif.open(configFile);
		if (wif.is_open()) {
			// set UTF-8
			wif.imbue(locale(locale(), new codecvt_utf8<wchar_t>));
			wchar_t *buffer = new wchar_t[READ_BUFFER_SIZE];
			while (wif.getline(buffer, READ_BUFFER_SIZE)) {
				if (numOfArgs > ARGS_COUNT-1) { break; }
				args[numOfArgs++] = buffer;
			}
			delete[] buffer;
		} else {
			return log_error(LOG_FILE, COULD_NOT_OPEN_CONFIG_FILE) ? 35 : 36;
		}
		wif.close();

		// check arg count
		if (numOfArgs == 1 || args[1].compare(L"") == 0) {
			if (filesystem::exists(args[0])) {
				return open_file(args[0].c_str(), L"open", NULL, SW_SHOW, LOG_FILE);
			} else {
				return log_error(LOG_FILE, FILE_NOT_EXISTS) ? 9 : 36;
			}
		}

		//
		// check args order
		//
		int argFile = -1;
		int argMode = -1;
		if (order_args(args, argFile, argMode, LOG_FILE) != 1) {
			return log_error(LOG_FILE, INVALID_ARGS) ? 13 : 36;
		}

		if (!is_mode_valid(args[argMode])) {
			return log_error(LOG_FILE, INVALID_FILE_LAUNCH_MODE) ? 1 : 36;
		}

		return open_file(args[argFile].c_str(), args[argMode].c_str(), NULL, SW_SHOW, LOG_FILE);
	}

	return -1;
}

int log_error(string file, string errorText) {
	cout << errorText;
	if (file.empty()) { return 0; }
	ofstream of;
	of.open(file);
	if (!of.is_open()) { return 0; }
	of << errorText << endl;
	of.close();
	return 1;
}

int wstr_length(wchar_t *str) {
	int i = 0;
	if (str != nullptr) { while (str[i] != '\0') { i++; }; }
	return i;
}

int wstr_length(const wchar_t* str) {
	int i = 0;
	if (str != nullptr) { while (str[i] != '\0') { i++; }; }
	return i;
}

int open_file(const wchar_t *file, const wchar_t *mode, const wchar_t* params, int showCmd, string logFile) {
	HINSTANCE result = ShellExecuteW(NULL, mode, file, params, NULL, showCmd);
	unsigned long long int code = reinterpret_cast<unsigned long long int>(result);
	// handle errors
	if (code <= 32ull) {
		switch (code) {
		case 0:
			return log_error(logFile, SYS_OUT_OF_RESOURCES) ? 7 : 36;
		case ERROR_FILE_NOT_FOUND:
			return log_error(logFile, FILE_NOT_FOUND) ? 2 : 36;
		case ERROR_PATH_NOT_FOUND:
			return log_error(logFile, PATH_NOT_FOUND) ? 3 : 36;
		case SE_ERR_ACCESSDENIED:
			return log_error(logFile, SE_ACCESSDENIED) ? 5 : 36;
		case SE_ERR_OOM:
			return log_error(logFile, SE_OOM) ? 8 : 36;
		case ERROR_BAD_FORMAT:
			return log_error(logFile, BAD_FORMAT) ? 11 : 36;
		case SE_ERR_SHARE:
			return log_error(logFile, SE_SHARE) ? 26 : 36;
		case SE_ERR_ASSOCINCOMPLETE:
			return log_error(logFile, SE_ASSOCINCOMPLETE) ? 27 : 36;
		case SE_ERR_DDETIMEOUT:
			return log_error(logFile, SE_DDETIMEOUT) ? 28 : 36;
		case SE_ERR_DDEFAIL:
			return log_error(logFile, SE_DDEFAIL) ? 29 : 36;
		case SE_ERR_DDEBUSY:
			return log_error(logFile, SE_DDEBUSY) ? 30 : 36;
		case SE_ERR_NOASSOC:
			return log_error(logFile, SE_NOASSOC) ? 31 : 36;
		case SE_ERR_DLLNOTFOUND:
			return log_error(logFile, SE_DLLNOTFOUND) ? 32 : 36;
		default:
			return log_error(logFile, UNKNOWN_ERROR) ? 33 : 36;
		}
	}
	return 0;
}

int is_mode_valid(wstring &mode) {
	if (mode.compare(L"-r") == 0 || mode.compare(L"-runas") == 0) {
		mode = L"runas";
	}
	else if (mode.compare(L"-o") == 0 || mode.compare(L"-open") == 0) {
		mode = L"open";
	}
	else if (mode.compare(L"-e") == 0 || mode.compare(L"-edit") == 0) {
		mode = L"edit";
	}

	if (mode.compare(L"runas") == 0 || mode.compare(L"open") == 0 || mode.compare(L"edit") == 0 ) {
		return 1;
	}

	return 0;
}

int order_args(unique_ptr<wstring[]> &args, int &argFile, int &argMode, string logFile) {
	if (filesystem::exists(args[0])) {
		argFile = 0;
		argMode = 1;
		return 1;
	} else if (filesystem::exists(args[1])) {
		argFile = 1;
		argMode = 0;
		return 1;
	}
	return 0;
}