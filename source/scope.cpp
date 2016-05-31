
#include <iostream>
#include "../include/die.h"
#include "../include/scope.h"

using namespace std;

// Scopes
void print_scopes(Scope* scope, unsigned int indent){

    for(unsigned int i = 0; i < indent; i++){
        cout << "    ";
    }
    cout << scope->name << " " << endl;

    for(unsigned int i = 0; i < scope->children.size(); i++){
        print_scopes(scope->children[i], indent+1);
    }
}
void print_scopes_variables(Scope* scope, unsigned int indent){

    for(unsigned int i = 0; i < indent; i++){
        cout << "    ";
    }
    cout << scope->name << endl;

    for(unsigned int i = 0; i < scope->variables.size(); i++){
        for(unsigned int i = 0; i < indent + 1; i++){
            cout << "    ";
        }
        cout << scope->variables[i].type << " " << scope->variables[i].name << endl;
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
bool arguments_equal(std::vector<MethodArgument> a, std::vector<MethodArgument> b){
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
        if(a[i].type.name != b[i].type.name){
            return false;
        }
    }

    return true;
}

// Methods
bool environment_method_exists(Scope* scope, Method method){
    for(unsigned int i = 0; i < scope->methods.size(); i++){
        if( (scope->methods[i].name == method.name               or method.name==IGNORE)
        and (scope->methods[i].parent == method.parent           or method.parent==NULL)
        and (scope->methods[i].return_type == method.return_type or method.return_type==IGNORE)
        and (arguments_equal(scope->methods[i].arguments, method.arguments))){
            return true;
        }
    }

    return false;
}
unsigned int environment_method_index(Scope* scope, Method method){
    for(unsigned int i = 0; i < scope->methods.size(); i++){
        if( (scope->methods[i].name == method.name               or method.name==IGNORE)
        and (scope->methods[i].parent == method.parent           or method.parent==NULL)
        and (scope->methods[i].return_type == method.return_type or method.return_type==IGNORE)
        and (arguments_equal(scope->methods[i].arguments, method.arguments))){
            return i;
        }
    }

    #ifdef DEV_ERRORS
    fail(DEV_BLANK_TYPE);
    #endif // DEV_ERRORS

    return 0;
}
Method environment_method_get(Scope* scope, Method method){
    for(unsigned int i = 0; i < scope->methods.size(); i++){
        if( (scope->methods[i].name == method.name               or method.name==IGNORE)
        and (scope->methods[i].parent == method.parent           or method.parent==NULL)
        and (scope->methods[i].return_type == method.return_type or method.return_type==IGNORE)
        and (arguments_equal(scope->methods[i].arguments, method.arguments))){
            return scope->methods[i];
        }
    }

    #ifdef DEV_ERRORS
    fail(DEV_BLANK_TYPE);
    #endif // DEV_ERRORS

    return Method{"", NULL, IGNORE_ARGS, ""};
}

// Templates
bool environment_template_exists(Scope* scope, Template type){
    for(unsigned int i = 0; i < scope->templates.size(); i++){
        if( (scope->templates[i].name == type.name or type.name==IGNORE)){
            return true;
        }
    }

    return false;
}
unsigned int environment_template_index(Scope* scope, Template type){
    for(unsigned int i = 0; i < scope->templates.size(); i++){
        if( (scope->templates[i].name == type.name               or type.name==IGNORE)){
            return i;
        }
    }

    #ifdef DEV_ERRORS
    fail(DEV_BLANK_TYPE);
    #endif // DEV_ERRORS

    return 0;
}
Template environment_template_get(Scope* scope, Template type){
    for(unsigned int i = 0; i < scope->templates.size(); i++){
        if( (scope->templates[i].name == type.name or type.name==IGNORE)){
            return scope->templates[i];
        }
    }

    #ifdef DEV_ERRORS
    fail(DEV_BLANK_TYPE);
    #endif // DEV_ERRORS

    return Template{""};
}
Template environment_template_get_first(Scope* scope, Template type, Template type2){
    for(unsigned int i = 0; i < scope->templates.size(); i++){
        if( (scope->templates[i].name == type.name or type.name==IGNORE)){
            return scope->templates[i];
        }

        if( (scope->templates[i].name == type2.name or type2.name==IGNORE)){
            return scope->templates[i];
        }
    }

    #ifdef DEV_ERRORS
    fail(DEV_BLANK_TYPE);
    #endif // DEV_ERRORS

    return Template{""};
}

// Template Variables
bool environment_template_variable_exists(Environment& environment, Template base, Variable variable){
    unsigned int variables_size = environment_get_child(&environment.global, string(TEMPLATE_PREFIX) + base.name)->variables.size();

    for(unsigned int i = 0; i < variables_size; i++){
        if( (environment_get_child(&environment.global, string(TEMPLATE_PREFIX) + base.name)->variables[i].name == variable.name or variable.name == IGNORE)
        and (environment_get_child(&environment.global, string(TEMPLATE_PREFIX) + base.name)->variables[i].type == variable.type or variable.type == IGNORE) ){
            // Variable Found
            return true;
        }
    }

    return false;
}
Variable environment_template_variable_get(Environment& environment, Template base, Variable variable){
    unsigned int variables_size = environment_get_child(&environment.global, string(TEMPLATE_PREFIX) + base.name)->variables.size();

    for(unsigned int i = 0; i < variables_size; i++){
        if( (environment_get_child(&environment.global, string(TEMPLATE_PREFIX) + base.name)->variables[i].name == variable.name or variable.name == IGNORE)
        and (environment_get_child(&environment.global, string(TEMPLATE_PREFIX) + base.name)->variables[i].type == variable.type or variable.type == IGNORE) ){
            // Variable Found
            return environment_get_child(&environment.global, string(TEMPLATE_PREFIX) + base.name)->variables[i];
        }
    }

    #ifdef DEV_ERRORS
    fail(DEV_BLANK_TYPE);
    #endif // DEV_ERRORS

    return Variable{IGNORE, IGNORE};
}

// Variables
void environment_print_variables(Scope* scope, unsigned int indent){
    for(unsigned int v = 0; v < scope->variables.size(); v++){
        for(unsigned int i = 0; i < indent; i++){
            cout << "    ";
        }

        cout << scope->variables[v].type + " " + scope->variables[v].name << endl;
    }

    for(unsigned int i = 0; i < scope->children.size(); i++){
        environment_print_variables(scope->children[i], indent+1);
    }
}
bool environment_variable_exists(Scope* scope, Variable variable){
    if(scope->parent == NULL) return false;

    if(environment_variable_exists(scope->parent, variable)){
        return true;
    }

    for(unsigned int v = 0; v < scope->variables.size(); v++){
        if( (scope->variables[v].name == variable.name or variable.name==IGNORE)
        and (scope->variables[v].type == variable.type or variable.type==IGNORE)){
            return true;
        }
    }

    return false;
}

Variable environment_variable_get(Scope* scope, Variable variable){
    if(scope->parent == NULL){
        #ifdef DEV_ERRORS
        fail(DEV_BLANK_TYPE);
        #endif // DEV_ERRORS

        return Variable{"",""};
    }

    if(environment_variable_exists(scope->parent, variable)){
        return environment_variable_get(scope->parent, variable);
    }

    for(unsigned int v = 0; v < scope->variables.size(); v++){
        if( (scope->variables[v].name == variable.name or variable.name==IGNORE)
        and (scope->variables[v].type == variable.type or variable.type==IGNORE)){
            return scope->variables[v];
        }
    }

    #ifdef DEV_ERRORS
    fail(DEV_BLANK_TYPE);
    #endif // DEV_ERRORS

    return Variable{"",""};
}
