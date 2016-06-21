
#ifndef LOCATE_H_INCLUDED
#define LOCATE_H_INCLUDED

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
