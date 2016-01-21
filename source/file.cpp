
#include <string>
#include "../include/globals.h"

using namespace std;

//Imports boomslang package
void import_boomslang(string name){
    string package_filename;

    if (!ve_packages.exists(name)){
        package_filename = "C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\packages\\";

        if (string_contains(name,".")){
            while(string_contains(name,".")){
                package_filename += string_get_until(name,".") + "\\";
                name = string_delete_until(name,".");
                name = string_delete_amount(name,1);
            }
        }

        package_filename += name + "\\";

        package_filename = package_filename + name + ".boomslang";

        if (file_exists(package_filename)){
            ve_packages.add(name);

            ifstream importFile;
            importFile.open(package_filename.c_str());

            string line;
            string new_code = "";

            if(importFile){
                while(getline(importFile,line)){
                    new_code += line + "\n";
                }
            } else {
                error_show("Failed to Import Package '" + name + "'");
            }

            compile_code = new_code + compile_code;
            importFile.close();
        } else {
            error_show("Unknown Package '" + name + "'");
        }
    }
}

//Runs a program silently
int execute_silent(string strFunct, string strParams){
    #if (BUILD_OS == WINDOWS)
	STARTUPINFO StartupInfo;
	PROCESS_INFORMATION ProcessInfo;
	char Args[4096];
	char *pEnvCMD = NULL;
	char *pDefaultCMD = "CMD.EXE";
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
    #endif

	//return rc;
    return EXIT_SUCCESS;
}

//Gets the name of the file from path and filename
string filename_name(string a){
    if (a.find_last_of("\\")==string::npos){
        return a;
    } else {
        return string_delete_amount(a.substr(a.find_last_of("\\"), a.length() - a.find_last_of("\\")),1);
    }
}

//Gets the path of the file from path and filename
string filename_path(string a){
    if (a.find_last_of("\\")==string::npos){
        return a;
    } else {
        return a.substr(0, a.find_last_of("\\")) + "\\";
    }
}

//Gets the new filename from filename and new extension
string filename_change_ext(string filename, string ext_without_dot){
    string file_no_ext = string_get_until(filename,".");
    return file_no_ext + "." + ext_without_dot;
}

ifstream::pos_type file_size(string size_filename){
    std::ifstream file_stream(size_filename.c_str(), std::ifstream::ate | std::ifstream::binary);
    unsigned int size_of_file = file_stream.tellg();
    file_stream.close();
    return size_of_file;
}

//Writes to ve_main_code or straight to file
void write(string code, int write_to_main){
    if(write_to_main==1)
        ve_main_code += code;
    else if(write_to_main==false)
        file_write << code;
}
