
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

#include "../include/die.h"

bool wait_after_death = false;
unsigned int error_count = 0;

void die(std::string error) {
    std::cerr << error << std::endl;
    if(wait_after_death){
        std::cout << "Press enter to continue... ";
        std::cin.get();
    }
    exit(1);
}

void fail(std::string error) {
    std::cerr << error << std::endl;
    error_count++;
    if(error_count > 20){
        std::cerr << "...Exiting because there are more than 20 errors..." << std::endl;
        if(wait_after_death){
            std::cout << "Press enter to continue... ";
            std::cin.get();
        }
        exit(1);
    }
}
