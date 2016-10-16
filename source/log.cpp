
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
LoggingContext logging_context;

void LoggingContext::create(){
    log = true;
    lexer = new std::ofstream(LOGHOME + LOG_LEXER);
    enforcer = new std::ofstream(LOGHOME + LOG_ENFORCER);
    assembler = new std::ofstream(LOGHOME + LOG_ASSEMBLER);

    if( !lexer->is_open() )     die("Failed to open lexer log file");
    if( !enforcer->is_open() )  die("Failed to open enforcer log file");
    if( !assembler->is_open() ) die("Failed to open assembler log file");
}
void LoggingContext::destroy(){
    log = false;
    lexer->close();
    enforcer->close();
    assembler->close();

    delete lexer;
    delete enforcer;
    delete assembler;
}

void LoggingContext::log_lexer(std::string message){
    if(log and LOGGING_LEXER) *lexer << message << std::endl;
}
void LoggingContext::log_enforcer(std::string message){
    if(log and LOGGING_ENFORCER) *enforcer << message << std::endl;
}
void LoggingContext::log_assembler(std::string message){
    if(log and LOGGING_ASSEMBLER) *assembler << message << std::endl;
}
