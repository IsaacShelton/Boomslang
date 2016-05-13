
#include <iostream>
#include <stdlib.h>
#include "../include/die.h"
#include "../include/scope.h"
#include "../include/enforcer.h"
#include "../include/management.h"

#define ERROR_INDICATOR  string("ERROR - ")
#define GLOBAL_STATEMENT string("Unexpected statement in global scope\n")
#define UNEXPECTED_TERMINATE string("Unexpected statement termination\n")

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
void token_sequence_value(const TokenList& tokens, unsigned int& index){
    int balance = 0;

    if(!advance_index(index,tokens.size())){
        die(ERROR_INDICATOR + UNEXPECTED_TERMINATE);
    }

    while(balance != 0 or (tokens[index].id != TOKENINDEX_NEXT and tokens[index].id != TOKENINDEX_TERMINATE and tokens[index].id != TOKENINDEX_CLOSE)){
        if(tokens[index].id == TOKENINDEX_OPEN){
            balance++;
        }
        else if(tokens[index].id == TOKENINDEX_CLOSE){
            balance--;
        }
        else if(tokens[index].id == TOKENINDEX_WORD
        or tokens[index].id == TOKENINDEX_STRING_LITERAL
        or tokens[index].id == TOKENINDEX_NUMERIC_LITERAL
        or tokens[index].id == TOKENINDEX_ADD
        or tokens[index].id == TOKENINDEX_SUBTRACT
        or tokens[index].id == TOKENINDEX_MULTIPLY
        or tokens[index].id == TOKENINDEX_DIVIDE
        or tokens[index].id == TOKENINDEX_ASSIGN
        or tokens[index].id == TOKENINDEX_NOT){
            // ok
        }
        else {
            die("Unexpected token " + token_name(tokens[index]));
        }

        if(!advance_index(index,tokens.size())){
            die(ERROR_INDICATOR + UNEXPECTED_TERMINATE);
        }
    }
}
void token_sequence_arguments(const TokenList& tokens, unsigned int& index){
    int balance = 1;

    token_force(tokens, index, TOKENINDEX_OPEN, ERROR_INDICATOR + "Unexpected statement termination\nExpected opening parentheses", ERROR_INDICATOR + "Expected opening parentheses");

    while(balance != 0){
        if(!advance_index(index,tokens.size())){
            die(UNEXPECTED_TERMINATE);
        }

        if(tokens[index].id == TOKENINDEX_OPEN){
            balance++;
        }
        else if(tokens[index].id == TOKENINDEX_CLOSE){
            balance--;
        }
        else if(tokens[index].id == TOKENINDEX_WORD
        or tokens[index].id == TOKENINDEX_STRING_LITERAL
        or tokens[index].id == TOKENINDEX_NUMERIC_LITERAL
        or tokens[index].id == TOKENINDEX_ADD
        or tokens[index].id == TOKENINDEX_SUBTRACT
        or tokens[index].id == TOKENINDEX_MULTIPLY
        or tokens[index].id == TOKENINDEX_DIVIDE
        or tokens[index].id == TOKENINDEX_ASSIGN
        or tokens[index].id == TOKENINDEX_NEXT
        or tokens[index].id == TOKENINDEX_NOT){
            // ok
        }
        else {
            die("Unexpected token " + token_name(tokens[index]));
        }
    }
}
void token_sequence_method_declaration_arguments(const TokenList& tokens, unsigned int& index){

    token_force(tokens, index, TOKENINDEX_OPEN, ERROR_INDICATOR + "Unexpected statement termination\nExpected opening parentheses", ERROR_INDICATOR + "Expected opening parentheses");

    if(!advance_index(index,tokens.size())){
        die(UNEXPECTED_TERMINATE);
    }

    while(tokens[index].id != TOKENINDEX_CLOSE){
        if(!advance_index(index,tokens.size())){
            die(UNEXPECTED_TERMINATE);
        }

        while(tokens[index].id != TOKENINDEX_ASSIGN and tokens[index].id != TOKENINDEX_NEXT and tokens[index].id != TOKENINDEX_CLOSE){
            if(!advance_index(index,tokens.size())){
                die(UNEXPECTED_TERMINATE);
            }
        }

        if(tokens[index].id == TOKENINDEX_ASSIGN){
            token_sequence_value(tokens, index);
        }
        else if(tokens[index].id == TOKENINDEX_NEXT){

        }
        else if(tokens[index].id == TOKENINDEX_CLOSE){

        }
    }
}

void enforce_token(const TokenList& tokens, unsigned int& index, Scope** current_scope, Environment& environment){
    static unsigned int next_block = 0;

    if(tokens[index].id == TOKENINDEX_TERMINATE){
        // Do nothing
    }
    else if(tokens[index].id == TOKENINDEX_STRING_LITERAL or tokens[index].id == TOKENINDEX_NUMERIC_LITERAL){
        if(*current_scope == &environment.global){
            die(ERROR_INDICATOR + GLOBAL_STATEMENT);
        }

        token_force(tokens, index, TOKENINDEX_MEMBER, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
        token_force(tokens, index, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
        token_sequence_arguments(tokens,index);
        token_force(tokens, index, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
    }
    else if(tokens[index].id == TOKENINDEX_INDENT){
        (*current_scope)->children.push_back(new Scope{"block_" + to_string(next_block), (*current_scope)});
        (*current_scope) = (*current_scope)->children[(*current_scope)->children.size()-1];

        next_block++;
    }
    else if(tokens[index].id == TOKENINDEX_DEDENT){
        if((*current_scope)->parent != NULL){
            (*current_scope) = (*current_scope)->parent;
        }
    }
    else if(tokens[index].id == TOKENINDEX_KEYWORD){
        if(tokens[index].data == "on"){
            string method_name;
            string method_arguments;
            string method_return_type;

            token_force(tokens, index, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method name after 'on'", ERROR_INDICATOR + "Expected method name after 'on'");
            method_name = tokens[index].data;

            token_sequence_method_declaration_arguments(tokens, index);

            token_force(tokens, index, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");

            (*current_scope)->children.push_back(new Scope{"method_" + method_name, (*current_scope)});
            (*current_scope) = (*current_scope)->children[(*current_scope)->children.size()-1];
            environment.methods.push_back( Method{method_name, &environment.global, "", "void"} );
        }
        else if(tokens[index].data == "return"){
            token_sequence_value(tokens, index);
            environment.methods[environment.methods.size()-1].return_type = "Number";
            // ...
        }
        else {
            die("Unknown keyword " + tokens[index].data);
        }
    }
    else if(tokens[index].id == TOKENINDEX_WORD){
        if( !advance_index(index, tokens.size()) ){
            die(ERROR_INDICATOR + UNEXPECTED_TERMINATE);
        }

        if(tokens[index].id == TOKENINDEX_OPEN){
            index--;
            token_sequence_arguments(tokens, index);

            if( !advance_index(index, tokens.size()) ){
                die(ERROR_INDICATOR + UNEXPECTED_TERMINATE);
            }

            while(tokens[index].id == TOKENINDEX_MEMBER){
                token_force(tokens, index, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                token_sequence_arguments(tokens,index);

                if( !advance_index(index, tokens.size()) ){
                    die(ERROR_INDICATOR + UNEXPECTED_TERMINATE);
                }
            }

            index--;
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

Environment enforce(TokenList tokens){
    // Enforce grammer and context

    Environment environment;
    Scope* current_scope = &environment.global;

    for(unsigned int index = 0; index < tokens.size(); index++){
        enforce_token(tokens,index,&current_scope,environment);
    }

    if( !environment_method_exists(&environment, Method{"main", &environment.global, "", "void"}) ){
        die(ERROR_INDICATOR + "No main method");
    }

    clean_scopes(&environment.global);
    return environment;
}
