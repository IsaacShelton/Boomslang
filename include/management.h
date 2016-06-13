
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

#include <string>
#include <fstream>

double to_double(std::string);
std::string to_string(double);

bool string_contains(std::string, std::string);
std::string string_get_until(std::string, std::string);
std::string string_get_until_or(std::string, std::string);
std::string string_delete_until(std::string, std::string);
std::string string_delete_until_or(std::string, std::string);
std::string string_delete_amount(std::string, int);
std::string string_get_until_last(std::string text, std::string character_set);

unsigned int string_count(std::string, std::string);
std::string string_replace(std::string, std::string, std::string);
std::string string_replace_all(std::string, std::string, std::string);

std::string string_kill_whitespace(std::string);
std::string string_kill_all_whitespace(std::string);
std::string string_kill_newline(std::string);

std::string delete_slash(std::string);
std::string string_upper(std::string);

std::string resource(std::string);
bool is_identifier(std::string);
bool is_indent(std::string);

std::string filename_name(std::string);
std::string filename_path(std::string);
std::string filename_change_ext(std::string, std::string);
std::ifstream::pos_type file_size(std::string);

std::string encode_function_name(std::string);
std::string encode_method_name(std::string, std::string);

#endif // MANAGEMENT_H_INCLUDED
