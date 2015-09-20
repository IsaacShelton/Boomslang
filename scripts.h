/**
    ============ scripts.h ============
    Code for General Purpose Functions

    This file should NOT be included
    anywhere besides from main.cpp.

    This Header does NOT contain a
    header guard so include with
    cation.
*/

using namespace std;

double to_double(string str){
    double dec;
    if( ! (std::istringstream(str) >> dec) ) dec = 0;
    return dec;
}

string to_string(double dec){
    std::ostringstream convert;
    convert << dec;
    return convert.str();
}

bool file_exists(string filename){
    ifstream file;
    file.open(filename.c_str());
    bool open = file.is_open();
    file.close();
    return open;
}

void error_fatal(string error){
    errors += 1;
    cout << endl << "Fatal Error - " + error << endl;
}

void error_show(string error){
    errors += 1;
    cout << "Error - " + error << endl;
}

void error_warn(string error){
    warnings += 1;
    cout << "Warning - " + error << endl;
}

void error_log(string error){
    cout << "Log - " + error << endl;
}

void error_debug(string error){
    #ifdef DEBUG
    cout << "Debug - " + error << endl;
    #endif
}

void error_suggest(string error){
    cout << "Suggestion - " + error << endl;
}

void pend(){
    cout << endl << "Errors: " + to_string(errors) + " Warnings: " + to_string(warnings) << endl;
}
