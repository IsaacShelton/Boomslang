
#ifndef DUMP_H_INCLUDED
#define DUMP_H_INCLUDED

#include <string>
#include "token.h"

void tokens_dump(std::string filename, TokenList& tokens);
bool tokens_load(std::string filename, TokenList& tokens);

#endif // DUMP_H_INCLUDED
