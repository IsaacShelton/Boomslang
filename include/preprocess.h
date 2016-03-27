
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

using namespace std;

//String of packages
string packages;

{ /// draft.cpp -> final.cpp
    //New Input File
    ifstream preprocess_file_read;

    #if (BUILD_OS == WINDOWS)
    preprocess_file_read.open( ("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\draft.cpp").c_str() );
    #elif (BUILD_OS == LINUX)
    preprocess_file_read.open( ("/home/" + USERNAME + "/Boomslang/source/draft.cpp").c_str() );
    #endif

    //New Output File
    ofstream preprocess_file_write;

    #if (BUILD_OS == WINDOWS)
    preprocess_file_write.open( ("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\final.cpp").c_str() );
    #elif (BUILD_OS == LINUX)
    preprocess_file_write.open( ("/home/" + USERNAME + "/Boomslang/source/final.cpp").c_str() );
    #endif

    //Ensure they're open
    if (!preprocess_file_read.is_open() and !preprocess_file_write.is_open()){
        error_fatal("Couldn't open files");
        pend();
        return EXIT_FAILURE;
    }

    //Variable to hold output code
    string read;

    //Move code to new file
    getline(preprocess_file_read,read);
    preprocess_file_write << read + "\n";
    while(getline(preprocess_file_read,read)){
        preprocess_file_write << read + "\n";
    }

    //Close Files
    preprocess_file_read.close();
    preprocess_file_write.close();
}

{ /// draft.h -> final.h
    //New Input File
    ifstream preprocess_file_read;

    #if (BUILD_OS == WINDOWS)
    preprocess_file_read.open( ("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\draft.h").c_str() );
    #elif (BUILD_OS == LINUX)
    preprocess_file_read.open( ("/home/" + USERNAME + "/Boomslang/source/draft.h").c_str() );
    #endif

    //New Output File
    ofstream preprocess_file_write;

    #if (BUILD_OS == WINDOWS)
    preprocess_file_write.open( ("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\final.h").c_str() );
    #elif (BUILD_OS == LINUX)
    preprocess_file_write.open( ("/home/" + USERNAME + "/Boomslang/source/final.h").c_str() );
    #endif

    //Ensure they're open
    if (!preprocess_file_read.is_open() and !preprocess_file_write.is_open()){
        error_fatal("Couldn't open files");
        pend();
        return EXIT_FAILURE;
    }

    //Variable to hold output code
    string read;

    //Move code to new file
    getline(preprocess_file_read,read);
    preprocess_file_write << read + "\n";
    while(getline(preprocess_file_read,read)){
        preprocess_file_write << read + "\n";
    }

    //Close Files
    preprocess_file_read.close();
    preprocess_file_write.close();
}
