
#ifndef LOCATE_H_INCLUDED
#define LOCATE_H_INCLUDED

#include <string>

#if defined(__WIN32__)
#define HOME          string("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\")
#define LOGHOME       string("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\logs\\")
#define CPP_SOURCE    string("source.cpp")
#define CPP_HEADER    string("source.h")
#define CPP_DEFINE    string("define.h")
#define LOG_LEXER     string("lexer.log")
#define LOG_ENFORCER  string("enforcer.log")
#define LOG_ASSEMBLER string("assembler.log")
#endif

extern std::string USERNAME;

void login();

#endif // LOCATE_H_INCLUDED
