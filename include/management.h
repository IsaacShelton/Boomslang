
#ifndef MANAGEMENT_H_INCLUDED
#define MANAGEMENT_H_INCLUDED

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

#include <string.h>
#include "base.h"
#include "scripts.h"

using namespace std;

bool string_contains(string, string);
string string_get_until(string, string);
string string_delete_until(string, string);
string string_get_until_or(string, string);
string string_delete_until_or(string, string);
string string_delete_amount(string, int);
unsigned int string_count(string, string);
string string_replace(string, const string, const string);
string string_replace_all(string, const string, const string);
string string_kill_whitespace(string);
string string_kill_all_whitespace(string);
string string_kill_newline(string);
string resource(string);
string delete_backslash(string a);
string string_upper(string a);
bool is_identifier(string what);
bool is_identifier(string what, string extras);
bool is_indent(string what);
bool rawvalue_exists(string code);

#endif // MANAGEMENT_H_INCLUDED
