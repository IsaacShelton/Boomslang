
#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

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

using namespace std;

//Class for parsing common code
int code_parse_args(string& code, string, string, string& output);
int code_parse_declaration_args(string& code, string, string, string& output);
int code_parse_function_from(string& code, bool, int, string, string, string& output);
void code_chop(string& code);
int code_harvest_value(string& code, string &, string, string, string ,unsigned int, string& output);
int code_harvest_value_type(string code, string &, string , string, unsigned int);
int code_harvest_raw_expression(string& code, string&, string, string, string, string& output);
string code_harvest_string(string& code);
string code_harvest_decimal(string& code);
int code_arg_type(string);

#endif // BASE_H_INCLUDED
