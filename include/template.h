
#ifndef TEMPLATE_H_INCLUDED
#define TEMPLATE_H_INCLUDED

#include <string>

using namespace std;

int compile_template(int arg_count,char** args, unsigned int indentation,bool unique_template,string method_name,string template_name,string& write_template_buffer);

#endif // TEMPLATE_H_INCLUDED
