
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

#include <fstream>
#include "../include/die.h"
#include "../include/log.h"
#include "../include/locate.h"

bool LOGGING_LEXER     = 0;
bool LOGGING_ENFORCER  = 0;
bool LOGGING_ASSEMBLER = 0;

void log_lexer(std::string message){
    if(LOGGING_LEXER){
        std::ofstream lexer_logfile(LOGHOME + LOG_LEXER, std::ios::app);

        if(!lexer_logfile) die("Failed to open lexer log file");

        lexer_logfile <<  message << std::endl;
        lexer_logfile.close();
    }
}
void log_enforcer(std::string message){
    if(LOGGING_ENFORCER){
        std::ofstream enforcer_logfile(LOGHOME + LOG_ENFORCER, std::ios::app);

        if(!enforcer_logfile) die("Failed to open enforcer log file");

        enforcer_logfile <<  message << std::endl;
        enforcer_logfile.close();
    }
}
void log_assembler(std::string message){
    if(LOGGING_ASSEMBLER){
        std::ofstream assembler_logfile(LOGHOME + LOG_ASSEMBLER, std::ios::app);

        if(!assembler_logfile) die("Failed to open assembler log file");

        assembler_logfile <<  message << std::endl;
        assembler_logfile.close();
    }
}

void clear_lexer_log(){
    if(LOGGING_LEXER){
        std::remove( (LOGHOME + LOG_LEXER).c_str() );
    }
}
void clear_enforcer_log(){
    if(LOGGING_ENFORCER){
        std::remove( (LOGHOME + LOG_ENFORCER).c_str() );
    }
}
void clear_assembler_log(){
    if(LOGGING_ASSEMBLER){
        std::remove( (LOGHOME + LOG_ASSEMBLER).c_str() );
    }
}
