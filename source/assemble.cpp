
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

#include <iostream>
#include "../include/file.h"
#include "../include/branch.h"
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

        if(file_exists( ("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\final.o") )){
            if(remove( ("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\final.o").c_str() )!=0){
                error_fatal("Failed to delete object file.");
                pend();
                return EXIT_FAILURE;
            }
        }

        //Run MinGW
        execute_silent("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\resources\\MinGW\\bin\\g++","-c \"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\final.cpp\" -O3 " + compile_flags + " -o \"" + "C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\final.o" + "\" 2>\"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\logs\\native_errors.log\"");

        if(package){
            execute_silent("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\resources\\MinGW\\bin\\ar","rvs \"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\final.a\" \"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\final.o\" \"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\core\\libboomslangcore.a\" " + linker_flags + " 2>\"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\logs\\linker_errors.log\"");
            branch_create(filename_path(file_read_name) + filename_change_ext(filename_name(file_read_name),"branch"),"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\final.a","C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\final.h","C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\final.boomslang");
        } else {
            execute_silent("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\resources\\MinGW\\bin\\g++","\"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\final.o\" " + linker_flags + "\"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\core\\libboomslangcore.a\" -o \"" + filename_path(file_read_name) + filename_change_ext(filename_name(file_read_name),"exe") + "\" 2>\"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\logs\\linker_errors.log\"");
        }

        //Check if MinGW created an executable
        if (!package and !file_exists(filename_path(file_read_name) + filename_change_ext(filename_name(file_read_name),"exe"))){
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
