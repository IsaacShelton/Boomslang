
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
#include <iostream>
#include <direct.h>
#include "../include/globals.h"

using namespace std;

//Imports boomslang package
void import_boomslang(string package_identifier){
    string package_filename;
    string name = package_identifier;

    if (!ve_packages.exists(package_identifier)){
        if (string_contains(name,".")){
            while(string_contains(name,".")){
                package_filename += string_get_until(name,".") + "\\";
                name = string_delete_until(name,".");
                name = string_delete_amount(name,1);
            }
        }

        package_filename += name + "\\";

        package_filename = package_filename + name + ".boomslang";

        if (file_exists("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\packages\\" + package_filename)){
            package_filename ="C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\packages\\" + package_filename;

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
            ve_packages.add(package_identifier);
        } else if (file_exists(terminal_path + package_filename)){
            package_filename = terminal_path + package_filename;

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
            ve_packages.add(package_identifier);
        } else {
            error_show("Unknown Package '" + package_identifier + "'");
        }
    }
}

//Runs a program silently
int execute_silent(string strFunct, string strParams){
    #if (BUILD_OS == WINDOWS)
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
	strcat(args, strFunct.c_str());
	strcat(args, " ");
	//the parameters passed to the application being run from the command window.
	strcat(args, strParams.c_str());

	if (!CreateProcess( NULL, args, NULL, NULL, FALSE,CREATE_NEW_CONSOLE,NULL,NULL,&StartupInfo,&ProcessInfo)){
		//return GetLastError();
		delete[] pEnvCMD;
		delete[] pDefaultCMD;
		return 1;
	}

	WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
	if(!GetExitCodeProcess(ProcessInfo.hProcess, &rc)) rc = 0;

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

int file_create_path(const string& filename){
    string path = string_replace_all(filename,"\\","/");
    size_t pre = 0, pos;
    string folder;
    int error;

    if(path[path.size()-1]!='/'){
        // force trailing / so we can handle everything in loop
        path+='/';
    }

    while( (pos = path.find_first_of('/',pre))!=string::npos ){
        folder = path.substr(0,pos++);
        pre = pos;
        if(folder.size()==0) continue;

        if( (error = mkdir(folder.c_str())) && errno!=EEXIST ){
            return error;
        }
    }
    return error;
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
