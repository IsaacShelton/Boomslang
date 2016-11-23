
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
#include "options.h"

bool context_variable_exists(Environment&, Variable);
bool context_class_exists(TokenContext context, Environment&, Class);
bool context_method_exists(TokenContext context, Environment&, Method);
bool context_method_exists(TokenContext context, Environment&, Class, Method);
std::string context_root_class(std::string);
Class context_root_class(Class);

Variable context_variable_get(Environment&, Variable);
Class context_class_get(Environment&, Class);
Method context_method_get(TokenContext context, Environment&, Method);

void context_enforce_expression(TokenContext, Environment&, Class&);
void context_enforce_expression_open(TokenContext, Environment&, Class&);
void context_enforce_expression_pointer(TokenContext, Environment&, Class&);
void context_enforce_expression_address(TokenContext, Environment&, Class&);
void context_enforce_expression_word(TokenContext, Environment&, Class&);
void context_enforce_expression_keyword(TokenContext, Environment&, Class&, bool&, bool&);
void context_enforce_expression_string(TokenContext, Environment&, Class&);
void context_enforce_expression_numeric(TokenContext, Environment&, Class&);
void context_enforce_expression_literal(TokenContext, Environment&, Class&, std::string);

void context_enforce_arguments(TokenContext, Environment&, Class&, std::string override_method = "", bool is_static = false);
void context_enforce_method_declaration_arguments(TokenContext, Environment&, MethodArgumentList&, std::string&);
void context_enforce_following_method_calls(TokenContext, Environment&, Class&);
void context_enforce_string(TokenContext, Environment&, std::string);
void context_enforce_string_escape(TokenContext, Environment&, std::string&, size_t&);
void context_enforce_string_escape_expression(TokenContext, Environment&, std::string&, size_t&);
void context_enforce_type(TokenContext, Environment&, Class&);
void context_enforce_followup_block(Configuration*, TokenContext, Environment&);

std::string context_assemble_string_expression(TokenContext context, Environment&, std::string&, size_t&);
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
