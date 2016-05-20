
#include <fstream>
#include "../include/die.h"
#include "../include/log.h"
#include "../include/locate.h"

using namespace std;

void log_lexer(std::string message){
    ofstream lexer_logfile(LOGHOME + LOG_LEXER, ios::app);

    if(!lexer_logfile) die("Failed to open lexer log file");

    lexer_logfile <<  message << endl;
    lexer_logfile.close();
}
void log_enforcer(std::string message){
    ofstream enforcer_logfile(LOGHOME + LOG_ENFORCER, ios::app);

    if(!enforcer_logfile) die("Failed to open enforcer log file");

    enforcer_logfile <<  message << endl;
    enforcer_logfile.close();
}
void log_assembler(std::string message){
    ofstream assembler_logfile(LOGHOME + LOG_ASSEMBLER, ios::app);

    if(!assembler_logfile) die("Failed to open assembler log file");

    assembler_logfile <<  message << endl;
    assembler_logfile.close();
}

void clear_lexer_log(){
    remove( (LOGHOME + LOG_LEXER).c_str() );
}
void clear_enforcer_log(){
    remove( (LOGHOME + LOG_ENFORCER).c_str() );
}
void clear_assembler_log(){
    remove( (LOGHOME + LOG_ASSEMBLER).c_str() );
}
