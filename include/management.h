
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

double to_double(const std::string&);
std::string to_string(double);

bool string_contains(const std::string&, const std::string&);
std::string string_get_until(const std::string&, const std::string&);
std::string string_get_until_or(const std::string&, const std::string&);
std::string string_iter_until(std::string&, size_t&, char);
std::string string_iter_until_or(std::string&, size_t&, const std::string&);
std::string string_itertest_until(std::string&, size_t, char);
std::string string_itertest_until_or(std::string&, size_t, const std::string&);
std::string string_delete_until(const std::string&, const std::string&);
std::string string_delete_until_or(const std::string&, const std::string&);
std::string string_delete_amount(const std::string&, int);
std::string string_get_until_last(const std::string& text, const std::string& character_set);

unsigned int string_count(const std::string&, const std::string&);
std::string string_replace(std::string, const std::string&, const std::string&);
std::string string_replace_all(std::string, const std::string&, const std::string&);

std::string string_kill_whitespace(const std::string&);
std::string string_kill_all_whitespace(const std::string&);
std::string string_kill_newline(const std::string&);
std::string string_flatten(std::string);
void string_iter_kill_whitespace(const std::string& code, size_t& i);

std::string delete_slash(const std::string&);
std::string string_upper(std::string);

std::string resource(const std::string&);
std::string resource_type(const std::string&);
bool is_identifier(const std::string&);
bool is_indent(const std::string&, size_t i = 0);

std::string filename_name(const std::string&);
std::string filename_path(const std::string&);
std::string filename_change_ext(const std::string&, const std::string&);
std::ifstream::pos_type file_size(const std::string&);

#endif // MANAGEMENT_H_INCLUDED
