
#ifndef SCRIPTS_H_INCLUDED
#define SCRIPTS_H_INCLUDED

using namespace std;

#include <string>

double to_double(string);
string to_string(double);
bool file_exists(string);
void error_fatal(string);
void error_show(string);
void error_warn(string);
void error_log(string);
void error_debug(string);
void error_suggest(string);
void pend();

#endif // SCRIPTS_H_INCLUDED
