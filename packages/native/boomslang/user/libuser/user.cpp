
#if defined(__WIN32__)
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>
#endif

#include "C:/Boomslang/core/boomslangcore.h"

boomslang_String boomslang_getUsername(){
    boomslang_String username;
    char* cstr_username;

    #if defined(__WIN32__)
    cstr_username = getenv("USERNAME");
    #elif defined(__linux__)
    cstr_username = getlogin();
    #endif

    username.data = (cstr_username == NULL) ? "" : cstr_username;

    delete[] cstr_username;
    return username;
}
