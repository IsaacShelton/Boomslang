
#ifndef SCOPE_H_INCLUDED
#define SCOPE_H_INCLUDED

#include <string>
#include <vector>
#include "token.h"

#define IGNORE      "..."
#define IGNORE_ARGS {MethodArgument{Class{IGNORE}, 1}}

#define METHOD_PREFIX   std::string("METHOD ")
#define CLASS_PREFIX std::string("CLASS ")

struct Variable;
struct Scope;
struct Class;
struct MethodArgument;
struct Method;
struct Environment;

struct Variable {
    std::string name;
    std::string type;
    bool is_final;
    bool is_global;
};

struct Class {
    std::string name;
};

struct MethodArgument {
    Class type;
    bool optional;
};

struct Method {
    std::string name;
    Scope* parent;
    std::vector<MethodArgument> arguments;
    std::string return_type;
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
