/**
    ============ boot.h ============
    Code for Booting up Boomslang

    This file should NOT be included
    anywhere besides from main.cpp.

    This Header does NOT contain a
    header guard so include with
    cation.
*/

#if (BUILD_OS == LINUX)
USERNAME = getlogin();
string file_write_name = "/home/" + USERNAME + "/Boomslang/source/prenative.cpp";
#endif

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
    error_fatal("No file Specified");
    error_suggest("Type 'boomslang -help' for help");
    pend();
    return EXIT_FAILURE;
} else {
    //Filename will be figured out later
    file_read_name = "";
}

#include "../Parsing/dash.h"

//Add Newline
if(!runafter)
    cout << endl;

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
