
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

#include <fstream>
#include <iostream>
#include "../include/file.h"
#include "../include/dump.h"
#include "../include/lexer.h"
#include "../include/token.h"
#include "../include/management.h"

void tokens_dump(std::string filename, TokenList tokens){
    std::ofstream file(filename.c_str(), std::ios::out | std::ios::binary);
    uint16_t id = TOKENINDEX_TERMINATE;
    uint32_t len = 0;
    char* data = new char();

    if(file.is_open()){
        for(unsigned int i = 0; i < tokens.size(); i++){
            if(tokens[i].id == TOKENINDEX_KEYWORD and tokens[i].data == "import"){
                i++;
                if(tokens[i].id == TOKENINDEX_STRING_LITERAL){
                    TokenList imported = tokenize(contents(tokens[i].data));
                    tokens.insert(tokens.begin() + i + 1, imported.begin(), imported.end());
                }
                else {
                    i--;

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
            }
            else {
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
    uint16_t id = TOKENINDEX_TERMINATE;
    uint32_t len = 0;
    char* data;

    if(file.is_open()){
        file.seekg(0);

        if(!file.eof()){
            while(!file.eof()){
                file.read( (char*)(&id), 2 );
                file.read( (char*)(&len), 4 );

                if( (unsigned int)(len) > 0 ){
                    data = new char[len+1];

                    file.read(data, len);
                }
                else {
                    data = new char('\0');
                }

                tokens.push_back( Token{id, std::string(data, len) } );
                delete[] data;
            }
        }

        file.close();
    }

    return true;
}
