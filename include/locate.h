
#ifndef LOCATE_H_INCLUDED
#define LOCATE_H_INCLUDED

#include <string>

#if defined(__WIN32__)
#define HOME       string("C:\\Users\\isaac\\AppData\\Roaming\\Boomslang\\source\\")
#define CPP_SOURCE string("source.cpp")
#define CPP_HEADER string("source.h")
#endif

extern std::string USERNAME;

void login();

#endif // LOCATE_H_INCLUDED
