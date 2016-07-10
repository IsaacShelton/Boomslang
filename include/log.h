
#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

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

#include <string>

extern bool LOGGING_LEXER;
extern bool LOGGING_ENFORCER;
extern bool LOGGING_ASSEMBLER;

void log_lexer(std::string message);
void log_enforcer(std::string message);
void log_assembler(std::string message);

void clear_lexer_log();
void clear_enforcer_log();
void clear_assembler_log();

#endif // LOG_H_INCLUDED
