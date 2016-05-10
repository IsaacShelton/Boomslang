
#include <iostream>
#include <stdlib.h>
#include "../include/token.h"
#include "../include/lexer.h"
#include "../include/management.h"

using namespace std;

void process_indentation(TokenList& tokens, string& code, int indentation){
    unsigned int line_indentaion = 0;

    while(is_indent(code)){
        line_indentaion++;

        if(code.substr(0,4) == "    "){
            code = string_delete_amount(code,4);
        } else {
            code = string_delete_amount(code,1);
        }
    }
    if(line_indentaion > indentation){
        while(line_indentaion > indentation){
            indentation++;
            tokens.push_back( TOKEN_INDENT );
        }
    }
    else if(line_indentaion < indentation){
        while(line_indentaion < indentation){
            indentation--;
            tokens.push_back( TOKEN_DEDENT );
        }
    }
}

TokenList tokenize(string code){
    // Converts code into a stream of tokens

    TokenList tokens;
    unsigned int indentation = 0;
    string prev;

    process_indentation(tokens, code, indentation);

    if(code == ""){
        return tokens;
    }

    if(code[code.length()-1] != '\n'){
        code += "\n";
    }

    while(code != "" and code!=prev){
        prev = code;

        // Remove Newlines and track indentation
        while(code.substr(0,1) == "\n"){
            code = string_delete_amount(code,1);
            tokens.push_back( TOKEN_TERMINATE );

            process_indentation(tokens, code, indentation);
        }

        if(code.substr(0,1) == "\"" or code.substr(0,1) == "'"){ // String Literal
            code = string_delete_amount(code,1);
            tokens.push_back( TOKEN_STRING_LITERAL(string_get_until_or(code,"\"'")) );
            code = string_delete_until_or(code,"\"'");
            code = string_delete_amount(code,1);
        }
        else if( (int)code[0] >= 48 and (int)code[0] <= 57){     // Numeric Literal
            tokens.push_back( TOKEN_NUMERIC_LITERAL(string_get_until_or(code," ,)]\n")) );
            code = string_delete_until_or(code," ,)]\n");
        }
        else if( code.substr(0,1) == "."){                       // Method Call
            code = string_delete_amount(code,1);
            tokens.push_back( TOKEN_METHOD_CALL(string_get_until_or(code," (")) );
            code = string_delete_until_or(code," (");
        }
        else if( code.substr(0,1) == "("){                       // Open
            tokens.push_back( TOKEN_OPEN );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == ")"){                       // Close
            tokens.push_back( TOKEN_CLOSE );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == "="){                       // Assign
            tokens.push_back( TOKEN_ASSIGN );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == "+"){                       // Add
            tokens.push_back( TOKEN_ADD );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == "-"){                       // Subtract
            tokens.push_back( TOKEN_SUBTRACT );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == "*"){                       // Multiply
            tokens.push_back( TOKEN_MULTIPLY );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == "/"){                       // Divide
            tokens.push_back( TOKEN_DIVIDE );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == ","){                       // Next
            tokens.push_back( TOKEN_NEXT );
            code = string_delete_amount(code,1);
        }
        else if( string_get_until(code," ") == "on"){            // Method Declaration
            tokens.push_back( TOKEN_KEYWORD("on") );

            code = string_delete_amount(code,2);
            code = string_kill_whitespace(code);

            tokens.push_back( TOKEN_WORD(string_get_until_or(code," (\n")) );
            code = string_delete_until_or(code," (\n");
        }
        else if( string_get_until(code," ") == "import"){        // import
            tokens.push_back( TOKEN_KEYWORD("import") );

            code = string_delete_amount(code,6);
            code = string_kill_whitespace(code);

            tokens.push_back( TOKEN_WORD(string_get_until(code,"\n")) );
            code = string_delete_until(code,"\n");
        }
        else if( string_get_until(code," ") == "unique"){        // Method Declaration
            tokens.push_back( TOKEN_KEYWORD("unique") );

            code = string_delete_amount(code,6);
            code = string_kill_whitespace(code);
        }
        else if( is_identifier(string_get_until_or(code, " ,()[]\n.+-*/=")) ){
            tokens.push_back( TOKEN_WORD(string_get_until_or(code, " ,()[]\n.+-*/=")) );
            code = string_delete_until_or(code, " ,()[]\n.+-*/=");
        }

        code = string_kill_whitespace(code);
    }

    if(code == prev){
        cerr << "Unrecognized operator '" + code.substr(0,1) + "'" << endl;
        exit(1);
    }

    return tokens;
}
