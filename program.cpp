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
*			g++ -Wall -municode -mwindows uafl_api.cpp program.cpp -o uafl-debug.exe debug_res.o
* 
*		production:
*			
*			windres prod.rc prod_res.o
*			g++ -Wall -O2 -municode -mwindows uafl_api.cpp program.cpp -o uafl.exe prod_res.o
*	
* 
* or use the .cmd files for compiling.
*
*/

// win app entry point
// compile with -mwindows to completely hide terminal
// -municode needed to compile wWinMain
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {
	// terminate non-null terminated cmd arguments
	lpCmdLine += '\0';
	const int argsLength = wstr_length(lpCmdLine);
	if (argsLength == 0) {
		return launch_uafl(LAUNCH_FROM_FILE, nullptr, argsLength);
	} else {
		return launch_uafl(LAUNCH_FROM_ARGS, lpCmdLine, argsLength);
	}
	return 0;
}
