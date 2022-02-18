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
#include <vector>
#include "uafl_api.h"

using namespace std;
using namespace UAFLErrorMessages;
using namespace UAFLErrorCodes;
using namespace UAFLSettings;
using namespace UAFLModes;

int launch_uafl(int launchFrom, wstring cmdLine) {
	vector<wstring> args = launchFrom == LAUNCH_FROM_ARGS ? split_str(cmdLine, SPACE, true) : vector<wstring>();

	if (launchFrom == LAUNCH_FROM_FILE) {
		// setup config file path
		filesystem::path configFile = filesystem::current_path();
		configFile += "\\";
		configFile += CONFIG_FILE;

		// check if config file exists
		if (!filesystem::exists(configFile)) {
			cout << "Missing: " << configFile << endl;
			return log_error(LOG_FILE, NO_CONFIG_FILE_FOUND) ? NO_CONFIG_FILE_FOUND_ERROR_CODE : LOGGING_FAILED_ERROR_CODE;
		}
		
		// read args from config file
		wifstream wif;
		wif.open(configFile);
		if (wif.is_open()) {
			wif.imbue(locale(wif.getloc(), new codecvt_utf8<wchar_t>)); // set UTF-8
			wchar_t* buffer = new wchar_t[READ_BUFFER_SIZE];
			while (wif.getline(buffer, READ_BUFFER_SIZE)) {
				if (args.size() == ARGS_COUNT) { break; }
				args.push_back(buffer);
			}
			delete[] buffer;
			wif.close();
		} else {
			wif.close();
			return log_error(LOG_FILE, COULD_NOT_OPEN_CONFIG_FILE) ? COULD_NOT_OPEN_CONFIG_FILE_ERROR_CODE : LOGGING_FAILED_ERROR_CODE;
		}
	}

	if (args.size() == 1 || args[1].compare(L"") == 0) {
		if (filesystem::exists(args[0]) || is_valid_url(args[0])) {
			return open_url_or_file(args[0].c_str(), OPEN_MODE.data(), wstring().c_str(), SW_SHOW, LOG_FILE);
		} else {
			return log_error(LOG_FILE, NO_URL_NOR_FILE_IN_ARGS) ? NO_URL_NOR_FILE_IN_ARGS_ERROR_CODE : LOGGING_FAILED_ERROR_CODE;
		}
	} else if (args.size() == 2) {
		// check args order
		int argFile = -1;
		int argMode = -1;
		int orderRes = order_args(args, argFile, argMode);
		if (orderRes == 1) {
			return log_error(LOG_FILE, NO_URL_NOR_FILE_IN_ARGS) ? NO_URL_NOR_FILE_IN_ARGS_ERROR_CODE : LOGGING_FAILED_ERROR_CODE;
		} else if (orderRes == 2) {
			return log_error(LOG_FILE, INVALID_LAUNCH_MODE) ? INVALID_LAUNCH_MODE_ERROR_CODE : LOGGING_FAILED_ERROR_CODE;
		}
		// open the file or URL
		return open_url_or_file(args[argFile].c_str(), args[argMode].c_str(), wstring().c_str(), SW_SHOW, LOG_FILE);
	} else {
		return log_error(LOG_FILE, TOO_MANY_ARGS_ERROR) ? TOO_MANY_ARGS_ERROR_CODE : LOGGING_FAILED_ERROR_CODE;
	}
}

int log_error(string_view file, string_view errorText) {
	cout << errorText;
	if (file.empty()) { return 0; }
	ofstream of;
	of.open(filesystem::path(file));
	if (!of.is_open()) { return 0; }
	of << errorText << endl;
	of.close();
	return 1;
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

int open_url_or_file(const wchar_t* fileOrURL, const wchar_t* mode, const wchar_t* params, int showCmd, string_view logFile) {
	HINSTANCE result = ShellExecuteW(NULL, mode, fileOrURL, params, NULL, showCmd);
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


bool is_valid_url(wstring url) {
	if (url.empty()) { return 0; }
	// thanks @diegoperini for the URL regex pattern
	wregex url_regex(L"^(?:(?:https?|ftp):\\/\\/)(?:\\S+@)?(?:(?!10(?:\\.\\d{1,3}){3})(?!127(?:\\.\\d{1,3}){3})(?!169\\.254(?:\\.\\d{1,3}){2})(?!192\\.168(?:\\.\\d{1,3}){2})(?!172\\.(?:1[6-9]|2\\d|3[0-1])(?:\\.\\d{1,3}){2})(?:[1-9]\\d?|1\\d\\d|2[01]\\d|22[0-3])(?:\\.(?:1?\\d{1,2}|2[0-4]\\d|25[0-5])){2}(?:\\.(?:[1-9]\\d?|1\\d\\d|2[0-4]\\d|25[0-4]))|(?:[a-z\u00a1-\uffff0-9]+-)*[a-z\u00a1-\uffff0-9]+(?:\\.(?:[a-z\u00a1-\uffff0-9]+-)*[a-z\u00a1-\uffff0-9]+)*(?:\\.(?:[a-z\u00a1-\uffff]{2,})))(?::\\d{2,5})?(?:\\/\\S*)?$");
	return regex_match(url, url_regex);
}

int order_args(vector<wstring> args, int& argFile, int& argMode) {
	if (args.size() != ARGS_COUNT) { return 3; }
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

vector<wstring> split_str(wstring str, wchar_t separator, bool checkBetweenQMarks = false) {
	vector<wstring> result = vector<wstring>();
	if (str.empty()) { return result; }
	bool inQMarks = false;
	wstring tmp = wstring();
	for (auto c : str) {
		if (c == QMARK && separator != QMARK) {
			inQMarks = checkBetweenQMarks ? !inQMarks : false;
		} else if (c == separator) {
			if (checkBetweenQMarks && inQMarks) {
				tmp += c;
			} else {
				if (!tmp.empty()) {
					result.push_back(tmp);
					tmp.clear();
				}
			}
		} else {
			tmp += c;
		}
	}

	if (!tmp.empty()) {
		result.push_back(tmp);
	}
	
	return result;
}