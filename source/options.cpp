
#include <iostream>
#include <stdlib.h>
#include "../include/log.h"
#include "../include/errors.h"
#include "../include/locate.h"
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

    // Get current login information
    login();

    for(unsigned int item = 2; item < (unsigned int)(*argc); item++){
        option = (*argv)[item];

        if(option == "-help"){
            cout << "BoomslangCompiler <filename> [options]" << endl << endl;
            cout << "  -help     : help" << endl;
            cout << "  -console  : uses the console" << endl;
            cout << "  -optimize : optimizes code" << endl;
            cout << "  -package  : packages code" << endl;
            cout << "  -wait     : wait after complete" << endl;
            cout << "  -run      : run after compiled" << endl;
            cout << "  -windows  : compile for windows" << endl;
            cout << "  -osx      : compile for mac osx" << endl;
            cout << "  -jvm      : compile for the jvm" << endl;
            cout << "  -linux    : compile for linux" << endl;
            cout << "  -debian   : compile for debian linux" << endl;
            exit(1);
        }
        else if(option == "-optimize"){
            config.optimize = true;
        }
        else if(option == "-console"){
            config.console = true;
        }
        else if(option == "-package"){
            config.package = true;
        }
        else if(option == "-wait"){
            config.wait = true;
        }
        else if(option == "-run"){
            config.run = true;
        }
        else if(option == "-windows"){
            config.platform = PLATFORM_WINDOWS;
        }
        else {
            cerr << "Unknown option '" + option + "'" << endl;
            cerr << "Type -help for a list of options" << endl;
            exit(1);
        }
    }

    clear_lexer_log();
    clear_enforcer_log();
    clear_assembler_log();

    log_lexer    ("-=-=- Lexer Log -=-=-");
    log_enforcer ("-=-=- Enforcer Log -=-=-");
    log_assembler("-=-=- Assembler Log -=-=-");

    option = (*argv)[1];

    // Set the current source file
    current_filename = option;

    if(config.package){
        config.output_filename = filename_change_ext(option, "branch");
    }
    else if(config.platform == PLATFORM_AUTO){
        #if defined(__WIN32__)
        config.output_filename = filename_change_ext(option, "exe");

        #elif defined(__APPLE__)
        config.output_filename = filename_change_ext(option, "app");

        #elif defined(__linux__)
        config.output_filename = filename_change_ext(option, "bin");

        #endif
    }

    return config;
}
