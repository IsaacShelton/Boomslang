
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

// Template Additions
std::vector<TemplateAdditions> template_additions;
void template_add_method(string name, string function_declaration){

    for(unsigned int i = 0; i < template_additions.size(); i++){
        if(template_additions[i].name == name){
            template_additions[i].additions += function_declaration;
            return;
        }
    }

    template_additions.push_back( TemplateAdditions{name, function_declaration} );
}

// Validate Token Statement
void enforce_token(TokenContext context, Environment& environment){
    static unsigned int next_block = 0;

    if(context.tokens[context.index].id == TOKENINDEX_TERMINATE){
        // Do nothing
    }
    else if(context.tokens[context.index].id == TOKENINDEX_STRING_LITERAL or context.tokens[context.index].id == TOKENINDEX_NUMERIC_LITERAL){
        if(environment.scope == &environment.global){
            die(GLOBAL_STATEMENT);
        }

        Template base_template;

        if(context.tokens[context.index].id == TOKENINDEX_STRING_LITERAL){
            base_template.name = "String";
        }
        else if(context.tokens[context.index].id == TOKENINDEX_NUMERIC_LITERAL){
            base_template.name = "Number";
        }

        token_force(context, TOKENINDEX_MEMBER, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");

        while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
            token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
            context_enforce_arguments(context, environment, base_template);
            index_increase(context);
        }

        index_decrease(context);
        token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
    }
    else if(context.tokens[context.index].id == TOKENINDEX_OPEN){
        if(environment.scope == &environment.global){
            die(GLOBAL_STATEMENT);
        }

        Template expression_type;

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
        environment.scope->children.push_back(new Scope{"block_" + to_string(next_block), environment.scope});
        environment.scope = environment.scope->children[environment.scope->children.size()-1];

        next_block++;
    }
    else if(context.tokens[context.index].id == TOKENINDEX_DEDENT){
        if(environment.scope->parent != NULL){
            environment.scope = environment.scope->parent;
        }
    }
    else if(context.tokens[context.index].id == TOKENINDEX_KEYWORD){
        if(context.tokens[context.index].data == "on"){
            string method_name;
            vector<MethodArgument> method_arguments;
            string arguments_string;
            string method_return_type;

            token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method name after 'on'", ERROR_INDICATOR + "Expected method name after 'on'");
            method_name = context.tokens[context.index].data;

            environment.scope->children.push_back(new Scope{METHOD_PREFIX + method_name, environment.scope});
            environment.scope = environment.scope->children[environment.scope->children.size()-1];

            context_enforce_method_declaration_arguments(context, environment, method_arguments, arguments_string);
            token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");

            environment.global.methods.push_back( Method{method_name, &environment.global, method_arguments, "void"} );

            if( string_count(method_name,".") == 1 ){
                environment.scope->variables.push_back( Variable{"self", string_get_until(method_name,".")} );

                if( context_template_get(environment, Template{string_get_until(method_name,".")}).is_final ){
                    fail(TEMPLATE_IS_FINAL(string_get_until(method_name,".")));
                }

                if( environment_template_get_first(&environment.global, Template{string_get_until(method_name,".")}, Template{method_return_type}).name != string_get_until(method_name,".")){
                    fail(TEMPLATE_DECLARED_AFTER(method_return_type, string_get_until(method_name,".")));
                }

                for(MethodArgument argument : method_arguments){
                    if( environment_template_get_first(&environment.global, Template{string_get_until(method_name,".")}, Template{argument.type.name}).name != argument.type.name){
                        fail(TEMPLATE_DECLARED_AFTER(argument.type.name, string_get_until(method_name,".")));
                    }
                }
            }

            index_increase(context);
            if(context.tokens[context.index].id != TOKENINDEX_INDENT){
                index_decrease(context);
            }
        }
        else if(context.tokens[context.index].data == "return"){
            Template value_template;

            context_enforce_expression(context, environment, value_template);

            environment.global.methods[environment.global.methods.size()-1].return_type = value_template.name;
        }
        else if(context.tokens[context.index].data == "template"){
            string template_name;

            token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected template name in template declaration", ERROR_INDICATOR + "Expected template name in template declaration");
            template_name = context.tokens[context.index].data;
            token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");

            environment.scope->children.push_back(new Scope{TEMPLATE_PREFIX + template_name, environment.scope});
            environment.scope = environment.scope->children[environment.scope->children.size()-1];

            environment.global.templates.push_back( Template{template_name} );

            index_increase(context);
            if(context.tokens[context.index].id != TOKENINDEX_INDENT){
                index_decrease(context);

                if(environment.scope->parent != NULL){
                    environment.scope = environment.scope->parent;
                }
            }
        }
        else {
            die("Unknown keyword " + context.tokens[context.index].data);
        }
    }
    else if(context.tokens[context.index].id == TOKENINDEX_WORD){
        string template_name = context.tokens[context.index].data;

        if( !advance_index(context.index, context.tokens.size()) ){
            die(ERROR_INDICATOR + UNEXPECTED_TERMINATE);
        }

        if(context.tokens[context.index].id == TOKENINDEX_OPEN){ // Function Call
            if(environment.scope == &environment.global){
                die(GLOBAL_STATEMENT);
            }

            index_decrease(context);

            string method_name = context.tokens[context.index].data;
            Template return_type;

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
        else if(environment_template_exists(&environment.global, Template{template_name})){
            // Variable Declaration

            string type = template_name;

            while( context.tokens[context.index].id == TOKENINDEX_WORD
            and    environment_template_exists(&environment.global, Template{context.tokens[context.index].data})){
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
        else {
            // Variable might be declared

            retreat_index(context.index);

            if( !environment_variable_exists(environment.scope, Variable{context.tokens[context.index].data, IGNORE}) ){
                fail(UNDECLARED_VARIABLE(context.tokens[context.index].data));
            }

            Template base_template;

            if(environment_variable_exists(environment.scope, Variable{context.tokens[context.index].data,IGNORE})){
                base_template.name = environment_variable_get(environment.scope, Variable{context.tokens[context.index].data,IGNORE}).type;
            }
            else if(environment_variable_exists(&environment.global, Variable{context.tokens[context.index].data,IGNORE})){
                base_template.name = environment_variable_get(&environment.global, Variable{context.tokens[context.index].data,IGNORE}).type;
            }

            index_increase(context);

            while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                if( environment_template_exists(&environment.global, base_template) ){
                    index_increase(context);

                    if( environment_template_variable_exists(environment,base_template,Variable{context.tokens[context.index].data,IGNORE}) ){
                        base_template.name = environment_template_variable_get(environment,base_template,Variable{context.tokens[context.index].data,IGNORE}).type;
                    }
                    else {
                        index_decrease(context);
                        token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                        context_enforce_arguments(context, environment, base_template);
                        index_increase(context);
                    }
                }
                else {
                    die(UNDECLARED_TEMPLATE(template_name));
                }
            }

            index_increase(context);

            if(context.tokens[context.index].id == TOKENINDEX_ASSIGN){
                index_increase(context);
                context_enforce_expression(context, environment, base_template);
            }
            else {
                index_decrease(context);
            }

            // At this point token should be a terminate
            index_decrease(context);
            token_force(context, TOKENINDEX_TERMINATE, ERROR_INDICATOR + "Unexpected statement termination\nExpected newline at end of statement", ERROR_INDICATOR + "Expected newline at end of statement");
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
        enforce_token(TokenContext{tokens, index}, environment);
    }

    if( !environment_method_exists(environment.scope, Method{"main", &environment.global, std::vector<MethodArgument>(), IGNORE}) ){
        die(NO_MAIN);
    }

    if(error_count > 0){
        exit(1);
    }

    return environment;
}

