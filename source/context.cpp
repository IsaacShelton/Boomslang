
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
    if(environment_get_child(&e.global, CLASS_PREFIX + base.name) == NULL) return false;
    return environment_method_exists(context, environment_get_child(&e.global, CLASS_PREFIX + base.name), method);
}
Class context_root_class(Class base){
    base.name = string_get_until(base.name, "<");

    if(base.name != "")
        if(base.name.substr(base.name.length()-1, 1) == "^")
            base.name = base.name.substr(0, base.name.length()-1);

    return base;
}
std::string context_root_class(std::string base){
    base = string_get_until(base, "<");

    if(base != "")
        if(base.substr(base.length()-1, 1) == "^")
            base = base.substr(0, base.length()-1);

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
    bool compared = false; // Whether =, ==, >, <, <=, or >=
    bool andor = false;    // Whether 'and' or 'or'
    unsigned int token_id; // Cached beginning token

    while( (tokenid(context) != TOKENINDEX_NEXT and tokenid(context) != TOKENINDEX_TERMINATE and tokenid(context) != TOKENINDEX_CLOSE) ){
        token_id = tokenid(context); // Cache beginning token
        switch(token_id){
            case TOKENINDEX_OPEN:
                context_enforce_expression_open(context, e, type);
                break;
            case TOKENINDEX_LESSTHAN:
            case TOKENINDEX_GREATERTHAN:
                compared = true;
                break;
            case TOKENINDEX_POINTER:
                context_enforce_expression_pointer(context, e, type);
                break;
            case TOKENINDEX_ADDRESS:
                context_enforce_expression_address(context, e, type);
                break;
            case TOKENINDEX_WORD:
                context_enforce_expression_word(context, e, type);
                break;
            case TOKENINDEX_KEYWORD:
                context_enforce_expression_keyword(context, e, type, compared, andor);
                break;
            case TOKENINDEX_STRING_LITERAL:
                context_enforce_expression_string(context, e, type);
                break;
            case TOKENINDEX_NUMERIC_LITERAL:
                context_enforce_expression_numeric(context, e, type);
                break;
            case TOKENINDEX_DOUBLE_LITERAL:
                context_enforce_expression_literal(context, e, type, "Double");
                break;
            case TOKENINDEX_FLOAT_LITERAL:
                context_enforce_expression_literal(context, e, type, "Float");
                break;
            case TOKENINDEX_INTEGER_LITERAL:
                context_enforce_expression_literal(context, e, type, "Integer");
                break;
            case TOKENINDEX_UNSIGNED_LITERAL:
                context_enforce_expression_literal(context, e, type, "UnsignedInteger");
                break;
            case TOKENINDEX_ADD:
            case TOKENINDEX_SUBTRACT:
            case TOKENINDEX_MULTIPLY:
            case TOKENINDEX_DIVIDE:
            case TOKENINDEX_NOT:
                break;
            case TOKENINDEX_ASSIGN:
                compared = true;
                break;
            default:
                die(UNEXPECTED_OPERATOR_INEXP);
        }
        index_increase(context);
    }

    if( andor and !compared and !context_class_compare(context, type, Class("Boolean")) ) fail( EXPRESSION_BOOLEAN_ANDOR(type.toString()) );
}
void context_enforce_expression_open(TokenContext context, Environment& e, Class& type){
    Class base_class;
    index_increase(context);
    context_enforce_expression(context, e, base_class);
    index_increase(context);

    while(tokenid(context) == TOKENINDEX_MEMBER){
        if( context_class_exists(context, e, base_class) ){
            index_increase(context);
            if( environment_class_variable_exists(e, base_class, Variable(tokendata(context), IGNORE_CLASS, false, false)) ){
                base_class.name = environment_class_variable_get(e, base_class, Variable(tokendata(context), IGNORE_CLASS, false, false)).type.name;
                index_increase(context);
            }
            else {
                index_decrease(context);
                token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                context_enforce_arguments(context, e, base_class);
                index_increase(context);
            }
        }
        else die(UNDECLARED_CLASS(base_class.name));
    }

    index_decrease(context);
    if(type.name == "") { type.name = base_class.name; }
    else if(!context_class_compare(context, type, base_class)) fail( INCOMPATIBLE_CLASSES(type.toString(), base_class.toString()) );
}
void context_enforce_expression_pointer(TokenContext context, Environment& e, Class& type){
    index_increase(context);

    if(tokenid(context) == TOKENINDEX_WORD){
        std::string name = tokendata(context);
        index_increase(context);
        if( !environment_variable_exists(e.scope, Variable(name, IGNORE_CLASS, false, false)) ) fail(UNDECLARED_VARIABLE(name));
        Class base_class(environment_variable_get(e.scope, Variable(name, IGNORE_CLASS, false, false)).type);

        while(tokenid(context) == TOKENINDEX_MEMBER){
            if( context_class_exists(context, e, base_class) ){
                index_increase(context);

                if( environment_class_variable_exists(e, base_class, Variable(tokendata(context), IGNORE_CLASS, false, false)) ){
                    base_class.name = environment_class_variable_get(e, base_class, Variable(tokendata(context), IGNORE_CLASS, false, false)).type.name;
                    index_increase(context);
                }
                else {
                    index_decrease(context);
                    token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                    context_enforce_arguments(context, e, base_class);
                    index_increase(context);
                }
            }
            else die(UNDECLARED_CLASS(base_class.name));
        }

        index_decrease(context);
        context_class_dereference(context, base_class);

        if(type.name == "") { type.name = base_class.name; }
        else if(!context_class_compare(context, type, base_class)) fail( INCOMPATIBLE_CLASSES(type.toString(), base_class.toString()) );
    }
    else if(tokenid(context) == TOKENINDEX_OPEN){
        Class base;
        index_increase(context);
        context_enforce_expression(context, e, base);
        context_class_dereference(context, base);

        if(type.name == "") { type.name = base.name; }
        else if(!context_class_compare(context, type, base)) fail( INCOMPATIBLE_CLASSES(type.toString(), base.toString()) );
    }
    else die(UNEXPECTED_OPERATOR_INEXP);
}
void context_enforce_expression_address(TokenContext context, Environment& e, Class& type){
    index_increase(context);

    if(tokenid(context) == TOKENINDEX_WORD){
        std::string name = tokendata(context);
        index_increase(context);
        if( !environment_variable_exists(e.scope, Variable(name, IGNORE_CLASS, false, false)) ) fail(UNDECLARED_VARIABLE(name));
        Class base_class = environment_variable_get(e.scope, Variable(name, IGNORE_CLASS, false, false)).type;
        Class root_class = context_root_class(base_class);

        while(tokenid(context) == TOKENINDEX_MEMBER){
            bool dereferenced = context_class_can_dereference(context, base_class);

            if(dereferenced) tokenid(context) = TOKENINDEX_POINTERMEMBER;

            if( context_class_exists(context, e, root_class) ){
                index_increase(context);
                if(tokenid(context) != TOKENINDEX_WORD) die(UNEXPECTED_OPERATOR);
                name = tokendata(context);

                if( environment_class_variable_exists(e, root_class, Variable(tokendata(context), IGNORE_CLASS, false, false)) ){
                    base_class = environment_class_variable_get(e, root_class, Variable(tokendata(context), IGNORE_CLASS, false, false)).type;
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
            else die(UNDECLARED_CLASS(base_class.name));
        }

        index_decrease(context);
        base_class.name += "^";

        if(type.name == "") { type.name = base_class.name; }
        else if(!context_class_compare(context, type, base_class)) fail( INCOMPATIBLE_CLASSES(type.toString(), base_class.toString()) );
    }
    else if(tokenid(context) == TOKENINDEX_OPEN){
        Class base;
        index_increase(context);
        context_enforce_expression(context, e, base);
        base.name += "^";
        if(type.name == "") { type.name = base.name; }
        else if(!context_class_compare(context, type, base)) fail( INCOMPATIBLE_CLASSES(type.toString(), base.toString()) );
    }
    else die(UNEXPECTED_OPERATOR_INEXP);
}
void context_enforce_expression_word(TokenContext context, Environment& e, Class& type){
    std::string class_name = tokendata(context);
    index_increase(context);

    if(tokenid(context) == TOKENINDEX_OPEN){ // Function call
        std::string method_name = class_name;
        Class return_type;

        if(e.scope == &e.global) die(GLOBAL_STATEMENT);
        index_decrease(context);

        context_enforce_arguments(context, e, return_type);
        index_increase(context);

        while(tokenid(context) == TOKENINDEX_MEMBER){
            token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
            context_enforce_arguments(context, e, return_type);
            index_increase(context);
        }

        if(type.name == "") { type = return_type; }
        else if(!context_class_compare(context, type, return_type)) fail( INCOMPATIBLE_CLASSES(type.toString(), return_type.toString()) );
        index_decrease(context);
    }
    else if( environment_class_exists(&e.global, Class(class_name)) ){ // Class Name (for static methods)
        Class type = Class(class_name);

        if(tokenid(context) == TOKENINDEX_MEMBER){
            std::string method_name;

            tokenid(context) = TOKENINDEX_SCOPE_MEMBER;
            index_increase(context);
            if(tokenid(context) != TOKENINDEX_WORD) die(UNEXPECTED_OPERATOR);
            method_name = tokenid(context);
            context_enforce_arguments(context, e, type, "", true);
        }
        else die(UNEXPECTED_OPERATOR);
    }
    else { // Variable with possible method calls after
        std::string variable_name;
        Class base_class;

        // Go back to the word token containing the variable data
        retreat_index(context.index);
        variable_name = tokendata(context);

        // See whats after the variable name
        index_increase(context);

        // Ensure variable is declared
        if( !environment_variable_exists(e.scope, Variable(variable_name, IGNORE_CLASS, false, false)) ) fail(UNDECLARED_VARIABLE(variable_name));

        // Get the class name depending on where the variable was found
        if(environment_variable_exists(e.scope, Variable(variable_name, IGNORE_CLASS, false, false))){
            base_class = environment_variable_get(e.scope, Variable(variable_name, IGNORE_CLASS, false, false)).type;
        }
        else if(environment_variable_exists(&e.global, Variable(variable_name, IGNORE_CLASS, false, false))){
            base_class = environment_variable_get(&e.global, Variable(variable_name, IGNORE_CLASS, false, false)).type;
        }

        Class root_class = context_root_class(base_class);

        // Handle following fields and methods
        if(tokenid(context) == TOKENINDEX_MEMBER){
            while(tokenid(context) == TOKENINDEX_MEMBER){
                bool dereferenced = context_class_can_dereference(context, base_class);
                if(dereferenced) tokenid(context) = TOKENINDEX_POINTERMEMBER;

                if( context_class_exists(context, e, root_class) ){ // Make sure the current class is valid
                    // Next token should be a word
                    index_increase(context);

                    if(tokenid(context) != TOKENINDEX_WORD){
                        die(UNEXPECTED_OPERATOR);
                    }

                    variable_name = tokendata(context);

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
                else die(UNDECLARED_CLASS(root_class.name));
            }
        }

        if(type.name == "") { type = base_class; }
        else if(!context_class_compare(context, type, base_class)) fail( INCOMPATIBLE_CLASSES(type.toString(), base_class.toString()) );
        index_decrease(context);
    }
}
void context_enforce_expression_keyword(TokenContext context, Environment& e, Class& type, bool& compared, bool& andor){
    if(tokendata(context) == "new"){
        std::string class_name;

        index_increase(context);
        if(tokenid(context) != TOKENINDEX_WORD) die(UNEXPECTED_OPERATOR_INEXP);
        class_name = tokendata(context);

        if(!context_class_exists(context, e, Class(class_name))) die(UNDECLARED_CLASS(class_name));

        if(type.name == "") { type.name = class_name; }
        else if(!context_class_compare(context, type, Class(class_name))) fail( INCOMPATIBLE_CLASSES(type.toString(), class_name) );
        index_increase(context);

        if(tokenid(context) == TOKENINDEX_OPEN){
            Class created_class(class_name);
            index_decrease(context);
            context_enforce_arguments(context, e, created_class, "new");
        }
        else index_decrease(context);
    }
    else if(tokendata(context) == "create"){
        std::string class_name;

        index_increase(context);

        if(tokenid(context) != TOKENINDEX_WORD) die(UNEXPECTED_OPERATOR_INEXP);
        Class created_class(tokendata(context));
        class_name = tokendata(context) + "^";
        if(!context_class_exists(context, e, Class(class_name))) die(UNDECLARED_CLASS(class_name));

        if(type.name == "") { type.name = class_name; }
        else if(!context_class_compare(context, type, Class(class_name))) fail( INCOMPATIBLE_CLASSES(type.toString(), class_name) );
        index_increase(context);

        if(tokenid(context) == TOKENINDEX_OPEN){
            index_decrease(context);
            context_enforce_arguments(context, e, created_class, "new");
        }
        else index_decrease(context);
    }
    else if(tokendata(context) == "void"){
        if(type.name == "") { type.name = "any^"; }
        else if(!context_class_compare(context, type, Class("any^"))) fail( INCOMPATIBLE_CLASSES(type.toString(), "any^") );
    }
    else if(tokendata(context) == "and"){
        if( !compared and !context_class_compare(context, type, Class("Boolean")) ) fail( EXPRESSION_BOOLEAN_AND(type.toString()) );
        compared = false;
        andor = true;
        type = Class();
    }
    else if(tokendata(context) == "or"){
        if( !compared and !context_class_compare(context, type, Class("Boolean")) ) fail( EXPRESSION_BOOLEAN_OR(type.toString()) );
        compared = false;
        andor = true;
        type = Class();
    }
    else if(tokendata(context) == "cast"){
        index_increase(context);
        if(tokenid(context) != TOKENINDEX_WORD) die(UNEXPECTED_OPERATOR_INEXP);

        std::string conversion_type = tokendata(context);
        Class expression_type;

        index_increase(context);
        if(tokenid(context) != TOKENINDEX_OPEN and tokenid(context) != TOKENINDEX_WORD) die(EXPECTED_OPEN_IN_CONVERSION);

        if(tokenid(context) == TOKENINDEX_OPEN){
            context_enforce_expression(context, e, expression_type);
            index_decrease(context);
        }
        else if(tokenid(context) == TOKENINDEX_WORD){
            std::string variable_name = tokendata(context);

            if(!context_variable_exists(e, Variable(variable_name, IGNORE_CLASS, false, false))) die(UNDECLARED_CLASS(variable_name));
            expression_type = context_variable_get(e, Variable(variable_name, IGNORE_CLASS, false, false)).type;
        }

        if(type.name == "") { type.name = conversion_type; }
        else if(!context_class_compare(context, Class(conversion_type), type)) die(INCOMPATIBLE_CLASSES(type.toString(), conversion_type));
    }
    else die(UNEXPECTED_KEYWORD(tokendata(context)));
}
void context_enforce_expression_string(TokenContext context, Environment& e, Class& type){
    Class base_class = Class("String");
    index_increase(context);

    while(tokenid(context) == TOKENINDEX_MEMBER){
        if( context_class_exists(context, e, base_class) ){
            index_increase(context);

            if( environment_class_variable_exists(e, base_class, Variable(tokendata(context), IGNORE_CLASS, false, false)) ){
                base_class = environment_class_variable_get(e, base_class, Variable(tokendata(context), IGNORE_CLASS, false, false)).type;
                index_increase(context);
            }
            else {
                index_decrease(context);
                token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                context_enforce_arguments(context, e, base_class);
                index_increase(context);
            }
        }
        else die(UNDECLARED_CLASS(base_class.name));
    }

    index_decrease(context);

    if(type.name == "") { type = base_class; }
    else if(!context_class_compare(context, type, base_class)) fail( INCOMPATIBLE_CLASSES(type.toString(), base_class.toString()) );
}
void context_enforce_expression_numeric(TokenContext context, Environment& e, Class& type){
    Class base_class = Class("Double");
    tokenid(context) = TOKENINDEX_DOUBLE_LITERAL;

    index_increase(context);

    while(tokenid(context) == TOKENINDEX_MEMBER){
        if( context_class_exists(context, e, base_class) ){
            index_increase(context);

            if( environment_class_variable_exists(e, base_class, Variable(tokendata(context), IGNORE_CLASS, false, false)) ){
                base_class = environment_class_variable_get(e, base_class, Variable(tokendata(context), IGNORE_CLASS, false, false)).type;
                index_increase(context);
            }
            else {
                index_decrease(context);
                token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                context_enforce_arguments(context, e, base_class);
                index_increase(context);
            }
        }
        else die(UNDECLARED_CLASS(base_class.name));
    }

    index_decrease(context);

    if(type.name == ""){
        type = base_class;
    }
    else if(!context_class_compare(context, type, base_class)
    and type.name!="Integer"
    and type.name!="UnsignedInteger"
    and type.name!="Byte"
    and type.name!="uint"
    and type.name!="int"
    and type.name!="float"
    and type.name!="double"
    and type.name!="short"
    and type.name!="ushort"
    and type.name!="long"
    and type.name!="ulong"
    and type.name!="bool"
    and type.name!="char"
    and type.name!="uchar"){
        fail( INCOMPATIBLE_CLASSES(type.toString(), base_class.toString()) );
    }
}
void context_enforce_expression_literal(TokenContext context, Environment& e, Class& type, std::string literal){
    Class base_class = Class(literal);
    index_increase(context);

    while(tokenid(context) == TOKENINDEX_MEMBER){
        if( context_class_exists(context, e, base_class) ){
            index_increase(context);

            if( environment_class_variable_exists(e, base_class, Variable(tokendata(context), IGNORE_CLASS, false, false)) ){
                base_class = environment_class_variable_get(e, base_class, Variable(tokendata(context), IGNORE_CLASS, false, false)).type;
                index_increase(context);
            }
            else {
                index_decrease(context);
                token_force(context, TOKENINDEX_WORD, ERROR_INDICATOR + "Unexpected statement termination\nExpected method call after literal", ERROR_INDICATOR + "Expected method call after literal");
                context_enforce_arguments(context, e, base_class);
                index_increase(context);
            }
        }
        else die(UNDECLARED_CLASS(base_class.name));
    }

    index_decrease(context);
    if(type.name == ""){ type = base_class; }
    else if(!context_class_compare(context, type, base_class)) fail( INCOMPATIBLE_CLASSES(type.toString(), base_class.toString()) );
}

void context_enforce_arguments(TokenContext context, Environment& e, Class& base_class, std::string override_method, bool is_static){
    std::vector<MethodArgument> arguments;
    std::string type;
    std::string method_name = tokendata(context);
    std::string function_type = "function^(";

    Class root_class = context_root_class(base_class);

    if(method_name == "new") { die(NEW_METHOD_RESERVED); }
    else if(method_name == "delete") { die(DELETE_METHOD_RESERVED); }

    // Override Method Name
    if(override_method != "") method_name = override_method;

    token_force(context, TOKENINDEX_OPEN, ERROR_INDICATOR + "Unexpected statement termination\nExpected opening parentheses for method '" + method_name + "'", ERROR_INDICATOR + "Expected opening parentheses for method '" + method_name + "'");

    index_increase(context);
    if(tokenid(context) != TOKENINDEX_CLOSE) context.index--;

    while(tokenid(context) != TOKENINDEX_CLOSE){
        Class expression_type = Class("");
        index_increase(context);
        context_enforce_expression(context, e, expression_type);

        if(expression_type.name != ""){
            arguments.push_back( MethodArgument{expression_type, 0} );
            expression_type.name = "";
        }
        else die(EMPTY_EXPRESSION);
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
        if( !environment_generic_method_exists(context, root_class_scope, Method{method_name, NULL, arguments, IGNORE_CLASS, is_static}, actual_class, base_class) ){
            if( environment_generic_method_exists(context, root_class_scope, Method{method_name, NULL, IGNORE_ARGS, IGNORE_CLASS}, actual_class, base_class) ){
                std::string call_string = root_class.name + "."  + method_name + "(";
                std::string others = environment_similar_methods(context, root_class_scope, Method{method_name, NULL, IGNORE_ARGS, IGNORE_CLASS});

                for(unsigned int i = 0; i < arguments.size(); i++){
                    call_string += arguments[i].type.name;

                    if(i != arguments.size()-1){
                        call_string += ", ";
                    }
                }

                call_string += ")";

                die(UNDECLARED_METHOD_OTHERS(call_string) + others);
            }
            else {
                die(UNDECLARED_METHOD(root_class.name + "." + method_name));
            }
        }

        // Update return type
        if( environment_generic_method_exists(context, root_class_scope, Method{method_name, NULL, arguments, IGNORE_CLASS, is_static}, actual_class, base_class) ){
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
            logging_context.log_enforcer("Method exists, but no implementation for the scope");
        }
    }
    else {
        if( !environment_method_exists(context, e.scope, Method{method_name, &e.global, arguments, IGNORE_CLASS})
        and !environment_method_exists(context, &e.global, Method{method_name, &e.global, arguments, IGNORE_CLASS})
        /*and !environment_variable_exists(e.scope, Variable(method_name, IGNORE))*/ ){
            if(environment_method_exists(context, e.scope, Method{method_name, &e.global, IGNORE_ARGS, IGNORE_CLASS}) ){
                std::string others = environment_similar_methods(context, e.scope, Method{method_name, &e.global, IGNORE_ARGS, IGNORE_CLASS});

                if(base_class.name != ""){
                    std::string call_string = base_class.name + "."  + method_name + "(";

                    for(unsigned int i = 0; i < arguments.size(); i++){
                        call_string += arguments[i].type.name;

                        if(i != arguments.size()-1){
                            call_string += ", ";
                        }
                    }

                    call_string += ")";
                    die(UNDECLARED_METHOD_OTHERS(call_string) + others);
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
                    die(UNDECLARED_METHOD_OTHERS(call_string) + others);
                }
            }
            else if( environment_method_exists(context, &e.global, Method{method_name, &e.global, IGNORE_ARGS, IGNORE_CLASS}) ){
                std::string others = environment_similar_methods(context, &e.global, Method{method_name, &e.global, IGNORE_ARGS, IGNORE_CLASS});

                if(base_class.name != ""){
                    std::string call_string = base_class.name + "."  + method_name + "(";

                    for(unsigned int i = 0; i < arguments.size(); i++){
                        call_string += arguments[i].type.name;

                        if(i != arguments.size()-1){
                            call_string += ", ";
                        }
                    }

                    call_string += ")";
                    die(UNDECLARED_METHOD_OTHERS(call_string) + others);
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
                    die(UNDECLARED_METHOD_OTHERS(call_string) + others);
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
            logging_context.log_enforcer("Method exists, but no implementation for the scope");
        }
    }
}
void context_enforce_method_declaration_arguments(TokenContext context, Environment& e, MethodArgumentList& method_arguments, std::string& argument_string){
    std::string type;
    bool optional = false;

    std::string variable_name;
    std::string class_name;
    std::string method_name = tokendata(context);

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
        die(INVALID_METHOD_NAME(tokendata(context)));
    }

    token_force(context, TOKENINDEX_OPEN, ERROR_INDICATOR + "Unexpected statement termination\nExpected opening parentheses", ERROR_INDICATOR + "Expected opening parentheses");

    if(!advance_index(context.index,context.tokens.size())){
        die(UNEXPECTED_TERMINATE);
    }

    bool accept_type = true;
    bool first_word = true;

    while(tokenid(context) != TOKENINDEX_CLOSE){
        // Before assign, close, or next context.tokens
        while(tokenid(context) != TOKENINDEX_ASSIGN and tokenid(context) != TOKENINDEX_NEXT and tokenid(context) != TOKENINDEX_CLOSE){
            if(tokenid(context) ==TOKENINDEX_WORD){
                if(environment_class_exists(e.scope, Class(tokendata(context))) or environment_class_exists(&e.global, Class(tokendata(context)))){
                    if(accept_type){
                        // Add type to type list
                        type += tokendata(context);
                        argument_string += tokendata(context);

                        // Allow Pointers
                        index_increase(context);
                        while(tokenid(context) == TOKENINDEX_POINTER){
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
                            e.scope->variables.push_back( Variable(tokendata(context), type, false, false) );
                            variable_name = tokendata(context);
                        }
                        else { // Undeclared class
                            die(UNDECLARED_CLASS(tokendata(context)));
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
        if(tokenid(context) == TOKENINDEX_ASSIGN){
            Class value_class;
            index_increase(context);
            context_enforce_expression(context, e, value_class);
            optional = true;
        }
        else if(tokenid(context) == TOKENINDEX_WORD){
            Class value_class;

            index_increase(context);
            context_enforce_expression(context, e, value_class);
            if(!advance_index(context.index,context.tokens.size())){
                die(UNEXPECTED_TERMINATE);
            }
        }
        else if(tokenid(context) == TOKENINDEX_NEXT){
            method_arguments.push_back( MethodArgument{ Class(type), optional } );

            argument_string += ",";
            type = "";
            accept_type = true;

            if(!advance_index(context.index,context.tokens.size())){
                die(UNEXPECTED_TERMINATE);
            }
        }
        else if(tokenid(context) == TOKENINDEX_CLOSE){

        }

        first_word = true;
    }

    if(type != ""){
        method_arguments.push_back( MethodArgument{ Class(type), optional } );
    }
}
void context_enforce_following_method_calls(TokenContext context, Environment& e, Class& type){
    while(tokenid(context) == TOKENINDEX_MEMBER){
        if( environment_class_exists(&e.global, type) ){

            if(context_class_dereference_ifcan(context, type)){
                tokenid(context) = TOKENINDEX_POINTERMEMBER;
            }

            index_increase(context);

            if( environment_class_variable_exists(e, type ,Variable(tokendata(context), IGNORE_CLASS, false, false)) ){
                type = environment_class_variable_get(e, type, Variable(tokendata(context), IGNORE_CLASS, false, false)).type;
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
    for(size_t i = 0; i < str.length(); i++){
        if(str[i] == '\\'){
            if(++i < str.length()) { context_enforce_string_escape(context, e, str, i); }
            else die(UNEXPECTED_STRING_TERMINATION);
        }
    }
}
void context_enforce_string_escape(TokenContext context, Environment& e, std::string& str, size_t& i){
    char escape = str[i];

    switch(escape){
        case '\\':
        case 'n':
        case 'r':
        case 'a':
        case 't':
        case 'v':
        case 'x':
            break;
        case '(':
               context_enforce_string_escape_expression(context, e, str, i);
               break;
        default:
            fail(UNKNOWN_STRING_ESCAPE(str[i]));
    }
}
void context_enforce_string_escape_expression(TokenContext context, Environment& e, std::string& str, size_t& i){
    std::string expression;
    TokenList expression_tokens;
    unsigned int expression_index = 0;
    unsigned int balance = 0;
    Class base = Class("String");

    if(++i >= str.length()) die(UNEXPECTED_STRING_TERMINATION);

    while(str[i] != ')' or balance != 0){
        if(str[i] == '(') { balance++; }
        else if(str[i] == ')') { balance--; }
        expression += str[i];

        if(++i >= str.length()) die(UNEXPECTED_STRING_TERMINATION);
    }

    expression_tokens = tokenize(expression);
    context_enforce_expression(TokenContext{expression_tokens, expression_index}, e, base);
}
void context_enforce_type(TokenContext context, Environment& e, Class& type){
    if(tokenid(context) != TOKENINDEX_WORD) die(UNEXPECTED_OPERATOR);

    type.name = tokendata(context);
    type.generics.clear();
    index_increase(context);

    if(tokenid(context) == TOKENINDEX_LESSTHAN){
        unsigned int balance = 0;

        index_increase(context);

        while(tokenid(context) != TOKENINDEX_GREATERTHAN or balance != 0){
            if(tokenid(context) == TOKENINDEX_LESSTHAN){
                balance++;
                type.name += "<";
                index_increase(context);
            }
            else if(tokenid(context) == TOKENINDEX_GREATERTHAN){
                balance--;
                type.name += ">";
                index_increase(context);
            }
            else if(tokenid(context) == TOKENINDEX_NEXT){
                type.name += ",";
                index_increase(context);
            }
            else if(tokenid(context) == TOKENINDEX_WORD){
                Class inner_type; context_enforce_type(context, e, inner_type);
                type.generics.push_back(inner_type);
            }
            else {
                die(UNEXPECTED_OPERATOR);
            }
        }

        index_increase(context);

        while(tokenid(context) == TOKENINDEX_POINTER){
            type.name += "^";
            index_increase(context);
        }
    }
    else if(tokenid(context) == TOKENINDEX_POINTER){
        while(tokenid(context) == TOKENINDEX_POINTER){
            type.name += "^";
            index_increase(context);
        }
    }
}
void context_enforce_followup_block(Configuration* config, TokenContext context, Environment& e){
    // Check for and enforce follow up block
    // Should end off on terminate token

    if(tokenid(context) == TOKENINDEX_NEXT){
        index_increase(context);
        enforce_token(config, context, e);
    }
}

void context_assemble_string(TokenContext context, Environment& e, std::string str, std::string& output){
    output += "(boomslang_String(\"";
    for(size_t i = 0; i < str.length(); i++){
        if(str[i] == '\\' and i+1 < str.length()) {
            i++;
            if(str[i] == '\\') output += "\\\\";
            else if(str[i] == 'n') output += "\\n";
            else if(str[i] == 'r') output += "\\r";
            else if(str[i] == 'a') output += "\\a";
            else if(str[i] == 't') output += "\\t";
            else if(str[i] == 'v') output += "\\v";
            else if(str[i] == '"') output += "\\\"";
            else if(str[i] == 'x') output += "\\x";
            else if(str[i] == '(') output += context_assemble_string_expression(context, e, str, i);
        }
        else output += str[i];
    }
    output += "\"))";
}
std::string context_assemble_string_expression(TokenContext context, Environment& e, std::string& str, size_t& i){
    std::string expression;
    std::string expression_output;
    TokenList expression_tokens;
    unsigned int expression_index = 0;
    unsigned int balance = 0;
    i++;
    while(str[i] != ')' or balance != 0){
        if(str[i] == '(') balance++;
        else if(str[i] == ')') balance--;

        expression += str[i];
        i++;

        if(i >= str.length()) die(UNEXPECTED_STRING_TERMINATION);
    }

    expression_tokens = tokenize(expression);
    assemble_expression(TokenContext{expression_tokens, expression_index}, expression_output, e);
    return "\")+(" + expression_output + ")+boomslang_String(\"";
}

void context_class_dereference(TokenContext context, Class& type){
    if( type.name.length() == 0 ) {
        #ifdef DEV_ERRORS
        std::cerr << "A blank string was passed to context_class_dereference, returning" << std::endl;
        #endif // DEV_ERRORS
        return;
    }

    if(type.name.substr(type.name.length()-1,1) != "^") { die(CANT_DEREFERENCE_NON_POINTER_VALUE); }
    else type.name = type.name.substr(0, type.name.length()-1);
}
bool context_class_dereference_ifcan(TokenContext context, Class& type){
    if( type.name.length() == 0 ) return false;

    if(type.name.substr(type.name.length()-1, 1) == "^") {
        type.name = type.name.substr(0, type.name.length()-1);
        return true;
    }

    return false;
}
bool context_class_can_dereference(TokenContext context, Class type){
    if(type.name.length() == 0) return false;
    if(type.name.substr(type.name.length()-1, 1) == "^") return true;

    return false;
}
bool context_class_compare(TokenContext context, Class a, Class b){
    // Types are compatible with themselves
    if(a.name == b.name) return true;

    // A pointer and an any^ are compatible
    if(a.name == "any^" and context_class_can_dereference(context, b)) return true;
    else if(b.name == "any^" and context_class_can_dereference(context, a)) return true;

    if( (a.name == "Integer" and b.name == "int")           or (a.name == "int" and b.name == "Integer") )          return true;
    if( (a.name == "UnsignedInteger" and b.name == "uint")  or (a.name == "uint" and b.name == "UnsignedInteger") ) return true;
    if( (a.name == "Boolean" and b.name == "bool")          or (a.name == "bool" and b.name == "Boolean") )         return true;
    if( (a.name == "Double" and b.name == "double")         or (a.name == "double" and b.name == "Double") )        return true;
    if( (a.name == "Float" and b.name == "float")           or (a.name == "float" and b.name == "Float") )          return true;
    if( (a.name == "Byte" and b.name == "char")             or (a.name == "char" and b.name == "Byte") )            return true;

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

