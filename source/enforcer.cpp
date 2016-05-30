
#include <iostream>
#include <stdlib.h>
#include "../include/die.h"
#include "../include/log.h"
#include "../include/core.h"
#include "../include/scope.h"
#include "../include/errors.h"
#include "../include/context.h"
#include "../include/enforcer.h"
#include "../include/management.h"

using namespace std;

std::vector<TemplateAdditions> template_additions;
void tokens_arguments(const TokenList& tokens, unsigned int& index, Environment& environment, string& base_template);

void template_add_method(string name, string function_declaration){

    for(unsigned int i = 0; i < template_additions.size(); i++){
        if(template_additions[i].name == name){
            template_additions[i].additions += function_declaration;
            return;
        }
    }

    template_additions.push_back( TemplateAdditions{name, function_declaration} );
}

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
void tokens_value(const TokenList& tokens, unsigned int& index, Environment& environment, Template& type){
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
            std::string name = tokens[index].data;
            index_increase(tokens, index);

            if(tokens[index].id == TOKENINDEX_OPEN){ // Function call
                if(!context_method_exists(environment, Method{name, NULL, IGNORE_ARGS, IGNORE})){
                    fail(UNDECLARED_METHOD(name));
                }

                // ...
            }
            else { // Variable with possible method calls after
                if( !environment_variable_exists(environment.scope, Variable{name,IGNORE}) ){
                    fail(UNDECLARED_VARIABLE(tokens[index].data));
                }

                string variable_type = environment_variable_get(environment.scope, Variable{name,IGNORE}).type;

                while(tokens[index].id == TOKENINDEX_MEMBER){
                    if( environment_template_exists(&environment.global, Template{variable_type}) ){
                        index_increase(tokens, index);

                        if( environment_template_variable_exists(environment,Template{variable_type},Variable{tokens[index].data,IGNORE}) ){
                            variable_type = environment_template_variable_get(environment,Template{variable_type},Variable{tokens[index].data,IGNORE}).type;
                        }
                        else {
                            index_decrease(tokens, index);
                            token_force(tokens, index, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                            tokens_arguments(tokens, index, environment, variable_type);
                            index_increase(tokens, index);
                        }
                    }
                    else {
                        die(UNDECLARED_TEMPLATE(variable_type));
                    }
                }

                if(type.name == ""){
                    type.name = variable_type;
                }
                else if(type.name != variable_type){
                    fail( INCOMPATIBLE_TEMPLATES(type.name,variable_type) );
                }
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
void tokens_arguments(const TokenList& tokens, unsigned int& index, Environment& environment, string& base_template){
    int balance = 1;
    vector<MethodArgument> arguments;
    string type;
    string method_name = tokens[index].data;

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
        else if(tokens[index].id == TOKENINDEX_WORD){
            index_increase(tokens,index);

            if(tokens[index].id == TOKENINDEX_OPEN){ // Function Call
                if(environment.scope == &environment.global){
                    die(GLOBAL_STATEMENT);
                }

                index_decrease(tokens,index);
                string method_name = tokens[index].data;
                string return_type = "";
                tokens_arguments(tokens, index, environment, return_type);

                if( !advance_index(index, tokens.size()) ){
                    die(ERROR_INDICATOR + UNEXPECTED_TERMINATE);
                }

                index_increase(tokens, index);
                while(tokens[index].id == TOKENINDEX_MEMBER){
                    token_force(tokens, index, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                    tokens_arguments(tokens, index, environment, return_type);
                    index_increase(tokens, index);
                }

                index_decrease(tokens, index);

                if(return_type == "void"){
                    die(CANT_PASS_VOID_AS_ARGUMENT);
                }
                else if(type == ""){
                    type = return_type;
                }
                else if(type != return_type){
                    fail(INCOMPATIBLE_TEMPLATES(type, return_type));
                }
            }
            else {
                index_decrease(tokens,index);

                if( !environment_variable_exists(environment.scope, Variable{tokens[index].data,IGNORE}) and !environment_variable_exists(environment.scope, Variable{tokens[index].data,IGNORE})){
                    die(UNDECLARED_VARIABLE(tokens[index].data));
                }

                string variable_type;

                if(environment_variable_exists(environment.scope, Variable{tokens[index].data,IGNORE})){
                    variable_type = environment_variable_get(environment.scope, Variable{tokens[index].data,IGNORE}).type;
                }
                else if(environment_variable_exists(&environment.global, Variable{tokens[index].data,IGNORE})){
                    variable_type = environment_variable_get(&environment.global, Variable{tokens[index].data,IGNORE}).type;
                }

                index_increase(tokens, index);
                while(tokens[index].id == TOKENINDEX_MEMBER){
                    token_force(tokens, index, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                    tokens_arguments(tokens, index, environment, variable_type);
                    index_increase(tokens, index);
                }

                index_decrease(tokens, index);

                if(variable_type == "void"){
                    die(CANT_PASS_VOID_AS_ARGUMENT);
                }
                else if(type == ""){
                    type = variable_type;
                }
                else if(type != variable_type){
                    fail(INCOMPATIBLE_TEMPLATES(type, variable_type));
                }
            }
        }
        else if(tokens[index].id == TOKENINDEX_STRING_LITERAL){
            string base_template = "String";

            index_increase(tokens, index);
            while(tokens[index].id == TOKENINDEX_MEMBER){
                token_force(tokens, index, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                tokens_arguments(tokens, index, environment, base_template);
                index_increase(tokens, index);
            }

            index_decrease(tokens, index);

            if(base_template == "void"){
                die(CANT_PASS_VOID_AS_ARGUMENT);
            }
            else if(type == ""){
                type = base_template;
            }
            else if(type != base_template){
                fail(INCOMPATIBLE_TEMPLATES(type, base_template));
            }
        }
        else if(tokens[index].id == TOKENINDEX_NUMERIC_LITERAL){
            string base_template = "Number";

            index_increase(tokens, index);
            while(tokens[index].id == TOKENINDEX_MEMBER){
                token_force(tokens, index, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                tokens_arguments(tokens, index, environment, base_template);
                index_increase(tokens, index);
            }

            index_decrease(tokens, index);

            if(base_template == "void"){
                die(CANT_PASS_VOID_AS_ARGUMENT);
            }
            else if(type == ""){
                type = base_template;
            }
            else if(type != base_template){
                fail(INCOMPATIBLE_TEMPLATES(type, base_template));
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
        else if(tokens[index].id == TOKENINDEX_NEXT){
            arguments.push_back( MethodArgument{Template{type},0} );
            type = "";
        }
        else {
            die(UNEXPECTED_OPERATOR_INEXP);
        }
    }

    if(type != ""){
        arguments.push_back( MethodArgument{Template{type},0} );
        type = "";
    }

    string prefix;

    if(base_template == "void"){
        die(CANT_CALL_METHODS_OF_VOID);
    }
    else if(base_template != ""){
        prefix = base_template + ".";
    }

    if( !environment_method_exists(environment.scope, Method{prefix + method_name, &environment.global, arguments, IGNORE})
    and !environment_method_exists(&environment.global, Method{prefix + method_name, &environment.global, arguments, IGNORE})){
        if(environment_method_exists(environment.scope, Method{prefix + method_name, &environment.global, IGNORE_ARGS, IGNORE}) or environment_method_exists(&environment.global, Method{prefix + method_name, &environment.global, IGNORE_ARGS, IGNORE})){
            string call_string = prefix + method_name + "(";

            for(unsigned int i = 0; i < arguments.size(); i++){
                call_string += arguments[i].type.name;

                if(i != arguments.size()-1){
                    call_string += ", ";
                }
            }

            call_string += ")";
            die(UNDECLARED_METHOD_OTHERS(call_string));
        }
        else {
            die(UNDECLARED_METHOD(prefix + method_name));
        }
    }

    if(environment_method_exists(environment.scope, Method{prefix + method_name, &environment.global, arguments, IGNORE})){
        base_template = environment_method_get(environment.scope, Method{prefix + method_name, &environment.global, arguments, IGNORE}).return_type;
    }
    else if(environment_method_exists(&environment.global, Method{prefix + method_name, &environment.global, arguments, IGNORE})){
        base_template = environment_method_get(&environment.global, Method{prefix + method_name, &environment.global, arguments, IGNORE}).return_type;
    }
    else {
        log_enforcer("Method exists, but no implementation for the scope");
    }
}
void tokens_method_declaration_arguments(const TokenList& tokens, unsigned int& index, Environment& environment, std::vector<MethodArgument>& method_arguments, string& argument_string){
    string type;
    bool optional = false;

    string variable_name;
    string template_name;
    string method_name = tokens[index].data;

    if( string_count(method_name, ".") == 1 ){
        template_name = string_get_until(method_name, ".");
        method_name = string_delete_amount(string_delete_until(method_name, "."), 1);

        if(!is_identifier(template_name)){
            die(INVALID_TEMPLATE_NAME(template_name));
        }

        if( !environment_template_exists(environment.scope, Template{template_name}) and !environment_template_exists(&environment.global, Template{template_name}) ){
            die(UNDECLARED_TEMPLATE(template_name));
        }

        if(!is_identifier(method_name)){
            die(INVALID_METHOD_NAME(tokens[index].data));
        }
    }
    else {
        if(!is_identifier(method_name)){
            die(INVALID_METHOD_NAME(tokens[index].data));
        }
    }

    token_force(tokens, index, TOKENINDEX_OPEN, ERROR_INDICATOR + "Unexpected statement termination\nExpected opening parentheses", ERROR_INDICATOR + "Expected opening parentheses");

    if(!advance_index(index,tokens.size())){
        die(UNEXPECTED_TERMINATE);
    }

    bool accept_type = true;
    bool first_word = true;

    while(tokens[index].id != TOKENINDEX_CLOSE){
        // Before assign, close, or next tokens
        while(tokens[index].id != TOKENINDEX_ASSIGN and tokens[index].id != TOKENINDEX_NEXT and tokens[index].id != TOKENINDEX_CLOSE){
            if(tokens[index].id ==TOKENINDEX_WORD){
                if(environment_template_exists(environment.scope, Template{tokens[index].data}) or environment_template_exists(&environment.global, Template{tokens[index].data})){
                    if(accept_type){
                        type += tokens[index].data;
                        argument_string += tokens[index].data;
                    }
                    else {
                        die(UNEXPECTED_WORD);
                    }
                }
                else {
                    if(accept_type){
                        if(!first_word){ // Variable name
                            accept_type = false;
                            environment.scope->variables.push_back( Variable{tokens[index].data, type} );
                            variable_name = tokens[index].data;
                        }
                        else { // Undeclared template
                            die(UNDECLARED_TEMPLATE(tokens[index].data));
                        }
                    }
                    else {
                        die(UNEXPECTED_WORD_AFTER_VARNAME(variable_name));
                    }
                }
            }
            else {
                die(UNEXPECTED_OPERATOR);
            }

            if(!advance_index(index,tokens.size())){
                die(UNEXPECTED_TERMINATE);
            }

            first_word = false;
        }

        // After That
        if(tokens[index].id == TOKENINDEX_ASSIGN){
            Template value_template;
            tokens_value(tokens, index, environment, value_template);
            optional = true;
        }
        else if(tokens[index].id == TOKENINDEX_WORD){
            Template value_template;

            tokens_value(tokens, index, environment, value_template);
            if(!advance_index(index,tokens.size())){
                die(UNEXPECTED_TERMINATE);
            }
        }
        else if(tokens[index].id == TOKENINDEX_NEXT){
            method_arguments.push_back( MethodArgument{Template{type}, optional} );

            argument_string += ",";
            type = "";
            accept_type = true;

            if(!advance_index(index,tokens.size())){
                die(UNEXPECTED_TERMINATE);
            }
        }
        else if(tokens[index].id == TOKENINDEX_CLOSE){

        }

        first_word = true;
    }

    if(type != ""){
        method_arguments.push_back( MethodArgument{Template{type}, optional} );
    }
}

// Validate Token Statement
void enforce_token(const TokenList& tokens, unsigned int& index, Environment& environment){
    static unsigned int next_block = 0;

    if(tokens[index].id == TOKENINDEX_TERMINATE){
        // Do nothing
    }
    else if(tokens[index].id == TOKENINDEX_STRING_LITERAL or tokens[index].id == TOKENINDEX_NUMERIC_LITERAL){
        if(environment.scope == &environment.global){
            die(GLOBAL_STATEMENT);
        }

        string base_template;

        if(tokens[index].id == TOKENINDEX_STRING_LITERAL){
            base_template = "String";
        }
        else if(tokens[index].id == TOKENINDEX_NUMERIC_LITERAL){
            base_template = "Number";
        }

        token_force(tokens, index, TOKENINDEX_MEMBER, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");

        while(tokens[index].id == TOKENINDEX_MEMBER){
            token_force(tokens, index, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
            tokens_arguments(tokens, index, environment, base_template);
            index_increase(tokens, index);
        }

        index_decrease(tokens, index);
        token_force(tokens, index, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
    }
    else if(tokens[index].id == TOKENINDEX_OPEN){
        if(environment.scope == &environment.global){
            die(GLOBAL_STATEMENT);
        }

        Template expression_type;

        tokens_value(tokens, index, environment, expression_type);
        token_force(tokens, index, TOKENINDEX_MEMBER, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");

        while(tokens[index].id == TOKENINDEX_MEMBER){
            token_force(tokens, index, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
            tokens_arguments(tokens, index, environment, expression_type.name);
            index_increase(tokens, index);
        }

        index_decrease(tokens, index);
        token_force(tokens, index, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
    }
    else if(tokens[index].id == TOKENINDEX_INDENT){
        environment.scope->children.push_back(new Scope{"block_" + to_string(next_block), environment.scope});
        environment.scope = environment.scope->children[environment.scope->children.size()-1];

        next_block++;
    }
    else if(tokens[index].id == TOKENINDEX_DEDENT){
        if(environment.scope->parent != NULL){
            environment.scope = environment.scope->parent;
        }
    }
    else if(tokens[index].id == TOKENINDEX_KEYWORD){
        if(tokens[index].data == "on"){
            string method_name;
            vector<MethodArgument> method_arguments;
            string arguments_string;
            string method_return_type;

            token_force(tokens, index, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method name after 'on'", ERROR_INDICATOR + "Expected method name after 'on'");
            method_name = tokens[index].data;

            environment.scope->children.push_back(new Scope{METHOD_PREFIX + method_name, environment.scope});
            environment.scope = environment.scope->children[environment.scope->children.size()-1];

            tokens_method_declaration_arguments(tokens, index, environment, method_arguments, arguments_string);

            token_force(tokens, index, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");

            environment.global.methods.push_back( Method{method_name, &environment.global, method_arguments, "void"} );

            if( string_count(method_name,".") == 1 ){
                environment.scope->variables.push_back( Variable{"self", string_get_until(method_name,".")} );
            }

            index_increase(tokens, index);
            if(tokens[index].id != TOKENINDEX_INDENT){
                index_decrease(tokens, index);
            }
        }
        else if(tokens[index].data == "return"){
            Template value_template;

            tokens_value(tokens, index, environment, value_template);

            environment.global.methods[environment.global.methods.size()-1].return_type = value_template.name;
            // ...
        }
        else if(tokens[index].data == "template"){
            string template_name;

            token_force(tokens, index, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected template name in template declaration", ERROR_INDICATOR + "Expected template name in template declaration");
            template_name = tokens[index].data;
            token_force(tokens, index, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");

            environment.scope->children.push_back(new Scope{TEMPLATE_PREFIX + template_name, environment.scope});
            environment.scope = environment.scope->children[environment.scope->children.size()-1];

            environment.global.templates.push_back( Template{template_name} );

            index_increase(tokens, index);
            if(tokens[index].id != TOKENINDEX_INDENT){
                index_decrease(tokens, index);

                if(environment.scope->parent != NULL){
                    environment.scope = environment.scope->parent;
                }
            }
        }
        else {
            die("Unknown keyword " + tokens[index].data);
        }
    }
    else if(tokens[index].id == TOKENINDEX_WORD){
        string template_name = tokens[index].data;

        if( !advance_index(index, tokens.size()) ){
            die(ERROR_INDICATOR + UNEXPECTED_TERMINATE);
        }

        if(tokens[index].id == TOKENINDEX_OPEN){ // Function Call
            if(environment.scope == &environment.global){
                die(GLOBAL_STATEMENT);
            }

            index--;
            string method_name = tokens[index].data;
            string return_type = "";
            tokens_arguments(tokens, index, environment, return_type);

            if( !advance_index(index, tokens.size()) ){
                die(ERROR_INDICATOR + UNEXPECTED_TERMINATE);
            }

            while(tokens[index].id == TOKENINDEX_MEMBER){
                token_force(tokens, index, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");

                tokens_arguments(tokens, index, environment, return_type);

                if( !advance_index(index, tokens.size()) ){
                    die(ERROR_INDICATOR + UNEXPECTED_TERMINATE);
                }
            }

            index--;
            token_force(tokens, index, TOKENINDEX_TERMINATE,  ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        }
        else if(environment_template_exists(&environment.global, Template{template_name})){
            // Variable Declaration

            string type = template_name;

            while( tokens[index].id == TOKENINDEX_WORD
            and    environment_template_exists(&environment.global, Template{tokens[index].data})){
                type += " " + tokens[index].data;
                index_increase(tokens, index);
            }

            if(tokens[index].id != TOKENINDEX_WORD){
                die(UNEXPECTED_OPERATOR);
            }

            string variable_name = tokens[index].data;
            environment.scope->variables.push_back( Variable{variable_name, type} );
            token_force(tokens, index, TOKENINDEX_TERMINATE,  ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        }
        else {
            // Variable might be declared

            retreat_index(index);

            if( !environment_variable_exists(environment.scope, Variable{tokens[index].data, IGNORE}) ){
                fail(UNDECLARED_VARIABLE(tokens[index].data));
            }

            Template base_template;

            if(environment_variable_exists(environment.scope, Variable{tokens[index].data,IGNORE})){
                base_template.name = environment_variable_get(environment.scope, Variable{tokens[index].data,IGNORE}).type;
            }
            else if(environment_variable_exists(&environment.global, Variable{tokens[index].data,IGNORE})){
                base_template.name = environment_variable_get(&environment.global, Variable{tokens[index].data,IGNORE}).type;
            }

            index_increase(tokens, index);

            while(tokens[index].id == TOKENINDEX_MEMBER){
                if( environment_template_exists(&environment.global, base_template) ){
                    index_increase(tokens, index);

                    if( environment_template_variable_exists(environment,base_template,Variable{tokens[index].data,IGNORE}) ){
                        base_template.name = environment_template_variable_get(environment,base_template,Variable{tokens[index].data,IGNORE}).type;
                    }
                    else {
                        index_decrease(tokens, index);
                        token_force(tokens, index, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                        tokens_arguments(tokens, index, environment, base_template.name);
                        index_increase(tokens, index);
                    }
                }
                else {
                    die(UNDECLARED_TEMPLATE(template_name));
                }
            }

            index_increase(tokens, index);

            if(tokens[index].id == TOKENINDEX_ASSIGN){
                index_increase(tokens, index);
                tokens_value(tokens, index, environment, base_template);
            }
            else {
                index_decrease(tokens, index);
            }

            // At this point token should be a terminate
            index_decrease(tokens, index);
            token_force(tokens, index, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
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

    // Load Boomslang Core Templates
    load_core(environment);

    for(unsigned int index = 0; index < tokens.size(); index++){
        enforce_token(tokens, index, environment);
    }

    if( !environment_method_exists(environment.scope, Method{"main", &environment.global, std::vector<MethodArgument>(), IGNORE}) ){
        die(NO_MAIN);
    }

    if(error_count > 0){
        exit(1);
    }

    return environment;
}

