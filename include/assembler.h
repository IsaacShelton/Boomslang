
#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

/*
    (c) 2016 Isaac Shelton

    This file is part of Boomslang.

    Boomslang is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Boomslang is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Boomslang. If not, see <http://www.gnu.org/licenses/>.
*/

#include <string>
#include "token.h"
#include "scope.h"
#include "options.h"

void assemble_expression(TokenContext, std::string&, Environment&);
void assemble_token(Configuration*, TokenContext, bool&, std::string&, std::ofstream&, std::ofstream&, unsigned int&, Environment&);

void compile(Configuration*, TokenList&, Environment&);
void build(Configuration*);
void assemble(Configuration*, TokenList&, Environment&);

void assemble_expression_keyword(TokenContext, std::string&);
void assemble_keyword(Configuration*, TokenContext, bool&, std::string&, std::ofstream&, std::ofstream&, unsigned int&, Environment&);
void assemble_keyword_def(Configuration*, TokenContext, bool&, std::string&, std::ofstream&, std::ofstream&, unsigned int&, Environment&);
void assemble_keyword_class(Configuration*, TokenContext, bool&, std::string&, std::ofstream&, std::ofstream&, unsigned int&, Environment&);
void assemble_keyword_if(Configuration*, TokenContext, bool&, std::string&, std::ofstream&, std::ofstream&, unsigned int&, Environment&);
void assemble_keyword_unless(Configuration*, TokenContext, bool&, std::string&, std::ofstream&, std::ofstream&, unsigned int&, Environment&);
void assemble_keyword_else(Configuration*, TokenContext, bool&, std::string&, std::ofstream&, std::ofstream&, unsigned int&, Environment&);
void assemble_keyword_while(Configuration*, TokenContext, bool&, std::string&, std::ofstream&, std::ofstream&, unsigned int&, Environment&);
void assemble_keyword_until(Configuration*, TokenContext, bool&, std::string&, std::ofstream&, std::ofstream&, unsigned int&, Environment&);
void assemble_keyword_forever(Configuration*, TokenContext, bool&, std::string&, std::ofstream&, std::ofstream&, unsigned int&, Environment&);
void assemble_keyword_for(Configuration*, TokenContext, bool&, std::string&, std::ofstream&, std::ofstream&, unsigned int&, Environment&);
void assemble_keyword_new(Configuration*, TokenContext, bool&, std::string&, std::ofstream&, std::ofstream&, unsigned int&, Environment&);
void assemble_keyword_create(Configuration*, TokenContext, bool&, std::string&, std::ofstream&, std::ofstream&, unsigned int&, Environment&);
void assemble_keyword_import(Configuration*, TokenContext, bool&, std::string&, std::ofstream&, std::ofstream&, unsigned int&, Environment&);
void assemble_keyword_native(Configuration*, TokenContext, bool&, std::string&, std::ofstream&, std::ofstream&, unsigned int&, Environment&);
void assemble_keyword_functionptr(Configuration*, TokenContext, bool&, std::string&, std::ofstream&, std::ofstream&, unsigned int&, Environment&);

#endif // ASSEMBLER_H_INCLUDED
