
#ifndef SCOPE_H_INCLUDED
#define SCOPE_H_INCLUDED

#include <string>
#include <vector>

#define IGNORE      "..."
#define IGNORE_ARGS {MethodArgument{Template{IGNORE}, 1}}

#define METHOD_PREFIX   "METHOD "
#define TEMPLATE_PREFIX "TEMPLATE "

struct Variable;
struct Scope;
struct Template;
struct MethodArgument;
struct Method;
struct Environment;

struct Variable {
    std::string name;
    std::string type;
};

struct Template {
    std::string name;
};

struct MethodArgument {
    Template type;
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
    std::vector<Template> templates;
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
typedef std::vector<Template> TemplateList;

// Scopes
void print_scopes(Scope* scope, unsigned int indent = 0);
void print_scopes_variables(Scope* scope, unsigned int indent = 0);
void clean_scopes(Scope* scope);
Scope* environment_get_child(Scope* scope, std::string name);

// Methods
bool environment_method_exists(Scope*, Method);
unsigned int environment_method_index(Scope*, Method);
Method environment_method_get(Scope*, Method);

// Templates
bool environment_template_exists(Scope*, Template);
unsigned int environment_template_index(Scope*, Template);
Template environment_template_get(Scope*, Template);
bool environment_template_variable_exists(Environment& environment, Template base, Variable variable);
Variable environment_template_variable_get(Environment& environment, Template base, Variable variable);

// Variables
void environment_print_variables(Scope* scope, unsigned int indent = 0);
bool environment_variable_exists(Scope* scope, Variable);
Variable environment_variable_get(Scope* scope, Variable);

#endif // SCOPE_H_INCLUDED
