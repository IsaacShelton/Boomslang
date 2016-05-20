
#ifndef ENFORCER_H_INCLUDED
#define ENFORCER_H_INCLUDED

#include <string>
#include <vector>
#include "token.h"
#include "scope.h"

#define METHOD_PREFIX   "METHOD "
#define TEMPLATE_PREFIX "TEMPLATE "

struct TemplateAdditions {
    std::string name;
    std::string additions;
};

extern std::vector<TemplateAdditions> template_additions;

Environment enforce(TokenList);
void template_add_method(std::string, std::string);

#endif // ENFORCER_H_INCLUDED
