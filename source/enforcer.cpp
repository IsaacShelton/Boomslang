
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

// Validate Token Statement
void enforce_token(TokenContext context, Environment& environment){
    static unsigned int next_block = 0;

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

        context_enforce_expression(context, environment, expression_type);
        token_force(context, TOKENINDEX_MEMBER, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");

        while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
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

        if(context.tokens[context.index].data == "on"){          // Method Declaration
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

            // Add self variable to methods of a type
            if(name_is_class(environment.scope->parent->name)){
                environment.scope->variables.push_back( Variable{"self", name_get_class(environment.scope->parent->name)} );
            }

            index_increase(context);
            if(context.tokens[context.index].id != TOKENINDEX_INDENT){
                index_decrease(context);
                environment.scope = environment.scope->parent;
            }
        }
        else if(context.tokens[context.index].data == "class"){  // Class Declaration
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
        else if(context.tokens[context.index].data == "return"){ // Return Statement
            Class value_class;

            context_enforce_expression(context, environment, value_class);

            environment.global.methods[environment.global.methods.size()-1].return_type = value_class.name;
        }
        else if(context.tokens[context.index].data == "break"){ // Break Statement
            token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        }
        else if(context.tokens[context.index].data == "continue"){ // Continue Statement
            token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        }
        else if(context.tokens[context.index].data == "if"){     // If Statement
            Class value_class;

            context_enforce_expression(context, environment, value_class);
        }
        else if(context.tokens[context.index].data == "unless"){ // Unless Statement
            Class value_class;

            context_enforce_expression(context, environment, value_class);
        }
        else if(context.tokens[context.index].data == "else"){   // Else Keyword
            // We're fine with that
            token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        }
        else if(context.tokens[context.index].data == "while"){  // While Statement
            Class value_class;

            context_enforce_expression(context, environment, value_class);
        }
        else if(context.tokens[context.index].data == "until"){  // Until Statement
            Class value_class;

            context_enforce_expression(context, environment, value_class);
        }
        else {
            die("Unknown keyword " + context.tokens[context.index].data);
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
        else if(environment_class_exists(&environment.global, Class{class_name})){ // Variable Declaration
            string type = class_name;

            while( context.tokens[context.index].id == TOKENINDEX_WORD
            and    environment_class_exists(&environment.global, Class{context.tokens[context.index].data})){
                type += " " + context.tokens[context.index].data;
                index_increase(context);
            }

            if(context.tokens[context.index].id != TOKENINDEX_WORD){
                die(UNEXPECTED_OPERATOR);
            }

            string variable_name = context.tokens[context.index].data;
            environment.scope->variables.push_back( Variable{variable_name, type} );
            token_force(context, TOKENINDEX_TERMINATE,  ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        }
        else { // Plain Variable
            Class base_class;

            // Go back to the word token containing the variable data
            retreat_index(context.index);

            // Ensure variable is declared
            if( !environment_variable_exists(environment.scope, Variable{context.tokens[context.index].data, IGNORE}) ){
                fail(UNDECLARED_VARIABLE(context.tokens[context.index].data));
            }

            // Get the class name depending on where the variable was found
            if(environment_variable_exists(environment.scope, Variable{context.tokens[context.index].data,IGNORE})){
                base_class.name = environment_variable_get(environment.scope, Variable{context.tokens[context.index].data,IGNORE}).type;
            }
            else if(environment_variable_exists(&environment.global, Variable{context.tokens[context.index].data,IGNORE})){
                base_class.name = environment_variable_get(&environment.global, Variable{context.tokens[context.index].data,IGNORE}).type;
            }

            // See whats after the variable name
            index_increase(context);

            // Handle following fields and methods
            while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                if( environment_class_exists(&environment.global, base_class) ){ // Make sure the current class is valid
                    // Next token should be a word
                    index_increase(context);

                    // Is it a field of the class
                    if( environment_class_variable_exists(environment,base_class,Variable{context.tokens[context.index].data,IGNORE}) ){
                        base_class.name = environment_class_variable_get(environment,base_class,Variable{context.tokens[context.index].data,IGNORE}).type;
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

            // Are we gonna assign it to something
            if(context.tokens[context.index].id == TOKENINDEX_ASSIGN){
                index_increase(context);
                context_enforce_expression(context, environment, base_class);
            }

            // At this point token should be a terminate
            index_decrease(context);
            token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
        }
    }
    else { // Unexpected operator
        die(UNEXPECTED_OPERATOR);
    }
}

// Validate Token Stream
Environment enforce(TokenList tokens){
    // Enforce grammer and context

    Environment environment;

    // Load Boomslang Core Classes
    load_core(environment);

    // Validate Tokens
    for(unsigned int index = 0; index < tokens.size(); index++){
        enforce_token(TokenContext{tokens, index}, environment);
    }

    // Make sure a main method was declared
    if( !environment_method_exists(environment.scope, Method{"main", &environment.global, std::vector<MethodArgument>(), IGNORE}) ){
        die(NO_MAIN);
    }

    // Exit if mild errors were encountered
    if(error_count > 0){
        exit(1);
    }

    return environment;
}

