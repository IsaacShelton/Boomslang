
#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

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

#include "build.h"
#include "management.h"
#include <windows.h>
#include <fstream>

//Platforms
#define APPTYPE_WINDOWS WINDOWS
#define APPTYPE_ANDROID ANDROID
#define APPTYPE_LINUX LINUX

//Scope Types
#define SCOPETYPE_VARIABLE 0
#define SCOPETYPE_FUNCTION 1
#define SCOPETYPE_GLOBAL 2
#define SCOPETYPE_MAIN 3
#define SCOPETYPE_TEMPLATE 4

//Specified 'Null' Values
#define S_NULL " "
#define I_NULL -1

//Argument Types
#define ARGTYPE_STRING 0
#define ARGTYPE_NUMBER 1
#define ARGTYPE_VARIABLE 2
#define ARGTYPE_FUNCTION 3
#define ARGTYPE_LIST 4

//Current User Name
extern string USERNAME;

//Names of read and write files
extern string file_read_name;
extern string file_write_name;
extern string file_write_header_name;
extern string file_write_register_name;

//Platform to compile to
extern int ve_apptype;

//Does the program use the console?
extern bool console;

//Do we need to package our output?
extern bool package;

//Current Line Indentation
extern unsigned int indentation;

//Simple String Container Class
class Container{
    public:
    string data;
    Container();
    bool exists(string what);
    void add(string what);
};

//Containers to store names of actions, constants, keywords, and packages
extern Container ve_actions;
extern Container ve_constants;
extern Container ve_keywords;
extern Container ve_packages;

//Main Code Variable
extern string ve_main_code;

//Current Scope
extern string ve_scope;

//Current Context
extern string ve_context;

//Unique Templates
extern unsigned int next_unique_template;

//Embedded id
extern unsigned int next_embedded_id;

//Method Code
extern unsigned int next_method_id;

//Branch Code
extern unsigned int next_branch_id;

//For In Id
extern unsigned int next_for_in_id;

//Wait after compiled?
extern bool waitafter;

//Run after compile?
extern bool runafter;

//Local Path?
extern char terminal_complete_path[255];
extern string terminal_path;

//Compiler and Linker Flags
extern string compile_flags;
extern string linker_flags;

//Boomslang Code
extern string compile_code;

//Input and Output files
extern ifstream file_read;
extern ofstream file_write;
extern ofstream file_write_header;
extern ofstream file_write_register;

//Number of Errors and Warnings
extern unsigned int errors;
extern unsigned int warnings;

#endif // GLOBALS_H_INCLUDED
