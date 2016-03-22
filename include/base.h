
#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

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
