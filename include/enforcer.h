
#ifndef ENFORCER_H_INCLUDED
#define ENFORCER_H_INCLUDED

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
#include <vector>
#include "token.h"
#include "scope.h"
#include "options.h"

Environment enforce(Configuration*, TokenList&);
void enforce_package(Configuration*, TokenList&, Environment&);

void enforce_token(Configuration*, TokenContext, Environment&);
void enforce_literal(Configuration*, TokenContext, Environment&);
void enforce_beginning_expression(Configuration*, TokenContext, Environment&);
void enforce_keyword(Configuration*, TokenContext, Environment&, bool&, unsigned int&);
void enforce_word(Configuration*, TokenContext, Environment&, bool&);
void enforce_pointer(Configuration*, TokenContext, Environment&);
void enforce_library_file(TokenContext, unsigned int&);

#endif // ENFORCER_H_INCLUDED
