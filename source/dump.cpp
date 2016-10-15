
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

#include <string>
#include <fstream>
#include <iostream>
#include "../include/file.h"
#include "../include/dump.h"
#include "../include/lexer.h"
#include "../include/token.h"
#include "../include/errors.h"
#include "../include/management.h"

void tokens_dump(std::string filename, TokenList tokens){
    std::ofstream file(filename.c_str(), std::ios::out | std::ios::binary);
    uint16_t id = TOKENINDEX_TERMINATE;
    uint32_t len = 0;
    DumpHeader header{1};

    if(file.is_open()){
        file.write( (char*)(&header), sizeof(header) );
        for(unsigned int i = 0; i < tokens.size(); i++){
            if(tokens[i].id == TOKENINDEX_KEYWORD and tokens[i].data == "import"){
                if(tokens[++i].id == TOKENINDEX_STRING_LITERAL){
                    TokenList imported = tokenize(contents(tokens[i].data));
                    tokens.insert(tokens.begin() + i + 1, imported.begin(), imported.end());
                }
                else {
                    id = tokens[--i].id;
                    len = tokens[i].data.length();

                    file.write( (char*)(&id), 2 );
                    file.write( (char*)(&len), 4 );
                    file.write(tokens[i].data.c_str(), len);
                }
            }
            else {
                id = tokens[i].id;
                len = tokens[i].data.length();

                file.write( (char*)(&id), 2 );
                file.write( (char*)(&len), 4 );
                file.write(tokens[i].data.c_str(), len);
            }
        }
        file.close();
    }
}

bool tokens_load(std::string filename, TokenList& tokens){
    std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
    uint16_t id = TOKENINDEX_TERMINATE;
    uint32_t len = 0;
    char* data;
    DumpHeader header;

    if(file.is_open() and !file.eof()){
        file.seekg(0);
        file.read( (char*)(&header), sizeof(DumpHeader) );

        if( !token_compat(header.dump_version) ){
            std::cerr << INCOMPATIBLE_PACKAGE << std::endl;
            return false;
        }

        while(!file.eof()){
            file.read( (char*)(&id), 2 );
            file.read( (char*)(&len), 4 );

            if( (unsigned int)(len) > 0 ){
                data = new char[len];
                file.read(data, len);
            }
            else data = new char('\0');

            tokens.push_back( Token{id, std::string(data, len) } );
            delete[] data;
        }

        file.close();
    }
    else return false;
    return true;
}

bool token_compat(uint16_t dump_version){
    if(dump_version != 1) return false;
    return true; // Package is supported
}
