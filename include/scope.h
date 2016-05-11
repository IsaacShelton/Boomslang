
#ifndef SCOPE_H_INCLUDED
#define SCOPE_H_INCLUDED

#include <string>
#include <vector>

struct Variable {
    std::string name;
    std::string type;
};

typedef std::vector<Variable> VariableList;

struct Scope {
    std::string name;
    Scope* parent;
    std::vector<Scope*> children;
    VariableList variables;
};

typedef std::vector<Scope> ScopeList;

void print_scopes(Scope* scope, unsigned int indent = 0);
void clean_scopes(Scope* scope);

#endif // SCOPE_H_INCLUDED
