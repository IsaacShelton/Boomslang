/**
    ============ file.h ============
    Code for File Handling Functions

    This file should NOT be included
    anywhere besides from main.cpp.

    This Header does NOT contain a
    header guard so include with
    cation.
*/

using namespace std;

void import_boomslang(string imported_package){
    string name;
    string package;
    string last;
    string new_code;
    ifstream file;

    if (!ve_packages.exists(imported_package)){
        name = imported_package;
        package = "C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\packages\\";
        last = "";
        new_code = "";

        if (string_contains(name,".")){
            while(string_contains(name,".")){
                package += string_get_until(name,".") + "\\";
                name = string_delete_until(name,".");
                name = string_delete_amount(name,1);
            }
        }

        package += name + "\\";

        //name = console in library._console_

        if (file_exists(package + name + ".boomslang")){
            ve_packages.add(imported_package);
            string filename = package + name + ".boomslang";
            file.open(filename.c_str());

            string line;

            if (file.is_open()){
                while(getline(file,line)){
                    new_code += line + "\n";
                }
            } else {
                error_show("Failed to Import Package '" + imported_package + "'");
            }

            file.close();
            compile_code = new_code + compile_code;
        } else {
            error_show("Unknown Package '" + imported_package + "'");
        }
    }
}

int execute_silent(string strFunct, string strParams){
	STARTUPINFO StartupInfo;
	PROCESS_INFORMATION ProcessInfo;
	char Args[4096];
	char *pEnvCMD = NULL;
	char *pDefaultCMD = (char*)"CMD.EXE";
	ULONG rc;

	memset(&StartupInfo, 0, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(STARTUPINFO);
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow = SW_HIDE;

	Args[0] = 0;

	pEnvCMD = getenv("COMSPEC");

	if(pEnvCMD){

		strcpy(Args, pEnvCMD);
	}
	else{
		strcpy(Args, pDefaultCMD);
	}

	// "/c" option - Do the command then terminate the command window
	strcat(Args, " /c ");
	//the application you would like to run from the command window
	strcat(Args, strFunct.c_str());
	strcat(Args, " ");
	//the parameters passed to the application being run from the command window.
	strcat(Args, strParams.c_str());

	if (!CreateProcess( NULL, Args, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&StartupInfo,
		&ProcessInfo))
	{
		//return GetLastError();
		return 1;
	}

	WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
	if(!GetExitCodeProcess(ProcessInfo.hProcess, &rc))
		rc = 0;

	CloseHandle(ProcessInfo.hThread);
	CloseHandle(ProcessInfo.hProcess);

	//return rc;
    return EXIT_SUCCESS;
}

string filename_name(string a){
    return string_delete_amount(a.substr(a.find_last_of("\\"), a.length() - a.find_last_of("\\")),1);
}

string filename_path(string a){
    return a.substr(0, a.find_last_of("\\")) + "\\";
}

string filename_change_ext(string filename, string ext_without_dot){
    string file_no_ext = string_get_until(filename,".");
    return file_no_ext + "." + ext_without_dot;
}

void write(string code, bool write_to_main){
    if(write_to_main)
        ve_main_code += code;
    else
        file_write << code;
}
