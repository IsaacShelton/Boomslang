/**
    ============ boot.h ============
    Code for Booting up Boomslang

    This file should NOT be included
    anywhere besides from main.cpp.

    This Header does NOT contain a
    header guard so include with
    cation.
*/

cout << endl;
error_log("Checking for basic errors");

if (arg_count==1 or arg_count==0){
    error_fatal("No file Specified");
    error_suggest("Type 'boomslang -help' for help");
    pend();
    return EXIT_FAILURE;
} else {
    file_read_name = "";
}

#include "dash.h"

if (file_read_name==""){
    error_fatal("No File Specified");

    error_suggest("Ensure the file is specified.");
    pend();
    return EXIT_FAILURE;
} else if (!file_exists(file_read_name)){
    error_fatal("The File '" + file_read_name + "' does not exist");

    error_suggest("Ensure the file exists and that it is surrounded by double quotes.");
    pend();
    return EXIT_FAILURE;
} else if (file_read_name.substr(file_read_name.length()-10,10)!=".boomslang"){
    error_warn("File type isn't '.boomslang'");
}
