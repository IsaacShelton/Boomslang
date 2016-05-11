
#include <iostream>
#include <stdlib.h>
#include "../include/file.h"

using namespace std;

string contents(string filename){
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
        cerr << "The file specified does not exist" << endl;
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
