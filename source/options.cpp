
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

void show_help(){
    std::cout << "BoomslangCompiler <filename> [options]" << std::endl << std::endl;

    std::cout << "             General" << std::endl;
    std::cout << "  -help         : help" << std::endl;
    std::cout << "  -console      : uses the console" << std::endl;
    std::cout << "  -optimize     : optimizes code" << std::endl;
    std::cout << "  -package      : packages code" << std::endl;
    std::cout << "  -output       : sets output destination" << std::endl << std::endl;
    std::cout << "            Platform" << std::endl;
    std::cout << "  -windows      : compile for windows" << std::endl;
    std::cout << "  -osx          : compile for mac osx" << std::endl;
    std::cout << "  -linux        : compile for linux" << std::endl;
    std::cout << "  -debian       : compile for debian linux" << std::endl << std::endl;
    std::cout << "            Utilities" << std::endl;
    std::cout << "  -wait         : wait after compiled" << std::endl;
    std::cout << "  -run          : run after compiled" << std::endl;
    std::cout << "  -log          : log everything" << std::endl;
}

Configuration configure(int* argc, char*** argv){
    Configuration config;
    std::string option;

    // Start Timer
    config.start_time = std::chrono::steady_clock::now();

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

    for(size_t item = 2; item < (size_t)(*argc); item++){
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
            if(++item >= (size_t) *argc) die("Expected output filename after -output flag");
            config.output_filename = (*argv)[item];
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
            std::cerr << "Unknown option '" + option + "'" << std::endl;
            std::cerr << "Type -help for a list of options" << std::endl;
            exit(1);
        }
    }

    if(config.log){
        LOGGING_LEXER = true;
        LOGGING_ENFORCER = true;
        LOGGING_ASSEMBLER = true;

        std::remove( (LOGHOME + LOG_LEXER).c_str()     );
        std::remove( (LOGHOME + LOG_ENFORCER).c_str()  );
        std::remove( (LOGHOME + LOG_ASSEMBLER).c_str() );

        logging_context.create();
        logging_context.log_lexer("-=-=- Lexer Log -=-=-");
        logging_context.log_enforcer("-=-=- Enforcer Log -=-=-");
        logging_context.log_assembler("-=-=- Assembler Log -=-=-");
    }

    return config;
}
