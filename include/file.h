
#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <string>
#include <fstream>

std::string contents(std::string, std::string error_message = "The file specified does not exist");
bool file_exists(std::string);
std::string full_path(std::string filename, std::string location = "");

#endif // FILE_H_INCLUDED
