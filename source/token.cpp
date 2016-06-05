
#include <fstream>
#include <iostream>
#include "../include/die.h"
#include "../include/log.h"
#include "../include/lexer.h"
#include "../include/token.h"
#include "../include/errors.h"
#include "../include/locate.h"

using namespace std;

// Token identification
string token_name(Token token){
    string token_name;

    switch(token.id){
        case  0 : token_name = "string literal"; break;
        case  1 : token_name = "numeric literal"; break;
        case  2 : token_name = "method call"; break;
        case  3 : token_name = "function call"; break;
        case  4 : token_name = "assign"; break;
        case  5 : token_name = "add"; break;
        case  6 : token_name = "subtract"; break;
        case  7 : token_name = "multiply"; break;
        case  8 : token_name = "divide"; break;
        case  9 : token_name = "new"; break;
        case 10 : token_name = "open"; break;
        case 11 : token_name = "close"; break;
        case 12 : token_name = "indent"; break;
        case 13 : token_name = "dedent"; break;
        case 14 : token_name = "macro"; break;
        case 15 : token_name = "keyword"; break;
        case 16 : token_name = "word"; break;
        case 17 : token_name = "terminate"; break;
        case 18 : token_name = "next"; break;
        case 19 : token_name = "square open bracket"; break;
        case 20 : token_name = "square close bracket"; break;
        case 21 : token_name = "curly open bracket"; break;
        case 22 : token_name = "curly close bracket"; break;
        case 23 : token_name = "not"; break;
        case 24 : token_name = "address"; break;
        case 25 : token_name = "member"; break;
        case 26 : token_name = "address member"; break;
        case 27 : token_name = "pointer"; break;
        case 28 : token_name = "less than"; break;
        case 29 : token_name = "greater than"; break;
    }

    return token_name;
}
string token_operator(Token token){
    string token_name;

    switch(token.id){
        case  0 : token_name = "\""; break;
        case  1 : token_name = "number"; break;
        case  2 : token_name = "method call"; break;
        case  3 : token_name = "function call"; break;
        case  4 : token_name = "="; break;
        case  5 : token_name = "+"; break;
        case  6 : token_name = "-"; break;
        case  7 : token_name = "*"; break;
        case  8 : token_name = "/"; break;
        case  9 : token_name = "new"; break;
        case 10 : token_name = "("; break;
        case 11 : token_name = ")"; break;
        case 12 : token_name = "indent"; break;
        case 13 : token_name = "dedent"; break;
        case 14 : token_name = "macro"; break;
        case 15 : token_name = "keyword"; break;
        case 16 : token_name = "word"; break;
        case 17 : token_name = "terminate"; break;
        case 18 : token_name = ","; break;
        case 19 : token_name = "["; break;
        case 20 : token_name = "]"; break;
        case 21 : token_name = "{"; break;
        case 22 : token_name = "}"; break;
        case 23 : token_name = "!"; break;
        case 24 : token_name = "&"; break;
        case 25 : token_name = "."; break;
        case 26 : token_name = ":"; break;
        case 27 : token_name = "^"; break;
        case 28 : token_name = "<"; break;
        case 29 : token_name = ">"; break;
    }

    return token_name;
}

// Token Logging
void token_print(Token token){
    cout << "Token => " + token_name(token) + " :   '" + token.data + "'" << endl;
}
void lexer_log_tokens(TokenList tokens){
    ofstream lexer_logfile(LOGHOME + LOG_LEXER, ios::app);

    if(!lexer_logfile) die("Failed to open lexer log file");

    lexer_logfile << "Resulting Tokens (" << tokens.size()-1 << ")" << endl;

    for(unsigned int i = 0; i < tokens.size(); i++){
        lexer_logfile << i << " '" + token_name(tokens[i]) + "'" << endl;
    }

    lexer_logfile.close();
}

// Safe Index Navigation
bool advance_index(unsigned int& index, unsigned int length){
    if(index + 1 >= length){return false;}
    index++;

    return true;
}
bool retreat_index(unsigned int& index){
    if(index - 1 < 0){return false;}
    index--;

    return true;
}

// Shorthand Index Navigation with errors
void index_increase(TokenContext context){
    if(!advance_index(context.index ,context.tokens.size())) die(UNEXPECTED_TERMINATE);
}
void index_decrease(TokenContext context){
    if(!retreat_index(context.index)) die(UNEXPECTED_TERMINATE);
}

// Token Forcing
void token_force(TokenContext context, unsigned int token_id, std::string cant_advance, std::string fail){
    if( !advance_index(context.index, context.tokens.size()) ){
        die(cant_advance);
    }
    if(context.tokens[context.index].id != token_id){
        die(fail);
    }
}
