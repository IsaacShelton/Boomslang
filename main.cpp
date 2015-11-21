///Build Information
#include "Core/build.h"

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
#include "Core/predefinition.h"

using namespace std;

//Boomslang Global Variables
#include "Core/globals.h"

//Boomslang String Handling Functions
#include "Handling/stringHandling.h"

//Boomslang Resource Handling Functions
#include "Handling/resourceHandling.h"

//Boomslang General Purpose Functions
#include "Handling/scripts.h"

//Boomslang File Handling Functions
#include "Handling/file.h"

//Boomslang Base Compile Functions
#include "Core/base.h"

int main(int arg_count, char* arg[])
{
    //Start Up Boomslang
    #include "Core/boot.h"

    //Run Boomslang
    #include "Core/init.h"

    //Return Success
    if(!runafter) pend();
    return EXIT_SUCCESS;
}
