/**
    ============ assemble.h ============
    Code to Assemble the Final Executable

    This file should NOT be included
    anywhere besides from init.h at
    the assembling stage of compiling.

    This Header does NOT contain a
    header guard so include with
    cation.
*/

using namespace std;

if (ve_apptype == APPTYPE_WINDOWS){

    //Compile without console unless specified
    if (!console){
        compile_flags += "-mwindows ";
    }

    //Remove old executable
    if(file_exists(filename_path(file_read_name) + filename_change_ext(filename_name(file_read_name),"exe"))){
        if(remove((filename_path(file_read_name) + filename_change_ext(filename_name(file_read_name),"exe")).c_str())!=0){
        error_fatal("Failed to delete output file.");
        pend();
        return EXIT_FAILURE;
        }
    }

    //Run MinGW
    execute_silent("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\resources\\MinGW\\bin\\g++","\"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\native.cpp\" " + compile_flags + " -o \"" + filename_path(file_read_name) + filename_change_ext(filename_name(file_read_name),"exe") + "\" 2>\"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\logs\\native_errors.log\"");

    //Check if MinGW created an executable
    if (!file_exists(filename_path(file_read_name) + filename_change_ext(filename_name(file_read_name),"exe"))){
        error_fatal("Native compile error");
        error_suggest("Details at C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\logs\\native.log");
        pend();
        return EXIT_FAILURE;
    }
}

if (ve_apptype == APPTYPE_ANDROID){
    error_log("Sorry, Android is not supported yet.");
    errors += 1;
    pend();
    return EXIT_FAILURE;
}

if (ve_apptype == APPTYPE_LINUX){
    error_log("Sorry, Linux is not supported yet.");
    errors += 1;
    pend();
    return EXIT_FAILURE;
}
