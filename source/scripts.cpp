
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
#include <sstream>
#include <fstream>
#include "../include/globals.h"

using namespace std;

//Converts string to double
double to_double(string str){
    double dec;
    if( ! (std::istringstream(str) >> dec) ) dec = 0;
    return dec;
}

//Converts double to string
string to_string(double dec){
    std::ostringstream convert;
    convert << dec;
    return convert.str();
}

//Checks if the file exists
bool file_exists(string filename){
    ifstream file;
    file.open(filename.c_str());
    bool open = file;
    file.close();
    return open;
}

//Shows a fatal error
void error_fatal(string error){
    errors += 1;
    cout << "Fatal Error - " + error << endl;
}

//Shows a regular error
void error_show(string error){
    errors += 1;
    cout << "Error - " + error << endl;
}

//Shows a warning
void error_warn(string error){
    warnings += 1;
    cout << "Warning - " + error << endl;
}

//Shows a message
void error_log(string error){
    if (runafter==false)
        cout << "Log - " + error << endl;
}

//Shows a debug message
void error_debug(string error){
    #ifdef DEBUG
    cout << "Debug - " + error << endl;
    #endif
}

//Shows a suggestion
void error_suggest(string error){
    cout << "Suggestion - " + error << endl;
}

void pend(){
    cout << "Errors: " + to_string(errors) + " Warnings: " + to_string(warnings) << endl;
    if (waitafter){
        string blankstring;
        getline(cin,blankstring);
    }
}
