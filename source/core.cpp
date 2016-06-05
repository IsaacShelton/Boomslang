
#include <string>
#include <vector>
#include "../include/die.h"
#include "../include/core.h"
#include "../include/context.h"

using namespace std;

Scope* add_class(Environment& e, Class type){
    Scope* created = new Scope{CLASS_PREFIX + type.name, &e.global};
    e.global.classes.push_back(type);
    e.global.children.push_back(created);
    return created;
}

void add_method(Environment& e, Class type, Method method){
    Scope* child = environment_get_child(&e.global, CLASS_PREFIX + type.name);

    if(child == NULL){
        die("Failed to add method");
    }

    child->methods.push_back(method);
    child->children.push_back( new Scope{METHOD_PREFIX + method.name, child} );
}

void load_core(Environment& environment){
    Scope* type_scope;

    // Base String Class
    type_scope = add_class(environment, Class{"String", true});
    add_method(environment, Class{"String"}, Method{"output",type_scope, std::vector<MethodArgument>(),"void"});
    add_method(environment, Class{"String"}, Method{"print",type_scope, std::vector<MethodArgument>(),"void"});
    add_method(environment, Class{"String"}, Method{"input",type_scope, std::vector<MethodArgument>(),"void"});
    add_method(environment, Class{"String"}, Method{"toNumber",type_scope, std::vector<MethodArgument>(),"Number"});

    // Base Number Class
    type_scope = add_class(environment, Class{"Number", true});
    add_method(environment, Class{"Number"}, Method{"output",type_scope, std::vector<MethodArgument>(),"void"});
    add_method(environment, Class{"Number"}, Method{"print",type_scope, std::vector<MethodArgument>(),"void"});
    add_method(environment, Class{"Number"}, Method{"toString",type_scope, std::vector<MethodArgument>(),"String"});
}