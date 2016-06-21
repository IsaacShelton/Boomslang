
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

using namespace std;

// Validate Token Statement
void enforce_token(Configuration* config, TokenContext context, Environment& environment){
    static unsigned int next_block = 0;

    log_enforcer("Enforcing Token '" + token_name(context.tokens[context.index]) + "' with a value of '" + context.tokens[context.index].data + "'");

    if(context.tokens[context.index].id == TOKENINDEX_TERMINATE){
        // Terminate
    }
    else if( context.tokens[context.index].id == TOKENINDEX_STRING_LITERAL or context.tokens[context.index].id == TOKENINDEX_NUMERIC_LITERAL){
        // Literal

        if(environment.scope == &environment.global){
            die(GLOBAL_STATEMENT);
        }

        Class base_class;

        if(context.tokens[context.index].id == TOKENINDEX_STRING_LITERAL){
            base_class.name = "String";
            context_enforce_string(context, environment, context.tokens[context.index].data);
        }
        else if(context.tokens[context.index].id == TOKENINDEX_NUMERIC_LITERAL){
            base_class.name = "Number";
        }

        token_force(context, TOKENINDEX_MEMBER, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");

        while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
            token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
            context_enforce_arguments(context, environment, base_class);
            index_increase(context);
        }

        index_decrease(context);
        token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
    }
    else if(context.tokens[context.index].id == TOKENINDEX_OPEN){
        // Expression

        if(environment.scope == &environment.global){
            die(GLOBAL_STATEMENT);
        }

        Class expression_type;

        index_increase(context);
        context_enforce_expression(context, environment, expression_type);
        token_force(context, TOKENINDEX_MEMBER, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");

        while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
            context_class_dereference_ifcan(context, expression_type);
            token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
            context_enforce_arguments(context, environment, expression_type);
            index_increase(context);
        }

        index_decrease(context);
        token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
    }
    else if(context.tokens[context.index].id == TOKENINDEX_INDENT){
        // Indentaion

        environment.scope->children.push_back(new Scope{"block_" + to_string(next_block), environment.scope});
        environment.scope = environment.scope->children[environment.scope->children.size()-1];

        next_block++;
    }
    else if(context.tokens[context.index].id == TOKENINDEX_DEDENT){
        // Dedentation

        if(environment.scope->parent != NULL){
            environment.scope = environment.scope->parent;
        }
    }
    else if(context.tokens[context.index].id == TOKENINDEX_KEYWORD){
        // Keyword

        if(context.tokens[context.index].data == "new"){            // New Statement
            die(UNEXPECTED_KEYWORD("new"));
        }
        else if(context.tokens[context.index].data == "create"){    // Create Statement
            die(UNEXPECTED_KEYWORD("create"));
        }
        else if(context.tokens[context.index].data == "delete"){    // Delete Statement
            Class base;

            index_increase(context);
            context_enforce_expression(context, environment, base);

            if(!context_class_can_dereference(context,base)){
                die(CANT_DEREFERENCE_NON_POINTER_VALUE);
            }

            index_decrease(context);
            token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        }
        else if(context.tokens[context.index].data == "on"){        // Method Declaration
            string method_name;
            vector<MethodArgument> method_arguments;
            string arguments_string;
            string method_return_type;

            token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method name after 'on'", ERROR_INDICATOR + "Expected method name after 'on'");
            method_name = context.tokens[context.index].data;

            // Create method scope
            environment.scope->children.push_back(new Scope{METHOD_PREFIX + method_name, environment.scope});
            environment.scope = environment.scope->children[environment.scope->children.size()-1];

            // Enforce method declaration arguments
            context_enforce_method_declaration_arguments(context, environment, method_arguments, arguments_string);
            token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");

            // Add method reference to parent scope
            environment.scope->parent->methods.push_back( Method{method_name, environment.scope->parent, method_arguments, "void"} );

            std::string func_type = "function^(";
            for(unsigned int i = 0; i < method_arguments.size(); i++){
                func_type += method_arguments[i].type.name;

                if(i != method_arguments.size()-1){
                    func_type += ",";
                }
            }
            func_type += ")->void";

            environment.global.variables.push_back( Variable{method_name, func_type} );

            // Add self variable to methods of a type
            if(name_is_class(environment.scope->parent->name)){
                environment.scope->variables.push_back( Variable{"self", name_get_class(environment.scope->parent->name), false, false} );
            }

            index_increase(context);
            if(context.tokens[context.index].id != TOKENINDEX_INDENT){
                index_decrease(context);
                environment.scope = environment.scope->parent;
            }
        }
        else if(context.tokens[context.index].data == "class"){     // Class Declaration
            string class_name;

            token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected class name in class declaration", ERROR_INDICATOR + "Expected class name in class declaration");
            class_name = context.tokens[context.index].data;
            token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");

            environment.scope->children.push_back(new Scope{CLASS_PREFIX + class_name, environment.scope});
            environment.scope = environment.scope->children[environment.scope->children.size()-1];

            environment.global.classes.push_back( Class{class_name} );

            index_increase(context);
            if(context.tokens[context.index].id != TOKENINDEX_INDENT){
                index_decrease(context);

                if(environment.scope->parent != NULL){
                    environment.scope = environment.scope->parent;
                }
            }
        }
        else if(context.tokens[context.index].data == "return"){    // Return Statement
            Class value_class;
            std::string method_name = name_get_method(environment.scope->name);

            index_increase(context);
            context_enforce_expression(context, environment, value_class);

            environment.global.methods[environment.global.methods.size()-1].return_type = value_class.name;

            for(unsigned int i = 0; i < environment.global.variables.size(); i++){
                if(environment.global.variables[i].name == method_name and environment.global.variables[i].type.length() >= 6){
                    if(environment.global.variables[i].type.substr(environment.global.variables[i].type.length()-6, 6) == "->void"){
                        environment.global.variables[i].type = environment.global.variables[i].type.substr(0, environment.global.variables[i].type.length()-4);
                        environment.global.variables[i].type += value_class.name;
                        break;
                    }
                }
            }
        }
        else if(context.tokens[context.index].data == "break"){     // Break Statement
            token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        }
        else if(context.tokens[context.index].data == "continue"){  // Continue Statement
            token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        }
        else if(context.tokens[context.index].data == "if"){        // If Statement
            Class value_class;

            index_increase(context);
            context_enforce_expression(context, environment, value_class);
        }
        else if(context.tokens[context.index].data == "unless"){    // Unless Statement
            Class value_class;

            index_increase(context);
            context_enforce_expression(context, environment, value_class);
        }
        else if(context.tokens[context.index].data == "else"){      // Else Keyword
            index_increase(context);

            if(context.tokens[context.index].id == TOKENINDEX_KEYWORD and context.tokens[context.index].data == "if"){ // else if
                Class value_class;

                index_increase(context);
                context_enforce_expression(context, environment, value_class);
            }
            if(context.tokens[context.index].id == TOKENINDEX_KEYWORD and context.tokens[context.index].data == "unless"){ // else unless
                Class value_class;

                index_increase(context);
                context_enforce_expression(context, environment, value_class);
            }
            else {
                index_decrease(context);
                token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
            }
        }
        else if(context.tokens[context.index].data == "while"){     // While Statement
            Class value_class;

            index_increase(context);
            context_enforce_expression(context, environment, value_class);
        }
        else if(context.tokens[context.index].data == "until"){     // Until Statement
            Class value_class;

            index_increase(context);
            context_enforce_expression(context, environment, value_class);
        }
        else if(context.tokens[context.index].data == "var"){       // Var Statement
            std::string variable_name;
            Class value_class;
            unsigned int var_token = context.index;

            index_increase(context);

            if(context.tokens[context.index].id != TOKENINDEX_WORD){
                die(UNEXPECTED_OPERATOR_INEXP);
            }

            variable_name = context.tokens[context.index].data;

            index_increase(context);

            if(context.tokens[context.index].id != TOKENINDEX_ASSIGN){
                die(UNEXPECTED_OPERATOR_INEXP);
            }

            index_increase(context);
            context_enforce_expression(context, environment, value_class);

            context.tokens[var_token] = TOKEN_WORD(value_class.name);

            environment.scope->variables.push_back( Variable{variable_name, value_class.name, false, false} );

            index_decrease(context);
        }
        else if(context.tokens[context.index].data == "import"){    // Import Statement
            std::string package;
            std::string prev_filename;
            bool is_package;
            TokenList tokens;
            Configuration custom_config = *config;
            custom_config.package = true;

            index_increase(context);

            if(context.tokens[context.index].id != TOKENINDEX_WORD and context.tokens[context.index].id != TOKENINDEX_STRING_LITERAL){
                die(EXPECTED_PACKAGE_NAME_AFTER_IMPORT);
            }

            if(context.tokens[context.index].id == TOKENINDEX_WORD){
                is_package = true;
            }
            else {
                is_package = false;
            }

            package = context.tokens[context.index].data;

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
                    if(!tokens_load(current_filename, tokens)) die("Failed to load package '" + package + "'");
                    enforce_package(&custom_config, tokens, environment);
                }
                else {
                    tokens = tokenize(contents(current_filename));
                    enforce_package(&custom_config, tokens, environment);
                }

                current_filename = prev_filename;
            }
            else if( file_exists( filename_path(current_filename) + delete_slash(package) ) ){ // Relative Path
                prev_filename = current_filename;
                current_filename = filename_path(current_filename) + delete_slash(package);

                if(is_package){
                    if(!tokens_load(current_filename, tokens)) die("Failed to load package '" + package + "'");
                    enforce_package(&custom_config, tokens, environment);
                }
                else {
                    tokens = tokenize(contents(current_filename));
                    enforce_package(&custom_config, tokens, environment);
                }

                current_filename = prev_filename;
            }
            else if( file_exists( PACKAGEHOME + delete_slash(package) ) ){ // Package Path
                prev_filename = current_filename;
                current_filename = PACKAGEHOME + delete_slash(package);

                if(is_package){
                    if(!tokens_load(current_filename, tokens)) die("Failed to load package '" + package + "'");
                    enforce_package(&custom_config, tokens, environment);
                }
                else {
                    tokens = tokenize(contents(current_filename));
                    enforce_package(&custom_config, tokens, environment);
                }

                current_filename = prev_filename;
            }
            else {
                if(is_package){
                    fail(PACKAGE_DOESNT_EXIST(package));
                }
                else {
                    fail(FILE_DOESNT_EXIST(package));
                }
            }
        }
        else if(context.tokens[context.index].data == "native"){    // Native Statement
            std::string action;
            std::string filename;
            index_increase(context);

            if(context.tokens[context.index].id != TOKENINDEX_WORD){
                die(UNEXPECTED_OPERATOR);
            }
            action = context.tokens[context.index].data;
            index_increase(context);

            if(context.tokens[context.index].id != TOKENINDEX_STRING_LITERAL){
                die(UNEXPECTED_OPERATOR);
            }
            filename = context.tokens[context.index].data;

            if(action != "link" and action != "include"){
                die("Unknown native directive '" + action + "'");
            }

            token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        }
        else if(context.tokens[context.index].data == "register"){  // Register Statement
            std::string construct; // Type of Language Construct
            std::string structure; // Structure of Data

            index_increase(context);
            construct = context.tokens[context.index].data;
            index_increase(context);
            structure = context.tokens[context.index].data;

            if( construct != "class"
            and construct != "method"
            and construct != "function"
            and construct != "constant" ){
                die(UNKNOWN_LANG_CONSTRUCT(construct));
            }

            if(construct == "class"){
                std::string class_name = string_get_until_or(string_kill_whitespace(structure), " \n");

                add_class(environment, Class{class_name});
            }
            else if(construct == "function"){
                std::string function_name;
                std::vector<MethodArgument> function_arguments;
                std::string function_return_type;
                Class arg_type;

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
                while(structure.substr(0,1) != ")"){
                    if(structure.substr(0,1) == ","){
                        structure = string_delete_amount(structure, 1);
                    }

                    structure = string_kill_whitespace(structure);
                    arg_type.name = string_get_until_or(structure, " ,)");
                    structure = string_kill_whitespace(structure);
                    structure = string_delete_amount(structure, arg_type.name.length());
                    structure = string_kill_whitespace(structure);

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
                function_return_type = string_get_until_or(structure, " \n");
                structure = string_delete_amount(structure, function_return_type.length());
                structure = string_kill_whitespace(structure);

                add_function(environment, Method{function_name, &environment.global, function_arguments, function_return_type});
            }
            else if(construct == "constant"){
                std::string type = string_get_until(structure, " ");
                structure = string_delete_amount(structure, type.length());
                structure = string_kill_whitespace(structure);

                std::string name = string_get_until(structure, " ");
                structure = string_delete_amount(structure, name.length());
                structure = string_kill_whitespace(structure);

                if(!context_class_exists(context, environment, Class{type})){
                    fail(UNDECLARED_CLASS(type));
                }

                environment.global.variables.push_back(Variable{name, type, true, false});
            }

            token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        }
        else if(context.tokens[context.index].data == "any^"){      // any^ Statement
            Class type = Class{"any^"};

            index_increase(context);
            if(context.tokens[context.index].id != TOKENINDEX_WORD){
                die(UNEXPECTED_OPERATOR);
            }

            string variable_name = context.tokens[context.index].data;
            environment.scope->variables.push_back( Variable{variable_name, type.name, false, false} );
            index_increase(context);

            // Are we gonna assign it to something
            if(context.tokens[context.index].id == TOKENINDEX_ASSIGN){
                index_increase(context);
                context_enforce_expression(context, environment, type);
            }

            index_decrease(context);
            token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        }
        else if(context.tokens[context.index].data == "function^"){ // function^ Statement
            Class type = Class{"function^"};

            std::string function_return_type = "";
            std::vector<std::string> function_args;

            index_increase(context);
            if(context.tokens[context.index].id != TOKENINDEX_OPEN){
                die(UNEXPECTED_OPERATOR);
            }
            index_increase(context);

            while(context.tokens[context.index].id != TOKENINDEX_CLOSE){
                std::string arg;

                if(context.tokens[context.index].id != TOKENINDEX_WORD and context.tokens[context.index].id != TOKENINDEX_KEYWORD){
                    die(UNEXPECTED_OPERATOR);
                }

                if(context.tokens[context.index].id == TOKENINDEX_KEYWORD){
                    if(context.tokens[context.index].data == "void"){
                        arg = "any^";
                    }
                    else {
                        die(UNEXPECTED_KEYWORD(context.tokens[context.index].data));
                    }
                }
                else {
                    arg = context.tokens[context.index].data;
                }

                if(!context_class_exists(context, environment, Class{arg})){
                    die(UNDECLARED_CLASS(arg));
                }

                function_args.push_back(arg);
                index_increase(context);

                if(context.tokens[context.index].id == TOKENINDEX_NEXT){
                    index_increase(context);
                }
                else if(context.tokens[context.index].id != TOKENINDEX_CLOSE){
                    die(UNEXPECTED_OPERATOR);
                }
            }

            index_increase(context);

            if(context.tokens[context.index].id == TOKENINDEX_SUBTRACT){
                index_increase(context);
                if(context.tokens[context.index].id == TOKENINDEX_GREATERTHAN){
                    index_increase(context);

                    if(context.tokens[context.index].id != TOKENINDEX_WORD and context.tokens[context.index].id != TOKENINDEX_KEYWORD){
                        die(UNEXPECTED_OPERATOR);
                    }

                    if(context.tokens[context.index].id == TOKENINDEX_WORD){
                        if(!context_class_exists(context, environment, Class{context.tokens[context.index].data})){
                            die(UNDECLARED_CLASS(context.tokens[context.index].data));
                        }

                        function_return_type = context.tokens[context.index].data;
                    }
                    else if(context.tokens[context.index].id == TOKENINDEX_KEYWORD){
                        if(context.tokens[context.index].data == "void"){
                            function_return_type = "void";
                        }
                        else {
                            die(UNEXPECTED_KEYWORD(context.tokens[context.index].data));
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

            if(context.tokens[context.index].id != TOKENINDEX_WORD){
                die(UNEXPECTED_OPERATOR);
            }

            string variable_name = context.tokens[context.index].data;
            environment.scope->variables.push_back( Variable{variable_name, type.name, false, false} );
            index_increase(context);

            // Are we gonna assign it to something
            if(context.tokens[context.index].id == TOKENINDEX_ASSIGN){
                index_increase(context);
                context_enforce_expression(context, environment, type);
            }

            index_decrease(context);
            token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        }
        else if(context.tokens[context.index].data == "cast"){      // Cast Statement
            index_increase(context);

            if(context.tokens[context.index].id != TOKENINDEX_WORD){
                die(UNEXPECTED_OPERATOR_INEXP);
            }
            Class conversion_type = Class{context.tokens[context.index].data};
            Class expression_type;

            index_increase(context);
            if(context.tokens[context.index].id != TOKENINDEX_OPEN){
                die(EXPECTED_OPEN_IN_CONVERSION);
            }
            index_increase(context);

            context_enforce_expression(context, environment, expression_type);
            index_increase(context);
            context_enforce_following_method_calls(context, environment, conversion_type);

            // Are we gonna assign it to something
            if(context.tokens[context.index].id == TOKENINDEX_ASSIGN){
                index_increase(context);
                context_enforce_expression(context, environment, conversion_type);
            }
            else {
                Token token_one = context.tokens[context.index];
                index_increase(context);

                Token token_two = context.tokens[context.index];
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
        }
        else {
            die(UNEXPECTED_KEYWORD(context.tokens[context.index].data));
        }

    }
    else if(context.tokens[context.index].id == TOKENINDEX_WORD){
        // Identifier

        string class_name = context.tokens[context.index].data;

        if( !advance_index(context.index, context.tokens.size()) ){
            die(ERROR_INDICATOR + UNEXPECTED_TERMINATE);
        }

        if(context.tokens[context.index].id == TOKENINDEX_OPEN){ // Function Call
            if(environment.scope == &environment.global){
                die(GLOBAL_STATEMENT);
            }

            index_decrease(context);

            string method_name = context.tokens[context.index].data;
            Class return_type;

            context_enforce_arguments(context, environment, return_type);

            if( !advance_index(context.index, context.tokens.size()) ){
                die(ERROR_INDICATOR + UNEXPECTED_TERMINATE);
            }

            while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");

                context_enforce_arguments(context, environment, return_type);
                index_increase(context);
            }

            index_decrease(context);
            token_force(context, TOKENINDEX_TERMINATE,  ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        }
        else if(environment_class_exists(&environment.global, Class{class_name}) or (class_name == "final")){ // Variable Declaration
            bool is_final = false;
            Class type = Class{class_name};

            if(class_name == "final"){
                index_decrease(context);

                while( context.tokens[context.index].id == TOKENINDEX_WORD
                and  ( context.tokens[context.index].data == "final") ){
                    if(!is_final){
                        is_final = true;
                    }
                    else {
                        fail(VARIBLE_ALREADY_FINAL);
                    }
                    index_increase(context);
                }

                if(context.tokens[context.index].id != TOKENINDEX_WORD){
                    die(UNEXPECTED_OPERATOR_INEXP);
                }

                class_name = context.tokens[context.index].data;
                type.name = class_name;

                index_increase(context);
            }

            while( (context.tokens[context.index].id == TOKENINDEX_WORD
            and    environment_class_exists(&environment.global, Class{context.tokens[context.index].data}))
            or     context.tokens[context.index].id == TOKENINDEX_POINTER){
                if(context.tokens[context.index].id == TOKENINDEX_POINTER){
                    type.name += "^";
                    index_increase(context);
                }
                else {
                    type.name += " " + context.tokens[context.index].data;
                    index_increase(context);
                }
            }

            if(context.tokens[context.index].id != TOKENINDEX_WORD){
                die(UNEXPECTED_OPERATOR);
            }

            string variable_name = context.tokens[context.index].data;
            environment.scope->variables.push_back( Variable{variable_name, type.name, is_final, false} );
            index_increase(context);

            // Are we gonna assign it to something
            if(context.tokens[context.index].id == TOKENINDEX_ASSIGN){
                index_increase(context);
                context_enforce_expression(context, environment, type);
            }
            else {
                Token token_one = context.tokens[context.index];
                index_increase(context);

                Token token_two = context.tokens[context.index];
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
        }
        else { // Plain Variable
            std::string variable_name;
            Class base_class;

            // Go back to the word token containing the variable data
            retreat_index(context.index);
            variable_name = context.tokens[context.index].data;

            // See whats after the variable name
            index_increase(context);

            // Handle following fields and methods
            if(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                // Ensure variable is declared
                if( !environment_variable_exists(environment.scope, Variable{variable_name, IGNORE, false, false}) ){
                    fail(UNDECLARED_VARIABLE(variable_name));
                }

                // Get the class name depending on where the variable was found
                if(environment_variable_exists(environment.scope, Variable{variable_name, IGNORE, false, false})){
                    base_class.name = environment_variable_get(environment.scope, Variable{variable_name, IGNORE, false, false}).type;
                }
                else if(environment_variable_exists(&environment.global, Variable{variable_name, IGNORE, false, false})){
                    base_class.name = environment_variable_get(&environment.global, Variable{variable_name, IGNORE, false, false}).type;
                }

                while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                    bool dereferenced = context_class_dereference_ifcan(context, base_class);

                    if(dereferenced){
                        context.tokens[context.index].id = TOKENINDEX_POINTERMEMBER;
                    }

                    if( environment_class_exists(&environment.global, base_class) ){ // Make sure the current class is valid
                        // Next token should be a word
                        index_increase(context);

                        // Is it a field of the class
                        if( environment_class_variable_exists(environment,base_class,Variable{variable_name, IGNORE, false, false}) ){
                            base_class.name = environment_class_variable_get(environment,base_class,Variable{variable_name, IGNORE, false, false}).type;
                        }
                        else { // Method of the class
                            index_decrease(context);
                            token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                            context_enforce_arguments(context, environment, base_class);
                            index_increase(context);
                        }
                    }
                    else { // Undeclared Class
                        die(UNDECLARED_CLASS(class_name));
                    }
                }
            }

            // Are we gonna assign it to something
            if(context.tokens[context.index].id == TOKENINDEX_ASSIGN){
                if(!context_variable_exists(environment, Variable{variable_name, IGNORE})){
                    die(UNDECLARED_VARIABLE(variable_name));
                }

                if(context_variable_get(environment, Variable{variable_name, IGNORE}).is_final){
                    die(VARIABLE_IS_FINAL(variable_name));
                }

                index_increase(context);
                context_enforce_expression(context, environment, base_class);
            }
            else {
                Token token_one = context.tokens[context.index];
                index_increase(context);

                Token token_two = context.tokens[context.index];
                index_decrease(context);

                if(token_two.id == TOKENINDEX_ASSIGN){
                    if(token_one.id == TOKENINDEX_ADD){
                        if(!context_variable_exists(environment, Variable{variable_name, IGNORE})){
                            die(UNDECLARED_VARIABLE(variable_name));
                        }

                        if(context_variable_get(environment, Variable{variable_name, IGNORE}).is_final){
                            die(VARIABLE_IS_FINAL(variable_name));
                        }

                        index_increase(context);
                        index_increase(context);
                        context_enforce_expression(context, environment, base_class);
                    }
                    else if(token_one.id == TOKENINDEX_SUBTRACT){
                        if(!context_variable_exists(environment, Variable{variable_name, IGNORE})){
                            die(UNDECLARED_VARIABLE(variable_name));
                        }

                        if(context_variable_get(environment, Variable{variable_name, IGNORE}).is_final){
                            die(VARIABLE_IS_FINAL(variable_name));
                        }

                        index_increase(context);
                        index_increase(context);
                        context_enforce_expression(context, environment, base_class);
                    }
                    else if(token_one.id == TOKENINDEX_MULTIPLY){
                        if(!context_variable_exists(environment, Variable{variable_name, IGNORE})){
                            die(UNDECLARED_VARIABLE(variable_name));
                        }

                        if(context_variable_get(environment, Variable{variable_name, IGNORE}).is_final){
                            die(VARIABLE_IS_FINAL(variable_name));
                        }

                        index_increase(context);
                        index_increase(context);
                        context_enforce_expression(context, environment, base_class);
                    }
                    else if(token_one.id == TOKENINDEX_DIVIDE){
                        if(!context_variable_exists(environment, Variable{variable_name, IGNORE})){
                            die(UNDECLARED_VARIABLE(variable_name));
                        }

                        if(context_variable_get(environment, Variable{variable_name, IGNORE}).is_final){
                            die(VARIABLE_IS_FINAL(variable_name));
                        }

                        index_increase(context);
                        index_increase(context);
                        context_enforce_expression(context, environment, base_class);
                    }
                }
            }

            // At this point token should be a terminate
            index_decrease(context);
            token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        }
    }
    else if(context.tokens[context.index].id == TOKENINDEX_POINTER){
        index_increase(context);

        if(context.tokens[context.index].id != TOKENINDEX_WORD and context.tokens[context.index].id != TOKENINDEX_OPEN){
            die(EXPECTED_EXPRESSION_AFTER_POINTER);
        }

        Class base_class;

        if(context.tokens[context.index].id == TOKENINDEX_WORD){
            std::string variable_name = context.tokens[context.index].data;

            // See whats after the variable name
            index_increase(context);

            // Handle following fields and methods
            if(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                // Ensure variable is declared
                if( !environment_variable_exists(environment.scope, Variable{variable_name, IGNORE, false, false}) ){
                    fail(UNDECLARED_VARIABLE(context.tokens[context.index].data));
                }

                // Get the class name depending on where the variable was found
                if(environment_variable_exists(environment.scope, Variable{variable_name, IGNORE, false, false})){
                    base_class.name = environment_variable_get(environment.scope, Variable{variable_name, IGNORE, false, false}).type;
                }
                else if(environment_variable_exists(&environment.global, Variable{variable_name, IGNORE, false, false})){
                    base_class.name = environment_variable_get(&environment.global, Variable{variable_name, IGNORE, false, false}).type;
                }

                while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                    if( environment_class_exists(&environment.global, base_class) ){ // Make sure the current class is valid
                        // Next token should be a word
                        index_increase(context);

                        // Is it a field of the class
                        if( environment_class_variable_exists(environment,base_class,Variable{variable_name, IGNORE, false, false}) ){
                            base_class.name = environment_class_variable_get(environment,base_class,Variable{variable_name, IGNORE, false, false}).type;
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
        else if(context.tokens[context.index].id == TOKENINDEX_OPEN){
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
        if(context.tokens[context.index].id == TOKENINDEX_ASSIGN){
            index_increase(context);
            context_enforce_expression(context, environment, base_class);
        }
        else {
            Token token_one = context.tokens[context.index];
            index_increase(context);

            Token token_two = context.tokens[context.index];
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
    }
    else { // Unexpected operator
        die(UNEXPECTED_OPERATOR);
    }
}

// Validate Token Stream
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
    if( !environment_method_exists(TokenContext{tokens, index}, environment.scope, Method{"main", &environment.global, std::vector<MethodArgument>(), IGNORE}) and !config->package ){
        die(NO_MAIN);
    }

    // Exit if mild errors were encountered
    if(error_count > 0){
        exit(1);
    }

    log_enforcer("The Program is Valid");
    return environment;
}

void enforce_package(Configuration* config, TokenList& tokens, Environment& environment){

    // Validate Tokens
    for(unsigned int index = 0; index < tokens.size(); index++){
        enforce_token(config, TokenContext{tokens, index}, environment);
    }

    // Exit if mild errors were encountered
    if(error_count > 0){
        exit(1);
    }

    log_enforcer("The Package is Valid");
}
