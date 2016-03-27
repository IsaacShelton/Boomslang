
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

#include <stdlib.h>
#include "../include/globals.h"

using namespace std;

int boot(int,char**);
int init(int,char**);

int main(int arg_count, char* arg[])
{
    //Start up boomslang
    if(boot(arg_count,arg)==EXIT_FAILURE) return EXIT_FAILURE;

    //Run boomslang
    if(init(arg_count,arg)==EXIT_FAILURE) return EXIT_FAILURE;

    //Return Success
    if(!runafter) pend();
    return EXIT_SUCCESS;
}
