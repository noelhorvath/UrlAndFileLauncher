// UAFL (URLAndFileLauncher) interface file 
#ifndef _UAFL_H_API
#define _UAFL_H_API

#include <string>
#include <vector>

// sets the correct access value for each argument
extern int order_args(std::vector<std::wstring>& args, int& argFile, int& argMode);
// checks wether the given mode is valid
extern bool is_mode_valid(std::wstring& mode);
// executes shellexecute on the given arg, which can be a path to a file or a URL, with the given launch mode
extern int open_url_or_file(const wchar_t* arg, const wchar_t* mode, const wchar_t* params, int showCmd, const std::string_view logFile);
// writes errorText to file and also prints it
// if write is successful returns true else false
extern int log_error(std::string_view file, std::string_view errorText);
// launch file or URL from args or file
extern int launch_uafl(int launchFrom, std::wstring cmdLine);
// checks if the given wstring is a valid url
extern bool is_valid_url(std::wstring url);
// splits a wstring_view by the spparator char and returns the result as an unique_ptr<wstring_view[]>
extern std::vector<std::wstring> split_str(std::wstring str, wchar_t separator, bool checkBetweenQMarks);
//
// MiXTools specific functions
//
// gets the full path of a file in MiXTools's AppData Roaming folder 
extern std::wstring get_app_data_file_path(std::wstring_view fileName, std::wstring_view appName, std::wstring_view appVersion);

namespace UAFLSettings {
	inline constexpr auto ARGS_COUNT = 2; // number of max possible arguments that needs to be processed
	inline constexpr auto NUM_OF_URL_AND_FILE_ARGS = 1; // number of file args used
	inline constexpr auto READ_BUFFER_SIZE = 512; // maybe support for max file char length?
	inline constexpr auto QMARK = 34; // value of '"'
	inline constexpr auto SPACE = 32; // value of ' '
	inline constexpr std::string_view CONFIG_FILE = "uafl"; // config file
	inline constexpr std::wstring_view CONFIG_FILE_W = L"uafl";
	inline constexpr std::string_view LOG_FILE = "uafl_error.txt"; // log file
	inline constexpr std::wstring_view LOG_FILE_W = L"uafl_error.txt";
	inline constexpr auto LAUNCH_FROM_FILE = 0;
	inline constexpr auto LAUNCH_FROM_ARGS = 1;
	// MiXTools specific settings
	inline constexpr std::string_view APP_NAME = "MiXTools";
	inline constexpr std::wstring_view APP_NAME_W = L"MiXTools";
	inline constexpr std::string_view APP_VERSION_CODE = "1.0.0";
	inline constexpr std::wstring_view APP_VERSION_CODE_W = L"1.0.0";

}

namespace UAFLModes {
	inline constexpr std::wstring_view RUNAS_MODE = L"runas";
	inline constexpr std::wstring_view OPEN_MODE = L"open";
	inline constexpr std::wstring_view EDIT_MODE = L"edit";
	inline constexpr std::wstring_view _RUNAS_MODE = L"-runas";
	inline constexpr std::wstring_view _OPEN_MODE = L"-open";
	inline constexpr std::wstring_view _EDIT_MODE = L"-edit";
	inline constexpr std::wstring_view R_MODE = L"-r";
	inline constexpr std::wstring_view O_MODE = L"-o";
	inline constexpr std::wstring_view E_MODE = L"-e";
}

namespace UAFLErrorCodes {
	inline constexpr auto FILE_NOT_FOUND_ERROR_CODE = 2;
	inline constexpr auto PATH_NOT_FOUND_ERROR_CODE = 3;
	inline constexpr auto SE_ACCESSDENIED_ERROR_CODE = 5;
	inline constexpr auto SYS_OUT_OF_RESOURCES_ERROR_CODE = 7;
	inline constexpr auto SE_OOM_ERROR_CODE = 8;
	inline constexpr auto BAD_FORMAT_ERROR_CODE = 11;
	inline constexpr auto SE_SHARE_ERROR_CODE = 26;
	inline constexpr auto SE_ASSOCINCOMPLETE_ERROR_CODE = 27;
	inline constexpr auto SE_DDETIMEOUT_ERROR_CODE = 28;
	inline constexpr auto SE_DDEFAIL_ERROR_CODE = 29;
	inline constexpr auto SE_DDEBUSY_ERROR_CODE = 30;
	inline constexpr auto SE_NOASSOC_ERROR_CODE = 31;
	inline constexpr auto SE_DLLNOTFOUND_ERROR_CODE = 32;
	inline constexpr auto UNKNOWN_ERROR_ERROR_CODE = 33;
	inline constexpr auto LOGGING_FAILED_ERROR_CODE = 36;
	inline constexpr auto NO_URL_NOR_FILE_IN_ARGS_ERROR_CODE = 40;
	inline constexpr auto TOO_MANY_ARGS_ERROR_CODE = 41;
	inline constexpr auto INVALID_LAUNCH_MODE_ERROR_CODE = 42;
	inline constexpr auto NO_CONFIG_FILE_FOUND_ERROR_CODE = 43;
	inline constexpr auto COULD_NOT_OPEN_CONFIG_FILE_ERROR_CODE = 44;
}

namespace UAFLErrorMessages {
	inline constexpr std::string_view SYS_OUT_OF_RESOURCES = "System is out of memory or resources!";
	inline constexpr std::string_view FILE_NOT_FOUND = "File was not found!";
	inline constexpr std::string_view PATH_NOT_FOUND = "Path was not found!";
	inline constexpr std::string_view SE_ACCESSDENIED = "File has been deleted or access is denied!";
	inline constexpr std::string_view SE_OOM = "There was not enough memory to complete the operation!";
	inline constexpr std::string_view BAD_FORMAT = "Invalid executable file!";
	inline constexpr std::string_view SE_SHARE = "Sharing violatin occured!";
	inline constexpr std::string_view SE_ASSOCINCOMPLETE = "File name association is incomplete or invalid!";
	inline constexpr std::string_view SE_DDETIMEOUT = "DDE transaction could not be completed, bacause the request timed out!";
	inline constexpr std::string_view SE_DDEFAIL = "DDE transaction failed!";
	inline constexpr std::string_view SE_DDEBUSY = "DDE transaction could not be completed, because other DDE transactions were being processed!";
	inline constexpr std::string_view SE_NOASSOC = "There is no application associated with the given file name extension!";
	inline constexpr std::string_view SE_DLLNOTFOUND = "DDL was not found!";
	inline constexpr std::string_view UNKNOWN_ERROR = "Unknown error!";
	inline constexpr std::string_view NO_URL_NOR_FILE_IN_ARGS = "No valid URL nor file path found in the given arguments!";
	inline constexpr std::string_view TOO_MANY_ARGS_ERROR = "Too many arguments!";
	inline constexpr std::string_view INVALID_LAUNCH_MODE = "Invalid launch mode!";
	inline constexpr std::string_view COULD_NOT_OPEN_CONFIG_FILE = "Couldn't open config file!";
	inline constexpr std::string_view NO_CONFIG_FILE_FOUND = "Config file named \"uafl\" doesn't exist in the executable's folder";
}

#endif