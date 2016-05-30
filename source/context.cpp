
#include "../include/scope.h"
#include "../include/context.h"

using namespace std;

bool context_variable_exists(Environment& e, Variable variable){
    return environment_variable_exists(e.scope, variable);
}
bool context_template_exists(Environment& e, Template base){
    return environment_template_exists(&e.global, base);
}
bool context_method_exists(Environment& e, Method method){
    return environment_method_exists(&e.global, method);
}
bool context_method_exists(Environment& e, Template base, Method method){
    return environment_method_exists(environment_get_child(&e.global, base.name), method);
}

Variable context_variable_get(Environment& e, Variable variable){
    return environment_variable_get(e.scope, variable);
}
Template context_template_get(Environment& e, Template base){
    return environment_template_get(&e.global, base);
}
Method context_method_get(Environment& e, Method method){
    return environment_method_get(&e.global, method);
}
Method context_method_get(Environment& e, Template base, Method method){
    return environment_method_get(environment_get_child(&e.global, base.name), method);
}
