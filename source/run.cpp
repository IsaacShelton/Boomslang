
#include <string>
#include "../include/run.h"

#ifdef __WIN32__
#include <windows.h>
#endif // __WIN32__

using namespace std;

int execute_silent(string exe, string params){
    //Run a program silently

    #if defined(__WIN32__)
	STARTUPINFO StartupInfo;
	PROCESS_INFORMATION ProcessInfo;
	char args[4096];
	char* pEnvCMD = NULL;
	char* pDefaultCMD = "CMD.EXE";
	ULONG rc;

	memset(&StartupInfo, 0, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(STARTUPINFO);
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow = SW_HIDE;

	args[0] = 0;

	pEnvCMD = getenv("COMSPEC");

	if(pEnvCMD){
		strcpy(args, pEnvCMD);
	}
	else{
		strcpy(args, pDefaultCMD);
	}

	// "/c" option - Do the command then terminate the command window
	strcat(args, " /c ");
	//the application you would like to run from the command window
	strcat(args, exe.c_str());
	strcat(args, " ");
	//the parameters passed to the application being run from the command window.
	strcat(args, params.c_str());

	if (!CreateProcess( NULL, args, NULL, NULL, FALSE,CREATE_NEW_CONSOLE,NULL,NULL,&StartupInfo,&ProcessInfo)){
		return GetLastError();
		delete[] pEnvCMD;
		delete[] pDefaultCMD;
		return 1;
	}

	WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
	if(!GetExitCodeProcess(ProcessInfo.hProcess, &rc)) rc = 0;

	CloseHandle(ProcessInfo.hThread);
	CloseHandle(ProcessInfo.hProcess);
    #endif // __WIN32__

    return rc;
}
