
#ifndef LOCATE_H_INCLUDED
#define LOCATE_H_INCLUDED

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

#if defined(__WIN32__)
#define HOME          std::string("C:\\Boomslang\\source\\")
#define PACKAGEHOME   std::string("C:\\Boomslang\\packages\\")
#define COREHOME      std::string("C:\\Boomslang\\core\\")
#define LOGHOME       std::string("C:\\Boomslang\\logs\\")
#define RESOURCEHOME  std::string("C:\\Boomslang\\resources\\")
#define MINGWHOME     std::string("C:\\Boomslang\\resources\\MinGW\\")
#define CPP_SOURCE    std::string("source.cpp")
#define CPP_HEADER    std::string("source.h")
#define CPP_OBJECT    std::string("source.o")
#define CPP_DEFINE    std::string("define.h")
#define LOG_LEXER     std::string("lexer.log")
#define LOG_ENFORCER  std::string("enforcer.log")
#define LOG_ASSEMBLER std::string("assembler.log")
#endif

extern std::string USERNAME;

void login();

#endif // LOCATE_H_INCLUDED
