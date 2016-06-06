
#ifndef ENFORCER_H_INCLUDED
#define ENFORCER_H_INCLUDED

#include <string>
#include <vector>
#include "token.h"
#include "scope.h"
#include "options.h"

Environment enforce(Configuration* config, TokenList&);

#endif // ENFORCER_H_INCLUDED
