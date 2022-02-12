// UAFL (URLAndFileLauncher) functions implementation file
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <codecvt>
#include <memory>
#include <regex>
#include "uafl_api.h"
// MiXTools
#include <shlobj.h>
#include <initguid.h>
#include <knownfolders.h>

using namespace std;
using namespace UAFLErrorMessages;
using namespace UAFLErrorCodes;
using namespace UAFLSettings;
using namespace UAFLModes;

int launch_uafl(int launchFrom, const wchar_t* cmdLine, const int argsLength) {
	if (launchFrom == LAUNCH_FROM_ARGS) {
		//
		// cmdLine args to array + arg(s) validation
		//
		unique_ptr<wstring[]> args(new wstring[ARGS_COUNT]);
		int argsCount = 0;
		int qmarkCount = 0;
		bool inQmarks = false;
		// allow argument interchangeability
		int suspectedFileOrURL = -1;
		//int suspectedMode = -1;
		for (int i = 0; i < argsLength; i++) {
			if (cmdLine[i] == QMARK) {
				inQmarks = !inQmarks;
				suspectedFileOrURL = argsCount;
				//suspectedMode = suspectedFileOrURL == 0 ? 1 : 0;
				if (++qmarkCount > NUM_OF_URL_AND_FILE_ARGS * 2) {	// allow only a max number of quotationmarks
					return log_error(LOG_FILE, ARG_ERROR) ? TOO_MANY_QMARKS_ERROR_CODE : LOGGING_FAILED_ERROR_CODE;
				}
			} else {
				if ((cmdLine[i] == SPACE && inQmarks) || cmdLine[i] != SPACE) {
					args[argsCount] += cmdLine[i];
				} else {
					if (i > 0 && i < argsLength - 1) {
						if (cmdLine[i - 1] == QMARK || cmdLine[i + 1] == QMARK) {
							if (++argsCount > ARGS_COUNT - 1) {
								return log_error(LOG_FILE, TOO_MANY_ARGS) ? TOO_MANY_ARGS_ERROR_CODE : LOGGING_FAILED_ERROR_CODE;
							}
						} else {
							if (qmarkCount == 0) {
								if (++argsCount > ARGS_COUNT - 1) {
									return log_error(LOG_FILE, TOO_MANY_ARGS) ? TOO_MANY_ARGS_ERROR_CODE : LOGGING_FAILED_ERROR_CODE;
								}
							}
						}
					}
				}
			}
		}

		if (!args[1].empty()) {
			argsCount++;
		}

		/*
		cout << "args count: " << argsCount << endl;
		cout << "arg0: \"" << wstr_to_str(args[0]) << "\"" << endl;
		cout << "arg1: \"" << wstr_to_str(args[1]) << "\"" << endl;
		cout << "is arg1 empty: " << args[1].empty() << endl;
		*/

		// prevent odd number of quotationmarks, beacuse path or URL shouldn't contain any quotationmark anyway...
		if (qmarkCount % 2 == 1) {
			return log_error(LOG_FILE, ARG_ERROR) ? ODD_NUM_OF_QMARKS_ERROR_CODE : LOGGING_FAILED_ERROR_CODE;
		}

		// check arg count
		if (argsCount == 1) {
			suspectedFileOrURL = 0;
			if (filesystem::exists(args[suspectedFileOrURL]) || is_valid_url(args[suspectedFileOrURL])) {
				return open_url_or_file(args[0].c_str(), OPEN_MODE, NULL, SW_SHOW, LOG_FILE);
			} else {
				return log_error(LOG_FILE, NOT_URL_NOR_FILE) ? NOT_URL_NOR_FILE_ERROR_CODE : LOGGING_FAILED_ERROR_CODE;
			}
		} else if (argsCount > ARGS_COUNT) {
			return log_error(LOG_FILE, TOO_MANY_ARGS) ? TOO_MANY_ARGS_ERROR_CODE : LOGGING_FAILED_ERROR_CODE;
		}

		//
		// check args order
		//
		//
		int argFile = -1;
		int argMode = -1;
		int orderRes = order_args(args, argFile, argMode, LOG_FILE);
		if (orderRes == 1) {
			return log_error(LOG_FILE, NOT_URL_NOR_FILE) ? NOT_URL_NOR_FILE_ERROR_CODE : LOGGING_FAILED_ERROR_CODE;
		} else if (orderRes == 2) {
			return log_error(LOG_FILE, INVALID_MODE) ? INVALID_MODE_ERROR_CODE : LOGGING_FAILED_ERROR_CODE;
		}

		// open the file or URL
		return open_url_or_file(args[argFile].c_str(), args[argMode].c_str(), NULL, SW_SHOW, LOG_FILE);
	} else if (launchFrom == LAUNCH_FROM_FILE) {
		// check config file
		wstring configFile = get_app_data_file_path(CONFIG_FILE_W, APP_NAME_W, APP_VERSION_CODE_W);
		if (!filesystem::exists(configFile)) {
			string config_file_path = wstr_to_str(configFile);
			cout << "Missing file: " << config_file_path << endl;
			return log_error(LOG_FILE, NO_CONFIG_FILE_FOUND) ? NO_CONFIG_FILE_FOUND_ERROR_CODE : LOGGING_FAILED_ERROR_CODE;
		}
		//
		// read args from config file
		//
		unique_ptr<wstring[]> args(new wstring[ARGS_COUNT]);
		int numOfArgs = 0;
		wifstream wif;
		wif.open(filesystem::path(configFile));
		if (wif.is_open()) {
			wif.imbue(locale(locale(), new codecvt_utf8<wchar_t>)); // set UTF-8
			wchar_t* buffer = new wchar_t[READ_BUFFER_SIZE];
			while (wif.getline(buffer, READ_BUFFER_SIZE)) {
				if (numOfArgs > ARGS_COUNT - 1) { break; }
				args[numOfArgs++] = buffer;
			}
			delete[] buffer;
		} else {
			return log_error(LOG_FILE, COULD_NOT_OPEN_CONFIG_FILE) ? COULD_NOT_OPEN_CONFIG_FILE_ERROR_CODE : LOGGING_FAILED_ERROR_CODE;
		}
		wif.close();

		// check arg count
		if (numOfArgs == 1 || args[1].compare(L"") == 0) {
			if (filesystem::exists(args[0]) || is_valid_url(args[0])) {
				return open_url_or_file(args[0].c_str(), OPEN_MODE, NULL, SW_SHOW, LOG_FILE);
			} else {
				return log_error(LOG_FILE, NOT_URL_NOR_FILE) ? NOT_URL_NOR_FILE_ERROR_CODE : LOGGING_FAILED_ERROR_CODE;
			}
		}

		//
		// check args order
		//
		int argFile = -1;
		int argMode = -1;
		int orderRes = order_args(args, argFile, argMode, LOG_FILE);
		if (orderRes == 1) {
			return log_error(LOG_FILE, NOT_URL_NOR_FILE) ? NOT_URL_NOR_FILE_ERROR_CODE : LOGGING_FAILED_ERROR_CODE;
		} else if (orderRes == 2) {
			return log_error(LOG_FILE, INVALID_MODE) ? INVALID_MODE_ERROR_CODE : LOGGING_FAILED_ERROR_CODE;
		}

		// open the file or URL
		return open_url_or_file(args[argFile].c_str(), args[argMode].c_str(), NULL, SW_SHOW, LOG_FILE);
	}

	return -1;
}

int log_error(string file, string errorText) {
	cout << errorText;
	if (file.empty()) { return 0; }
	ofstream of;
	of.open(filesystem::path(get_app_data_file_path(LOG_FILE_W, APP_NAME_W, APP_VERSION_CODE_W).c_str()));
	if (!of.is_open()) { return 0; }
	of << errorText << endl;
	of.close();
	return 1;
}

int wstr_length(wchar_t* str) {
	int i = 0;
	if (str != nullptr) { while (str[i] != '\0') { i++; }; }
	return i;
}

int wstr_length(const wchar_t* str) {
	int i = 0;
	if (str != nullptr) { while (str[i] != '\0') { i++; }; }
	return i;
}


bool is_mode_valid(wstring& mode) {
	if (mode.compare(R_MODE) == 0 || mode.compare(_RUNAS_MODE) == 0) {
		mode = RUNAS_MODE;
	} else if (mode.compare(O_MODE) == 0 || mode.compare(_OPEN_MODE) == 0) {
		mode = OPEN_MODE;
	} else if (mode.compare(E_MODE) == 0 || mode.compare(_EDIT_MODE) == 0) {
		mode = EDIT_MODE;
	}
	return mode.compare(RUNAS_MODE) == 0 || mode.compare(OPEN_MODE) == 0 || mode.compare(EDIT_MODE) == 0;
}

int open_url_or_file(const wchar_t* arg, const wchar_t* mode, const wchar_t* params, int showCmd, string logFile) {
	HINSTANCE result = ShellExecuteW(NULL, mode, arg, params, NULL, showCmd);
	unsigned long long int code = reinterpret_cast<unsigned long long int>(result);
	// handle errors
	if (code <= 32ull) {
		switch (code) {
		case 0:
			return log_error(logFile, SYS_OUT_OF_RESOURCES) ? SYS_OUT_OF_RESOURCES_ERROR_CODE : LOGGING_FAILED_ERROR_CODE;
		case ERROR_FILE_NOT_FOUND:
			return log_error(logFile, FILE_NOT_FOUND) ? ERROR_FILE_NOT_FOUND : LOGGING_FAILED_ERROR_CODE;
		case ERROR_PATH_NOT_FOUND:
			return log_error(logFile, PATH_NOT_FOUND) ? ERROR_PATH_NOT_FOUND : LOGGING_FAILED_ERROR_CODE;
		case SE_ERR_ACCESSDENIED:
			return log_error(logFile, SE_ACCESSDENIED) ? SE_ERR_OOM : LOGGING_FAILED_ERROR_CODE;
		case SE_ERR_OOM:
			return log_error(logFile, SE_OOM) ? SE_ERR_OOM : LOGGING_FAILED_ERROR_CODE;
		case ERROR_BAD_FORMAT:
			return log_error(logFile, BAD_FORMAT) ? ERROR_BAD_FORMAT : LOGGING_FAILED_ERROR_CODE;
		case SE_ERR_SHARE:
			return log_error(logFile, SE_SHARE) ? SE_ERR_SHARE : LOGGING_FAILED_ERROR_CODE;
		case SE_ERR_ASSOCINCOMPLETE:
			return log_error(logFile, SE_ASSOCINCOMPLETE) ? SE_ERR_ASSOCINCOMPLETE : LOGGING_FAILED_ERROR_CODE;
		case SE_ERR_DDETIMEOUT:
			return log_error(logFile, SE_DDETIMEOUT) ? SE_ERR_DDETIMEOUT : LOGGING_FAILED_ERROR_CODE;
		case SE_ERR_DDEFAIL:
			return log_error(logFile, SE_DDEFAIL) ? SE_ERR_DDEFAIL : LOGGING_FAILED_ERROR_CODE;
		case SE_ERR_DDEBUSY:
			return log_error(logFile, SE_DDEBUSY) ? SE_ERR_DDEBUSY : LOGGING_FAILED_ERROR_CODE;
		case SE_ERR_NOASSOC:
			return log_error(logFile, SE_NOASSOC) ? SE_ERR_NOASSOC : LOGGING_FAILED_ERROR_CODE;
		case SE_ERR_DLLNOTFOUND:
			return log_error(logFile, SE_DLLNOTFOUND) ? SE_ERR_DLLNOTFOUND : LOGGING_FAILED_ERROR_CODE;
		default:
			return log_error(logFile, UNKNOWN_ERROR) ? UNKNOWN_ERROR_ERROR_CODE : LOGGING_FAILED_ERROR_CODE;
		}
	}
	return 0;
}

bool is_valid_url(wstring wstr_url) {
	if (wstr_url.empty()) { return 0; }
	// thanks for @diegoperini
	string pattern = "^(?:(?:https?|ftp):\\/\\/)(?:\\S+@)?(?:(?!10(?:\\.\\d{1,3}){3})(?!127(?:\\.\\d{1,3}){3})(?!169\\.254(?:\\.\\d{1,3}){2})(?!192\\.168(?:\\.\\d{1,3}){2})(?!172\\.(?:1[6-9]|2\\d|3[0-1])(?:\\.\\d{1,3}){2})(?:[1-9]\\d?|1\\d\\d|2[01]\\d|22[0-3])(?:\\.(?:1?\\d{1,2}|2[0-4]\\d|25[0-5])){2}(?:\\.(?:[1-9]\\d?|1\\d\\d|2[0-4]\\d|25[0-4]))|(?:[a-z\u00a1-\uffff0-9]+-)*[a-z\u00a1-\uffff0-9]+(?:\\.(?:[a-z\u00a1-\uffff0-9]+-)*[a-z\u00a1-\uffff0-9]+)*(?:\\.(?:[a-z\u00a1-\uffff]{2,})))(?::\\d{2,5})?(?:\\/\\S*)?$";
	// construct regex object
	regex url_regex(pattern);
	string url = wstr_to_str(wstr_url);
	bool res = regex_match(url, url_regex);
	return res;
}

string wstr_to_str(wstring wstr) {
	return string(wstr.begin(), wstr.end());
}

int order_args(unique_ptr<wstring[]>& args, int& argFile, int& argMode, string logFile) {
	if (filesystem::exists(args[0]) || is_valid_url(args[0])) {
		argFile = 0;
		argMode = 1;
		if (!is_mode_valid(args[argMode])) { return 2; }
		return 0;
	} else if (filesystem::exists(args[1]) || is_valid_url(args[1])) {
		argFile = 1;
		argMode = 0;
		if (!is_mode_valid(args[argMode])) { return 2; }
		return 0;
	}
	return 1;
}

wstring get_app_data_file_path(wstring fileName, wstring appName, wstring appVersion) {
	wchar_t* appDataPath = nullptr;
	SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, 0, &appDataPath);
	wstringstream path_ss;
	path_ss << appDataPath << "\\" << appName << "\\" << appName << "\\" << appVersion << "\\" << fileName;
	SHFree(static_cast<void*>(appDataPath));
	return path_ss.str();
}