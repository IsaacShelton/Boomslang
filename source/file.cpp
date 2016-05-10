
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
