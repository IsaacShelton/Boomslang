
#include <iostream>
#include "../include/file.h"
#include "../include/globals.h"

int assemble(){
    if (ve_apptype == APPTYPE_WINDOWS){
        #if (BUILD_OS == WINDOWS)
        //Compile without console unless specified
        if (!console){
            linker_flags += "-Wl,--subsystem,windows ";
        }

        //Remove old executable
        if(file_exists(filename_path(file_read_name) + filename_change_ext(filename_name(file_read_name),"exe"))){
            if(remove((filename_path(file_read_name) + filename_change_ext(filename_name(file_read_name),"exe")).c_str())!=0){
                error_fatal("Failed to delete output file.");
                pend();
                return EXIT_FAILURE;
            }
        }

        if(file_exists( ("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\final native.o") )){
            if(remove( ("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\final native.o").c_str() )!=0){
                error_fatal("Failed to delete object file.");
                pend();
                return EXIT_FAILURE;
            }
        }

        //Run MinGW
        execute_silent("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\resources\\MinGW\\bin\\g++","-c \"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\final native.cpp\" " + compile_flags + " -o \"" + "C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\final native.o" + "\" 2>\"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\logs\\native_errors.log\"");
        execute_silent("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\resources\\MinGW\\bin\\g++","\"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\final native.o\" \"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\core\\libboomslangcore.a\" " + linker_flags + " -o \"" + filename_path(file_read_name) + filename_change_ext(filename_name(file_read_name),"exe") + "\" 2>\"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\logs\\linker_errors.log\"");

        //Check if MinGW created an executable
        if (!file_exists(filename_path(file_read_name) + filename_change_ext(filename_name(file_read_name),"exe"))){
            error_fatal("Native compile error");
            error_suggest("Details at C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\logs\\native.log");
            pend();
            return EXIT_FAILURE;
        }

        if(runafter){
            system(("\"" + filename_path(file_read_name) + filename_change_ext(filename_name(file_read_name),"exe") + "\"").c_str());
        }

        #else //(BUILD_OS == WINDOWS)
        error_fatal("Can't create Windows executable");
        error_suggest("Compile your code on a machine running Windows");
        pend();
        return EXIT_FAILURE;
        #endif //else (BUILD_OS == WINDOWS)
    }

    if (ve_apptype == APPTYPE_ANDROID){
        error_log("Sorry, Android is not supported yet.");
        errors += 1;
        pend();
        return EXIT_FAILURE;
    }

    if (ve_apptype == APPTYPE_LINUX){
        #if (BUILD_OS == LINUX)

        #else
        error_fatal("Can't create Linux executable");
        error_suggest("Compile your code on a machine running Linux");
        pend();
        return EXIT_FAILURE;
        #endif
    }

    return EXIT_SUCCESS;
}