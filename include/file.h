
#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <string>

using namespace std;

void import_boomslang(string);
int execute_silent(string, string);
string filename_name(string);
string filename_path(string);
string filename_change_ext(string, string);
void write(string, int);

#endif // FILE_H_INCLUDED
