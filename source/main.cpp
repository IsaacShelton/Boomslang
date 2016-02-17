
#include <stdlib.h>
#include "../include/globals.h"

#include <iostream>
#include "../include/resource.h"

using namespace std;

int boot(int,char**);
int init(int,char**);

int main(int arg_count, char* arg[])
{
    //Start up boomslang
    if(boot(arg_count,arg)==EXIT_FAILURE) return EXIT_FAILURE;

    //Run boomslang
    if(init(arg_count,arg)==EXIT_FAILURE) return EXIT_FAILURE;

    //Return Success
    if(!runafter) pend();
    return EXIT_SUCCESS;
}
