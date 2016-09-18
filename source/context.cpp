
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

bool context_variable_exists(Environment& e, Variable variable){
    return (environment_variable_exists(e.scope, variable) or environment_variable_exists(&e.global, variable));
}
bool context_class_exists(TokenContext context, Environment& e, Class base){
    if(base.name.substr(0, 4) == "any^") return true;
    if(base.name.substr(0, 10) == "function^(") return true;

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
Class context_root_class(Class base){
    base.name = string_get_until(base.name, "<");

    if(base.name != ""){
        if(base.name.substr(base.name.length()-1, 1) == "^"){
            base.name = base.name.substr(0, base.name.length()-1);
        }
    }

    return base;
}
std::string context_root_class(std::string base){
    base = string_get_until(base, "<");

    if(base != ""){
        if(base.substr(base.length()-1, 1) == "^"){
            base = base.substr(0, base.length()-1);
        }
    }

    return base;
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

        return Method{IGNORE, NULL, IGNORE_ARGS, IGNORE_CLASS};
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

                    if( environment_class_variable_exists(e, base_class, Variable(context.tokens[context.index].data, IGNORE_CLASS, false, false)) ){
                        base_class.name = environment_class_variable_get(e, base_class, Variable(context.tokens[context.index].data, IGNORE_CLASS, false, false)).type.name;
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
                fail( INCOMPATIBLE_CLASSES(type.toString(), base_class.toString()) );
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

                if( !environment_variable_exists(e.scope, Variable(name, IGNORE_CLASS, false, false)) ){
                    fail(UNDECLARED_VARIABLE(name));
                }

                Class base_class(environment_variable_get(e.scope, Variable(name, IGNORE_CLASS, false, false)).type);

                while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                    if( context_class_exists(context, e, base_class) ){
                        index_increase(context);

                        if( environment_class_variable_exists(e, base_class, Variable(context.tokens[context.index].data, IGNORE_CLASS, false, false)) ){
                            base_class.name = environment_class_variable_get(e, base_class, Variable(context.tokens[context.index].data, IGNORE_CLASS, false, false)).type.name;
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
                    fail( INCOMPATIBLE_CLASSES(type.toString(), base_class.toString()) );
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
                    fail( INCOMPATIBLE_CLASSES(type.toString(), base.toString()) );
                }
            }
            else {
                die(UNEXPECTED_OPERATOR_INEXP);
            }
        }
        else if(context.tokens[context.index].id == TOKENINDEX_ADDRESS){
            index_increase(context);

            if(context.tokens[context.index].id == TOKENINDEX_WORD){
                std::string name = context.tokens[context.index].data;
                index_increase(context);

                if( !environment_variable_exists(e.scope, Variable(name, IGNORE_CLASS, false, false)) ){
                    fail(UNDECLARED_VARIABLE(name));
                }

                Class base_class = environment_variable_get(e.scope, Variable(name, IGNORE_CLASS, false, false)).type;
                Class root_class = context_root_class(base_class);

                while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                    bool dereferenced = context_class_can_dereference(context, base_class);

                    if(dereferenced){
                        context.tokens[context.index].id = TOKENINDEX_POINTERMEMBER;
                    }

                    if( context_class_exists(context, e, root_class) ){
                        index_increase(context);

                        if(context.tokens[context.index].id != TOKENINDEX_WORD){
                            die(UNEXPECTED_OPERATOR);
                        }

                        name = context.tokens[context.index].data;

                        if( environment_class_variable_exists(e, root_class, Variable(context.tokens[context.index].data, IGNORE_CLASS, false, false)) ){
                            base_class = environment_class_variable_get(e, root_class, Variable(context.tokens[context.index].data, IGNORE_CLASS, false, false)).type;
                            root_class = context_root_class(base_class);
                            index_increase(context);
                        }
                        else {
                            index_decrease(context);
                            token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                            context_enforce_arguments(context, e, base_class);
                            root_class = context_root_class(base_class);
                            index_increase(context);
                        }
                    }
                    else {
                        die(UNDECLARED_CLASS(base_class.name));
                    }
                }

                index_decrease(context);
                base_class.name += "^";

                if(type.name == ""){
                    type.name = base_class.name;
                }
                else if(!context_class_compare(context, type, base_class)){
                    fail( INCOMPATIBLE_CLASSES(type.toString(), base_class.toString()) );
                }
            }
            else if(context.tokens[context.index].id == TOKENINDEX_OPEN){
                Class base;

                index_increase(context);
                context_enforce_expression(context, e, base);

                base.name += "^";

                if(type.name == ""){
                    type.name = base.name;
                }
                else if(!context_class_compare(context, type, base)){
                    fail( INCOMPATIBLE_CLASSES(type.toString(), base.toString()) );
                }
            }
            else {
                die(UNEXPECTED_OPERATOR_INEXP);
            }
        }
        else if(context.tokens[context.index].id == TOKENINDEX_WORD){
            index_increase(context);

            if(context.tokens[context.index].id == TOKENINDEX_OPEN){ // Function call
                if(e.scope == &e.global){
                    die(GLOBAL_STATEMENT);
                }

                index_decrease(context);

                std::string method_name = context.tokens[context.index].data;
                Class return_type;

                context_enforce_arguments(context, e, return_type);

                if( !advance_index(context.index, context.tokens.size()) ){
                    die(ERROR_INDICATOR + UNEXPECTED_TERMINATE);
                }

                while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                    token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");

                    context_enforce_arguments(context, e, return_type);
                    index_increase(context);
                }

                if(type.name == ""){
                    type = return_type;
                }
                else if(!context_class_compare(context, type, return_type)){
                    fail( INCOMPATIBLE_CLASSES(type.toString(), return_type.toString()) );
                }

                index_decrease(context);
            }
            else { // Variable with possible method calls after
                std::string variable_name;
                Class base_class;

                // Go back to the word token containing the variable data
                retreat_index(context.index);
                variable_name = context.tokens[context.index].data;

                // See whats after the variable name
                index_increase(context);

                // Ensure variable is declared
                if( !environment_variable_exists(e.scope, Variable(variable_name, IGNORE_CLASS, false, false)) ){
                    fail(UNDECLARED_VARIABLE(variable_name));
                }

                // Get the class name depending on where the variable was found
                if(environment_variable_exists(e.scope, Variable(variable_name, IGNORE_CLASS, false, false))){
                    base_class = environment_variable_get(e.scope, Variable(variable_name, IGNORE_CLASS, false, false)).type;
                }
                else if(environment_variable_exists(&e.global, Variable(variable_name, IGNORE_CLASS, false, false))){
                    base_class = environment_variable_get(&e.global, Variable(variable_name, IGNORE_CLASS, false, false)).type;
                }

                Class root_class = context_root_class(base_class);

                // Handle following fields and methods
                if(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                    while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                        bool dereferenced = context_class_can_dereference(context, base_class);

                        if(dereferenced){
                            context.tokens[context.index].id = TOKENINDEX_POINTERMEMBER;
                        }

                        if( context_class_exists(context, e, root_class) ){ // Make sure the current class is valid
                            // Next token should be a word
                            index_increase(context);

                            if(context.tokens[context.index].id != TOKENINDEX_WORD){
                                die(UNEXPECTED_OPERATOR);
                            }

                            variable_name = context.tokens[context.index].data;

                            // Is it a field of the class
                            if( environment_class_variable_exists(e, root_class, Variable(variable_name, IGNORE_CLASS, false, false)) ){
                                base_class = environment_class_variable_get(e, root_class, Variable(variable_name, IGNORE_CLASS, false, false)).type;
                                root_class = context_root_class(base_class);
                                index_increase(context);
                            }
                            else { // Method of the class
                                context_enforce_arguments(context, e, base_class);
                                root_class = context_root_class(base_class);
                                index_increase(context);
                            }
                        }
                        else { // Undeclared Class
                            die(UNDECLARED_CLASS(root_class.name));
                        }
                    }
                }

                if(type.name == ""){
                    type = base_class;
                }
                else if(!context_class_compare(context, type, base_class)){
                    fail( INCOMPATIBLE_CLASSES(type.toString(), base_class.toString()) );
                }

                index_decrease(context);
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

                if(!context_class_exists(context, e, Class(class_name))){
                    die(UNDECLARED_CLASS(class_name));
                }

                if(type.name == ""){
                    type.name = class_name;
                }
                else if(!context_class_compare(context, type, Class(class_name))){
                    fail( INCOMPATIBLE_CLASSES(type.toString(), class_name) );
                }

                index_increase(context);

                if(context.tokens[context.index].id == TOKENINDEX_OPEN){
                    Class created_class(class_name);

                    index_decrease(context);
                    context_enforce_arguments(context, e, created_class, "new");
                }
                else {
                    index_decrease(context);
                }
            }
            else if(context.tokens[context.index].data == "create"){
                std::string class_name;

                index_increase(context);

                if(context.tokens[context.index].id != TOKENINDEX_WORD){
                    die(UNEXPECTED_OPERATOR_INEXP);
                }

                Class created_class(context.tokens[context.index].data);
                class_name = context.tokens[context.index].data + "^";

                if(!context_class_exists(context, e, Class(class_name))){
                    die(UNDECLARED_CLASS(class_name));
                }

                if(type.name == ""){
                    type.name = class_name;
                }
                else if(!context_class_compare(context, type, Class(class_name))){
                    fail( INCOMPATIBLE_CLASSES(type.toString(), class_name) );
                }

                index_increase(context);

                if(context.tokens[context.index].id == TOKENINDEX_OPEN){
                    index_decrease(context);
                    context_enforce_arguments(context, e, created_class, "new");
                }
                else {
                    index_decrease(context);
                }
            }
            else if(context.tokens[context.index].data == "void"){
                if(type.name == ""){
                    type.name = "any^";
                }
                else if(!context_class_compare(context, type, Class("any^"))){
                    fail( INCOMPATIBLE_CLASSES(type.toString(), "any^") );
                }
            }
            else if(context.tokens[context.index].data == "and"){
            }
            else if(context.tokens[context.index].data == "or"){
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

                    if(!context_variable_exists(e, Variable(variable_name, IGNORE_CLASS, false, false))){
                        die(UNDECLARED_CLASS(variable_name));
                    }
                    expression_type = context_variable_get(e, Variable(variable_name, IGNORE_CLASS, false, false)).type;
                }

                if(type.name == ""){
                    type.name = conversion_type;
                }
                else if(!context_class_compare(context, Class(conversion_type), type)){
                    die(INCOMPATIBLE_CLASSES(type.toString(), conversion_type));
                }
            }
            else {
                die(UNEXPECTED_KEYWORD(context.tokens[context.index].data));
            }
        }
        else if(context.tokens[context.index].id == TOKENINDEX_STRING_LITERAL){
            Class base_class = Class("String");

            index_increase(context);

            while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                if( context_class_exists(context, e, base_class) ){
                    index_increase(context);

                    if( environment_class_variable_exists(e, base_class, Variable(context.tokens[context.index].data, IGNORE_CLASS, false, false)) ){
                        base_class = environment_class_variable_get(e, base_class, Variable(context.tokens[context.index].data, IGNORE_CLASS, false, false)).type;
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
                type = base_class;
            }
            else if(!context_class_compare(context, type, base_class)){
                fail( INCOMPATIBLE_CLASSES(type.toString(), base_class.toString()) );
            }
        }
        else if(context.tokens[context.index].id == TOKENINDEX_NUMERIC_LITERAL){
            Class base_class = Class("Number");

            index_increase(context);

            while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                if( context_class_exists(context, e, base_class) ){
                    index_increase(context);

                    if( environment_class_variable_exists(e, base_class, Variable(context.tokens[context.index].data, IGNORE_CLASS, false, false)) ){
                        base_class = environment_class_variable_get(e, base_class, Variable(context.tokens[context.index].data, IGNORE_CLASS, false, false)).type;
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
                type = base_class;
            }
            else if(!context_class_compare(context, type, base_class)
            and type.name!="Integer"
            and type.name!="UnsignedInteger"
            and type.name!="Byte"){
                fail( INCOMPATIBLE_CLASSES(type.toString(), base_class.toString()) );
            }
        }
        else if(context.tokens[context.index].id == TOKENINDEX_NUMBER_LITERAL){
            Class base_class = Class("Number");

            index_increase(context);

            while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                if( context_class_exists(context, e, base_class) ){
                    index_increase(context);

                    if( environment_class_variable_exists(e, base_class, Variable(context.tokens[context.index].data, IGNORE_CLASS, false, false)) ){
                        base_class = environment_class_variable_get(e, base_class, Variable(context.tokens[context.index].data, IGNORE_CLASS, false, false)).type;
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
                type = base_class;
            }
            else if(!context_class_compare(context, type, base_class)){
                fail( INCOMPATIBLE_CLASSES(type.toString(), base_class.toString()) );
            }
        }
        else if(context.tokens[context.index].id == TOKENINDEX_INTEGER_LITERAL){
            Class base_class = Class("Integer");

            index_increase(context);

            while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                if( context_class_exists(context, e, base_class) ){
                    index_increase(context);

                    if( environment_class_variable_exists(e, base_class, Variable(context.tokens[context.index].data, IGNORE_CLASS, false, false)) ){
                        base_class = environment_class_variable_get(e, base_class, Variable(context.tokens[context.index].data, IGNORE_CLASS, false, false)).type;
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
                type = base_class;
            }
            else if(!context_class_compare(context, type, base_class)){
                fail( INCOMPATIBLE_CLASSES(type.toString(), base_class.toString()) );
            }
        }
        else if(context.tokens[context.index].id == TOKENINDEX_UNSIGNED_LITERAL){
            Class base_class = Class("UnsignedInteger");

            index_increase(context);

            while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
                if( context_class_exists(context, e, base_class) ){
                    index_increase(context);

                    if( environment_class_variable_exists(e, base_class, Variable(context.tokens[context.index].data, IGNORE_CLASS, false, false)) ){
                        base_class = environment_class_variable_get(e, base_class, Variable(context.tokens[context.index].data, IGNORE_CLASS, false, false)).type;
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
                type = base_class;
            }
            else if(!context_class_compare(context, type, base_class)){
                fail( INCOMPATIBLE_CLASSES(type.toString(), base_class.toString()) );
            }
        }
        else if(context.tokens[context.index].id == TOKENINDEX_ADD
        or context.tokens[context.index].id == TOKENINDEX_SUBTRACT
        or context.tokens[context.index].id == TOKENINDEX_MULTIPLY
        or context.tokens[context.index].id == TOKENINDEX_DIVIDE
        or context.tokens[context.index].id == TOKENINDEX_ASSIGN
        or context.tokens[context.index].id == TOKENINDEX_NOT){
            // We're fine with that
        }
        else {
            die(UNEXPECTED_OPERATOR_INEXP);
        }

        index_increase(context);
    }
}
void context_enforce_arguments(TokenContext context, Environment& e, Class& base_class, std::string override_method){
    std::vector<MethodArgument> arguments;
    std::string type;
    std::string method_name = context.tokens[context.index].data;
    std::string function_type = "function^(";

    Class root_class = context_root_class(base_class);

    if(method_name == "new"){
        die(NEW_METHOD_RESERVED);
    }

    // Override Method Name
    if(override_method != ""){
        method_name = override_method;
    }

    token_force(context, TOKENINDEX_OPEN, ERROR_INDICATOR + "Unexpected statement termination\nExpected opening parentheses for method '" + method_name + "'", ERROR_INDICATOR + "Expected opening parentheses for method '" + method_name + "'");

    index_increase(context);
    if(context.tokens[context.index].id != TOKENINDEX_CLOSE){
        context.index--;
    }

    while(context.tokens[context.index].id != TOKENINDEX_CLOSE){
        Class expression_type = Class("");
        if(!advance_index(context.index,context.tokens.size())){
            die(UNEXPECTED_TERMINATE);
        }

        context_enforce_expression(context, e, expression_type);

        if(expression_type.name != ""){
            arguments.push_back( MethodArgument{expression_type, 0} );
            expression_type.name = "";
        }
        else {
            die(EMPTY_EXPRESSION);
        }
    }

    if(base_class.name == "void"){
        die(CANT_CALL_METHODS_OF_VOID);
    }
    else if(base_class.name != ""){
        if(!context_class_exists(context, e, root_class)){
            die(UNDECLARED_CLASS(root_class.name));
        }

        Class actual_class = context_class_get(e, root_class);
        Scope* root_class_scope = environment_get_child(&e.global, CLASS_PREFIX + root_class.name);

        if(root_class_scope == NULL){
            die( UNDECLARED_CLASS(root_class.name) );
        }

        // Ensure method exists
        if( !environment_generic_method_exists(context, root_class_scope, Method{method_name, NULL, arguments, IGNORE_CLASS}, actual_class, base_class) ){
            if( environment_generic_method_exists(context, root_class_scope, Method{method_name, NULL, IGNORE_ARGS, IGNORE_CLASS}, actual_class, base_class) ){
                std::string call_string = root_class.name + "."  + method_name + "(";

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
                die(UNDECLARED_METHOD(root_class.name + "." + method_name));
            }
        }

        // Update return type
        if( environment_generic_method_exists(context, root_class_scope, Method{method_name, NULL, arguments, IGNORE_CLASS}, actual_class, base_class) ){
            Method method = environment_generic_method_get(context, root_class_scope, Method{method_name, NULL, arguments, IGNORE_CLASS}, actual_class, base_class);
            base_class = method.return_type;

            for(size_t g = 0; g < actual_class.generics.size(); g++){
                if(method.return_type == actual_class.generics[g]){
                    base_class = base_class.generics[g];
                    break;
                }
            }

            base_class.generics.clear();
        }
        else {
            log_enforcer("Method exists, but no implementation for the scope");
        }
    }
    else {
        if( !environment_method_exists(context, e.scope, Method{method_name, &e.global, arguments, IGNORE_CLASS})
        and !environment_method_exists(context, &e.global, Method{method_name, &e.global, arguments, IGNORE_CLASS})
        /*and !environment_variable_exists(e.scope, Variable(method_name, IGNORE))*/ ){
            if(environment_method_exists(context, e.scope, Method{method_name, &e.global, IGNORE_ARGS, IGNORE_CLASS}) or environment_method_exists(context, &e.global, Method{method_name, &e.global, IGNORE_ARGS, IGNORE_CLASS})){
                if(base_class.name != ""){
                    std::string call_string = base_class.name + "."  + method_name + "(";

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
                    std::string call_string = method_name + "(";

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

        if(environment_method_exists(context, e.scope, Method{method_name, &e.global, arguments, IGNORE_CLASS})){
            base_class = environment_method_get(context, e.scope, Method{method_name, &e.global, arguments, IGNORE_CLASS}).return_type;
        }
        else if(environment_method_exists(context, &e.global, Method{method_name, &e.global, arguments, IGNORE_CLASS})){
            base_class = environment_method_get(context, &e.global, Method{method_name, &e.global, arguments, IGNORE_CLASS}).return_type;
        }
        /*else if(environment_variable_exists(e.scope, Variable(method_name, IGNORE))){
            std::string type = context_variable_get(e, Variable(method_name, IGNORE)).type;
            base_class.name = string_kill_whitespace(string_delete_amount(string_get_until(type,"->"),2));
        }*/
        else {
            log_enforcer("Method exists, but no implementation for the scope");
        }
    }
}
void context_enforce_method_declaration_arguments(TokenContext context, Environment& e, MethodArgumentList& method_arguments, std::string& argument_string){
    std::string type;
    bool optional = false;

    std::string variable_name;
    std::string class_name;
    std::string method_name = context.tokens[context.index].data;

    if(!is_identifier(method_name)
    and method_name != "="
    and method_name != "+"
    and method_name != "-"
    and method_name != "*"
    and method_name != "/"
    and method_name != "+="
    and method_name != "-="
    and method_name != "*="
    and method_name != "/="
    and method_name != "=="
    and method_name != ">"
    and method_name != "<"
    and method_name != ">="
    and method_name != "<="){
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
                if(environment_class_exists(e.scope, Class(context.tokens[context.index].data)) or environment_class_exists(&e.global, Class(context.tokens[context.index].data))){
                    if(accept_type){
                        // Add type to type list
                        type += context.tokens[context.index].data;
                        argument_string += context.tokens[context.index].data;

                        // Allow Pointers
                        index_increase(context);
                        while(context.tokens[context.index].id == TOKENINDEX_POINTER){
                            type += "^";
                            argument_string += "^";
                            index_increase(context);
                        }
                        index_decrease(context);
                    }
                    else {
                        die(UNEXPECTED_WORD);
                    }
                }
                else {
                    if(accept_type){
                        if(!first_word){ // Variable name
                            accept_type = false;
                            e.scope->variables.push_back( Variable(context.tokens[context.index].data, type, false, false) );
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
            method_arguments.push_back( MethodArgument{ Class(type), optional } );

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
        method_arguments.push_back( MethodArgument{ Class(type), optional } );
    }
}
void context_enforce_following_method_calls(TokenContext context, Environment& e, Class& type){
    while(context.tokens[context.index].id == TOKENINDEX_MEMBER){
        if( environment_class_exists(&e.global, type) ){

            if(context_class_dereference_ifcan(context, type)){
                context.tokens[context.index].id = TOKENINDEX_POINTERMEMBER;
            }

            index_increase(context);

            if( environment_class_variable_exists(e, type ,Variable(context.tokens[context.index].data, IGNORE_CLASS, false, false)) ){
                type = environment_class_variable_get(e, type, Variable(context.tokens[context.index].data, IGNORE_CLASS, false, false)).type;
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
                else if(str[i] == 'n'){
                    // Fine with that
                }
                else if(str[i] == 'r'){
                    // Fine with that
                }
                else if(str[i] == 'a'){
                    // Fine with that
                }
                else if(str[i] == 't'){
                    // Fine with that
                }
                else if(str[i] == 'v'){
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
                    Class base = Class("String");

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
void context_enforce_type(TokenContext context, Environment& e, Class& type){

    if(context.tokens[context.index].id != TOKENINDEX_WORD){
        die(UNEXPECTED_OPERATOR);
    }

    type.name = context.tokens[context.index].data;
    type.generics.clear();

    index_increase(context);

    if(context.tokens[context.index].id == TOKENINDEX_LESSTHAN){
        unsigned int balance = 0;

        index_increase(context);

        while(context.tokens[context.index].id != TOKENINDEX_GREATERTHAN or balance != 0){
            if(context.tokens[context.index].id == TOKENINDEX_LESSTHAN){
                balance++;
                type.name += "<";
                index_increase(context);
            }
            else if(context.tokens[context.index].id == TOKENINDEX_GREATERTHAN){
                balance--;
                type.name += ">";
                index_increase(context);
            }
            else if(context.tokens[context.index].id == TOKENINDEX_NEXT){
                type.name += ",";
                index_increase(context);
            }
            else if(context.tokens[context.index].id == TOKENINDEX_WORD){
                Class inner_type; context_enforce_type(context, e, inner_type);
                type.generics.push_back(inner_type);
            }
            else {
                die(UNEXPECTED_OPERATOR);
            }
        }

        index_increase(context);

        while(context.tokens[context.index].id == TOKENINDEX_POINTER){
            type.name += "^";
            index_increase(context);
        }
    }
    else if(context.tokens[context.index].id == TOKENINDEX_POINTER){
        while(context.tokens[context.index].id == TOKENINDEX_POINTER){
            type.name += "^";
            index_increase(context);
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
                else if(str[i] == 'n'){
                    // Fine with that
                    output += "\\n";
                }
                else if(str[i] == 'r'){
                    // Fine with that
                    output += "\\r";
                }
                else if(str[i] == 'a'){
                    // Fine with that
                    output += "\\a";
                }
                else if(str[i] == 't'){
                    // Fine with that
                    output += "\\t";
                }
                else if(str[i] == 'v'){
                    // Fine with that
                    output += "\\v";
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
        std::cerr << "A blank string was passed to context_class_dereference, returning" << std::endl;
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

    // Types are compatible with themselves
    if(a.name == b.name){
        return true;
    }

    // A pointer and an any^ are compatible
    if(a.name == "any^" and context_class_can_dereference(context, b)){
        return true;
    }
    else if(b.name == "any^" and context_class_can_dereference(context, a)){
        return true;
    }

    return false;
}

bool name_is_class(std::string name){
    return (name.substr(0, std::string(CLASS_PREFIX).length()) == CLASS_PREFIX);
}
std::string name_get_class(std::string name){
    return name.substr(std::string(CLASS_PREFIX).length(), name.length() - std::string(CLASS_PREFIX).length());
}
bool name_is_method(std::string name){
    return (name.substr(0, std::string(METHOD_PREFIX).length()) == METHOD_PREFIX);
}
std::string name_get_method(std::string name){
    return name.substr(std::string(METHOD_PREFIX).length(), name.length() - std::string(METHOD_PREFIX).length());
}

