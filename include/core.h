
#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED

#include "scope.h"

void load_core(Environment& environment);
Scope* add_class(Environment& e, Class type);
void add_function(Environment& e, Method method);
void add_method(Environment& e, Class type, Method method);
void add_field(Environment& environment, Class base, Variable variable);

#endif // CORE_H_INCLUDED
