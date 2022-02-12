// UAFL (URLAndFileLauncher) interface file 
#ifndef _UAFL_H_API
#define _UAFL_H_API

#include <string>
#include <memory>
#include <tchar.h>

// sets the correct access value for each argument
extern int order_args(std::unique_ptr<std::wstring[]>& args, int& argFileOrURL, int& argMode, std::string logFile);
// checks wether the given mode is valid
extern bool is_mode_valid(std::wstring& mode);
// executes shellexecute on the given arg, which can be a path to a file or a URL, with the given launch mode
extern int open_url_or_file(const wchar_t* arg, const wchar_t* mode, const wchar_t* params, int showCmd, std::string logFile);
// writes errorText to file and also prints it
// if write is successful returns true else false
extern int log_error(std::string file, std::string errorText);
// launch file or URL from args or file
extern int launch_uafl(int launchFrom, const wchar_t* cmdLine, const int argsLength);
// returns the length of a wchar_t array
extern int wstr_length(wchar_t* str);
// returns the length of a const wchar_t array
extern int wstr_length(const wchar_t* str);
// checks if the given wstring is a valid url
extern bool is_valid_url(std::wstring wstr_url);
// converts a 16 bit wstring to a 8 bit string
extern std::string wstr_to_str(std::wstring wstr);
// MiXTools specific functions
// gets the full path of a file in MiXTools's AppData Roaming folder 
extern std::wstring get_app_data_file_path(std::wstring fileName, std::wstring appName, std::wstring appVersion);

namespace UAFLSettings {
	inline constexpr auto ARGS_COUNT = 2; // number of max possible arguments that needs to be processed
	inline constexpr auto NUM_OF_URL_AND_FILE_ARGS = 1; // number of file args used
	inline constexpr auto READ_BUFFER_SIZE = 512; // maybe support for max file char length?
	inline constexpr auto QMARK = 34; // value of '"'
	inline constexpr auto SPACE = 32; // value of ' '
	inline constexpr auto& CONFIG_FILE = "uafl"; // config file
	inline constexpr auto CONFIG_FILE_W = L"uafl";
	inline constexpr auto& LOG_FILE = "uafl_error.txt"; // log file
	inline constexpr auto LOG_FILE_W = L"uafl_error.txt";
	inline constexpr auto LAUNCH_FROM_FILE = 0;
	inline constexpr auto LAUNCH_FROM_ARGS = 1;
	// MiXTools specific settings
	inline constexpr auto APP_NAME = "MiXTools";
	inline constexpr auto APP_NAME_W = L"MiXTools";
	inline constexpr auto APP_VERSION_CODE = "1.0.0";
	inline constexpr auto APP_VERSION_CODE_W = L"1.0.0";

}

namespace UAFLModes {
	inline constexpr auto& RUNAS_MODE = L"runas";
	inline constexpr auto& OPEN_MODE = L"open";
	inline constexpr auto& EDIT_MODE = L"edit";
	inline constexpr auto& _RUNAS_MODE = L"-runas";
	inline constexpr auto& _OPEN_MODE = L"-open";
	inline constexpr auto& _EDIT_MODE = L"-edit";
	inline constexpr auto& R_MODE = L"-r";
	inline constexpr auto& O_MODE = L"-o";
	inline constexpr auto& E_MODE = L"-e";
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
	inline constexpr auto TOO_MANY_QMARKS_ERROR_CODE = 41;
	inline constexpr auto ODD_NUM_OF_QMARKS_ERROR_CODE = 42;
	inline constexpr auto TOO_MANY_ARGS_ERROR_CODE = 43;
	inline constexpr auto NOT_URL_NOR_FILE_ERROR_CODE = 44;
	inline constexpr auto QMARKED_LAUNCH_MODE_ERROR_CODE = 45;
	inline constexpr auto INVALID_ARGS_ERROR_CODE = 46;
	inline constexpr auto INVALID_MODE_ERROR_CODE = 47;
	inline constexpr auto NO_CONFIG_FILE_FOUND_ERROR_CODE = 48;
	inline constexpr auto COULD_NOT_OPEN_CONFIG_FILE_ERROR_CODE = 49;
}

namespace UAFLErrorMessages {
	inline constexpr auto& SYS_OUT_OF_RESOURCES = "System is out of memory or resources!";
	inline constexpr auto& FILE_NOT_FOUND = "File was not found!";
	inline constexpr auto& PATH_NOT_FOUND = "Path was not found!";
	inline constexpr auto& SE_ACCESSDENIED = "File has been deleted or access is denied!";
	inline constexpr auto& SE_OOM = "There was not enough memory to complete the operation!";
	inline constexpr auto& BAD_FORMAT = "Invalid executable file!";
	inline constexpr auto& SE_SHARE = "Sharing violatin occured!";
	inline constexpr auto& SE_ASSOCINCOMPLETE = "File name association is incomplete or invalid!";
	inline constexpr auto& SE_DDETIMEOUT = "DDE transaction could not be completed, bacause the request timed out!";
	inline constexpr auto& SE_DDEFAIL = "DDE transaction failed!";
	inline constexpr auto& SE_DDEBUSY = "DDE transaction could not be completed, because other DDE transactions were being processed!";
	inline constexpr auto& SE_NOASSOC = "There is no application associated with the given file name extension!";
	inline constexpr auto& SE_DLLNOTFOUND = "DDL was not found!";
	inline constexpr auto& UNKNOWN_ERROR = "Unknown error!";
	inline constexpr auto& INVALID_MODE = "Invalid file launch mode!";
	inline constexpr auto& NOT_URL_NOR_FILE = "The given argument is neither a valid URL nor a file!";
	inline constexpr auto& INVALID_ARGS = "Invalid arguments!";
	inline constexpr auto& COULD_NOT_OPEN_CONFIG_FILE = "Couldn't open config file!";
	inline constexpr auto& QMARKED_FILE_LAUNCH_MODE = "Usage of quotation marked launch mode (like \"open\") is not allowed!";
	inline constexpr auto& TOO_MANY_ARGS = "Too many arguments!";
	inline constexpr auto& LAUNCH_MODE_ARG_ERROR =
		"Error: Invalid file launch mode argument!\n"
		"Available modes:\n\n"
		"  -o, (-)open: Opens the given file or URL. (default)\n"
		"  -r, (-)runas: Launches the given application (or through a shortcut) as an Administrator.\n"
		"  -e, (-)edit: Launches an editor and opens the given file for editing.\n";
	inline constexpr auto& ARG_ERROR =
		"Error: Invalid arguments!\n"
		"Correct usage:\n\n"
		"  ( uafl = name of the executable )\n\n"
		"	uafl file_launch_mode \"path_to_file\"\n"
		"	uafl file_launch_mode \"url\"\n"
		"	uafl \"path_to_file\" file_launch_mode\n"
		"	uafl \"url\" file_launch_mode\n"
		"\n"
		"  Using only a single argument defaults to open!\n\n"
		"	uafl \"path_to_file\"\n"
		"	uafl \"url\"\n"
		"	uafl path_to_file\n"
		"	uafl url\n"
		"\n"
		"Available modes:\n\n"
		"  -o, (-)open: Opens the given file or URL. (default)\n"
		"  -r, (-)runas: Launches the given application (or through a shortcut) as an Administrator.\n"
		"  -e, (-)edit: Launches an editor and opens the given file for editing.\n";
	// MixTools specific
	inline constexpr auto& NO_CONFIG_FILE_FOUND = "Config file doesn't exist";
}

#endif