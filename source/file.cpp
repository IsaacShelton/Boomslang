
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
#include <stdlib.h>
#include <boost/filesystem.hpp>
#include "../include/file.h"

std::string contents(std::string filename, std::string error_message){
    std::ifstream file(filename.c_str());
    std::string text;
    std::string line;

    if(file.is_open()){

        while(getline(file, line)){
            text += line + "\n";
        }

        file.close();

        return text;
    }
    else {
        std::cerr << error_message << std::endl;
        exit(1);
    }
}

std::string binary_contents(std::string filename, std::string error_message){
    std::ifstream file(filename.c_str(), std::ios::binary);
    std::string text;
    char* binary;
    unsigned int length;

    if(file.is_open()){
        file.seekg (0, std::ios::end);
        length = file.tellg();
        file.seekg (0, std::ios::beg);

        binary = new char[length];

        file.read (binary, length);
        file.close();
    }
    else {
        std::cerr << error_message << std::endl;
        exit(1);
    }

    text = std::string(binary, length);
    delete[] binary;

    return text;
}

//Checks if the file exists
bool file_exists(std::string filename){
    std::ifstream file;
    file.open(filename.c_str());
    bool open = file.is_open();
    file.close();
    return open;
}

std::string full_path(std::string filename, std::string location){
    if(location != ""){
        return boost::filesystem::absolute(boost::filesystem::path(filename), boost::filesystem::path(location)).string();
    }
    else {
        return boost::filesystem::absolute(boost::filesystem::path(filename)).string();
    }
}
