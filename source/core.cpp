
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
    func_type += ")->" + method.return_type.toString();

    e.global.variables.push_back( Variable{method.name, func_type} );
}

void add_field(Environment& e, Class base, Variable variable){
    Scope* child = environment_get_child(&e.global, CLASS_PREFIX + base.name);
    child->variables.push_back(variable);
}

void load_core(Environment& environment){
    Scope* type_scope;

    // Primatives
    add_class(environment, Class{"uint"});
    add_class(environment, Class{"int"});
    add_class(environment, Class{"float"});
    add_class(environment, Class{"double"});
    add_class(environment, Class{"short"});
    add_class(environment, Class{"ushort"});
    add_class(environment, Class{"long"});
    add_class(environment, Class{"ulong"});
    add_class(environment, Class{"bool"});
    add_class(environment, Class{"char"});
    add_class(environment, Class{"uchar"});

    // Base Die Function
    add_function(environment, Method{"die", &environment.global, std::vector<MethodArgument>(), Class("void")});

    // Base Constants
    environment.global.variables.push_back(Variable{"true", Class{"Boolean"}, true, false});
    environment.global.variables.push_back(Variable{"false", Class{"Boolean"}, true, false});

    // Base String Class
    type_scope = add_class(environment, Class{"String"});
    add_method(environment, Class{"String"}, Method{"output", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"String"}, Method{"print", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"String"}, Method{"input", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"String"}, Method{"wait", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"String"}, Method{"fail", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"String"}, Method{"printError", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"String"}, Method{"log", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"String"}, Method{"printLog", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"String"}, Method{"toDouble", type_scope, std::vector<MethodArgument>(), Class("Double")});
    add_method(environment, Class{"String"}, Method{"toFloat", type_scope, std::vector<MethodArgument>(), Class("Float")});
    add_method(environment, Class{"String"}, Method{"toUnsignedInteger", type_scope, std::vector<MethodArgument>(), Class("UnsignedInteger")});
    add_method(environment, Class{"String"}, Method{"toInteger", type_scope, std::vector<MethodArgument>(), Class("Integer")});
    add_method(environment, Class{"String"}, Method{"toByte", type_scope, std::vector<MethodArgument>(), Class("Byte")});
    add_method(environment, Class{"String"}, Method{"toBoolean", type_scope, std::vector<MethodArgument>(), Class("Boolean")});
    add_method(environment, Class{"String"}, Method{"new", type_scope, { MethodArgument{Class{"String"}, false} }, Class("void")});

    // Base Double Class
    type_scope = add_class(environment, Class{"Double"});
    add_method(environment, Class{"Double"}, Method{"output", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"Double"}, Method{"input", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"Double"}, Method{"print", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"Double"}, Method{"toString", type_scope, std::vector<MethodArgument>(), Class("String")});
    add_method(environment, Class{"Double"}, Method{"toUnsignedInteger", type_scope, std::vector<MethodArgument>(), Class("UnsignedInteger")});
    add_method(environment, Class{"Double"}, Method{"toInteger", type_scope, std::vector<MethodArgument>(), Class("Integer")});
    add_method(environment, Class{"Double"}, Method{"toByte", type_scope, std::vector<MethodArgument>(), Class("Byte")});
    add_method(environment, Class{"Double"}, Method{"toBoolean", type_scope, std::vector<MethodArgument>(), Class("Boolean")});
    add_method(environment, Class{"Double"}, Method{"toFloat", type_scope, std::vector<MethodArgument>(), Class("Float")});
    add_method(environment, Class{"Double"}, Method{"new", type_scope, { MethodArgument{Class{"Double"}, false} }, Class("void")});

    // Base Double Class
    type_scope = add_class(environment, Class{"Float"});
    add_method(environment, Class{"Float"}, Method{"output", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"Float"}, Method{"input", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"Float"}, Method{"print", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"Float"}, Method{"toString", type_scope, std::vector<MethodArgument>(), Class("String")});
    add_method(environment, Class{"Float"}, Method{"toUnsignedInteger", type_scope, std::vector<MethodArgument>(), Class("UnsignedInteger")});
    add_method(environment, Class{"Float"}, Method{"toInteger", type_scope, std::vector<MethodArgument>(), Class("Integer")});
    add_method(environment, Class{"Float"}, Method{"toByte", type_scope, std::vector<MethodArgument>(), Class("Byte")});
    add_method(environment, Class{"Float"}, Method{"toBoolean", type_scope, std::vector<MethodArgument>(), Class("Boolean")});
    add_method(environment, Class{"Float"}, Method{"toDouble", type_scope, std::vector<MethodArgument>(), Class("Double")});
    add_method(environment, Class{"Float"}, Method{"new", type_scope, { MethodArgument{Class{"Float"}, false} }, Class("void")});

    // Base Unsigned Integer Class
    type_scope = add_class(environment, Class{"UnsignedInteger"});
    add_method(environment, Class{"UnsignedInteger"}, Method{"output", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"UnsignedInteger"}, Method{"input", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"UnsignedInteger"}, Method{"print", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"UnsignedInteger"}, Method{"toString", type_scope, std::vector<MethodArgument>(), Class("String")});
    add_method(environment, Class{"UnsignedInteger"}, Method{"toDouble", type_scope, std::vector<MethodArgument>(), Class("Double")});
    add_method(environment, Class{"UnsignedInteger"}, Method{"toFloat", type_scope, std::vector<MethodArgument>(), Class("Float")});
    add_method(environment, Class{"UnsignedInteger"}, Method{"toInteger", type_scope, std::vector<MethodArgument>(), Class("Integer")});
    add_method(environment, Class{"UnsignedInteger"}, Method{"toByte", type_scope, std::vector<MethodArgument>(), Class("Byte")});
    add_method(environment, Class{"UnsignedInteger"}, Method{"toBoolean", type_scope, std::vector<MethodArgument>(), Class("Boolean")});
    add_method(environment, Class{"UnsignedInteger"}, Method{"new", type_scope, { MethodArgument{Class{"UnsignedInteger"}, false} }, Class("void")});

    // Base Integer Class
    type_scope = add_class(environment, Class{"Integer"});
    add_method(environment, Class{"Integer"}, Method{"output", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"Integer"}, Method{"input", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"Integer"}, Method{"print", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"Integer"}, Method{"toString", type_scope, std::vector<MethodArgument>(), Class("String")});
    add_method(environment, Class{"Integer"}, Method{"toDouble", type_scope, std::vector<MethodArgument>(), Class("Double")});
    add_method(environment, Class{"Integer"}, Method{"toFloat", type_scope, std::vector<MethodArgument>(), Class("Float")});
    add_method(environment, Class{"Integer"}, Method{"toUnsignedInteger", type_scope, std::vector<MethodArgument>(), Class("UnsignedInteger")});
    add_method(environment, Class{"Integer"}, Method{"toByte", type_scope, std::vector<MethodArgument>(), Class("Byte")});
    add_method(environment, Class{"Integer"}, Method{"toBoolean", type_scope, std::vector<MethodArgument>(), Class("Boolean")});
    add_method(environment, Class{"Integer"}, Method{"new", type_scope, { MethodArgument{Class{"Integer"}, false} }, Class("void")});
    add_method(environment, Class{"Integer"}, Method{"new", type_scope, { MethodArgument{Class{"int"}, false} }, Class("void")});

    // Base Byte Class
    type_scope = add_class(environment, Class{"Byte"});
    add_method(environment, Class{"Byte"}, Method{"output", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"Byte"}, Method{"input", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"Byte"}, Method{"print", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"Byte"}, Method{"toString", type_scope, std::vector<MethodArgument>(), Class("String")});
    add_method(environment, Class{"Byte"}, Method{"toDouble", type_scope, std::vector<MethodArgument>(), Class("Double")});
    add_method(environment, Class{"Byte"}, Method{"toFloat", type_scope, std::vector<MethodArgument>(), Class("Float")});
    add_method(environment, Class{"Byte"}, Method{"toInteger", type_scope, std::vector<MethodArgument>(), Class("Integer")});
    add_method(environment, Class{"Byte"}, Method{"toUnsignedInteger", type_scope, std::vector<MethodArgument>(), Class("UnsignedInteger")});
    add_method(environment, Class{"Byte"}, Method{"toBoolean", type_scope, std::vector<MethodArgument>(), Class("Boolean")});
    add_method(environment, Class{"Byte"}, Method{"new", type_scope, { MethodArgument{Class{"Byte"}, false} }, Class("void")});

    // Base Boolean Class
    type_scope = add_class(environment, Class{"Boolean"});
    add_method(environment, Class{"Boolean"}, Method{"output", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"Boolean"}, Method{"print", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"Boolean"}, Method{"input", type_scope, std::vector<MethodArgument>(), Class("void")});
    add_method(environment, Class{"Boolean"}, Method{"toString", type_scope, std::vector<MethodArgument>(), Class("String")});
    add_method(environment, Class{"Boolean"}, Method{"toDouble", type_scope, std::vector<MethodArgument>(), Class("Double")});
    add_method(environment, Class{"Boolean"}, Method{"toFloat", type_scope, std::vector<MethodArgument>(), Class("Float")});
    add_method(environment, Class{"Boolean"}, Method{"toInteger", type_scope, std::vector<MethodArgument>(), Class("Integer")});
    add_method(environment, Class{"Boolean"}, Method{"toUnsignedInteger", type_scope, std::vector<MethodArgument>(), Class("UnsignedInteger")});
    add_method(environment, Class{"Boolean"}, Method{"toByte", type_scope, std::vector<MethodArgument>(), Class("Byte")});
    add_method(environment, Class{"Boolean"}, Method{"new", type_scope, { MethodArgument{Class{"Boolean"}, false} }, Class("void")});

    // Base List Class
    type_scope = add_class(environment, Class{"List", {Class("Type")}});
    add_method(environment, Class("List"), Method{"append",  type_scope, { MethodArgument{Class("Type"), false} }, Class("void")});
    add_method(environment, Class("List"), Method{"append",  type_scope, { MethodArgument{Class("List", {Class("Type")}), false} }, Class("void")});
    add_method(environment, Class("List"), Method{"prepend", type_scope, { MethodArgument{Class{"Type"}, false} }, Class("void")});
    add_method(environment, Class("List"), Method{"prepend", type_scope, { MethodArgument{Class("List", {Class("Type")}), false} }, Class("void")});
    add_method(environment, Class("List"), Method{"insert",  type_scope, { MethodArgument{Class{"Integer"}, false}, MethodArgument{Class("Type"), false} }, Class("void")});
    add_method(environment, Class("List"), Method{"insert",  type_scope, { MethodArgument{Class{"Integer"}, false}, MethodArgument{Class("List", {Class("Type")}), false} }, Class("void")});
    add_method(environment, Class("List"), Method{"remove",  type_scope, { MethodArgument{Class{"Integer"}, false} }, Class("void")});
    add_method(environment, Class("List"), Method{"remove",  type_scope, NO_ARGUMENTS, Class("void")});
    add_method(environment, Class("List"), Method{"get",     type_scope, { MethodArgument{Class{"Integer"}, false} }, Class("Type")});
    add_method(environment, Class("List"), Method{"length",  type_scope, NO_ARGUMENTS, Class("Integer")});

    // Base Array Class
    type_scope = add_class(environment, Class{"Array", {Class("Type")}});
    add_method(environment, Class("Array"), Method{"new",     type_scope, { MethodArgument{Class{"Integer"}, false} }, Class("void")});
    add_method(environment, Class("Array"), Method{"get",     type_scope, { MethodArgument{Class{"Integer"}, false} }, Class("Type")});
    add_method(environment, Class("Array"), Method{"resize",  type_scope, { MethodArgument{Class{"Integer"}, false} }, Class("void")});
}
