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

using namespace std;

//Boomslang Global Variables
#include "../include/globals define.h"

//Boomslang String Handling Functions
#include "../include/management.h"

//Boomslang Resource Handling Functions
#include "../include/resource.h"

//Boomslang General Purpose Functions
#include "../include/scripts.h"

//Boomslang File Handling Functions
#include "../include/file.h"

//Boomslang Base Compile Functions
#include "../include/base.h"

//Boomslang final assemble functions
#include "../include/assemble.h"

int boot(int, char**);
int init(int, char**);

int main(int arg_count, char* arg[])
{
    //Start Up Boomslang
    boot(arg_count,arg);

    //Run Boomslang
    init(arg_count,arg);

    //Return Success
    if(!runafter) pend();
    return EXIT_SUCCESS;
}
