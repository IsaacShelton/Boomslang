
#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

#include <string>
#include "token.h"
#include "scope.h"
#include "options.h"

void assemble(Configuration*, TokenList&, Environment&);
void assemble_expression(TokenContext, std::string&, Environment&);

#endif // ASSEMBLER_H_INCLUDED
