
#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include "build.h"
#include "management.h"
#include "resource.h"
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
string USERNAME;

//Names of read and write files
string file_read_name;
string file_write_name;

//Platform to compile to
int ve_apptype;

//Does the program use the console?
bool console;

//Current Line Indentation
unsigned int indentation;

//Resource Handlers
ClassHandler class_handler;
VariableHandler variable_handler;
FunctionHandler function_handler;

//Simple String Container Class
class Container{
    public:
    string data;
    Container();
    bool exists(string what);
    void add(string what);
};

//Containers to store names of actions, constants, keywords, and packages
Container ve_actions;
Container ve_constants;
Container ve_keywords;
Container ve_packages;

//Main Code Variable
string ve_main_code;

//Current Scope
string ve_scope;

//Current Context
string ve_context;

//Unique Templates
unsigned int next_unique_template;

//Wait after compiled?
bool waitafter;

//Run after compile?
bool runafter;

//Local Path?
char terminal_complete_path[255];
string terminal_path;

//Compiler and Linker Flags
string compile_flags;
string linker_flags;

//Boomslang Code
string compile_code;

//Input and Output files
ifstream file_read;
ofstream file_write;

//Pointer to Variable that functions write output code to
string* write_to;

//Number of Errors and Warnings
int errors;
int warnings;

#endif // GLOBALS_H_INCLUDED
