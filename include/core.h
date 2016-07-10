
#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED

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

#include "scope.h"

void load_core(Environment& environment);
Scope* add_class(Environment& e, Class type);
void add_function(Environment& e, Method method);
void add_method(Environment& e, Class type, Method method);
void add_field(Environment& environment, Class base, Variable variable);

#endif // CORE_H_INCLUDED
