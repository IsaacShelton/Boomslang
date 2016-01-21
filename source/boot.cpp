
#include <stdlib.h>
#include <iostream>
#include <string>
#include "../include/globals.h"
#include "../include/management.h"
#include "../include/file.h"

using namespace std;

Container::Container(){
    data = ";";
}
bool Container::exists(string what){
    if(string_contains(data,";" + what + ";") and what!=""){
        return true;
    } else {
        return false;
    }
}
void Container::add(string what){
    data += what + ";";
}

int boot(int arg_count, char** arg){
    #if (BUILD_OS == WINDOWS)
    ve_apptype = APPTYPE_WINDOWS;
    #elif (BUILD_OS == LINUX)
    ve_apptype = APPTYPE_LINUX;
    #endif

    #if (BUILD_OS == WINDOWS)
    USERNAME = getenv("USERNAME");
    #elif (BUILD_OS == LINUX)
    USERNAME = getlogin();
    #endif

    console = false;
    indentation = 0;
    errors = 0;
    warnings = 0;
    ve_scope = "BOOMSLANG";
    next_unique_template = 0;
    next_embedded_id = 0;
    next_method_id = 0;
    waitafter = false;
    runafter = false;
    compile_flags = "-std=c++11";
    ve_context = S_NULL;
    compile_code = "";
    file_write_name = "C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\first native.cpp";

    //Get current full path
    #ifndef DEBUG
    _fullpath(terminal_complete_path, arg[0], sizeof(terminal_complete_path));
    #endif

    //Remove Filename from full path
    #if (BUILD_OS == WINDOWS)
    terminal_path = filename_path(terminal_complete_path);
    #elif (BUILD_OS == LINUX)
    terminal_path = linux_filename_path(terminal_complete_path);
    #endif

    //Check the argument count
    if (arg_count==1 or arg_count==0){
        //No File was Specified
        cout << endl;
        cout << "boomslang <filename> [options]" << endl << endl;
        cout << " -help     | help" << endl;
        cout << " -version  | version info" << endl;
        cout << " -console  | uses the console" << endl;
        cout << " -wait     | wait after complete" << endl;
        cout << " -run      | run after compiled" << endl;
        cout << " -windows  | compile for windows" << endl;
        cout << " -linux    | compile for linux" << endl;
        cout << " -android  | compile for android" << endl;
        return EXIT_FAILURE;
    } else {
        //Filename will be figured out later
        file_read_name = "";
    }

    #include "../include/dash.h"

    //Add Newline
    if(!runafter) cout << endl;

    if(!runafter)
        error_log("Checking for basic errors");

    //Ensure Input Files Exists
    if (file_read_name==""){
        //None was Specified
        error_fatal("No File Specified");

        error_suggest("Ensure the file is specified.");
        pend();
        return EXIT_FAILURE;
    } else if (!file_exists(file_read_name) and !file_exists(terminal_path + file_read_name)){
        //It does not exist
        error_fatal("The File '" + file_read_name + "' does not exist");

        error_suggest("Ensure the file exists and that it is surrounded by double quotes.");
        pend();
        return EXIT_FAILURE;
    } else if (file_read_name.substr(file_read_name.length()-10,10)!=".boomslang"){
        //Does not have the file extension '.boomslang' do warn user
        error_warn("File type isn't '.boomslang'");
    }

    //Determine if the file read path is local
    if(file_exists(terminal_path + file_read_name)){
        file_read_name = terminal_path + file_read_name;
    }
}
