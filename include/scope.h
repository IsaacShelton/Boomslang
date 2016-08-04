
#ifndef SCOPE_H_INCLUDED
#define SCOPE_H_INCLUDED

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
#include "token.h"

#define IGNORE      "..."
#define IGNORE_ARGS {MethodArgument{Class{IGNORE}, 1}}
#define IGNORE_CLASS Class{IGNORE}
#define NO_ARGUMENTS std::vector<MethodArgument>()

#define METHOD_PREFIX   std::string("METHOD ")
#define CLASS_PREFIX std::string("CLASS ")

struct Variable;
struct Scope;
struct Class;
struct MethodArgument;
struct Method;
struct Environment;

struct Class {
    std::string name = "";
    std::vector<Class> generics;
    bool is_mutable = true;

    Class();
    Class(std::string);
    Class(std::string, std::vector<Class>);
    Class(std::string, std::vector<Class>, bool);
    void operator=(const Class&);
    bool operator==(const Class&);
    std::string native();
    std::string toString();
};

struct Variable {
    std::string name;
    Class type;
    bool is_final;
    bool is_uniform;
    bool is_private;

    Variable();
    Variable(std::string, Class);
    Variable(std::string, Class, bool, bool);
    Variable(std::string, Class, bool, bool, bool);
};

struct MethodArgument {
    Class type;
    bool optional;
};

struct Method {
    std::string name;
    Scope* parent;
    std::vector<MethodArgument> arguments;
    Class return_type;
};

struct Scope {
    std::string name;
    Scope* parent;
    std::vector<Scope*> children;
    std::vector<Variable> variables;
    std::vector<Method> methods;
    std::vector<Class> classes;
};

struct Environment {
    // Root Scope
    Scope global{"global", NULL};

    // Current Scope
    Scope* scope = &(this->global);
};

typedef std::vector<Variable> VariableList;
typedef std::vector<Scope> ScopeList;
typedef std::vector<Method> MethodList;
typedef std::vector<Class> ClassList;
typedef std::vector<MethodArgument> MethodArgumentList;

// Scopes
void print_scopes(Scope* scope, unsigned int indent = 0);
void print_scopes_variables(Scope* scope, unsigned int indent = 0);
void clean_scopes(Scope* scope);
Scope* environment_get_child(Scope* scope, std::string name);

// Methods
bool environment_method_exists(TokenContext context, Scope*, Method);
unsigned int environment_method_index(Scope*, Method);
Method environment_method_get(TokenContext context, Scope*, Method);
bool environment_generic_method_exists(TokenContext context, Scope*, Method, Class, Class);
Method environment_generic_method_get(TokenContext context, Scope*, Method, Class, Class);

// Classes
bool environment_class_exists(Scope*, Class);
unsigned int environment_class_index(Scope*, Class);
Class environment_class_get(Scope*, Class);
bool environment_class_variable_exists(Environment& environment, Class base, Variable variable);
Variable environment_class_variable_get(Environment& environment, Class base, Variable variable);
Class environment_class_get_first(Scope*, Class, Class);

// Variables
void environment_print_variables(Scope* scope, unsigned int indent = 0);
bool environment_variable_exists(Scope* scope, Variable);
Variable environment_variable_get(Scope* scope, Variable);

#endif // SCOPE_H_INCLUDED
