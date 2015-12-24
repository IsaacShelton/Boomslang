
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
    bool open = file.is_open();
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
