#include <windows.h>
#include "uafl_api.h"

using namespace std;
using namespace UAFLSettings;

/**
*
* Preffered compile commands:
*
*		debug:
* 
*			windres debug.rc debug_res.o
*			g++ -Wall -municode -std=c++23 uafl_api.cpp program.cpp -o uafl-debug.exe debug_res.o
* 
*		production:
*			
*			windres prod.rc prod_res.o
*			g++ -Wall -O2 -municode -mwindows -std=c++23 uafl_api.cpp program.cpp -o uafl.exe prod_res.o
*	
* 
* or use the .cmd files for compiling.
*
*/

// compile with -mwindows to completely hide terminal
// -municode needed to compile wWinMain

// win app entry point
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {
	wstring cmdLine = wstring(lpCmdLine);
	if (cmdLine.empty()) { // same as size == 0
		return launch_uafl(LAUNCH_FROM_FILE, wstring()); // pass a "null" wstring_view
	} else {
		return launch_uafl(LAUNCH_FROM_ARGS, cmdLine);
	}
	return 0;
}
