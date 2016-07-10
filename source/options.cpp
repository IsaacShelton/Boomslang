
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
#include <stdlib.h>
#include "../include/die.h"
#include "../include/log.h"
#include "../include/errors.h"
#include "../include/locate.h"
#include "../include/options.h"
#include "../include/management.h"

using namespace std;

void show_help(){
    cout << "BoomslangCompiler <filename> [options]" << endl << endl;

    cout << "             General" << endl;
    cout << "  -help         : help" << endl;
    cout << "  -console      : uses the console" << endl;
    cout << "  -optimize     : optimizes code" << endl;
    cout << "  -package      : packages code" << endl;
    cout << "  -output       : sets output destination" << endl << endl;
    cout << "            Platform" << endl;
    cout << "  -windows      : compile for windows" << endl;
    cout << "  -osx          : compile for mac osx" << endl;
    cout << "  -linux        : compile for linux" << endl;
    cout << "  -debian       : compile for debian linux" << endl << endl;
    cout << "            Utilities" << endl;
    cout << "  -wait         : wait after compiled" << endl;
    cout << "  -run          : run after compiled" << endl;
    cout << "  -log          : log everything" << endl;
}

Configuration configure(int* argc, char*** argv){
    Configuration config;
    string option;

    // Make sure we have at least to arguments
    if(*argc < 2){
        show_help();
        exit(1);
    }

    // Get current login information
    login();

    // Filename alternatives
    if(std::string((*argv)[1]) == "-help"){
        show_help();
        exit(1);
    }

    // Set output filename
    config.output_filename = (*argv)[1];

    // Set the current source file
    current_filename = (*argv)[1];

    #if defined(__WIN32__)
    config.output_filename = filename_change_ext(config.output_filename, "exe");

    #elif defined(__APPLE__)
    config.output_filename = filename_change_ext(config.output_filename, "app");

    #elif defined(__linux__)
    config.output_filename = filename_change_ext(config.output_filename, "bin");

    #endif

    for(unsigned int item = 2; item < (unsigned int)(*argc); item++){
        option = (*argv)[item];

        if(option == "-help"){
            show_help();
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
            config.output_filename = filename_change_ext(config.output_filename, "branch");
        }
        else if(option == "-output"){
            if(item + 1 >= (unsigned int)(*argc)){
                die("Expected output filename after -output flag");
            }

            item++;
            config.output_filename = (*argv)[item];
        }
        else if(option.substr(0,7) == "-output"){
            config.output_filename = option.substr(7, option.length()-7);
        }
        else if(option == "-wait"){
            config.wait = true;
        }
        else if(option == "-run"){
            config.run = true;
        }
        else if(option == "-log"){
            config.log = true;
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

    if(config.log){
        LOGGING_LEXER = true;
        LOGGING_ENFORCER = true;
        LOGGING_ASSEMBLER = true;
    }

    clear_lexer_log();
    clear_enforcer_log();
    clear_assembler_log();

    log_lexer    ("-=-=- Lexer Log -=-=-");
    log_enforcer ("-=-=- Enforcer Log -=-=-");
    log_assembler("-=-=- Assembler Log -=-=-");

    return config;
}
