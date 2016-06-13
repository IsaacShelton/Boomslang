
#include <iostream>
#include <stdlib.h>
#include <boost/filesystem.hpp>
#include "../include/file.h"

using namespace std;

string contents(string filename, string error_message){
    ifstream file(filename.c_str());
    string text;
    string line;

    if(file.is_open()){

        while(getline(file, line)){
            text += line + "\n";
        }

        file.close();

        return text;
    }
    else {
        cerr << error_message << endl;
        exit(1);
    }
}

//Checks if the file exists
bool file_exists(string filename){
    ifstream file;
    file.open(filename.c_str());
    bool open = file;
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
