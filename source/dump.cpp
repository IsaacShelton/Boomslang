
#include <fstream>
#include <iostream>
#include "../include/file.h"
#include "../include/dump.h"
#include "../include/management.h"

void tokens_dump(std::string filename, TokenList& tokens){
    std::ofstream file(filename.c_str(), std::ios::out | std::ios::binary);
    uint16_t id;
    uint32_t len;
    char* data = new char();

    if(file.is_open()){
        for(unsigned int i = 0; i < tokens.size(); i++){
            id = tokens[i].id;
            len = tokens[i].data.length();

            delete[] data;
            data = new char[len];

            for(unsigned int j = 0; j < len; j++){
                data[j] = tokens[i].data[j];
            }

            file.write( (char*)(&id), 2 );
            file.write( (char*)(&len), 4 );
            file.write(data, len);
        }

        file.close();
    }

    delete[] data;
}

bool tokens_load(std::string filename, TokenList& tokens){

    if(!file_exists(filename)){
        return false;
    }

    std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
    uint16_t id;
    uint32_t len;
    char* data;

    if(file.is_open()){
        file.seekg(0);

        while(!file.eof()){
            file.read( (char*)(&id), 2 );
            file.read( (char*)(&len), 4 );

            if( (unsigned int)(len) > 0 ){
                data = new char[len+1];

                file.read(data, len);
            }
            else {
                data = "";
            }

            tokens.push_back( Token{id, std::string(data, len) } );
            delete[] data;
        }

        file.close();
    }
}
