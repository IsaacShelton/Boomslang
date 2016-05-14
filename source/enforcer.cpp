
#include <iostream>
#include <stdlib.h>
#include "../include/die.h"
#include "../include/scope.h"
#include "../include/errors.h"
#include "../include/enforcer.h"
#include "../include/management.h"

using namespace std;

// Safe Index Navigation
bool advance_index(unsigned int& index, unsigned int length){
    if(index + 1 >= length){return false;}
    index++;

    return true;
}
bool retreat_index(unsigned int& index){
    if(index - 1 < 0){return false;}
    index--;

    return true;
}

// Shorthand Index Navigation with errors
void index_increase(const TokenList& tokens, unsigned int& index){
    if(!advance_index(index,tokens.size())) die(UNEXPECTED_TERMINATE);
}
void index_decrease(const TokenList& tokens, unsigned int& index){
    if(!retreat_index(index)) die(UNEXPECTED_TERMINATE);
}

// Token Forcing
void token_force(const TokenList& tokens, unsigned int& index, unsigned int token_id, string cant_advance, string fail){
    if( !advance_index(index,tokens.size()) ){
        die(cant_advance);
    }
    if(tokens[index].id != token_id){
        die(fail);
    }
}

// Token Enforcement Sequences
void tokens_value(const TokenList& tokens, unsigned int& index, Scope** scope, Template& type){
    int balance = 0;
    index_increase(tokens, index);

    while(balance != 0 or (tokens[index].id != TOKENINDEX_NEXT and tokens[index].id != TOKENINDEX_TERMINATE and tokens[index].id != TOKENINDEX_CLOSE)){
        if(tokens[index].id == TOKENINDEX_OPEN){
            balance++;
        }
        else if(tokens[index].id == TOKENINDEX_CLOSE){
            balance--;
        }
        else if(tokens[index].id == TOKENINDEX_WORD){
            if( !environment_variable_exists(*scope, Variable{tokens[index].data,IGNORE}) ){
                fail(UNDECLARED_VARIABLE);
            }

            string variable_type = environment_variable_get(*scope, Variable{tokens[index].data,IGNORE}).type;

            if(type.name == ""){
                type.name = variable_type;
            }
            else if(type.name != variable_type){
                fail( INCOMPATIBLE_TEMPLATES(type.name,variable_type) );
            }
        }
        else if(tokens[index].id == TOKENINDEX_STRING_LITERAL){
            if(type.name == ""){
                type.name = "String";
            }
            else if(type.name != "String"){
                fail( INCOMPATIBLE_TEMPLATES(type.name,"String") );
            }
        }
        else if(tokens[index].id == TOKENINDEX_NUMERIC_LITERAL){
            if(type.name == ""){
                type.name = "Number";
            }
            else if(type.name != "Number"){
                fail( INCOMPATIBLE_TEMPLATES(type.name,"Number") );
            }
        }
        else if(tokens[index].id == TOKENINDEX_ADD
        or tokens[index].id == TOKENINDEX_SUBTRACT
        or tokens[index].id == TOKENINDEX_MULTIPLY
        or tokens[index].id == TOKENINDEX_DIVIDE
        or tokens[index].id == TOKENINDEX_ASSIGN
        or tokens[index].id == TOKENINDEX_NOT){
            // ok
        }
        else {
            die(UNEXPECTED_OPERATOR_INEXP);
        }

        index_increase(tokens, index);
    }
}
void tokens_arguments(const TokenList& tokens, unsigned int& index){
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
            die(UNEXPECTED_OPERATOR_INEXP);
        }
    }
}
void tokens_method_declaration_arguments(const TokenList& tokens, unsigned int& index, Environment& environment, Scope** scope, string& method_arguments){
    string type;

    token_force(tokens, index, TOKENINDEX_OPEN, ERROR_INDICATOR + "Unexpected statement termination\nExpected opening parentheses", ERROR_INDICATOR + "Expected opening parentheses");

    if(!advance_index(index,tokens.size())){
        die(UNEXPECTED_TERMINATE);
    }

    bool accept_type = true;

    while(tokens[index].id != TOKENINDEX_CLOSE){
        while(tokens[index].id != TOKENINDEX_ASSIGN and tokens[index].id != TOKENINDEX_NEXT and tokens[index].id != TOKENINDEX_CLOSE){
            if(tokens[index].id ==TOKENINDEX_WORD){
                if(environment_template_exists(&environment, Template{tokens[index].data})){
                    if(accept_type){
                        method_arguments += tokens[index].data;
                        type += tokens[index].data;
                    }
                    else {
                        die(UNEXPECTED_WORD);
                    }
                }
                else {
                    if(accept_type){
                        accept_type = false;
                        (*scope)->variables.push_back( Variable{tokens[index].data, type} );
                    }
                    else {
                        die(UNEXPECTED_WORD);
                    }
                }
            }
            else {
                die(UNEXPECTED_OPERATOR);
            }

            if(!advance_index(index,tokens.size())){
                die(UNEXPECTED_TERMINATE);
            }
        }

        if(tokens[index].id == TOKENINDEX_ASSIGN){
            Template value_template;
            tokens_value(tokens, index, scope, value_template);
        }
        else if(tokens[index].id == TOKENINDEX_WORD){
            Template value_template;

            tokens_value(tokens, index, scope, value_template);
            if(!advance_index(index,tokens.size())){
                die(UNEXPECTED_TERMINATE);
            }
        }
        else if(tokens[index].id == TOKENINDEX_NEXT){
            method_arguments += ",";
            type = "";
            accept_type = true;

            if(!advance_index(index,tokens.size())){
                die(UNEXPECTED_TERMINATE);
            }
        }
        else if(tokens[index].id == TOKENINDEX_CLOSE){

        }
    }
}

// Validate Token Statement
void enforce_token(const TokenList& tokens, unsigned int& index, Scope** scope, Environment& environment){
    static unsigned int next_block = 0;

    if(tokens[index].id == TOKENINDEX_TERMINATE){
        // Do nothing
    }
    else if(tokens[index].id == TOKENINDEX_STRING_LITERAL or tokens[index].id == TOKENINDEX_NUMERIC_LITERAL){
        if(*scope == &environment.global){
            die(GLOBAL_STATEMENT);
        }

        token_force(tokens, index, TOKENINDEX_MEMBER, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");

        while(tokens[index].id == TOKENINDEX_MEMBER){
            token_force(tokens, index, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
            tokens_arguments(tokens,index);
            index_increase(tokens, index);
        }

        index_decrease(tokens, index);
        token_force(tokens, index, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
    }
    else if(tokens[index].id == TOKENINDEX_OPEN){
        if(*scope == &environment.global){
            die(GLOBAL_STATEMENT);
        }

        Template expression_type;

        tokens_value(tokens,index,scope,expression_type);
        token_force(tokens, index, TOKENINDEX_MEMBER, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");

        while(tokens[index].id == TOKENINDEX_MEMBER){
            token_force(tokens, index, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
            tokens_arguments(tokens,index);
            index_increase(tokens, index);
        }

        index_decrease(tokens, index);
        token_force(tokens, index, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
    }
    else if(tokens[index].id == TOKENINDEX_INDENT){
        (*scope)->children.push_back(new Scope{"block_" + to_string(next_block), (*scope)});
        (*scope) = (*scope)->children[(*scope)->children.size()-1];

        next_block++;
    }
    else if(tokens[index].id == TOKENINDEX_DEDENT){
        if((*scope)->parent != NULL){
            (*scope) = (*scope)->parent;
        }
    }
    else if(tokens[index].id == TOKENINDEX_KEYWORD){
        if(tokens[index].data == "on"){
            string method_name;
            string method_arguments;
            string method_return_type;

            token_force(tokens, index, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method name after 'on'", ERROR_INDICATOR + "Expected method name after 'on'");
            method_name = tokens[index].data;

            (*scope)->children.push_back(new Scope{"method_" + method_name, (*scope)});
            (*scope) = (*scope)->children[(*scope)->children.size()-1];

            tokens_method_declaration_arguments(tokens, index, environment, scope, method_arguments);

            token_force(tokens, index, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");

            environment.methods.push_back( Method{method_name, &environment.global, method_arguments, "void"} );
            index_increase(tokens, index);
            if(tokens[index].id != TOKENINDEX_INDENT){
                index_decrease(tokens, index);
            }
        }
        else if(tokens[index].data == "return"){
            Template value_template;

            tokens_value(tokens, index, scope, value_template);
            environment.methods[environment.methods.size()-1].return_type = value_template.name;
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
            if(*scope == &environment.global){
                die(GLOBAL_STATEMENT);
            }

            index--;
            tokens_arguments(tokens, index);

            if( !advance_index(index, tokens.size()) ){
                die(ERROR_INDICATOR + UNEXPECTED_TERMINATE);
            }

            while(tokens[index].id == TOKENINDEX_MEMBER){
                token_force(tokens, index, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                tokens_arguments(tokens,index);

                if( !advance_index(index, tokens.size()) ){
                    die(ERROR_INDICATOR + UNEXPECTED_TERMINATE);
                }
            }

            index--;
            token_force(tokens, index, TOKENINDEX_TERMINATE,  ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        }
        else {
            retreat_index(index);

            if( !environment_variable_exists(*scope, Variable{tokens[index].data,IGNORE}) ){
                fail(UNDECLARED_VARIABLE);
            }
        }
    }
    else {
        die(UNEXPECTED_OPERATOR);
    }
}

// Validate Token Stream
Environment enforce(TokenList tokens){
    // Enforce grammer and context

    Environment environment;
    Scope* scope = &environment.global;

    environment.templates.push_back( Template{"Number"} );
    environment.templates.push_back( Template{"String"} );

    for(unsigned int index = 0; index < tokens.size(); index++){
        enforce_token(tokens,index,&scope,environment);
    }

    if( !environment_method_exists(&environment, Method{"main", &environment.global, "", "void"}) ){
        die(NO_MAIN);
    }

    if(error_count > 0){
        exit(1);
    }

    return environment;
}

