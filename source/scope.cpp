
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

// Methods
bool environment_method_exists(Environment* environment, Method method){
    for(unsigned int i = 0; i < environment->methods.size(); i++){
        if( (environment->methods[i].name == method.name               or method.name==IGNORE)
        and (environment->methods[i].parent == method.parent           or method.parent==NULL)
        and (environment->methods[i].return_type == method.return_type or method.return_type==IGNORE)
        and (environment->methods[i].arguments == method.arguments     or method.arguments==IGNORE)){
            return true;
        }
    }

    return false;
}
unsigned int environment_method_index(Environment* environment, Method method){
    for(unsigned int i = 0; i < environment->methods.size(); i++){
        if( (environment->methods[i].name == method.name               or method.name==IGNORE)
        and (environment->methods[i].parent == method.parent           or method.parent==NULL)
        and (environment->methods[i].return_type == method.return_type or method.return_type==IGNORE)
        and (environment->methods[i].arguments == method.arguments     or method.arguments==IGNORE) ){
            return i;
        }
    }

    #ifdef DEV_ERRORS
    fail(DEV_BLANK_TYPE);
    #endif // DEV_ERRORS

    return 0;
}
Method environment_method_get(Environment* environment, Method method){
    for(unsigned int i = 0; i < environment->methods.size(); i++){
        if( (environment->methods[i].name == method.name               or method.name==IGNORE)
        and (environment->methods[i].parent == method.parent           or method.parent==NULL)
        and (environment->methods[i].return_type == method.return_type or method.return_type==IGNORE)
        and (environment->methods[i].arguments == method.arguments     or method.arguments==IGNORE) ){
            return environment->methods[i];
        }
    }

    #ifdef DEV_ERRORS
    fail(DEV_BLANK_TYPE);
    #endif // DEV_ERRORS

    return Method{"", NULL, "", ""};
}

// Templates
bool environment_template_exists(Environment* environment, Template type){
    for(unsigned int i = 0; i < environment->templates.size(); i++){
        if( (environment->templates[i].name == type.name               or type.name==IGNORE)){
            return true;
        }
    }

    return false;
}
unsigned int environment_template_index(Environment* environment, Template type){
    for(unsigned int i = 0; i < environment->templates.size(); i++){
        if( (environment->templates[i].name == type.name               or type.name==IGNORE)){
            return i;
        }
    }

    #ifdef DEV_ERRORS
    fail(DEV_BLANK_TYPE);
    #endif // DEV_ERRORS

    return 0;
}
Template environment_template_get(Environment* environment, Template type){
    for(unsigned int i = 0; i < environment->templates.size(); i++){
        if( (environment->templates[i].name == type.name               or type.name==IGNORE)){
            return environment->templates[i];
        }
    }

    #ifdef DEV_ERRORS
    fail(DEV_BLANK_TYPE);
    #endif // DEV_ERRORS

    return Template{""};
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
    for(unsigned int i = 0; i < scope->children.size(); i++){
        if(environment_variable_exists(scope->children[i], variable)){
            return true;
        }
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
    for(unsigned int i = 0; i < scope->children.size(); i++){
        if(environment_variable_exists(scope->children[i], variable)){
            return environment_variable_get(scope->children[i], variable);
        }
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
