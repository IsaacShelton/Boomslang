
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

#include <iostream>
#include <stdlib.h>
#include "../include/die.h"
#include "../include/log.h"
#include "../include/core.h"
#include "../include/file.h"
#include "../include/dump.h"
#include "../include/scope.h"
#include "../include/lexer.h"
#include "../include/errors.h"
#include "../include/locate.h"
#include "../include/context.h"
#include "../include/enforcer.h"
#include "../include/management.h"

Environment enforce(Configuration* config, TokenList& tokens){
    // Enforce grammer and context
    Environment environment;

    // Load Boomslang Core Classes
    load_core(environment);

    // Validate Tokens
    unsigned int index;
    for(index = 0; index < tokens.size(); index++){
        enforce_token(config, TokenContext{tokens, index}, environment);
    }

    // Make sure a main method was declared
    if( !environment_method_exists(TokenContext{tokens, index}, environment.scope, Method{"main", &environment.global, NO_ARGUMENTS, IGNORE_CLASS}) and !config->package ){
        die(NO_MAIN);
    }

    // Exit if mild errors were encountered
    if(error_count > 0){
        exit(1);
    }

    logging_context.log_enforcer("The Program is Valid");
    return environment;
}
void enforce_package(Configuration* config, TokenList& tokens, Environment& environment){
    for(unsigned int index = 0; index < tokens.size(); index++){
        enforce_token(config, TokenContext{tokens, index}, environment);
    }

    if(error_count > 0) exit(1);
    logging_context.log_enforcer("The Package is Valid");
}

void enforce_token(Configuration* config, TokenContext context, Environment& environment){
    static unsigned int next_block = 0;
    static unsigned int next_lib = 0;
    static bool private_member = false;

    logging_context.log_enforcer("Enforcing Token '" + token_name(tokenof(context)) + "' with a value of '" + tokendata(context) + "'");

    switch(tokenid(context)){
    case TOKENINDEX_TERMINATE:
        current_line++;
        break;
    case TOKENINDEX_STRING_LITERAL:
    case TOKENINDEX_NUMERIC_LITERAL:
    case TOKENINDEX_DOUBLE_LITERAL:
    case TOKENINDEX_FLOAT_LITERAL:
    case TOKENINDEX_INTEGER_LITERAL:
    case TOKENINDEX_UNSIGNED_LITERAL:
        enforce_literal(config, context, environment);
        break;
    case TOKENINDEX_OPEN:
        enforce_beginning_expression(config, context, environment);
        break;
    case TOKENINDEX_INDENT:
        environment.scope->children.push_back(new Scope{"block_" + to_string(next_block), environment.scope});
        environment.scope = environment.scope->children[environment.scope->children.size()-1];
        next_block++;
        break;
    case TOKENINDEX_DEDENT:
        if(environment.scope->parent != NULL) environment.scope = environment.scope->parent;
        if(environment.scope->parent == NULL) private_member = false;
        break;
    case TOKENINDEX_KEYWORD:
        enforce_keyword(config, context, environment, private_member, next_block);
        break;
    case TOKENINDEX_MACRO:
        if(tokendata(context) == "error"){
            index_increase(context);
            if(tokenid(context) != TOKENINDEX_STRING_LITERAL) die(UNEXPECTED_OPERATOR);
            die(ERROR_INDICATOR + tokendata(context));
        }
        else die(UNKNOWN_MACRO(tokendata(context)));

        index_decrease(context);
        token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        current_line++;
        break;
    case TOKENINDEX_WORD:
        enforce_word(config, context, environment, private_member);
        break;
    case TOKENINDEX_POINTER:
        enforce_pointer(config, context, environment);
        break;
    case TOKENINDEX_HEADERFILE:
        break;
    case TOKENINDEX_LIBRARYFILE:
        enforce_library_file(context, next_lib);
        break;
    case TOKENINDEX_SETFILENAME:
        current_filename = tokendata(context);
        break;
    case TOKENINDEX_SETLINENUMBER:
        current_line = to_double( tokendata(context) );
        break;
    default:
        die(UNEXPECTED_OPERATOR);
    }

}
void enforce_literal(Configuration* config, TokenContext context, Environment& environment){
    if(environment.scope == &environment.global) die(GLOBAL_STATEMENT);
    Class base_class;

    switch(tokenid(context)){
    case TOKENINDEX_STRING_LITERAL:
        base_class.name = "String";
        context_enforce_string(context, environment, tokendata(context));
        break;
    case TOKENINDEX_NUMERIC_LITERAL:
        base_class.name = "Double";
        break;
    case TOKENINDEX_DOUBLE_LITERAL:
        base_class.name = "Double";
        break;
    case TOKENINDEX_FLOAT_LITERAL:
        base_class.name = "Float";
        break;
    case TOKENINDEX_INTEGER_LITERAL:
        base_class.name = "Integer";
        break;
    case TOKENINDEX_UNSIGNED_LITERAL:
        base_class.name = "UnsignedInteger";
        break;
    }

    token_force(context, TOKENINDEX_MEMBER, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");

    while(tokenid(context) == TOKENINDEX_MEMBER){
        token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
        context_enforce_arguments(context, environment, base_class);
        index_increase(context);
    }

    index_decrease(context);
    token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
    current_line++;
}
void enforce_beginning_expression(Configuration* config, TokenContext context, Environment& environment){
    if(environment.scope == &environment.global) die(GLOBAL_STATEMENT);
    Class expression_type;

    index_increase(context);
    context_enforce_expression(context, environment, expression_type);
    token_force(context, TOKENINDEX_MEMBER, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");

    while(tokenid(context) == TOKENINDEX_MEMBER){
        bool dereferenced = context_class_dereference_ifcan(context, expression_type);
        if(dereferenced) tokenid(context) = TOKENINDEX_POINTERMEMBER;

        token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
        context_enforce_arguments(context, environment, expression_type);
        index_increase(context);
    }

    index_decrease(context);
    token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
    current_line++;
}
void enforce_keyword(Configuration* config, TokenContext context, Environment& environment, bool& private_member, unsigned int& next_block){
    if(tokendata(context) == "new"){
        die(UNEXPECTED_KEYWORD("new"));
    }
    else if(tokendata(context) == "create"){
        die(UNEXPECTED_KEYWORD("create"));
    }
    else if(tokendata(context) == "delete"){
        Class base;
        index_increase(context);
        context_enforce_expression(context, environment, base);
        if(!context_class_can_dereference(context, base)) die(CANT_DEREFERENCE_NON_POINTER_VALUE);
        index_decrease(context);
        token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        current_line++;
    }
    else if(tokendata(context) == "def"){
        std::string method_name;
        std::vector<MethodArgument> method_arguments;
        std::string arguments_string;
        std::string method_return_type;
        bool is_static = false;

        index_increase(context);
        while(tokenid(context) == TOKENINDEX_KEYWORD and (tokendata(context) == "static" or
            tokendata(context) == "public" or tokendata(context) == "private"
        )){
            if(tokendata(context) == "static"){
                if(is_static) fail(ALREADY_STATIC);
                is_static = true;
            }
            else if(tokendata(context) == "public"){
                private_member = false;

                if( !name_is_class(environment.scope->name) ){
                    die(PUBLIC_MUST_BE_IN_CLASS);
                }
            }
            else if(tokendata(context) == "private"){
                private_member = true;

                if( !name_is_class(environment.scope->name) ){
                    die(PRIVATE_MUST_BE_IN_CLASS);
                }
            }

            index_increase(context);
        }

        index_decrease(context);
        token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method name after 'def'", ERROR_INDICATOR + "Expected method name after 'def'");
        method_name = tokendata(context);

        // Create method scope
        environment.scope->children.push_back( new Scope{METHOD_PREFIX + method_name, environment.scope} );
        environment.scope = environment.scope->children[environment.scope->children.size()-1];

        // Enforce method declaration arguments
        context_enforce_method_declaration_arguments(context, environment, method_arguments, arguments_string);
        token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        current_line++;

        // Add method reference to parent scope
        environment.scope->parent->methods.push_back( Method(method_name, environment.scope->parent, method_arguments, Class("void"), is_static) );

        std::string func_type = "function^(";
        for(unsigned int i = 0; i < method_arguments.size(); i++){
            func_type += method_arguments[i].type.name;

            if(i != method_arguments.size()-1){
                func_type += ",";
            }
        }
        func_type += ")->void";

        if( !name_is_class(environment.scope->parent->name) and environment.scope->parent->parent == NULL ){
            environment.global.variables.push_back( Variable(method_name, func_type) );
        }

        index_increase(context);
        if(tokenid(context) != TOKENINDEX_INDENT){
            index_decrease(context);
            environment.scope = environment.scope->parent;
        }
    }
    else if(tokendata(context) == "class"){
        std::string class_name;
        std::vector<Class> parents;
        std::vector<Class> generics;

        token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected class name in class declaration", ERROR_INDICATOR + "Expected class name in class declaration");
        class_name = tokendata(context);

        environment.scope->children.push_back(new Scope{CLASS_PREFIX + class_name, environment.scope});
        environment.scope = environment.scope->children[environment.scope->children.size()-1];

        // Get Generic Classes
        index_increase(context);
        if(tokenid(context) == TOKENINDEX_LESSTHAN){
            index_increase(context);

            while(tokenid(context) != TOKENINDEX_GREATERTHAN){

                if(tokenid(context) != TOKENINDEX_WORD){
                    die(UNEXPECTED_OPERATOR);
                }
                generics.push_back( Class(tokendata(context)) );
                index_increase(context);

                if(tokenid(context) == TOKENINDEX_NEXT){
                    index_increase(context);
                }
            }

            index_increase(context);
        }

        // Get Parent Classes
        while(tokenid(context) == TOKENINDEX_WORD){
            parents.push_back( tokendata(context) );
            index_increase(context);
        }
        context.index--;

        for(size_t i = 0; i < parents.size(); i++){
            Scope* parent_class_scope = environment_get_child(&environment.global, CLASS_PREFIX + parents[i].name);

            if( parent_class_scope == NULL ){
                die( UNDECLARED_CLASS(parents[i].name) );
            }

            for(size_t v = 0; v < parent_class_scope->variables.size(); v++){
                if( !parent_class_scope->variables[v].is_private ){
                    environment.scope->variables.push_back( parent_class_scope->variables[v] );
                }
            }
        }

        // Add self variable to methods of a type
        environment.scope->variables.push_back( Variable("self", Class(class_name), true, false, true) );

        Class child(class_name);
        child.parents = parents;
        child.generics = generics;
        private_member = false;

        environment.global.classes.push_back( child );

        token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        current_line++;

        index_increase(context);
        if(tokenid(context) != TOKENINDEX_INDENT){
            index_decrease(context);

            if(environment.scope->parent != NULL){
                environment.scope = environment.scope->parent;
            }
        }
    }
    else if(tokendata(context) == "return"){
        Class value_class;
        std::string method_name;

        index_increase(context);

        if(tokenid(context) != TOKENINDEX_TERMINATE){
            context_enforce_expression(context, environment, value_class);

            Scope* method_scope = environment.scope;

            while(method_scope->parent != NULL and !name_is_method(method_scope->name)){
                method_scope = method_scope->parent;
            }

            if(method_scope->parent == NULL){
                die(GLOBAL_STATEMENT);
            }
            method_name = name_get_method(method_scope->name);
            method_scope->parent->methods[method_scope->parent->methods.size()-1].return_type = value_class.name;

            for(unsigned int i = 0; i < environment.global.variables.size(); i++){
                if(environment.global.variables[i].name == method_name and environment.global.variables[i].type.name.length() >= 6){
                    if(environment.global.variables[i].type.name.substr(environment.global.variables[i].type.name.length()-6, 6) == "->void"){
                        environment.global.variables[i].type.name = environment.global.variables[i].type.name.substr(0, environment.global.variables[i].type.name.length()-4);
                        environment.global.variables[i].type.name += value_class.name;
                        break;
                    }
                }
            }
        }

        index_decrease(context);
        token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        current_line++;
    }
    else if(tokendata(context) == "break"){
        token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        current_line++;
    }
    else if(tokendata(context) == "continue"){
        token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        current_line++;
    }
    else if(tokendata(context) == "if"){
        Class value_class;
        index_increase(context);
        context_enforce_expression(context, environment, value_class);
        context_enforce_followup_block(config, context, environment);
        current_line++;
    }
    else if(tokendata(context) == "unless"){
        Class value_class;
        index_increase(context);
        context_enforce_expression(context, environment, value_class);
        context_enforce_followup_block(config, context, environment);
        current_line++;
    }
    else if(tokendata(context) == "else"){
        index_increase(context);

        if(tokenid(context) == TOKENINDEX_KEYWORD and tokendata(context) == "if"){ // else if
            Class value_class;

            index_increase(context);
            context_enforce_expression(context, environment, value_class);
            context_enforce_followup_block(config, context, environment);
        }
        else if(tokenid(context) == TOKENINDEX_KEYWORD and tokendata(context) == "unless"){ // else unless
            Class value_class;

            index_increase(context);
            context_enforce_expression(context, environment, value_class);
            context_enforce_followup_block(config, context, environment);
        }
        else {
            index_decrease(context);
            context_enforce_followup_block(config, context, environment);
            token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        }

        current_line++;
    }
    else if(tokendata(context) == "while"){
        Class value_class;

        index_increase(context);
        context_enforce_expression(context, environment, value_class);
        context_enforce_followup_block(config, context, environment);
        current_line++;
    }
    else if(tokendata(context) == "until"){
        Class value_class;

        index_increase(context);
        context_enforce_expression(context, environment, value_class);
        context_enforce_followup_block(config, context, environment);
        current_line++;
    }
    else if(tokendata(context) == "forever"){
        context_enforce_followup_block(config, context, environment);
        token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        current_line++;
    }
    else if(tokendata(context) == "for"){
        std::string var_name;
        Class var_type;
        Class expression_type;
        Class increament_type;

        // Get variable type
        index_increase(context);
        if(tokenid(context) == TOKENINDEX_KEYWORD and tokendata(context) == "var"){
            index_increase(context);
        }
        else {
            context_enforce_type(context, environment, var_type);
        }

        // Get variable name
        if(tokenid(context) != TOKENINDEX_WORD){
            die(UNEXPECTED_OPERATOR);
        }
        var_name = tokendata(context);
        index_increase(context);

        // Assign a value to the new variable?
        if(tokenid(context) == TOKENINDEX_ASSIGN){
            index_increase(context);
            context_enforce_expression(context, environment, var_type);
        }

        // Register the variable
        environment.scope->children.push_back(new Scope{"block_" + to_string(next_block), environment.scope});
        environment.scope = environment.scope->children[environment.scope->children.size()-1];
        next_block++;
        environment.scope->variables.push_back( Variable(var_name, var_type, false, false, false) );

        // Move on to the expression
        if(tokenid(context) != TOKENINDEX_NEXT){
            die(UNEXPECTED_OPERATOR);
        }

        // Enforce the expression
        index_increase(context);
        context_enforce_expression(context, environment, expression_type);

        // Move on to the increamentation
        if(tokenid(context) != TOKENINDEX_NEXT){
            die(UNEXPECTED_OPERATOR);
        }
        index_increase(context);
        context_enforce_expression(context, environment, increament_type);
        current_line++;

        // Check for start of block
        index_increase(context);
        if(tokenid(context) != TOKENINDEX_INDENT){
            index_decrease(context);
            environment.scope = environment.scope->parent;
        }
    }
    else if(tokendata(context) == "var"){
        std::string variable_name;
        Class value_class;
        unsigned int var_token = context.index;

        index_increase(context);
        if(tokenid(context) != TOKENINDEX_WORD){
            die(UNEXPECTED_OPERATOR_INEXP);
        }

        variable_name = tokendata(context);
        index_increase(context);

        if(tokenid(context) != TOKENINDEX_ASSIGN){
            die(UNEXPECTED_OPERATOR_INEXP);
        }

        index_increase(context);
        context_enforce_expression(context, environment, value_class);

        if(value_class.name.substr(0,10) == "function^("){
            die(VAR_CANT_INFER_FUNCTION_PTRS);
        }

        context.tokens[var_token] = TOKEN_RAW_WORD( resource_type(value_class.name) );
        environment.scope->variables.push_back( Variable(variable_name, value_class.name, false, false) );

        index_decrease(context);
        token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        current_line++;
    }
    else if(tokendata(context) == "import"){
        std::string package;
        std::string prev_filename;
        unsigned int prev_line = current_line;
        TokenList::iterator statement;
        bool is_package;
        TokenList tokens;
        Configuration custom_config = *config;
        custom_config.package = true;

        statement = context.tokens.begin() + context.index;
        index_increase(context);

        if(tokenid(context) != TOKENINDEX_WORD and tokenid(context) != TOKENINDEX_STRING_LITERAL){
            die(EXPECTED_PACKAGE_NAME_AFTER_IMPORT);
        }

        if(tokenid(context) == TOKENINDEX_WORD){
            is_package = true;
        }
        else {
            is_package = false;
        }

        package = tokendata(context);

        #ifdef __WIN32__
        package = string_replace_all(package, "/", "\\");
        #endif // __WIN32__

        if(is_package){
            package += ".branch";
        }

        if( file_exists(package) ){ // Plain Absolute Path
            prev_filename = current_filename;
            current_filename = package;

            if(is_package){
                current_line = 0;
                if(!tokens_load(current_filename, tokens)) die("Failed to load package '" + package + "'");
                enforce_package(&custom_config, tokens, environment);
                current_line = prev_line;
                current_filename = prev_filename;
            }
            else {
                tokens = tokenize(contents(current_filename));

                if(!config->package){
                    current_line = 0;
                    tokens.push_back( TOKEN_SETFILENAME(prev_filename) );
                    tokens.push_back( TOKEN_SETLINENUMBER( to_string(prev_line) ) );
                }

                context.tokens.erase(statement, statement + 2);
                context.index -= 2;

                context.tokens.insert(statement, tokens.begin(), tokens.end());
            }
        }
        else if( file_exists( filename_path(current_filename) + delete_slash(package) ) ){ // Relative Path
            prev_filename = current_filename;
            current_filename = filename_path(current_filename) + delete_slash(package);

            if(is_package){
                current_line = 0;
                if(!tokens_load(current_filename, tokens)) die("Failed to load package '" + package + "'");
                enforce_package(&custom_config, tokens, environment);
                current_line = prev_line;
                current_filename = prev_filename;
            }
            else {
                tokens = tokenize(contents(current_filename));

                if(!config->package){
                    current_line = 0;
                    tokens.push_back( TOKEN_SETFILENAME(prev_filename) );
                    tokens.push_back( TOKEN_SETLINENUMBER( to_string(prev_line) ) );
                }

                context.tokens.erase(statement, statement + 2);
                context.index -= 2;

                context.tokens.insert(statement, tokens.begin(), tokens.end());
            }
        }
        else if( file_exists( PACKAGEHOME + delete_slash(package) ) ){ // Package Path
            prev_filename = current_filename;
            current_filename = PACKAGEHOME + delete_slash(package);

            if(is_package){
                current_line = 0;
                if(!tokens_load(current_filename, tokens)) die("Failed to load package '" + package + "'");
                enforce_package(&custom_config, tokens, environment);
                current_line = prev_line;
                current_filename = prev_filename;
            }
            else {
                tokens = tokenize(contents(current_filename));

                if(!config->package){
                    current_line = 0;
                    tokens.push_back( TOKEN_SETFILENAME(prev_filename) );
                    tokens.push_back( TOKEN_SETLINENUMBER( to_string(prev_line) ) );
                }

                context.tokens.erase(statement, statement + 2);
                context.index -= 2;

                context.tokens.insert(statement, tokens.begin(), tokens.end());
            }
        }
        else {
            if(is_package){
                fail(PACKAGE_DOESNT_EXIST(package));
            }
            else {
                fail(FILE_DOESNT_EXIST(package));
            }
        }

        token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        current_line++;
    }
    else if(tokendata(context) == "native"){
        std::string action;
        std::string filename;
        TokenList::iterator statement = context.tokens.begin() + context.index;
        index_increase(context);

        if(tokenid(context) != TOKENINDEX_WORD){
            die(UNEXPECTED_OPERATOR);
        }
        action = tokendata(context);
        index_increase(context);

        if(tokenid(context) != TOKENINDEX_STRING_LITERAL){
            die(UNEXPECTED_OPERATOR);
        }
        filename = tokendata(context);

        if(action != "link" and action != "include"){
            die("Unknown native directive '" + action + "'");
        }

        token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        current_line++;

        if(action == "include"){
            if(file_exists( MINGWHOME + "include\\" + delete_slash(filename) )){
                filename = full_path(MINGWHOME + "include\\" + delete_slash(filename));
            }
            else if(file_exists( filename_path(current_filename) + delete_slash(filename) )){
                filename = full_path(filename_path(current_filename) + delete_slash(filename));
            }
            else if( file_exists(PACKAGEHOME + delete_slash(filename)) ){
                filename = full_path(PACKAGEHOME + delete_slash(filename));
            }
            else if(file_exists(filename)){
                filename = full_path(filename);
            }
            else {
                die(FILE_DOESNT_EXIST(full_path(filename)));
            }

            context.tokens.erase(statement, statement + 4);
            context.index -= 4;

            bool exists = false;
            for(unsigned int i = 0; i < config->included.size(); i++){
                if(config->included[i] == filename){
                    exists = true;
                    break;
                }
            }

            if(!exists){
                context.tokens.insert(statement, TOKEN_HEADERFILE(contents(filename)));
                config->included.push_back(filename);
            }
        }
        else if(action == "link" and file_exists(filename_path(current_filename) + delete_slash(filename))){
            filename = full_path(filename_path(current_filename) + delete_slash(filename));
            context.tokens.erase(statement, statement + 4);
            context.index -= 4;

            context.tokens.insert(statement, TOKEN_LIBRARYFILE(binary_contents(filename)));
        }
    }
    else if(tokendata(context) == "register"){
        std::string construct; // Type of Language Construct
        std::string structure; // Structure of Data

        index_increase(context);
        construct = tokendata(context);
        index_increase(context);
        structure = tokendata(context);

        if( construct != "class"
        and construct != "method"
        and construct != "function"
        and construct != "field"
        and construct != "constant" ){
            die(UNKNOWN_LANG_CONSTRUCT(construct));
        }

        if(construct == "class"){
            std::string class_name = string_get_until_or(string_kill_whitespace(structure), " \n");

            add_class(environment, Class(class_name));
        }
        else if(construct == "function"){
            std::string function_name;
            std::vector<MethodArgument> function_arguments;
            Class function_return_type;
            Class arg_type;
            unsigned int balance = 0;

            // Function Name
            structure = string_kill_whitespace(structure);
            function_name = string_get_until_or(structure, " (");
            structure = string_delete_amount(structure, function_name.length());
            structure = string_kill_whitespace(structure);

            // Function Arguments
            if(structure.substr(0,1) != "("){
                die(EXPECTED_ARGS_AFTER_FUNCTION_NAME);
            }
            structure = string_delete_amount(structure, 1);
            while(structure.substr(0,1) != ")" or balance != 0){
                if(structure.substr(0,1) == ","){
                    structure = string_delete_amount(structure, 1);
                }

                arg_type.name = "";
                structure = string_kill_whitespace(structure);

                for(unsigned int i = 0; !(structure[i] == ')' or structure[i] == ' ' or structure[i] == ',') or balance!=0; i++){
                    if(structure[i] == '('){
                        balance++;
                    }
                    else if(structure[i] == ')'){
                        balance--;
                    }

                    arg_type.name += structure[i];
                }

                structure = string_delete_amount(structure, arg_type.name.length());
                structure = string_kill_whitespace(structure);

                arg_type.name = string_flatten(arg_type.name);

                if(!context_class_exists(context, environment, arg_type)){
                    Class arg_type_clone = arg_type;
                    context_class_dereference_ifcan(context, arg_type_clone);
                    fail(UNDECLARED_CLASS(arg_type_clone.name));
                }

                function_arguments.push_back( MethodArgument{arg_type, false} );
            }
            structure = string_delete_amount(structure, 1);
            structure = string_kill_whitespace(structure);

            // Function Return Type
            if(structure.substr(0,2) != "->"){
                die(EXPECTED_RETURN_TYPE);
            }
            structure = string_delete_amount(structure, 2);
            structure = string_kill_whitespace(structure);
            function_return_type.load( string_get_until_or(structure, " \n") );
            structure = string_delete_until_or(structure, " \n");
            structure = string_kill_whitespace(structure);

            add_function(environment, Method{function_name, &environment.global, function_arguments, function_return_type});
        }
        else if(construct == "method"){
            std::string class_name;
            std::string function_name;
            std::vector<MethodArgument> function_arguments;
            Class function_return_type;
            Class arg_type;
            unsigned int balance = 0;

            // Function Name
            structure = string_kill_whitespace(structure);
            class_name = string_get_until(structure, " ");
            structure = string_delete_amount(structure, class_name.length());
            structure = string_kill_whitespace(structure);
            function_name = string_get_until_or(structure, " (");
            structure = string_delete_amount(structure, function_name.length());
            structure = string_kill_whitespace(structure);

            if(!context_class_exists(context, environment, Class(class_name))){
                die(UNDECLARED_CLASS(class_name));
            }

            // Function Arguments
            if(structure.substr(0,1) != "("){
                die(EXPECTED_ARGS_AFTER_FUNCTION_NAME);
            }
            structure = string_delete_amount(structure, 1);
            while(structure.substr(0,1) != ")" or balance != 0){
                if(structure.substr(0,1) == ","){
                    structure = string_delete_amount(structure, 1);
                }

                arg_type.name = "";
                structure = string_kill_whitespace(structure);

                for(unsigned int i = 0; !(structure[i] == ')' or structure[i] == ' ' or structure[i] == ',') or balance!=0; i++){
                    if(structure[i] == '('){
                        balance++;
                    }
                    else if(structure[i] == ')'){
                        balance--;
                    }

                    arg_type.name += structure[i];
                }

                structure = string_delete_amount(structure, arg_type.name.length());
                structure = string_kill_whitespace(structure);

                arg_type.name = string_flatten(arg_type.name);

                if(!context_class_exists(context, environment, arg_type)){
                    Class arg_type_clone = arg_type;
                    context_class_dereference_ifcan(context, arg_type_clone);
                    fail(UNDECLARED_CLASS(arg_type_clone.name));
                }

                function_arguments.push_back( MethodArgument{arg_type, false} );
            }
            structure = string_delete_amount(structure, 1);
            structure = string_kill_whitespace(structure);

            // Function Return Type
            if(structure.substr(0,2) != "->"){
                die(EXPECTED_RETURN_TYPE);
            }
            structure = string_delete_amount(structure, 2);
            structure = string_kill_whitespace(structure);
            function_return_type.load( string_get_until_or(structure, " \n") );
            structure = string_delete_until_or(structure, " \n");
            structure = string_kill_whitespace(structure);

            add_function(environment, Method{function_name, &environment.global, function_arguments, function_return_type});
            add_method(environment, Class(class_name), Method{function_name, environment_get_child(&environment.global, CLASS_PREFIX + class_name), function_arguments, function_return_type});
        }
        else if(construct == "constant"){
            std::string type = string_get_until(structure, " ");
            structure = string_delete_amount(structure, type.length());
            structure = string_kill_whitespace(structure);

            std::string name = string_get_until(structure, " ");
            structure = string_delete_amount(structure, name.length());
            structure = string_kill_whitespace(structure);

            if(!context_class_exists(context, environment, Class(type))){
                fail(UNDECLARED_CLASS(type));
            }

            environment.global.variables.push_back( Variable(name, type, true, false) );
        }
        else if(construct == "field"){
            std::string class_name = string_get_until(structure, " ");
            structure = string_delete_amount(structure, class_name.length());
            structure = string_kill_whitespace(structure);

            std::string type = string_get_until(structure, " ");
            structure = string_delete_amount(structure, type.length());
            structure = string_kill_whitespace(structure);

            std::string name = string_get_until(structure, " ");
            structure = string_delete_amount(structure, name.length());
            structure = string_kill_whitespace(structure);

            if(!context_class_exists(context, environment, Class(class_name))){
                fail(UNDECLARED_CLASS(class_name));
            }
            else if(!context_class_exists(context, environment, Class(type))){
                fail(UNDECLARED_CLASS(type));
            }

            add_field(environment, Class(class_name), Variable(name, Class(type), false, false));
        }

        token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        current_line++;
    }
    else if(tokendata(context) == "any^"){
        Class type = Class("any^");
        index_increase(context);
        if(tokenid(context) != TOKENINDEX_WORD) die(UNEXPECTED_OPERATOR);

        std::string variable_name = tokendata(context);
        environment.scope->variables.push_back( Variable(variable_name, type.name, false, false) );
        index_increase(context);

        // Are we gonna assign it to something
        if(tokenid(context) == TOKENINDEX_ASSIGN){
            index_increase(context);
            context_enforce_expression(context, environment, type);
        }

        index_decrease(context);
        token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        current_line++;
    }
    else if(tokendata(context) == "function^"){
        Class type = Class("function^");

        std::string function_return_type = "";
        std::vector<std::string> function_args;

        index_increase(context);
        if(tokenid(context) != TOKENINDEX_OPEN){
            die(UNEXPECTED_OPERATOR);
        }
        index_increase(context);

        while(tokenid(context) != TOKENINDEX_CLOSE){
            std::string arg;

            if(tokenid(context) != TOKENINDEX_WORD and tokenid(context) != TOKENINDEX_KEYWORD){
                die(UNEXPECTED_OPERATOR);
            }

            if(tokenid(context) == TOKENINDEX_KEYWORD){
                if(tokendata(context) == "void"){
                    arg = "any^";
                }
                else {
                    die(UNEXPECTED_KEYWORD(tokendata(context)));
                }
            }
            else {
                arg = tokendata(context);
            }

            if(!context_class_exists(context, environment, Class(arg))){
                die(UNDECLARED_CLASS(arg));
            }

            function_args.push_back(arg);
            index_increase(context);

            if(tokenid(context) == TOKENINDEX_NEXT){
                index_increase(context);
            }
            else if(tokenid(context) != TOKENINDEX_CLOSE){
                die(UNEXPECTED_OPERATOR);
            }
        }

        index_increase(context);

        if(tokenid(context) == TOKENINDEX_SUBTRACT){
            index_increase(context);
            if(tokenid(context) == TOKENINDEX_GREATERTHAN){
                index_increase(context);

                if(tokenid(context) != TOKENINDEX_WORD and tokenid(context) != TOKENINDEX_KEYWORD){
                    die(UNEXPECTED_OPERATOR);
                }

                if(tokenid(context) == TOKENINDEX_WORD){
                    if(!context_class_exists(context, environment, Class(tokendata(context)))){
                        die(UNDECLARED_CLASS(tokendata(context)));
                    }

                    function_return_type = tokendata(context);
                }
                else if(tokenid(context) == TOKENINDEX_KEYWORD){
                    if(tokendata(context) == "void"){
                        function_return_type = "void";
                    }
                    else {
                        die(UNEXPECTED_KEYWORD(tokendata(context)));
                    }
                }

                index_increase(context);
            }
            else {
                die(NO_RETURN_TYPE_STATED);
            }
        }
        else {
            die(NO_RETURN_TYPE_STATED);
        }

        type.name += "(";
        for(unsigned int i = 0; i < function_args.size(); i++){
            type.name += function_args[i];

            if(i != function_args.size() - 1){
                type.name += ",";
            }
        }
        type.name += ")->" + function_return_type;

        if(tokenid(context) != TOKENINDEX_WORD){
            die(UNEXPECTED_OPERATOR);
        }

        std::string variable_name = tokendata(context);
        environment.scope->variables.push_back( Variable(variable_name, type.name, false, false) );
        index_increase(context);

        // Are we gonna assign it to something
        if(tokenid(context) == TOKENINDEX_ASSIGN){
            index_increase(context);
            context_enforce_expression(context, environment, type);
        }

        index_decrease(context);
        token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        current_line++;
    }
    else if(tokendata(context) == "cast"){
        index_increase(context);

        if(tokenid(context) != TOKENINDEX_WORD){
            die(UNEXPECTED_OPERATOR_INEXP);
        }
        Class conversion_type = Class(tokendata(context));
        Class expression_type;

        index_increase(context);
        if(tokenid(context) != TOKENINDEX_OPEN){
            die(EXPECTED_OPEN_IN_CONVERSION);
        }
        index_increase(context);

        context_enforce_expression(context, environment, expression_type);
        index_increase(context);
        context_enforce_following_method_calls(context, environment, conversion_type);

        // Are we gonna assign it to something
        if(tokenid(context) == TOKENINDEX_ASSIGN){
            index_increase(context);
            context_enforce_expression(context, environment, conversion_type);
        }
        else {
            Token token_one = tokenof(context);
            index_increase(context);

            Token token_two = tokenof(context);
            index_decrease(context);

            if(token_two.id == TOKENINDEX_ASSIGN){
                if(token_one.id == TOKENINDEX_ADD){
                    index_increase(context);
                    index_increase(context);
                    context_enforce_expression(context, environment, conversion_type);
                }
                else if(token_one.id == TOKENINDEX_SUBTRACT){
                    index_increase(context);
                    index_increase(context);
                    context_enforce_expression(context, environment, conversion_type);
                }
                else if(token_one.id == TOKENINDEX_MULTIPLY){
                    index_increase(context);
                    index_increase(context);
                    context_enforce_expression(context, environment, conversion_type);
                }
                else if(token_one.id == TOKENINDEX_DIVIDE){
                    index_increase(context);
                    index_increase(context);
                    context_enforce_expression(context, environment, conversion_type);
                }
            }
        }

        index_decrease(context);
        token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        current_line++;
    }
    else if(tokendata(context) == "public"){
        private_member = false;
        if( !name_is_class(environment.scope->name) ) die(PUBLIC_MUST_BE_IN_CLASS);
        token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline after 'public' keyword", ERROR_INDICATOR + "Expected newline after 'public' keyword");
        current_line++;
    }
    else if(tokendata(context) == "private"){
        private_member = true;
        if( !name_is_class(environment.scope->name) ) die(PRIVATE_MUST_BE_IN_CLASS);
        token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline after 'public' keyword", ERROR_INDICATOR + "Expected newline after 'public' keyword");
        current_line++;
    }
    else {
        die(UNEXPECTED_KEYWORD(tokendata(context)));
    }
}
void enforce_word(Configuration* config, TokenContext context, Environment& environment, bool& private_member){
    std::string class_name = tokendata(context);
    Class actual_class;
    bool actual_class_generic_exists = false;

    if(environment.scope->parent != NULL){
        Scope* surface_scope = environment.scope;
        Class base_class;

        while(surface_scope->parent->parent != NULL) surface_scope = surface_scope->parent;

        if( name_is_class(surface_scope->name) ){
            base_class = Class( name_get_class(surface_scope->name) );
            actual_class = context_class_get(environment, base_class);
            for(Class generic : actual_class.generics){
                if(class_name == generic.name) { actual_class_generic_exists = true; break; }
            }
        }
    }

    index_increase(context);

    if(tokenid(context) == TOKENINDEX_OPEN){ // Function Call
        if(environment.scope == &environment.global) die(GLOBAL_STATEMENT);
        index_decrease(context);

        std::string method_name = tokendata(context);
        Class return_type;
        context_enforce_arguments(context, environment, return_type);
        index_increase(context);

        while(tokenid(context) == TOKENINDEX_MEMBER){
            token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
            context_enforce_arguments(context, environment, return_type);
            index_increase(context);
        }

        index_decrease(context);
        token_force(context, TOKENINDEX_TERMINATE,  ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        current_line++;
    }
    else if(environment_class_exists(&environment.global, Class(class_name)) or (class_name == "final") or actual_class_generic_exists){ // Variable Declaration
        bool is_final = false;
        Class type = Class(class_name);

        if(class_name == "final"){
            index_decrease(context);

            while( tokenid(context) == TOKENINDEX_WORD
            and  ( tokendata(context) == "final") ){
                if(!is_final){
                    is_final = true;
                } else {
                    fail(VARIBLE_ALREADY_FINAL);
                }

                index_increase(context);
            }

            index_increase(context);
        }

        index_decrease(context);
        context_enforce_type(context, environment, type);

        if(tokenid(context) == TOKENINDEX_WORD){
            std::string variable_name = tokendata(context);
            environment.scope->variables.push_back( Variable(variable_name, type, is_final, false, private_member) );
            index_increase(context);

            // Are we gonna assign it to something
            if(tokenid(context) == TOKENINDEX_ASSIGN){
                index_increase(context);
                context_enforce_expression(context, environment, type);
            }
            else {
                Token token_one = tokenof(context);
                index_increase(context);

                Token token_two = tokenof(context);
                index_decrease(context);

                if(token_two.id == TOKENINDEX_ASSIGN){
                    if(token_one.id == TOKENINDEX_ADD){
                        fail(VARIABLE_NOT_EXIST_YET(variable_name));
                        index_increase(context);
                        index_increase(context);
                        context_enforce_expression(context, environment, type);
                    }
                    else if(token_one.id == TOKENINDEX_SUBTRACT){
                        fail(VARIABLE_NOT_EXIST_YET(variable_name));
                        index_increase(context);
                        index_increase(context);
                        context_enforce_expression(context, environment, type);
                    }
                    else if(token_one.id == TOKENINDEX_MULTIPLY){
                        fail(VARIABLE_NOT_EXIST_YET(variable_name));
                        index_increase(context);
                        index_increase(context);
                        context_enforce_expression(context, environment, type);
                    }
                    else if(token_one.id == TOKENINDEX_DIVIDE){
                        fail(VARIABLE_NOT_EXIST_YET(variable_name));
                        index_increase(context);
                        index_increase(context);
                        context_enforce_expression(context, environment, type);
                    }
                }
            }

            // At this point token should be a terminate
            index_decrease(context);
            token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
            current_line++;
        }
        else if(tokenid(context) == TOKENINDEX_MEMBER){
            std::string method_name;

            tokenid(context) = TOKENINDEX_SCOPE_MEMBER;
            index_increase(context);
            if(tokenid(context) != TOKENINDEX_WORD){
                die(UNEXPECTED_OPERATOR);
            }

            method_name = tokenid(context);
            context_enforce_arguments(context, environment, type, "", true);

            token_force(context, TOKENINDEX_TERMINATE,  ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
            current_line++;
        }
        else {
            die(UNEXPECTED_OPERATOR);
        }
    }
    else { // Plain Variable
        std::string variable_name;
        Class base_class;

        // Go back to the word token containing the variable data
        retreat_index(context.index);
        variable_name = tokendata(context);

        // See whats after the variable name
        index_increase(context);

        // Ensure variable is declared
        if( !environment_variable_exists(environment.scope, Variable(variable_name, IGNORE_CLASS, false, false)) ){
            fail(UNDECLARED_VARIABLE(variable_name));
        }

        // Get the class name depending on where the variable was found
        Variable variable;
        if(environment_variable_exists(environment.scope, Variable(variable_name, IGNORE_CLASS, false, false))){
            variable = environment_variable_get(environment.scope, Variable(variable_name, IGNORE_CLASS, false, false));
        }
        else if(environment_variable_exists(&environment.global, Variable(variable_name, IGNORE_CLASS, false, false))){
            variable = environment_variable_get(&environment.global, Variable(variable_name, IGNORE_CLASS, false, false));
        }

        Scope* root_scope = environment.scope;
        if(root_scope->parent != NULL){
            while(root_scope->parent->parent != NULL){
                root_scope = root_scope->parent;
            }
        }

        if( name_is_class(root_scope->name) ){
            if( !environment_variable_exists(root_scope, Variable(variable_name, IGNORE_CLASS, false, false)) ){
                if(variable.is_private){
                    die(VARIABLE_IS_PRIVATE(variable_name));
                }
            }
        }
        else if(variable.is_private){
            die(VARIABLE_IS_PRIVATE(variable_name));
        }

        base_class = variable.type;
        Class root_class = context_root_class(base_class);

        // Handle following fields and methods
        if(tokenid(context) == TOKENINDEX_MEMBER){
            while(tokenid(context) == TOKENINDEX_MEMBER){
                bool dereferenced = context_class_can_dereference(context, base_class);

                if(dereferenced){
                    tokenid(context) = TOKENINDEX_POINTERMEMBER;
                }

                if( context_class_exists(context, environment, root_class) ){ // Make sure the current class is valid
                    // Next token should be a word
                    index_increase(context);

                    if(tokenid(context) != TOKENINDEX_WORD){
                        die(UNEXPECTED_OPERATOR);
                    }

                    variable_name = tokendata(context);

                    // Is it a field of the class
                    if( environment_class_variable_exists(environment, root_class, Variable(variable_name, IGNORE_CLASS, false, false)) ){
                        variable = environment_class_variable_get(environment, root_class, Variable(variable_name, IGNORE_CLASS, false, false));

                        if(variable.is_private){
                            die(VARIABLE_IS_PRIVATE(variable_name));
                        }

                        base_class = variable.type;
                        root_class = context_root_class(base_class);
                        index_increase(context);
                    }
                    else { // Method of the class
                        context_enforce_arguments(context, environment, base_class);
                        root_class = context_root_class(base_class);
                        index_increase(context);
                    }
                }
                else { // Undeclared Class
                    die(UNDECLARED_CLASS(root_class.name));
                }
            }
        }

        // Are we gonna assign it to something
        if(tokenid(context) == TOKENINDEX_ASSIGN){
            if(variable.is_final){
                die(VARIABLE_IS_FINAL(variable_name));
            }

            index_increase(context);
            context_enforce_expression(context, environment, base_class);
            root_class = context_root_class(base_class);
        }
        else {
            Token token_one = tokenof(context);
            index_increase(context);

            Token token_two = tokenof(context);
            index_decrease(context);

            if(token_two.id == TOKENINDEX_ASSIGN){
                if(token_one.id == TOKENINDEX_ADD){
                    index_increase(context);
                    index_increase(context);
                    context_enforce_expression(context, environment, base_class);
                }
                else if(token_one.id == TOKENINDEX_SUBTRACT){
                    index_increase(context);
                    index_increase(context);
                    context_enforce_expression(context, environment, base_class);
                }
                else if(token_one.id == TOKENINDEX_MULTIPLY){
                    index_increase(context);
                    index_increase(context);
                    context_enforce_expression(context, environment, base_class);
                }
                else if(token_one.id == TOKENINDEX_DIVIDE){
                    index_increase(context);
                    index_increase(context);
                    context_enforce_expression(context, environment, base_class);
                }
            }
        }

        // At this point token should be a terminate
        index_decrease(context);
        token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        current_line++;
    }
}
void enforce_pointer(Configuration* config, TokenContext context, Environment& environment){
    index_increase(context);
    if(tokenid(context) != TOKENINDEX_WORD and tokenid(context) != TOKENINDEX_OPEN) die(EXPECTED_EXPRESSION_AFTER_POINTER);
    Class base_class;

    if(tokenid(context) == TOKENINDEX_WORD){
        std::string variable_name = tokendata(context);

        // See whats after the variable name
        index_increase(context);

        // Handle following fields and methods
        if(tokenid(context) == TOKENINDEX_MEMBER){
            // Ensure variable is declared
            if( !environment_variable_exists(environment.scope, Variable(variable_name, IGNORE_CLASS, false, false)) ){
                fail(UNDECLARED_VARIABLE(tokendata(context)));
            }

            // Get the class name depending on where the variable was found
            if(environment_variable_exists(environment.scope, Variable(variable_name, IGNORE_CLASS, false, false))){
                base_class = environment_variable_get(environment.scope, Variable(variable_name, IGNORE_CLASS, false, false)).type;
            }
            else if(environment_variable_exists(&environment.global, Variable(variable_name, IGNORE_CLASS, false, false))){
                base_class = environment_variable_get(&environment.global, Variable(variable_name, IGNORE_CLASS, false, false)).type;
            }

            while(tokenid(context) == TOKENINDEX_MEMBER){
                if( environment_class_exists(&environment.global, base_class) ){ // Make sure the current class is valid
                    // Next token should be a word
                    index_increase(context);

                    // Is it a field of the class
                    if( environment_class_variable_exists(environment,base_class, Variable(variable_name, IGNORE_CLASS, false, false)) ){
                        base_class = environment_class_variable_get(environment, base_class, Variable(variable_name, IGNORE_CLASS, false, false)).type;
                    }
                    else { // Method of the class
                        die(CANT_DEREFERENCE_NON_POINTER_VALUE);
                    }
                }
                else { // Undeclared Class
                    die(UNDECLARED_CLASS(base_class.name));
                }
            }
        }
    }
    else if(tokenid(context) == TOKENINDEX_OPEN){
        index_increase(context);
        context_enforce_expression(context, environment, base_class);

        if(!context_class_can_dereference(context, base_class)){
            die(CANT_DEREFERENCE_NON_POINTER_VALUE);
        }

        context_class_dereference(context, base_class);

        index_increase(context);
        context_enforce_following_method_calls(context, environment, base_class);
    }

    // Are we gonna assign it to something
    if(tokenid(context) == TOKENINDEX_ASSIGN){
        index_increase(context);
        context_enforce_expression(context, environment, base_class);
    }
    else {
        Token token_one = tokenof(context);
        index_increase(context);

        Token token_two = tokenof(context);
        index_decrease(context);

        if(token_two.id == TOKENINDEX_ASSIGN){
            if(token_one.id == TOKENINDEX_ADD){
                index_increase(context);
                index_increase(context);
                context_enforce_expression(context, environment, base_class);
            }
            else if(token_one.id == TOKENINDEX_SUBTRACT){
                index_increase(context);
                index_increase(context);
                context_enforce_expression(context, environment, base_class);
            }
            else if(token_one.id == TOKENINDEX_MULTIPLY){
                index_increase(context);
                index_increase(context);
                context_enforce_expression(context, environment, base_class);
            }
            else if(token_one.id == TOKENINDEX_DIVIDE){
                index_increase(context);
                index_increase(context);
                context_enforce_expression(context, environment, base_class);
            }
        }
    }

    // At this point token should be a terminate
    index_decrease(context);
    token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
    current_line++;
}
void enforce_library_file(TokenContext context, unsigned int& next_lib){
    std::ofstream file(HOME + "library" + to_string(next_lib) + ".a", std::ios::binary);
    if(file.is_open()){
        file.write(tokendata(context).c_str(), tokendata(context).length());
        file.close();
        next_lib++;
    }
    else die("Failed to create tmp library");
}
