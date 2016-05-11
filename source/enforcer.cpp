
#include <iostream>
#include <stdlib.h>
#include "../include/die.h"
#include "../include/scope.h"
#include "../include/enforcer.h"
#include "../include/management.h"

#define ERROR_INDICATOR string("ERROR - ")

using namespace std;

bool advance_index(unsigned int& index, unsigned int length){
    if(index + 1 >= length){
        return false;
    }

    index++;
    return true;
}
bool retreat_index(unsigned int& index){
    if(index - 1 < 0){
        return false;
    }

    index--;
    return true;
}

void token_force(const TokenList& tokens, unsigned int& index, unsigned int token_id, string cant_advance, string fail){
    if( !advance_index(index,tokens.size()) ){
        die(cant_advance);
    }
    if(tokens[index].id != token_id){
        die(fail);
    }
}

void enforce(TokenList tokens){
    // Enforce grammer and context

    Scope global{"global", NULL};
    Scope* current_scope = &global;
    unsigned int next_block = 0;

    for(unsigned int index = 0; index < tokens.size(); index++){
        if(tokens[index].id == TOKENINDEX_TERMINATE){
            // Do nothing
        }
        else if(tokens[index].id == TOKENINDEX_STRING_LITERAL or tokens[index].id == TOKENINDEX_NUMERIC_LITERAL){
            token_force(tokens, index, TOKENINDEX_METHOD_CALL, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
            token_force(tokens, index, TOKENINDEX_OPEN, ERROR_INDICATOR + "Unexpected statement termination\nExpected '(' at beginning of method call", ERROR_INDICATOR + "Expected '(' in method call");
            token_force(tokens, index, TOKENINDEX_CLOSE, ERROR_INDICATOR + "Unexpected statement termination\nExpected ')' at end of method call", ERROR_INDICATOR + "Expected ')' in method call before termination");
            token_force(tokens, index, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        }
        else if(tokens[index].id == TOKENINDEX_INDENT){
            current_scope->children.push_back(new Scope{"block_" + to_string(next_block), current_scope});
            current_scope = current_scope->children[current_scope->children.size()-1];

            next_block++;
        }
        else if(tokens[index].id == TOKENINDEX_DEDENT){
            if(current_scope->parent != NULL){
                current_scope = current_scope->parent;
            }
        }
        else if(tokens[index].id == TOKENINDEX_KEYWORD){
            if(tokens[index].data == "on"){

                token_force(tokens, index, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method name after 'on'", ERROR_INDICATOR + "Expected method name after 'on'");
                token_force(tokens, index, TOKENINDEX_OPEN, ERROR_INDICATOR + "Unexpected statement termination\nExpected '(' after method name in method declaration", ERROR_INDICATOR + "Expected '(' after method name in method declaration");
                token_force(tokens, index, TOKENINDEX_CLOSE, ERROR_INDICATOR + "Unexpected statement termination\nExpected ')' after method arguments in method declaration", ERROR_INDICATOR + "Expected ')' after method arguments in method declaration");
                token_force(tokens, index, TOKENINDEX_TERMINATE,  ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
            }
            else {
                die("Unknown keyword " + tokens[index].data);
            }
        }
        else if(tokens[index].id == TOKENINDEX_WORD){
            if( !advance_index(index, tokens.size()) ){
                die(ERROR_INDICATOR + "Unexpected statement termination\n");
            }

            if(tokens[index].id == TOKENINDEX_OPEN){
                token_force(tokens, index, TOKENINDEX_CLOSE, ERROR_INDICATOR + "Unexpected statement termination\nExpected ')' after method arguments in method call", ERROR_INDICATOR + "Expected ')' after method arguments in method call");
                token_force(tokens, index, TOKENINDEX_TERMINATE,  ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
            }
            else {
                retreat_index(index);
            }
        }
        else {
            die("Unknown token #" << tokens[index].id);
        }
    }

    clean_scopes(&global);
}
