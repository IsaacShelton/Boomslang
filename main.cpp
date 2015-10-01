///Build Information
#include "build.h"

///Basic Headers
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>

///Windows Specific Headers
#if (BUILD_OS == WINDOWS)
#include <windows.h>
#endif

///Linux Specific Headers
#if (BUILD_OS == LINUX)
//Not supported
#include <stdlib.h>
#endif

///Mac Specific Headers
#if (BUILD_OS == Mac)
//Not supported
#include <stdlib.h>
#endif

using namespace std;

//Code Parser Predefinition
class CodeParser{
public:
    int parse_args(string& code, bool write_to_main);
    int parse_function_from(string& code, bool write_to_main, bool check_semicolon, int class_id);
    void chop(string& code);
    int harvest_from_variable_value(string& code, string &type, int write_to_main);
    int harvest_from_variable_value_type(string code, string &type);
    int arg_type(string code);
}code_parser;

//Boomslang Global Variables
#include "globals.h"

//Boomslang String Handling Functions
#include "string_handling.h"

//Boomslang Resource Handling Functions
#include "resource_handling.h"

//Boomslang General Purpose Functions
#include "scripts.h"

//Boomslang File Handling Functions
#include "file.h"

//Boomslang Base Compile Functions
#include "base.h"

int main(int arg_count, char* arg[])
{
    //Start Up Boomslang
    #include "boot.h"

    //Run Boomslang
    #include "init.h"

    //Return Success
    pend();
    return EXIT_SUCCESS;
}
