
#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

/*
    Higher Level Context Aware Functions
    that take an Environment& instead of a Scope*
    like most of the recursive functions called
*/

bool context_variable_exists(Environment&, Variable);
bool context_template_exists(Environment&, Template);
bool context_method_exists(Environment&, Method);
bool context_method_exists(Environment&, Template, Method);

Variable context_variable_get(Environment&, Variable);
Template context_template_get(Environment&, Template);
Method context_method_get(Environment&, Method);

#endif // CONTEXT_H_INCLUDED
