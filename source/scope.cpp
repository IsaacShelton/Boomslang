
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
#include "../include/die.h"
#include "../include/scope.h"
#include "../include/token.h"
#include "../include/errors.h"
#include "../include/context.h"

Class::Class(){}
Class::Class(std::string class_name){
    name = class_name;
}
Class::Class(std::string class_name, std::vector<Class> class_generics){
    name = class_name;
    generics = class_generics;
}
Class::Class(std::string class_name, std::vector<Class> class_generics, bool class_mutable){
    name = class_name;
    generics = class_generics;
    is_mutable = class_mutable;
}
void Class::operator=(const Class& other){
    this->name = other.name;
    this->generics = other.generics;
}
bool Class::operator==(const Class& other){
    if(this->name != other.name) return false;
    if(this->generics.size() != other.generics.size()) return false;

    for(size_t i = 0; i < generics.size(); i++){
        if(!( this->generics[i] == other.generics[i] )) return false;
    }

    return true;
}
void Class::load(std::string rawtype){
    name = string_get_until(rawtype, "<");
    rawtype = string_delete_amount(rawtype, name.length());
    rawtype = string_kill_whitespace(rawtype);

    if(rawtype.length() != 0){
        if(rawtype.substr(0,1) == "<"){
            int balance = 0;
            rawtype = string_delete_amount(rawtype, 1);

            while(rawtype.substr(0,1) != ">" or balance != 0){
                Class generic_type;
                std::string generic_rawtype;

                generic_rawtype = string_get_until_or(rawtype, " ,>");
                rawtype = string_delete_amount( rawtype, generic_rawtype.length() );
                generic_type.load(generic_rawtype);
                generics.push_back( generic_type );

                if(rawtype.substr(0,1) == ",") {
                    rawtype = string_delete_amount(rawtype, 1);
                }
            }
        }
    }
}
std::string Class::native(){
    std::string type;

    if(generics.size() > 0){
        type = resource(name) + "<";
        for(size_t i = 0; i < generics.size(); i++){
            type += generics[i].native();
            if(i + 1 < generics.size()) type += ",";
        }
        type += ">";
    }
    else {
        type = resource(name);
    }

    return type;
}
std::string Class::toString(){
    std::string type;

    if(generics.size() > 0){
        type = name + "<";
        for(size_t i = 0; i < generics.size(); i++){
            type += generics[i].toString();
            if(i + 1 < generics.size()) type += ",";
        }
        type += ">";
    }
    else {
        type = name;
    }

    return type;
}

Variable::Variable(){
    name = "";
    type = Class("");
    is_final = false;
    is_static = false;
    is_private = false;
}
Variable::Variable(std::string var_name, Class var_class){
    name = var_name;
    type = var_class;
    is_final = false;
    is_static = false;
    is_private = false;
}
Variable::Variable(std::string var_name, Class var_class, bool var_final, bool var_static){
    name = var_name;
    type = var_class;
    is_final = var_final;
    is_static = var_static;
    is_private = false;
}
Variable::Variable(std::string var_name, Class var_class, bool var_final, bool var_static, bool var_private){
    name = var_name;
    type = var_class;
    is_final = var_final;
    is_static = var_static;
    is_private = var_private;
}

Method::Method(){
    name = "";
    parent = NULL;
    is_static = false;
}
Method::Method(std::string method_name, Scope* method_parent, const std::vector<MethodArgument>& method_args, Class method_return_type){
    name = method_name;
    parent = method_parent;
    arguments = method_args;
    return_type = method_return_type;
    is_static = false;
}
Method::Method(std::string method_name, Scope* method_parent, const std::vector<MethodArgument>& method_args, Class method_return_type, bool method_is_static){
    name = method_name;
    parent = method_parent;
    arguments = method_args;
    return_type = method_return_type;
    is_static = method_is_static;
}

// Scopes
void print_scopes(Scope* scope, unsigned int indent){

    for(unsigned int i = 0; i < indent; i++){
        std::cout << "    ";
    }
    std::cout << scope->name << " " << std::endl;

    for(unsigned int i = 0; i < scope->children.size(); i++){
        print_scopes(scope->children[i], indent+1);
    }
}
void print_scopes_variables(Scope* scope, unsigned int indent){

    for(unsigned int i = 0; i < indent; i++){
        std::cout << "    ";
    }
    std::cout << scope->name << std::endl;

    for(unsigned int i = 0; i < scope->variables.size(); i++){
        for(unsigned int i = 0; i < indent + 1; i++){
            std::cout << "    ";
        }
        std::cout << scope->variables[i].type.name << " " << scope->variables[i].name << std::endl;
    }

    for(unsigned int i = 0; i < scope->children.size(); i++){
        print_scopes_variables(scope->children[i], indent+1);
    }
}
void clean_scopes(Scope* scope){
    for(unsigned int i = 0; i < scope->children.size(); i++){
        clean_scopes(scope->children[i]);

        delete scope->children[i];
    }
}
Scope* environment_get_child(Scope* scope, std::string name){
    for(unsigned int i = 0; i < scope->children.size(); i++){
        if(scope->children[i]->name == name){
            return scope->children[i];
        }
    }

    #ifdef DEV_ERRORS
    fail(DEV_BLANK_TYPE);
    #endif // DEV_ERRORS

    return NULL;
}

// Arguments
bool arguments_equal(TokenContext context, std::vector<MethodArgument> a, std::vector<MethodArgument> b){
    if(a.size() == 1){
        if(a[0].type.name == IGNORE and a[0].optional == true){
            return true;
        }
    }

    if(b.size() == 1){
        if(b[0].type.name == IGNORE and b[0].optional == true){
            return true;
        }
    }

    if(a.size() != b.size()){
        return false;
    }

    for(unsigned int i = 0; i < a.size(); i++){
        if( !context_class_compare(context, a[i].type, b[i].type) ){
            return false;
        }
    }

    return true;
}
bool generic_arguments_equal(TokenContext context, std::vector<MethodArgument> a, std::vector<MethodArgument> b, Class actual_class, Class generic_class){
    if(a.size() == 1){
        if(a[0].type.name == IGNORE and a[0].optional == true){
            return true;
        }
    }

    if(b.size() == 1){
        if(b[0].type.name == IGNORE and b[0].optional == true){
            return true;
        }
    }

    if(a.size() != b.size()){
        return false;
    }

    if(actual_class.generics.size() != generic_class.generics.size()){
        die(GENERIC_MISMATCH( context_root_class(generic_class).name ));
    }

    for(unsigned int i = 0; i < a.size(); i++){
        for(unsigned int g = 0; g < actual_class.generics.size(); g++){

            for(unsigned int c = 0; c <  b[i].type.generics.size(); c++){
                if(a[i].type == actual_class.generics[g] and generic_class.generics[g] == b[i].type.generics[i]){
                    b[i].type.generics[i] = actual_class.generics[g];
                }
            }

            if(a[i].type == actual_class.generics[g] and generic_class.generics[g] == b[i].type){
                b[i].type = actual_class.generics[g];
                break;
            }
            b[i].type = context_root_class(b[i].type);
        }
    }

    for(unsigned int i = 0; i < a.size(); i++){
        if( !context_class_compare(context, a[i].type, b[i].type) ){
            return false;
        }
    }

    return true;
}

// Methods
bool environment_method_exists(TokenContext context, Scope* scope, Method method){
    for(size_t i = 0; i < scope->methods.size(); i++){
        if( (scope->methods[i].name == method.name or method.name==IGNORE)
        and (scope->methods[i].parent == method.parent or method.parent==NULL)
        and (context_class_compare(context, Class(scope->methods[i].return_type), Class(method.return_type)) or method.return_type == IGNORE_CLASS)
        and (arguments_equal(context, scope->methods[i].arguments, method.arguments))){
            return true;
        }
    }

    return false;
}
unsigned int environment_method_index(TokenContext context, Scope* scope, Method method){
    for(size_t i = 0; i < scope->methods.size(); i++){
        if( (scope->methods[i].name == method.name or method.name==IGNORE)
        and (scope->methods[i].parent == method.parent or method.parent==NULL)
        and (context_class_compare(context, Class(scope->methods[i].return_type), Class(method.return_type)) or method.return_type == IGNORE_CLASS)
        and (arguments_equal(context, scope->methods[i].arguments, method.arguments))){
            return i;
        }
    }

    #ifdef DEV_ERRORS
    fail(DEV_BLANK_TYPE);
    #endif // DEV_ERRORS

    return 0;
}
Method environment_method_get(TokenContext context, Scope* scope, Method method){
    for(size_t i = 0; i < scope->methods.size(); i++){
        if( (scope->methods[i].name == method.name or method.name==IGNORE)
        and (scope->methods[i].parent == method.parent or method.parent==NULL)
        and (context_class_compare(context, Class(scope->methods[i].return_type), Class(method.return_type)) or method.return_type == IGNORE_CLASS)
        and (arguments_equal(context, scope->methods[i].arguments, method.arguments))){
            return scope->methods[i];
        }
    }

    #ifdef DEV_ERRORS
    fail(DEV_BLANK_TYPE);
    #endif // DEV_ERRORS

    return Method{"", NULL, IGNORE_ARGS, IGNORE_CLASS};
}
bool environment_generic_method_exists(TokenContext context, Scope* scope, Method method, Class actual_class, Class generic_class){
    for(size_t i = 0; i < scope->methods.size(); i++){
        if( (scope->methods[i].name == method.name or method.name==IGNORE)
        and (scope->methods[i].parent == method.parent or method.parent==NULL)
        and (context_class_compare(context, Class(scope->methods[i].return_type), Class(method.return_type)) or method.return_type == IGNORE_CLASS)
        and (generic_arguments_equal(context, scope->methods[i].arguments, method.arguments, actual_class, generic_class))){
            return true;
        }
    }

    return false;
}
Method environment_generic_method_get(TokenContext context, Scope* scope, Method method, Class actual_class, Class generic_class){
    for(size_t i = 0; i < scope->methods.size(); i++){
        if( (scope->methods[i].name == method.name or method.name==IGNORE)
        and (scope->methods[i].parent == method.parent or method.parent==NULL)
        and (context_class_compare(context, Class(scope->methods[i].return_type), Class(method.return_type)) or method.return_type == IGNORE_CLASS)
        and (generic_arguments_equal(context, scope->methods[i].arguments, method.arguments, actual_class, generic_class))){
            Method return_method = scope->methods[i];

            for(size_t g = 0; g < actual_class.generics.size(); g++){
                if(return_method.return_type == actual_class.generics[g]){
                    return_method.return_type = generic_class.generics[g];
                }
            }

            return return_method;
        }
    }

    #ifdef DEV_ERRORS
    fail(DEV_BLANK_TYPE);
    #endif // DEV_ERRORS

    return Method{"", NULL, IGNORE_ARGS, IGNORE_CLASS};
}
std::string environment_similar_methods(TokenContext context, Scope* scope, Method method){
    std::string methods;

    for(size_t i = 0; i < scope->methods.size(); i++){
        if( (scope->methods[i].name == method.name or method.name==IGNORE) and (scope->methods[i].parent == method.parent or method.parent==NULL) ){
            std::string method_arg_string;

            for(size_t j = 0; j < scope->methods[i].arguments.size(); j++){
                method_arg_string += scope->methods[i].arguments[j].type.toString();
                if(j + 1 < scope->methods[i].arguments.size()) method_arg_string += ", ";
            }

            methods += "\n      " + scope->methods[i].name + "(" + method_arg_string + ") -> " + scope->methods[i].return_type.name;
        }
    }

    return methods;
}

// Classes
bool environment_class_exists(Scope* scope, Class type){
    for(unsigned int i = 0; i < type.name.length(); i++){
        if(type.name[i] == '^'){
            type.name.erase(type.name.begin() + i);
        }
    }

    for(unsigned int i = 0; i < scope->classes.size(); i++){
        if( (scope->classes[i].name == type.name or type.name==IGNORE)){
            return true;
        }
    }

    return false;
}
unsigned int environment_class_index(Scope* scope, Class type){
    for(unsigned int i = 0; i < scope->classes.size(); i++){
        if( (scope->classes[i].name == type.name               or type.name==IGNORE)){
            return i;
        }
    }

    #ifdef DEV_ERRORS
    fail(DEV_BLANK_TYPE);
    #endif // DEV_ERRORS

    return 0;
}
Class environment_class_get(Scope* scope, Class type){
    for(unsigned int i = 0; i < scope->classes.size(); i++){
        if( (scope->classes[i].name == type.name or type.name==IGNORE)){
            return scope->classes[i];
        }
    }

    #ifdef DEV_ERRORS
    fail(DEV_BLANK_TYPE);
    #endif // DEV_ERRORS

    return Class{""};
}
Class environment_class_get_first(Scope* scope, Class type, Class type2){
    for(unsigned int i = 0; i < scope->classes.size(); i++){
        if( (scope->classes[i].name == type.name or type.name==IGNORE)){
            return scope->classes[i];
        }

        if( (scope->classes[i].name == type2.name or type2.name==IGNORE)){
            return scope->classes[i];
        }
    }

    #ifdef DEV_ERRORS
    fail(DEV_BLANK_TYPE);
    #endif // DEV_ERRORS

    return Class{""};
}

// Class Variables
bool environment_class_variable_exists(Environment& environment, Class base, Variable variable){
    unsigned int variables_size = environment_get_child(&environment.global, CLASS_PREFIX + base.name)->variables.size();

    for(unsigned int i = 0; i < variables_size; i++){
        if( (environment_get_child(&environment.global, CLASS_PREFIX + base.name)->variables[i].name == variable.name or variable.name == IGNORE)
        and (environment_get_child(&environment.global, CLASS_PREFIX + base.name)->variables[i].type.name == variable.type.name or variable.type.name == IGNORE) ){
            // Variable Found
            return true;
        }
    }

    return false;
}
Variable environment_class_variable_get(Environment& environment, Class base, Variable variable){
    unsigned int variables_size = environment_get_child(&environment.global, CLASS_PREFIX + base.name)->variables.size();

    for(unsigned int i = 0; i < variables_size; i++){
        if( (environment_get_child(&environment.global, CLASS_PREFIX + base.name)->variables[i].name == variable.name or variable.name == IGNORE)
        and (environment_get_child(&environment.global, CLASS_PREFIX + base.name)->variables[i].type.name == variable.type.name or variable.type.name == IGNORE) ){
            // Variable Found
            return environment_get_child(&environment.global, CLASS_PREFIX + base.name)->variables[i];
        }
    }

    #ifdef DEV_ERRORS
    fail(DEV_BLANK_TYPE);
    #endif // DEV_ERRORS

    return Variable(IGNORE, IGNORE_CLASS, false, false);
}

// Variables
void environment_print_variables(Scope* scope, unsigned int indent){
    for(unsigned int v = 0; v < scope->variables.size(); v++){
        for(unsigned int i = 0; i < indent; i++){
            std::cout << "    ";
        }

        std::cout << scope->variables[v].type.name + " " + scope->variables[v].name << std::endl;
    }

    for(unsigned int i = 0; i < scope->children.size(); i++){
        environment_print_variables(scope->children[i], indent+1);
    }
}

bool environment_variable_exists(Scope* scope, Variable variable){
    for(unsigned int v = 0; v < scope->variables.size(); v++){
        if( (scope->variables[v].name == variable.name or variable.name==IGNORE)
        and (scope->variables[v].type.name == variable.type.name or variable.type.name==IGNORE)){
            return true;
        }
    }

    if(scope->parent == NULL) return false;

    if(environment_variable_exists(scope->parent, variable)){
        return true;
    }

    return false;
}
Variable environment_variable_get(Scope* scope, Variable variable){
    for(unsigned int v = 0; v < scope->variables.size(); v++){
        if( (scope->variables[v].name == variable.name or variable.name==IGNORE)
        and (scope->variables[v].type.name == variable.type.name or variable.type.name==IGNORE)){
            return scope->variables[v];
        }
    }

    if(scope->parent == NULL){
        #ifdef DEV_ERRORS
        fail(DEV_BLANK_TYPE);
        #endif // DEV_ERRORS

        return Variable("", Class(), false, false);
    }

    if(environment_variable_exists(scope->parent, variable)){
        return environment_variable_get(scope->parent, variable);
    }

    #ifdef DEV_ERRORS
    fail(DEV_BLANK_TYPE);
    #endif // DEV_ERRORS

    return Variable("", Class(), false, false);
}
