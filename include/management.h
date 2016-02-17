
#ifndef MANAGEMENT_H_INCLUDED
#define MANAGEMENT_H_INCLUDED

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
