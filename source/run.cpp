
/*
    (c) 2016 Isaac Shelton

    This file is part of Boomslang.

    Boomslang is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Boomslang is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Boomslang. If not, see <http://www.gnu.org/licenses/>.
*/

#include <string>
#include "../include/run.h"

#ifdef __WIN32__
#include <windows.h>
#endif // __WIN32__

#ifdef __linux__
#include <stdlib.h>
#endif // __linux__

using namespace std;

int execute_silent(string exe, string params){
    //Run a program silently

    #if defined(__WIN32__)
	STARTUPINFO StartupInfo;
	PROCESS_INFORMATION ProcessInfo;
	ULONG rc;

    const char* pDefaultCMD = "CMD.EXE";
	char args[4096];
	char* pEnvCMD = NULL;

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
	
	return rc;
    #endif // __WIN32__
    
    #ifdef __linux__
    	return system( (exe + " " + params).c_str() );
    #endif // __linux__
}
