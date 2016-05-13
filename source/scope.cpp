
#include <iostream>
#include "../include/die.h"
#include "../include/scope.h"

using namespace std;

bool environment_method_exists(Environment* environment, Method method){
    for(unsigned int i = 0; i < environment->methods.size(); i++){
        if( environment->methods[i].name == method.name
        and environment->methods[i].parent == method.parent
        and environment->methods[i].return_type == method.return_type){
            return true;
        }
    }

    return false;
}

unsigned int environment_method_get(Environment* environment, Method method){
    for(unsigned int i = 0; i < environment->methods.size(); i++){
        if( environment->methods[i].name == method.name
        and environment->methods[i].parent == method.parent){
            return i;
        }
    }

    die("Method doesn't exist");
}

void print_scopes(Scope* scope, unsigned int indent){

    for(int i = 0; i < indent; i++){
        cout << "    ";
    }
    cout << scope->name << endl;

    for(int i = 0; i < scope->children.size(); i++){
        print_scopes(scope->children[i], indent+1);
    }
}
void clean_scopes(Scope* scope){

    for(int i = 0; i < scope->children.size(); i++){
        clean_scopes(scope->children[i]);

        delete scope->children[i];
    }
}
