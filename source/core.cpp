
#include <string>
#include <vector>
#include "../include/die.h"
#include "../include/core.h"
#include "../include/errors.h"
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

void add_function(Environment& e, Method method){
    e.global.methods.push_back(method);
    e.global.children.push_back( new Scope{METHOD_PREFIX + method.name, &e.global} );

    std::string func_type = "function^(";
    for(unsigned int i = 0; i < method.arguments.size(); i++){
        func_type += method.arguments[i].type.name;

        if(i != method.arguments.size()-1){
            func_type += ",";
        }
    }
    func_type += ")->" + method.return_type;

    e.global.variables.push_back( Variable{method.name, func_type} );
}

void add_field(Environment& environment, Class base, Variable variable){

}

void load_core(Environment& environment){
    Scope* type_scope;

    // Base Die Function
    add_function(environment, Method{"die", &environment.global, std::vector<MethodArgument>(), "void"});

    // Base String Class
    type_scope = add_class(environment, Class{"String"});
    add_method(environment, Class{"String"}, Method{"output", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"String"}, Method{"print", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"String"}, Method{"input", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"String"}, Method{"toNumber", type_scope, std::vector<MethodArgument>(), "Number"});
    add_method(environment, Class{"String"}, Method{"toUnsignedInteger", type_scope, std::vector<MethodArgument>(), "Number"});
    add_method(environment, Class{"String"}, Method{"toInteger", type_scope, std::vector<MethodArgument>(), "Number"});

    // Base Number Class
    type_scope = add_class(environment, Class{"Number"});
    add_method(environment, Class{"Number"}, Method{"output", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"Number"}, Method{"print", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"Number"}, Method{"toString", type_scope, std::vector<MethodArgument>(), "String"});
    add_method(environment, Class{"Number"}, Method{"toUnsignedInteger", type_scope, std::vector<MethodArgument>(), "UnsignedInteger"});
    add_method(environment, Class{"Number"}, Method{"toInteger", type_scope, std::vector<MethodArgument>(), "Integer"});

    // Base Unsigned Integer Class
    type_scope = add_class(environment, Class{"UnsignedInteger"});
    add_method(environment, Class{"UnsignedInteger"}, Method{"output", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"UnsignedInteger"}, Method{"print", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"UnsignedInteger"}, Method{"toString", type_scope, std::vector<MethodArgument>(), "String"});
    add_method(environment, Class{"UnsignedInteger"}, Method{"toNumber", type_scope, std::vector<MethodArgument>(), "Number"});
    add_method(environment, Class{"UnsignedInteger"}, Method{"toInteger", type_scope, std::vector<MethodArgument>(), "Integer"});

    // Base Integer Class
    type_scope = add_class(environment, Class{"Integer"});
    add_method(environment, Class{"Integer"}, Method{"output", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"Integer"}, Method{"print", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"Integer"}, Method{"toString", type_scope, std::vector<MethodArgument>(), "String"});
    add_method(environment, Class{"Integer"}, Method{"toNumber", type_scope, std::vector<MethodArgument>(), "Number"});
    add_method(environment, Class{"Integer"}, Method{"toUnsignedInteger", type_scope, std::vector<MethodArgument>(), "UnsignedInteger"});

    // Base Boolean Class
    type_scope = add_class(environment, Class{"Boolean"});
    add_method(environment, Class{"Boolean"}, Method{"output", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"Boolean"}, Method{"print", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"Boolean"}, Method{"input", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"Boolean"}, Method{"toString", type_scope, std::vector<MethodArgument>(), "String"});
}
