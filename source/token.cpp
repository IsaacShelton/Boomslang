
#include <iostream>
#include "../include/lexer.h"
#include "../include/token.h"

using namespace std;

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
    }

    return token_name;
}

void token_print(Token token){
    cout << "Token => " + token_name(token) + " :   '" + token.data + "'" << endl;
}
