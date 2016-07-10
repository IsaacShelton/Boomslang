
#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

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

/*
    Higher Level Context Aware Functions
    that take an Environment& instead of a Scope*
    like most of the recursive functions called
*/

#include "token.h"

bool context_variable_exists(Environment&, Variable);
bool context_class_exists(TokenContext context, Environment&, Class);
bool context_method_exists(TokenContext context, Environment&, Method);
bool context_method_exists(TokenContext context, Environment&, Class, Method);
std::string context_root_class(std::string);
Class context_root_class(Class);

Variable context_variable_get(Environment&, Variable);
Class context_class_get(Environment&, Class);
Method context_method_get(TokenContext context, Environment&, Method);

void context_enforce_expression(TokenContext context, Environment& e, Class& type);
void context_enforce_arguments(TokenContext context, Environment& e, Class& type, std::string override_method = "");
void context_enforce_method_declaration_arguments(TokenContext tokens, Environment& e, MethodArgumentList& method_arguments, std::string& argument_string);
void context_enforce_following_method_calls(TokenContext context, Environment& e, Class& type);
void context_enforce_string(TokenContext context, Environment& environment, std::string str);
void context_enforce_type(TokenContext context, Environment& environment, Class& type);

void context_assemble_string(TokenContext context, Environment& environment, std::string, std::string&);

void context_class_dereference(TokenContext context, Class& type);
bool context_class_dereference_ifcan(TokenContext context, Class& type);
bool context_class_can_dereference(TokenContext context, Class type);
bool context_class_compare(TokenContext context, Class a, Class b);

bool name_is_class(std::string);
std::string name_get_class(std::string);
bool name_is_method(std::string);
std::string name_get_method(std::string);

#endif // CONTEXT_H_INCLUDED
