
#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include <string>

void log_lexer(std::string message);
void log_enforcer(std::string message);
void log_assembler(std::string message);

void clear_lexer_log();
void clear_enforcer_log();
void clear_assembler_log();

#endif // LOG_H_INCLUDED
