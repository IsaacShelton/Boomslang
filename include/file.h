
#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

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
#include <fstream>

std::string contents(std::string, std::string error_message = "The file specified does not exist");
std::string binary_contents(std::string, std::string error_message = "The file specified does not exist");
bool file_exists(std::string);
std::string full_path(std::string filename, std::string location = "");

#endif // FILE_H_INCLUDED
