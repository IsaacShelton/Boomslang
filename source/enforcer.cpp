
#include <iostream>
#include <stdlib.h>
#include "../include/die.h"
#include "../include/enforcer.h"

#define ERROR_INDICATOR string("ERROR - ")

using namespace std;

bool advance_index(unsigned int& index, unsigned int length){
    if(index + 1 >= length){
        return false;
    }

    index++;
    return true;
}

void token_check(const TokenList& tokens, unsigned int& index, unsigned int token_id, string cant_advance, string fail){
    if( !advance_index(index,tokens.size()) ){
        die(cant_advance);
    }
    if(tokens[index].id != token_id){
        die(fail);
    }
}

void enforce(TokenList tokens){
    // Enforce grammer and context

    for(unsigned int index = 0; index < tokens.size(); index++){
        if(tokens[index].id == TOKENINDEX_TERMINATE){
            // Do nothing
        }
        else if(tokens[index].id == TOKENINDEX_STRING_LITERAL or tokens[index].id == TOKENINDEX_NUMERIC_LITERAL){

            // Check for method call
            token_check(tokens, index, TOKENINDEX_METHOD_CALL, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");

            // Check for open token
            token_check(tokens, index, TOKENINDEX_OPEN, ERROR_INDICATOR + "Unexpected statement termination\nExpected '(' at beginning of method call", ERROR_INDICATOR + "Expected '(' in method call");

            // Check for close token
            token_check(tokens, index, TOKENINDEX_CLOSE, ERROR_INDICATOR + "Unexpected statement termination\nExpected ')' at end of method call", ERROR_INDICATOR + "Expected ')' in method call before termination");

            // Check for terminate token
            token_check(tokens, index, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        }
        else {
            die("Unknown token #" << tokens[index].id);
        }
    }
}
