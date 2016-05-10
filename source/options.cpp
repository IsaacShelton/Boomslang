
#include <iostream>
#include <stdlib.h>
#include "../include/options.h"
#include "../include/management.h"

using namespace std;

Configuration configure(int* argc, char*** argv){
    Configuration config;
    string option;

    // Make sure we have at least to arguments
    if(*argc < 2){
        cout << "BoomslangCompiler <filename> [options]" << endl;
        exit(1);
    }

    for(unsigned int item = 2; item < *argc; item++){
        option = (*argv)[item];

        if(option == "-help"){
            cout << "BoomslangCompiler <filename> [options]" << endl << endl;
            cout << "  -help     : help" << endl;
            cout << "  -console  : uses the console" << endl;
            cout << "  -wait     : wait after complete" << endl;
            cout << "  -run      : run after compiled" << endl;
            cout << "  -windows  : compile for windows" << endl;
            cout << "  -osx      : compile for mac osx" << endl;
            cout << "  -jvm      : compile for the jvm" << endl;
            cout << "  -linux    : compile for linux" << endl;
            cout << "  -debian   : compile for debian linux" << endl;
            exit(1);
        }
        else {
            cerr << "Unknown option '" + option + "'" << endl;
            cerr << "Type -help for a list of options" << endl;
            exit(1);
        }
    }

    option = (*argv)[1];

    if(config.platform == PLATFORM_AUTO){
        #if defined(__WIN32__)
        config.output_filename = filename_change_ext(option, "exe");

        #elif defined(__APPLE__)
        config.output_filename = filename_change_ext(option, "app");

        #elif defined(__linux__)
        config.output_filename = filename_change_ext(option, "bin");

        #endif // __APPLE__
    }

    return config;
}
