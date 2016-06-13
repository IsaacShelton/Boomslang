
#ifndef LOCATE_H_INCLUDED
#define LOCATE_H_INCLUDED

#include <string>

#if defined(__WIN32__)
#define HOME          string("C:\\Boomslang\\source\\")
#define COREHOME      string("C:\\Boomslang\\core\\")
#define LOGHOME       string("C:\\Boomslang\\logs\\")
#define RESOURCEHOME  string("C:\\Boomslang\\resources\\")
#define MINGWHOME     string("C:\\Boomslang\\resources\\MinGW\\")
#define CPP_SOURCE    string("source.cpp")
#define CPP_HEADER    string("source.h")
#define CPP_OBJECT    string("source.o")
#define CPP_DEFINE    string("define.h")
#define LOG_LEXER     string("lexer.log")
#define LOG_ENFORCER  string("enforcer.log")
#define LOG_ASSEMBLER string("assembler.log")
#endif

extern std::string USERNAME;

void login();

#endif // LOCATE_H_INCLUDED
