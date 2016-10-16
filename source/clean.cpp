
#include "../include/log.h"
#include "../include/clean.h"
#include "../include/scope.h"

void clean(Configuration* config, Environment& environment){
    clean_scopes(&environment.global);
    if(config->log) logging_context.destroy();
}
