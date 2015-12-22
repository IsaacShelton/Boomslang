
#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED

#include <string>

using namespace std;

int compile_function(int arg_count,char** args, unsigned int indentation,string method_name,string template_name,string& output_return_type,string& output);

#endif // FUNCTION_H_INCLUDED
