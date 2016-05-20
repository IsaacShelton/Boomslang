
#include "../include/errors.h"
#include "../include/management.h"

std::string current_filename;

unsigned int getLineNumber(const TokenList& tokens, unsigned int index){
    unsigned int newlines = 0;

    for(unsigned int i = 0; i < index; i++){
        if(tokens[i].id == TOKENINDEX_TERMINATE){
            newlines++;
        }
    }

    return newlines + 1;
}
std::string getLineString(const TokenList& tokens, unsigned int index){
    return to_string( getLineNumber(tokens,index) );
}

