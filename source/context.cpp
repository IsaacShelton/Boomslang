
#include "../include/die.h"
#include "../include/log.h"
#include "../include/token.h"
#include "../include/lexer.h"
#include "../include/scope.h"
#include "../include/errors.h"
#include "../include/context.h"
#include "../include/enforcer.h"
#include "../include/assembler.h"
#include "../include/management.h"

using namespace std;

bool context_variable_exists(Environment& e, Variable variable){
    return (environment_variable_exists(e.scope, variable) or environment_variable_exists(&e.global, variable));
}
bool context_class_exists(TokenContext context, Environment& e, Class base){
    if(base.name == "any^") return true;

    context_class_dereference_ifcan(context, base);
    return environment_class_exists(&e.global, base);
}
bool context_method_exists(TokenContext context, Environment& e, Method method){
    return environment_method_exists(context, &e.global, method);
}
bool context_method_exists(TokenContext context, Environment& e, Class base, Method method){
    if(environment_get_child(&e.global, CLASS_PREFIX + base.name) == NULL){
        return false;
    }

    return environment_method_exists(context, environment_get_child(&e.global, CLASS_PREFIX + base.name), method);
}

Variable context_variable_get(Environment& e, Variable variable){
    return environment_variable_get(e.scope, variable);
}
Class context_class_get(Environment& e, Class base){
    return environment_class_get(&e.global, base);
}
Method context_method_get(TokenContext context, Environment& e, Method method){
    return environment_method_get(context, &e.global, method);
}
Method context_method_get(TokenContext context, Environment& e, Class base, Method method){
    if(environment_get_child(&e.global, CLASS_PREFIX + base.name) == NULL){
        #ifdef DEV_ERRORS
        fail("context_method_get failed to return a valid method on line " + to_string(__LINE__) + " in context.cpp");
        #endif // DEV_ERRORS

        return Method{IGNORE, NULL, IGNORE_ARGS, IGNORE};
    }

    return environment_method_get(context, environment_get_child(&e.global, CLASS_PREFIX + base.name), method);
}

void context_enforce_expression(TokenContext context, Environment& e, Class& type){
    int balance = 0;

    while(balance != 0 or (context.tokens[context.index].id != TOKENINDEX_NEXT and context.tokens[context.index].id != TOKENINDEX_TERMINATE and context.tokens[context.index].id != TOKENINDEX_CLOSE)){
        if(context.tokens[context.index].id == TOKENINDEX_OPEN){
            Class base_class;

            balance++;
            index_increase(context);
            context_enforce_expression(context, e, base_class);
            balance--;

            index_increase(context);

            while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                if( context_class_exists(context, e, base_class) ){
                    index_increase(context);

                    if( environment_class_variable_exists(e, base_class, Variable{context.tokens[context.index].data, IGNORE, false, false}) ){
                        base_class.name = environment_class_variable_get(e, base_class, Variable{context.tokens[context.index].data, IGNORE, false, false}).type;
                        index_increase(context);
                    }
                    else {
                        index_decrease(context);
                        token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                        context_enforce_arguments(context, e, base_class);
                        index_increase(context);
                    }
                }
                else {
                    die(UNDECLARED_CLASS(base_class.name));
                }
            }

            index_decrease(context);

            if(type.name == ""){
                type.name = base_class.name;
            }
            else if(!context_class_compare(context, type, base_class)){
                fail( INCOMPATIBLE_CLASSES(type.name, base_class.name) );
            }
        }
        else if(context.tokens[context.index].id == TOKENINDEX_LESSTHAN){
            // We're fine with that
        }
        else if(context.tokens[context.index].id == TOKENINDEX_GREATERTHAN){
             // We're fine with that
        }
        else if(context.tokens[context.index].id == TOKENINDEX_POINTER){
            index_increase(context);

            if(context.tokens[context.index].id == TOKENINDEX_WORD){
                std::string name = context.tokens[context.index].data;
                index_increase(context);

                if( !environment_variable_exists(e.scope, Variable{name, IGNORE, false, false}) ){
                    fail(UNDECLARED_VARIABLE(name));
                }

                Class base_class{environment_variable_get(e.scope, Variable{name, IGNORE, false, false}).type};

                while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                    if( context_class_exists(context, e, base_class) ){
                        index_increase(context);

                        if( environment_class_variable_exists(e, base_class, Variable{context.tokens[context.index].data,IGNORE}) ){
                            base_class.name = environment_class_variable_get(e, base_class, Variable{context.tokens[context.index].data,IGNORE}).type;
                            index_increase(context);
                        }
                        else {
                            index_decrease(context);
                            token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                            context_enforce_arguments(context, e, base_class);
                            index_increase(context);
                        }
                    }
                    else {
                        die(UNDECLARED_CLASS(base_class.name));
                    }
                }

                index_decrease(context);
                context_class_dereference(context, base_class);

                if(type.name == ""){
                    type.name = base_class.name;
                }
                else if(!context_class_compare(context, type, base_class)){
                    fail( INCOMPATIBLE_CLASSES(type.name, base_class.name) );
                }
            }
            else if(context.tokens[context.index].id == TOKENINDEX_OPEN){
                Class base;

                index_increase(context);
                context_enforce_expression(context, e, base);

                context_class_dereference(context, base);

                if(type.name == ""){
                    type.name = base.name;
                }
                else if(!context_class_compare(context, type, base)){
                    fail( INCOMPATIBLE_CLASSES(type.name, base.name) );
                }
            }
            else {
                die(UNEXPECTED_OPERATOR_INEXP);
            }
        }
        else if(context.tokens[context.index].id == TOKENINDEX_WORD){
            std::string name = context.tokens[context.index].data;
            index_increase(context);

            if(context.tokens[context.index].id == TOKENINDEX_OPEN){ // Function call
                if(!context_method_exists(context, e, Method{name, NULL, IGNORE_ARGS, IGNORE})){
                    fail(UNDECLARED_METHOD(name));
                }

                Class base;

                index_decrease(context);
                context_enforce_arguments(context, e, base);
                index_increase(context);

                context_enforce_following_method_calls(context, e, base);
                index_decrease(context);
            }
            else { // Variable with possible method calls after
                if( !environment_variable_exists(e.scope, Variable{name, IGNORE, false, false}) ){
                    fail(UNDECLARED_VARIABLE(name));
                }

                Class base_class{environment_variable_get(e.scope, Variable{name, IGNORE, false, false}).type};

                while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                    if( context_class_exists(context, e, base_class) ){
                        index_increase(context);

                        if( environment_class_variable_exists(e, base_class, Variable{context.tokens[context.index].data, IGNORE, false, false}) ){
                            base_class.name = environment_class_variable_get(e, base_class, Variable{context.tokens[context.index].data, IGNORE, false, false}).type;
                            index_increase(context);
                        }
                        else {
                            index_decrease(context);
                            token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                            context_enforce_arguments(context, e, base_class);
                            index_increase(context);
                        }
                    }
                    else {
                        die(UNDECLARED_CLASS(base_class.name));
                    }
                }

                index_decrease(context);

                if(type.name == ""){
                    type.name = base_class.name;
                }
                else if(!context_class_compare(context, type, base_class)){
                    fail( INCOMPATIBLE_CLASSES(type.name, base_class.name) );
                }
            }
        }
        else if(context.tokens[context.index].id == TOKENINDEX_KEYWORD){
            if(context.tokens[context.index].data == "new"){
                std::string class_name;

                index_increase(context);

                if(context.tokens[context.index].id != TOKENINDEX_WORD){
                    die(UNEXPECTED_OPERATOR_INEXP);
                }

                class_name = context.tokens[context.index].data;

                if(!context_class_exists(context, e, Class{class_name})){
                    die(UNDECLARED_CLASS(class_name));
                }

                if(type.name == ""){
                    type.name = class_name;
                }
                else if(!context_class_compare(context, type, Class{class_name})){
                    fail( INCOMPATIBLE_CLASSES(type.name, class_name) );
                }
            }
            else if(context.tokens[context.index].data == "create"){
                std::string class_name;

                index_increase(context);

                if(context.tokens[context.index].id != TOKENINDEX_WORD){
                    die(UNEXPECTED_OPERATOR_INEXP);
                }

                class_name = context.tokens[context.index].data + "^";

                if(!context_class_exists(context, e, Class{class_name})){
                    die(UNDECLARED_CLASS(class_name));
                }

                if(type.name == ""){
                    type.name = class_name;
                }
                else if(!context_class_compare(context, type, Class{class_name})){
                    fail( INCOMPATIBLE_CLASSES(type.name, class_name) );
                }
            }
            else if(context.tokens[context.index].data == "void"){
            }
            else if(context.tokens[context.index].data == "cast"){
                index_increase(context);

                if(context.tokens[context.index].id != TOKENINDEX_WORD){
                    die(UNEXPECTED_OPERATOR_INEXP);
                }
                std::string conversion_type = context.tokens[context.index].data;
                Class expression_type;

                index_increase(context);
                if(context.tokens[context.index].id != TOKENINDEX_OPEN and context.tokens[context.index].id != TOKENINDEX_WORD){
                    die(EXPECTED_OPEN_IN_CONVERSION);
                }

                if(context.tokens[context.index].id == TOKENINDEX_OPEN){
                    context_enforce_expression(context, e, expression_type);
                    index_decrease(context);
                }
                else if(context.tokens[context.index].id == TOKENINDEX_WORD){
                    std::string variable_name = context.tokens[context.index].data;

                    if(!context_variable_exists(e, Variable{variable_name, IGNORE, false, false})){
                        die(UNDECLARED_CLASS(variable_name));
                    }
                    expression_type.name = context_variable_get(e, Variable{variable_name, IGNORE, false, false}).type;
                }

                if(type.name == ""){
                    type.name = conversion_type;
                }
                else if(!context_class_compare(context, Class{conversion_type}, type)){
                    die(INCOMPATIBLE_CLASSES(type.name, conversion_type));
                }
            }
            else {
                die(UNEXPECTED_KEYWORD(context.tokens[context.index].data));
            }
        }
        else if(context.tokens[context.index].id == TOKENINDEX_STRING_LITERAL){
            Class base_class = Class{"String"};

            index_increase(context);

            while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                if( context_class_exists(context, e, base_class) ){
                    index_increase(context);

                    if( environment_class_variable_exists(e, base_class, Variable{context.tokens[context.index].data, IGNORE, false, false}) ){
                        base_class.name = environment_class_variable_get(e, base_class, Variable{context.tokens[context.index].data, IGNORE, false, false}).type;
                        index_increase(context);
                    }
                    else {
                        index_decrease(context);
                        token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                        context_enforce_arguments(context, e, base_class);
                        index_increase(context);
                    }
                }
                else {
                    die(UNDECLARED_CLASS(base_class.name));
                }
            }

            index_decrease(context);

            if(type.name == ""){
                type.name = base_class.name;
            }
            else if(!context_class_compare(context, type, base_class)){
                fail( INCOMPATIBLE_CLASSES(type.name, base_class.name) );
            }
        }
        else if(context.tokens[context.index].id == TOKENINDEX_NUMERIC_LITERAL){
            Class base_class = Class{"Number"};

            index_increase(context);

            while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                if( context_class_exists(context, e, base_class) ){
                    index_increase(context);

                    if( environment_class_variable_exists(e, base_class, Variable{context.tokens[context.index].data, IGNORE, false, false}) ){
                        base_class.name = environment_class_variable_get(e, base_class, Variable{context.tokens[context.index].data, IGNORE, false, false}).type;
                        index_increase(context);
                    }
                    else {
                        index_decrease(context);
                        token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                        context_enforce_arguments(context, e, base_class);
                        index_increase(context);
                    }
                }
                else {
                    die(UNDECLARED_CLASS(base_class.name));
                }
            }

            index_decrease(context);

            if(type.name == ""){
                type.name = base_class.name;
            }
            else if(!context_class_compare(context, type, base_class)){
                fail( INCOMPATIBLE_CLASSES(type.name, base_class.name) );
            }
        }
        else if(context.tokens[context.index].id == TOKENINDEX_ADD
        or context.tokens[context.index].id == TOKENINDEX_SUBTRACT
        or context.tokens[context.index].id == TOKENINDEX_MULTIPLY
        or context.tokens[context.index].id == TOKENINDEX_DIVIDE
        or context.tokens[context.index].id == TOKENINDEX_ASSIGN
        or context.tokens[context.index].id == TOKENINDEX_NOT){
            // ok
        }
        else {
            die(UNEXPECTED_OPERATOR_INEXP);
        }

        index_increase(context);
    }
}
void context_enforce_arguments(TokenContext context, Environment& e, Class& base_class){
    int balance = 1;
    vector<MethodArgument> arguments;
    string type;
    string method_name = context.tokens[context.index].data;

    token_force(context, TOKENINDEX_OPEN, ERROR_INDICATOR + "Unexpected statement termination\nExpected opening parentheses", ERROR_INDICATOR + "Expected opening parentheses");

    while(balance != 0){
        if(!advance_index(context.index,context.tokens.size())){
            die(UNEXPECTED_TERMINATE);
        }

        if(context.tokens[context.index].id == TOKENINDEX_OPEN){
            balance++;
        }
        else if(context.tokens[context.index].id == TOKENINDEX_CLOSE){
            balance--;
        }
        else if(context.tokens[context.index].id == TOKENINDEX_WORD){
            index_increase(context);

            if(context.tokens[context.index].id == TOKENINDEX_OPEN){ // Function Call
                if(e.scope == &e.global){
                    die(GLOBAL_STATEMENT);
                }

                index_decrease(context);
                string method_name = context.tokens[context.index].data;
                Class return_type;
                context_enforce_arguments(context, e, return_type);

                if( !advance_index(context.index, context.tokens.size()) ){
                    die(ERROR_INDICATOR + UNEXPECTED_TERMINATE);
                }

                index_increase(context);
                while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                    token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                    context_enforce_arguments(context, e, return_type);
                    index_increase(context);
                }

                index_decrease(context);

                if(return_type.name == "void"){
                    die(CANT_PASS_VOID_AS_ARGUMENT);
                }
                else if(type == ""){
                    type = return_type.name;
                }
                else if(!context_class_compare(context, Class{type}, return_type)){
                    fail(INCOMPATIBLE_CLASSES(type, return_type.name));
                }
                context.index--;
            }
            else {
                index_decrease(context);

                if( !environment_variable_exists(e.scope, Variable{context.tokens[context.index].data, IGNORE, false, false}) ){
                    die(UNDECLARED_VARIABLE(context.tokens[context.index].data));
                }

                Class variable_type;

                if(environment_variable_exists(e.scope, Variable{context.tokens[context.index].data, IGNORE, false, false})){
                    variable_type.name = environment_variable_get(e.scope, Variable{context.tokens[context.index].data, IGNORE, false, false}).type;
                }
                else if(environment_variable_exists(&e.global, Variable{context.tokens[context.index].data, IGNORE, false, false})){
                    variable_type.name = environment_variable_get(&e.global, Variable{context.tokens[context.index].data,IGNORE}).type;
                }

                index_increase(context);
                while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                    token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                    context_enforce_arguments(context, e, variable_type);
                    index_increase(context);
                }

                index_decrease(context);

                if(variable_type.name == "void"){
                    die(CANT_PASS_VOID_AS_ARGUMENT);
                }
                else if(type == ""){
                    type = variable_type.name;
                }
                else if(!context_class_compare(context, Class{type}, variable_type)){
                    fail(INCOMPATIBLE_CLASSES(type, variable_type.name));
                }
            }
        }
        else if(context.tokens[context.index].id == TOKENINDEX_STRING_LITERAL){
            Class base_class = Class{"String"};

            index_increase(context);
            while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                context_enforce_arguments(context, e, base_class);
                index_increase(context);
            }

            index_decrease(context);

            if(base_class.name == "void"){
                die(CANT_PASS_VOID_AS_ARGUMENT);
            }
            else if(type == ""){
                type = base_class.name;
            }
            else if(!context_class_compare(context, Class{type}, base_class)){
                fail(INCOMPATIBLE_CLASSES(type, base_class.name));
            }
        }
        else if(context.tokens[context.index].id == TOKENINDEX_NUMERIC_LITERAL){
            Class base_class = Class{"Number"};

            index_increase(context);
            while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                context_enforce_arguments(context, e, base_class);
                index_increase(context);
            }

            index_decrease(context);

            if(base_class.name == "void"){
                die(CANT_PASS_VOID_AS_ARGUMENT);
            }
            else if(type == ""){
                type = base_class.name;
            }
            else if(!context_class_compare(context, Class{type}, base_class)){
                fail(INCOMPATIBLE_CLASSES(type, base_class.name));
            }
        }
        else if(context.tokens[context.index].id == TOKENINDEX_ADD
        or context.tokens[context.index].id == TOKENINDEX_SUBTRACT
        or context.tokens[context.index].id == TOKENINDEX_MULTIPLY
        or context.tokens[context.index].id == TOKENINDEX_DIVIDE
        or context.tokens[context.index].id == TOKENINDEX_ASSIGN
        or context.tokens[context.index].id == TOKENINDEX_NOT){
            // ok
        }
        else if(context.tokens[context.index].id == TOKENINDEX_NEXT){
            arguments.push_back( MethodArgument{Class{type},0} );
            type = "";
        }
        else if(context.tokens[context.index].id == TOKENINDEX_KEYWORD){
            if(context.tokens[context.index].data == "void"){
                Class void_class = Class{"any^"};

                if(type == ""){
                    type = "any^";
                }
                else if(!context_class_compare(context, Class{type}, Class{"any^"})){
                    die(INCOMPATIBLE_CLASSES(type,"any^"));
                }
            }
            else {
                die(UNEXPECTED_KEYWORD(context.tokens[context.index].data));
            }
        }
        else {
            die(UNEXPECTED_OPERATOR_INEXP);
        }
    }

    if(type != ""){
        arguments.push_back( MethodArgument{Class{type},0} );
        type = "";
    }

    if(base_class.name == "void"){
        die(CANT_CALL_METHODS_OF_VOID);
    }
    else if(base_class.name != ""){
        if(!context_class_exists(context, e, base_class)){
            die(UNDECLARED_CLASS(base_class.name));
        }

        if( !context_method_exists(context, e, base_class, Method{method_name, NULL, arguments, IGNORE})){
            if( context_method_exists(context, e, base_class, Method{method_name, NULL, IGNORE_ARGS, IGNORE}) ){
                string call_string = base_class.name + "."  + method_name + "(";

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
                die(UNDECLARED_METHOD(base_class.name + "." + method_name));
            }
        }

        if( context_method_exists(context, e, base_class, Method{method_name, NULL, arguments, IGNORE}) ){
            context_method_get(context, e, base_class, Method{method_name, NULL, arguments, IGNORE});

            base_class.name = context_method_get(context, e, base_class, Method{method_name, NULL, arguments, IGNORE}).return_type;
        }
        else {
            log_enforcer("Method exists, but no implementation for the scope");
        }
    }
    else {
        if( !environment_method_exists(context, e.scope, Method{method_name, &e.global, arguments, IGNORE})
        and !environment_method_exists(context, &e.global, Method{method_name, &e.global, arguments, IGNORE})
        and !context_variable_exists(e, Variable{method_name, IGNORE}) ){
            if(environment_method_exists(context, e.scope, Method{method_name, &e.global, IGNORE_ARGS, IGNORE}) or environment_method_exists(context, &e.global, Method{method_name, &e.global, IGNORE_ARGS, IGNORE})){
                if(base_class.name != ""){
                    string call_string = base_class.name + "."  + method_name + "(";

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
                    string call_string = method_name + "(";

                    for(unsigned int i = 0; i < arguments.size(); i++){
                        call_string += arguments[i].type.name;

                        if(i != arguments.size()-1){
                            call_string += ", ";
                        }
                    }

                    call_string += ")";
                    die(UNDECLARED_METHOD_OTHERS(call_string));
                }
            }
            else {
                if(base_class.name != ""){
                    die(UNDECLARED_METHOD(base_class.name + "." + method_name));
                }
                else {
                    die(UNDECLARED_METHOD(method_name));
                }
            }
        }

        if(environment_method_exists(context, e.scope, Method{method_name, &e.global, arguments, IGNORE})){
            base_class.name = environment_method_get(context, e.scope, Method{method_name, &e.global, arguments, IGNORE}).return_type;
        }
        else if(environment_method_exists(context, &e.global, Method{method_name, &e.global, arguments, IGNORE})){
            base_class.name = environment_method_get(context, &e.global, Method{method_name, &e.global, arguments, IGNORE}).return_type;
        }
        else if(context_variable_exists(e, Variable{method_name, IGNORE})){
            std::string type = context_variable_get(e, Variable{method_name, IGNORE}).type;
            base_class.name = string_kill_whitespace(string_delete_amount(string_get_until(type,"->"),2));
        }
        else {
            log_enforcer("Method exists, but no implementation for the scope");
        }
    }
}
void context_enforce_method_declaration_arguments(TokenContext context, Environment& e, MethodArgumentList& method_arguments, std::string& argument_string){
    string type;
    bool optional = false;

    string variable_name;
    string class_name;
    string method_name = context.tokens[context.index].data;

    if(!is_identifier(method_name)){
        die(INVALID_METHOD_NAME(context.tokens[context.index].data));
    }

    token_force(context, TOKENINDEX_OPEN, ERROR_INDICATOR + "Unexpected statement termination\nExpected opening parentheses", ERROR_INDICATOR + "Expected opening parentheses");

    if(!advance_index(context.index,context.tokens.size())){
        die(UNEXPECTED_TERMINATE);
    }

    bool accept_type = true;
    bool first_word = true;

    while(context.tokens[context.index].id != TOKENINDEX_CLOSE){
        // Before assign, close, or next context.tokens
        while(context.tokens[context.index].id != TOKENINDEX_ASSIGN and context.tokens[context.index].id != TOKENINDEX_NEXT and context.tokens[context.index].id != TOKENINDEX_CLOSE){
            if(context.tokens[context.index].id ==TOKENINDEX_WORD){
                if(environment_class_exists(e.scope, Class{context.tokens[context.index].data}) or environment_class_exists(&e.global, Class{context.tokens[context.index].data})){
                    if(accept_type){
                        type += context.tokens[context.index].data;
                        argument_string += context.tokens[context.index].data;
                    }
                    else {
                        die(UNEXPECTED_WORD);
                    }
                }
                else {
                    if(accept_type){
                        if(!first_word){ // Variable name
                            accept_type = false;
                            e.scope->variables.push_back( Variable{context.tokens[context.index].data, type, false, false} );
                            variable_name = context.tokens[context.index].data;
                        }
                        else { // Undeclared class
                            die(UNDECLARED_CLASS(context.tokens[context.index].data));
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

            if(!advance_index(context.index,context.tokens.size())){
                die(UNEXPECTED_TERMINATE);
            }

            first_word = false;
        }

        // After That
        if(context.tokens[context.index].id == TOKENINDEX_ASSIGN){
            Class value_class;
            index_increase(context);
            context_enforce_expression(context, e, value_class);
            optional = true;
        }
        else if(context.tokens[context.index].id == TOKENINDEX_WORD){
            Class value_class;

            index_increase(context);
            context_enforce_expression(context, e, value_class);
            if(!advance_index(context.index,context.tokens.size())){
                die(UNEXPECTED_TERMINATE);
            }
        }
        else if(context.tokens[context.index].id == TOKENINDEX_NEXT){
            method_arguments.push_back( MethodArgument{Class{type}, optional} );

            argument_string += ",";
            type = "";
            accept_type = true;

            if(!advance_index(context.index,context.tokens.size())){
                die(UNEXPECTED_TERMINATE);
            }
        }
        else if(context.tokens[context.index].id == TOKENINDEX_CLOSE){

        }

        first_word = true;
    }

    if(type != ""){
        method_arguments.push_back( MethodArgument{Class{type}, optional} );
    }
}
void context_enforce_following_method_calls(TokenContext context, Environment& e, Class& type){
    while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
        if( environment_class_exists(&e.global, type) ){

            if(context_class_dereference_ifcan(context, type)){
                context.tokens[context.index].id = TOKENINDEX_POINTERMEMBER;
            }

            index_increase(context);

            if( environment_class_variable_exists(e, type ,Variable{context.tokens[context.index].data, IGNORE, false, false}) ){
                type.name = environment_class_variable_get(e, type, Variable{context.tokens[context.index].data, IGNORE, false, false}).type;
            }
            else {
                index_decrease(context);
                token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                context_enforce_arguments(context, e, type);
                index_increase(context);
            }
        }
        else {
            die(UNDECLARED_CLASS(type.name));
        }
    }
}
void context_enforce_string(TokenContext context, Environment& e, std::string str){
    for(unsigned int i = 0; i < str.length(); i++){
        if(str[i] == '\\'){
            i++;

            if(i < str.length()){
                if(str[i] == '\\'){
                    // Fine with that
                }
                else if(str[i] == '"'){
                    // Fine with that
                }
                else if(str[i] == '('){
                    i++;

                    std::string expression;
                    TokenList expression_tokens;
                    unsigned int expression_index = 0;
                    unsigned int balance = 0;
                    Class base = Class{"String"};

                    if(i >= str.length()){
                        die(UNEXPECTED_STRING_TERMINATION);
                    }

                    while(str[i] != ')' or balance != 0){
                        if(str[i] == '('){
                            balance++;
                        }
                        else if(str[i] == ')'){
                            balance--;
                        }

                        expression += str[i];

                        i++;
                        if(i >= str.length()){
                            die(UNEXPECTED_STRING_TERMINATION);
                        }
                    }

                    expression_tokens = tokenize(expression);
                    context_enforce_expression(TokenContext{expression_tokens, expression_index}, e, base);
                }
                else if(str[i] == 'x'){
                    // Fine with that
                }
                else {
                    fail(UNKNOWN_STRING_ESCAPE(str[i]));
                }
            }
            else {
                die(UNEXPECTED_STRING_TERMINATION);
            }
        }
    }
}

void context_assemble_string(TokenContext context, Environment& e, std::string str, std::string& output){
    output += "(boomslang_String(\"";

    for(unsigned int i = 0; i < str.length(); i++){
        if(str[i] == '\\'){
            i++;

            if(i < str.length()){
                if(str[i] == '\\'){
                    // Fine with that
                    output += "\\\\";
                }
                else if(str[i] == '"'){
                    // Fine with that
                    output += "\\\"";
                }
                else if(str[i] == '('){
                    i++;

                    std::string expression;
                    std::string expression_output;
                    TokenList expression_tokens;
                    unsigned int expression_index = 0;
                    unsigned int balance = 0;

                    while(str[i] != ')' or balance != 0){
                        if(str[i] == '('){
                            balance++;
                        }
                        else if(str[i] == ')'){
                            balance--;
                        }

                        expression += str[i];

                        i++;
                        if(i >= str.length()){
                            die(UNEXPECTED_STRING_TERMINATION);
                        }
                    }

                    expression_tokens = tokenize(expression);
                    assemble_expression(TokenContext{expression_tokens, expression_index}, expression_output, e);
                    output += "\")+(" + expression_output + ")+boomslang_String(\"";
                }
                else if(str[i] == 'x'){
                    // Fine with that
                    output += "\\x";
                }
            }
        }
        else {
            output += str[i];
        }
    }

    output += "\"))";
}

void context_class_dereference(TokenContext context, Class& type){
    if( type.name.length() == 0 ) {
        #ifdef DEV_ERRORS
        cerr << "A blank string was passed to context_class_dereference, returning" << endl;
        #endif // DEV_ERRORS

        return;
    }

    if( type.name.substr(type.name.length()-1,1) != "^"){
        die(CANT_DEREFERENCE_NON_POINTER_VALUE);
    }
    else {
        type.name = type.name.substr(0, type.name.length()-1);
    }
}
bool context_class_dereference_ifcan(TokenContext context, Class& type){
    if( type.name.length() == 0 ) return false;

    if( type.name.substr(type.name.length()-1,1) == "^"){
        type.name = type.name.substr(0, type.name.length()-1);
        return true;
    }

    return false;
}
bool context_class_can_dereference(TokenContext context, Class type){
    if( type.name.length() == 0 ) return false;

    if( type.name.substr(type.name.length()-1,1) == "^"){
        return true;
    }

    return false;
}
bool context_class_compare(TokenContext context, Class a, Class b){
    if(a.name == "any^"){
        if(context_class_can_dereference(context, b)){
            return true;
        }
    }

    if(b.name == "any^"){
        if(context_class_can_dereference(context, a)){
            return true;
        }
    }

    return (a.name == b.name);
}

bool name_is_class(std::string name){
    return (name.substr(0, string(CLASS_PREFIX).length()) == CLASS_PREFIX);
}
std::string name_get_class(std::string name){
    return name.substr(string(CLASS_PREFIX).length(), name.length() - string(CLASS_PREFIX).length());
}
bool name_is_method(std::string name){
    return (name.substr(0, string(METHOD_PREFIX).length()) == METHOD_PREFIX);
}
std::string name_get_method(std::string name){
    return name.substr(string(METHOD_PREFIX).length(), name.length() - string(METHOD_PREFIX).length());
}

