
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

#include <string>
#include <vector>
#include "../include/die.h"
#include "../include/core.h"
#include "../include/errors.h"
#include "../include/context.h"

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
    add_method(environment, Class{"String"}, Method{"toByte", type_scope, std::vector<MethodArgument>(), "Byte"});
    add_method(environment, Class{"String"}, Method{"new", type_scope, { MethodArgument{Class{"String"}, false} }, "void"});

    // Base Number Class
    type_scope = add_class(environment, Class{"Number"});
    add_method(environment, Class{"Number"}, Method{"output", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"Number"}, Method{"input", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"Number"}, Method{"print", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"Number"}, Method{"toString", type_scope, std::vector<MethodArgument>(), "String"});
    add_method(environment, Class{"Number"}, Method{"toUnsignedInteger", type_scope, std::vector<MethodArgument>(), "UnsignedInteger"});
    add_method(environment, Class{"Number"}, Method{"toInteger", type_scope, std::vector<MethodArgument>(), "Integer"});
    add_method(environment, Class{"Number"}, Method{"toByte", type_scope, std::vector<MethodArgument>(), "Byte"});
    add_method(environment, Class{"Number"}, Method{"new", type_scope, { MethodArgument{Class{"Number"}, false} }, "void"});

    // Base Unsigned Integer Class
    type_scope = add_class(environment, Class{"UnsignedInteger"});
    add_method(environment, Class{"UnsignedInteger"}, Method{"output", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"UnsignedInteger"}, Method{"input", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"UnsignedInteger"}, Method{"print", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"UnsignedInteger"}, Method{"toString", type_scope, std::vector<MethodArgument>(), "String"});
    add_method(environment, Class{"UnsignedInteger"}, Method{"toNumber", type_scope, std::vector<MethodArgument>(), "Number"});
    add_method(environment, Class{"UnsignedInteger"}, Method{"toInteger", type_scope, std::vector<MethodArgument>(), "Integer"});
    add_method(environment, Class{"UnsignedInteger"}, Method{"toByte", type_scope, std::vector<MethodArgument>(), "Byte"});
    add_method(environment, Class{"UnsignedInteger"}, Method{"new", type_scope, { MethodArgument{Class{"UnsignedInteger"}, false} }, "void"});

    // Base Integer Class
    type_scope = add_class(environment, Class{"Integer"});
    add_method(environment, Class{"Integer"}, Method{"output", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"Integer"}, Method{"input", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"Integer"}, Method{"print", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"Integer"}, Method{"toString", type_scope, std::vector<MethodArgument>(), "String"});
    add_method(environment, Class{"Integer"}, Method{"toNumber", type_scope, std::vector<MethodArgument>(), "Number"});
    add_method(environment, Class{"Integer"}, Method{"toUnsignedInteger", type_scope, std::vector<MethodArgument>(), "UnsignedInteger"});
    add_method(environment, Class{"Integer"}, Method{"toByte", type_scope, std::vector<MethodArgument>(), "Byte"});
    add_method(environment, Class{"Integer"}, Method{"new", type_scope, { MethodArgument{Class{"Integer"}, false} }, "void"});

    // Base Byte Class
    type_scope = add_class(environment, Class{"Byte"});
    add_method(environment, Class{"Byte"}, Method{"output", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"Byte"}, Method{"input", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"Byte"}, Method{"print", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"Byte"}, Method{"toString", type_scope, std::vector<MethodArgument>(), "String"});
    add_method(environment, Class{"Byte"}, Method{"toNumber", type_scope, std::vector<MethodArgument>(), "Number"});
    add_method(environment, Class{"Byte"}, Method{"toInteger", type_scope, std::vector<MethodArgument>(), "Integer"});
    add_method(environment, Class{"Byte"}, Method{"toUnsignedInteger", type_scope, std::vector<MethodArgument>(), "UnsignedInteger"});
    add_method(environment, Class{"Byte"}, Method{"new", type_scope, { MethodArgument{Class{"Byte"}, false} }, "void"});

    // Base Boolean Class
    type_scope = add_class(environment, Class{"Boolean"});
    add_method(environment, Class{"Boolean"}, Method{"output", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"Boolean"}, Method{"print", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"Boolean"}, Method{"input", type_scope, std::vector<MethodArgument>(), "void"});
    add_method(environment, Class{"Boolean"}, Method{"toString", type_scope, std::vector<MethodArgument>(), "String"});
    add_method(environment, Class{"Boolean"}, Method{"new", type_scope, { MethodArgument{Class{"Boolean"}, false} }, "void"});

    // Base List Class
    type_scope = add_class(environment, Class{"List", {"Type"}});
    add_method(environment, Class{"List"}, Method{"append", type_scope, { MethodArgument{Class{"Type"}, false} }, "void"});
    add_method(environment, Class{"List"}, Method{"prepend", type_scope, { MethodArgument{Class{"Type"}, false} }, "void"});
    add_method(environment, Class{"List"}, Method{"insert", type_scope, { MethodArgument{Class{"Number"}, false}, MethodArgument{Class{"Type"}, false} }, "void"});
    add_method(environment, Class{"List"}, Method{"remove", type_scope, { MethodArgument{Class{"Number"}, false} }, "void"});
    add_method(environment, Class{"List"}, Method{"remove", type_scope, { MethodArgument{Class{"Integer"}, false} }, "void"});
    add_method(environment, Class{"List"}, Method{"get", type_scope, { MethodArgument{Class{"Number"}, false} }, "Type"});
    add_method(environment, Class{"List"}, Method{"get", type_scope, { MethodArgument{Class{"Integer"}, false} }, "Type"});
    add_method(environment, Class{"List"}, Method{"length", type_scope, std::vector<MethodArgument>(), "Integer"});
}
