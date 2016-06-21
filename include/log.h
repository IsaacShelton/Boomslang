
#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include <string>

extern bool LOGGING_LEXER;
extern bool LOGGING_ENFORCER;
extern bool LOGGING_ASSEMBLER;

void log_lexer(std::string message);
void log_enforcer(std::string message);
void log_assembler(std::string message);

void clear_lexer_log();
void clear_enforcer_log();
void clear_assembler_log();

#endif // LOG_H_INCLUDED
