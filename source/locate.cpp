
#include <iostream>
#include "../include/die.h"
#include "../include/locate.h"

#ifdef __WIN32__
#include <windows.h>
#endif // __WIN32__

using namespace std;

string USERNAME;

void login(){
    char* username;
    #if defined(__WIN32__)
    username = getenv("USERNAME");
    #elif defined(__linux__)
    username = getlogin();
    #endif

    if(username==NULL){
        die("Failed to get username");
    } else {
        USERNAME = username;
    }
}

