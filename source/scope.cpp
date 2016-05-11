
#include <iostream>
#include "../include/scope.h"

using namespace std;

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
