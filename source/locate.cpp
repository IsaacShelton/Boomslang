
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

#include <iostream>
#include "../include/die.h"
#include "../include/locate.h"

#ifdef __WIN32__
#include <windows.h>
#endif // __WIN32__

#ifdef __linux__
#include <unistd.h>
#endif // __linux__

std::string USERNAME;

void login(){
    #if defined(__WIN32__)
    char* username;
    username = getenv("USERNAME");
    
    if(username==NULL){
        die("Failed to get username");
    } else {
        USERNAME = username;
    }
    #elif defined(__linux__)
    char username[255];
    getlogin(&username[0], 255);
    USERNAME = username;
    #endif
}

