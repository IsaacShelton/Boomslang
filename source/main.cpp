///Build Information
#include "../include/build.h"

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
#include <unistd.h>
#include <stdlib.h>
#endif

///Mac Specific Headers
#if (BUILD_OS == Mac)
//Not supported
#include <stdlib.h>
#endif

///Predefinitions
#include "../include/predefinition.h"

using namespace std;

//Boomslang Global Variables
#include "../include/globals.h"

//Boomslang String Handling Functions
#include "../include/string handling.h"

//Boomslang Resource Handling Functions
#include "../include/resource handling.h"

//Boomslang General Purpose Functions
#include "../include/scripts.h"

//Boomslang File Handling Functions
#include "../include/file.h"

//Boomslang Base Compile Functions
#include "../include/base.h"

//Boomslang final assemble functions
#include "../include/assemble.h"

int main(int arg_count, char* arg[])
{
    //Start Up Boomslang
    #include "../include/boot.h"

    //Run Boomslang
    #include "../include/init.h"

    //Return Success
    if(!runafter) pend();
    return EXIT_SUCCESS;
}
