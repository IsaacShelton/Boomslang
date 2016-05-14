
#ifndef SCOPE_H_INCLUDED
#define SCOPE_H_INCLUDED

#include <string>
#include <vector>

#define IGNORE "..."

struct Variable {
    std::string name;
    std::string type;
};

struct Scope {
    std::string name;
    Scope* parent;
    std::vector<Scope*> children;
    std::vector<Variable> variables;
};

struct Method {
    std::string name;
    Scope* parent;
    std::string arguments;
    std::string return_type;
};

struct Template {
    std::string name;
};

struct Environment {
    Scope global{"global", NULL};
    std::vector<Method> methods;
    std::vector<Template> templates;
};

typedef std::vector<Variable> VariableList;
typedef std::vector<Scope> ScopeList;
typedef std::vector<Method> MethodList;
typedef std::vector<Template> TemplateList;

// Scopes
void print_scopes(Scope* scope, unsigned int indent = 0);
void print_scopes_variables(Scope* scope, unsigned int indent = 0);
void clean_scopes(Scope* scope);

// Methods
bool environment_method_exists(Environment*, Method);
unsigned int environment_method_index(Environment*, Method);
Method environment_method_get(Environment*, Method);

// Templates
bool environment_template_exists(Environment*, Template);
unsigned int environment_template_index(Environment*, Template);
Template environment_template_get(Environment*, Template);

// Variables
void environment_print_variables(Scope* scope, unsigned int indent = 0);
bool environment_variable_exists(Scope* scope, Variable);
Variable environment_variable_get(Scope* scope, Variable);

#endif // SCOPE_H_INCLUDED
