
#include <iostream>
#include <stdlib.h>
#include "../include/die.h"
#include "../include/log.h"
#include "../include/token.h"
#include "../include/lexer.h"
#include "../include/errors.h"
#include "../include/management.h"

#define LEXER_LOG_PREFIX (to_string(tokens.size()) + " ")

using namespace std;

void process_indentation(TokenList& tokens, string& code, unsigned int& indentation){
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
        tokens.push_back(TOKEN_TERMINATE);
        lexer_log_tokens(tokens);
        return tokens;
    }

    if(code[code.length()-1] != '\n'){
        code += "\n";
    }

    while(code != "" and code!=prev){
        prev = code;

        // Remove Newlines and track indentation
        while(code.substr(0,1) == "\n"){
            log_lexer(LEXER_LOG_PREFIX + "Found a newline, adding terminate token");
            code = string_delete_amount(code,1);
            tokens.push_back( TOKEN_TERMINATE );

            process_indentation(tokens, code, indentation);
        }

        if(code.substr(0,1) == "\""){ // String Literal
            log_lexer(LEXER_LOG_PREFIX + "Found string literal, adding string literal token");
            code = string_delete_amount(code,1);
            tokens.push_back( TOKEN_STRING_LITERAL(string_get_until(code,"\"")) );
            code = string_delete_until(code,"\"");
            code = string_delete_amount(code,1);
        }
        else if( (int)code[0] >= 48 and (int)code[0] <= 57){     // Numeric Literal
            std::string number = string_get_until_or(code," ,)].\n");
            code = string_delete_until_or(code," ,)].\n");

            if(code.substr(0,1) == "."){
                if( (int)code[1] >= 48 and (int)code[1] <= 57){
                    code = string_delete_amount(code, 1);
                    number += "." + string_get_until_or(code," ,)].\n");
                    code = string_delete_until_or(code," ,)].\n");
                }
            }

            log_lexer(LEXER_LOG_PREFIX + "Found numeric literal, adding numeric literal token");
            tokens.push_back( TOKEN_NUMERIC_LITERAL(number) );
        }
        else if( code.substr(0,1) == "."){                       // Member
            log_lexer(LEXER_LOG_PREFIX + "Found member operator, adding member token");
            code = string_delete_amount(code,1);
            tokens.push_back( TOKEN_MEMBER );
        }
        else if( code.substr(0,1) == "("){                       // Open
            log_lexer(LEXER_LOG_PREFIX + "Found open operator, adding open token");
            tokens.push_back( TOKEN_OPEN );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == ")"){                       // Close
            log_lexer(LEXER_LOG_PREFIX + "Found close operator, adding close token");
            tokens.push_back( TOKEN_CLOSE );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == "<"){                       // Less Than
            log_lexer(LEXER_LOG_PREFIX + "Found less than operator, adding open token");
            tokens.push_back( TOKEN_LESSTHAN );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == ">"){                       // Greater Than
            log_lexer(LEXER_LOG_PREFIX + "Found greater than operator, adding close token");
            tokens.push_back( TOKEN_GREATERTHAN );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == "["){                       // Square Open
            log_lexer(LEXER_LOG_PREFIX + "Found opening square bracket, adding sq open token");
            tokens.push_back( TOKEN_SQOPEN );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == "]"){                       // Square Close
            log_lexer(LEXER_LOG_PREFIX + "Found closing square bracket, adding sq close token");
            tokens.push_back( TOKEN_SQCLOSE );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == "{"){                       // Curly Open
            log_lexer(LEXER_LOG_PREFIX + "Found opening curly bracket, adding curly open token");
            tokens.push_back( TOKEN_CURLYOPEN );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == "}"){                       // Curly Close
            log_lexer(LEXER_LOG_PREFIX + "Found closing curly bracket, adding curly close token");
            tokens.push_back( TOKEN_CURLYCLOSE );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == "="){                       // Assign
            log_lexer(LEXER_LOG_PREFIX + "Found equal sign, adding assign token");
            tokens.push_back( TOKEN_ASSIGN );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == "+"){                       // Add
            log_lexer(LEXER_LOG_PREFIX + "Found addition sign, adding add token");
            tokens.push_back( TOKEN_ADD );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == "-"){                       // Subtract
            log_lexer(LEXER_LOG_PREFIX + "Found subtraction sign, adding subtract token");
            tokens.push_back( TOKEN_SUBTRACT );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == "*"){                       // Multiply
            log_lexer(LEXER_LOG_PREFIX + "Found asterisk, adding multiplication token");
            tokens.push_back( TOKEN_MULTIPLY );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == "/"){                       // Divide
            log_lexer(LEXER_LOG_PREFIX + "Found forward slash, adding division token");
            tokens.push_back( TOKEN_DIVIDE );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == ","){                       // Next
            log_lexer(LEXER_LOG_PREFIX + "Found comma, adding next argument token");
            tokens.push_back( TOKEN_NEXT );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == "!"){                       // Not
            log_lexer(LEXER_LOG_PREFIX + "Found exclamation point, adding not token");
            tokens.push_back( TOKEN_NOT );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == "&"){                       // Address
            log_lexer(LEXER_LOG_PREFIX + "Found `and` symbol, adding address token");
            tokens.push_back( TOKEN_ADDRESS );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == "^"){                       // Pointer
            log_lexer(LEXER_LOG_PREFIX + "Found `exponent` symbol, adding pointer token");
            tokens.push_back( TOKEN_POINTER );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == ":"){                       // Address Member
            log_lexer(LEXER_LOG_PREFIX + "Found colon, adding address member token");
            tokens.push_back( TOKEN_ADDRESSMEMBER );
            code = string_delete_amount(code,1);
        }
        else if( code.substr(0,1) == "#"){                       // Line Comment
            log_lexer(LEXER_LOG_PREFIX + "Found hashtag, ignoring comment");
            code = string_delete_amount(code,1);
            code = string_delete_until(code, "\n");
        }
        else if( string_get_until(code," ") == "on"){            // Method Declaration
            log_lexer(LEXER_LOG_PREFIX + "Found `on` keyword");
            tokens.push_back( TOKEN_KEYWORD("on") );

            code = string_delete_amount(code,2);
            code = string_kill_whitespace(code);

            tokens.push_back( TOKEN_WORD(string_get_until_or(code," (\n")) );
            code = string_delete_until_or(code," (\n");
        }
        else if( string_get_until(code," ") == "import"){        // import
            log_lexer(LEXER_LOG_PREFIX + "Found `import` keyword");
            tokens.push_back( TOKEN_KEYWORD("import") );

            code = string_delete_amount(code,6);
            code = string_kill_whitespace(code);

            if(code.substr(0,1) == "\""){ // String Literal
                log_lexer(LEXER_LOG_PREFIX + "Found string literal, adding string literal token");
                code = string_delete_amount(code,1);
                tokens.push_back( TOKEN_STRING_LITERAL(string_get_until(code,"\"")) );
                code = string_delete_until(code,"\"");
                code = string_delete_amount(code,1);
            }
            else { // Word
                tokens.push_back( TOKEN_WORD(string_get_until_or(code," \n")) );
                code = string_delete_until_or(code," \n");
                code = string_kill_whitespace(code);
            }
        }
        else if( string_get_until(code," ") == "register"){      // register
            log_lexer(LEXER_LOG_PREFIX + "Found `register` keyword");
            tokens.push_back( TOKEN_KEYWORD("register") );

            code = string_delete_amount(code,8);
            code = string_kill_whitespace(code);

            tokens.push_back( TOKEN_WORD(string_get_until(code," ")) );
            code = string_delete_until(code," ");
            code = string_kill_whitespace(code);

            tokens.push_back( TOKEN_WORD(string_get_until(code,"\n")) );
            code = string_delete_until(code,"\n");
            code = string_kill_whitespace(code);
        }
        else if( string_get_until(code," ") == "return"){        // return
            log_lexer(LEXER_LOG_PREFIX + "Found `return` keyword");
            tokens.push_back( TOKEN_KEYWORD("return") );

            code = string_delete_amount(code,6);
            code = string_kill_whitespace(code);
        }
        else if( string_get_until_or(code," (") == "new"){       // new
            log_lexer(LEXER_LOG_PREFIX + "Found `new` keyword");
            tokens.push_back( TOKEN_KEYWORD("new") );

            code = string_delete_amount(code,3);
            code = string_kill_whitespace(code);
        }
        else if( string_get_until_or(code," (") == "create"){    // create
            log_lexer(LEXER_LOG_PREFIX + "Found `create` keyword");
            tokens.push_back( TOKEN_KEYWORD("create") );

            code = string_delete_amount(code,6);
            code = string_kill_whitespace(code);
        }
        else if( string_get_until(code," ") == "delete"){        // delete
            log_lexer(LEXER_LOG_PREFIX + "Found `delete` keyword");
            tokens.push_back( TOKEN_KEYWORD("delete") );

            code = string_delete_amount(code,6);
            code = string_kill_whitespace(code);
        }
        else if( string_get_until(code," ") == "var"){           // var
            log_lexer(LEXER_LOG_PREFIX + "Found `var` keyword");
            tokens.push_back( TOKEN_KEYWORD("var") );

            code = string_delete_amount(code,3);
            code = string_kill_whitespace(code);
        }
        else if( string_get_until_or(code," (") == "if"){        // if
            log_lexer(LEXER_LOG_PREFIX + "Found `if` keyword");
            tokens.push_back( TOKEN_KEYWORD("if") );

            code = string_delete_amount(code,2);
            code = string_kill_whitespace(code);
        }
        else if( string_get_until_or(code," (") == "unless"){    // unless
            log_lexer(LEXER_LOG_PREFIX + "Found `unless` keyword");
            tokens.push_back( TOKEN_KEYWORD("unless") );

            code = string_delete_amount(code,6);
            code = string_kill_whitespace(code);
        }
        else if( string_get_until_or(code," \n") == "else"){     // else
            log_lexer(LEXER_LOG_PREFIX + "Found `else` keyword");
            tokens.push_back( TOKEN_KEYWORD("else") );

            code = string_delete_amount(code,4);
            code = string_kill_whitespace(code);
        }
        else if( string_get_until_or(code," \n") == "continue"){ // continue
            log_lexer(LEXER_LOG_PREFIX + "Found `continue` keyword");
            tokens.push_back( TOKEN_KEYWORD("continue") );

            code = string_delete_amount(code,8);
            code = string_kill_whitespace(code);
        }
        else if( string_get_until_or(code," \n") == "break"){    // break
            log_lexer(LEXER_LOG_PREFIX + "Found `break` keyword");
            tokens.push_back( TOKEN_KEYWORD("break") );

            code = string_delete_amount(code,5);
            code = string_kill_whitespace(code);
        }
        else if( string_get_until_or(code," (") == "switch"){    // switch
            log_lexer(LEXER_LOG_PREFIX + "Found `switch` keyword");
            tokens.push_back( TOKEN_KEYWORD("switch") );

            code = string_delete_amount(code,6);
            code = string_kill_whitespace(code);
        }
        else if( string_get_until_or(code," (") == "while"){     // while
            log_lexer(LEXER_LOG_PREFIX + "Found `while` keyword");
            tokens.push_back( TOKEN_KEYWORD("while") );

            code = string_delete_amount(code,5);
            code = string_kill_whitespace(code);
        }
        else if( string_get_until_or(code," (") == "until"){     // until
            log_lexer(LEXER_LOG_PREFIX + "Found `until` keyword");
            tokens.push_back( TOKEN_KEYWORD("until") );

            code = string_delete_amount(code,5);
            code = string_kill_whitespace(code);
        }
        else if( string_get_until_or(code," (") == "for"){       // for
            log_lexer(LEXER_LOG_PREFIX + "Found `for` keyword");
            tokens.push_back( TOKEN_KEYWORD("for") );

            code = string_delete_amount(code,3);
            code = string_kill_whitespace(code);
        }
        else if( string_get_until(code," ") == "class"){         // class
            log_lexer(LEXER_LOG_PREFIX + "Found `class` keyword");
            tokens.push_back( TOKEN_KEYWORD("class") );

            code = string_delete_amount(code,5);

            code = string_kill_whitespace(code);
        }
        else if( string_get_until(code," ") == "native"){        // native
            log_lexer(LEXER_LOG_PREFIX + "Found `native` keyword");
            tokens.push_back( TOKEN_KEYWORD("native") );

            code = string_delete_amount(code,6);
            code = string_kill_whitespace(code);
        }
        else if( string_get_until(code," ") == "any^"){          // any^
            log_lexer(LEXER_LOG_PREFIX + "Found `any^` keyword");
            tokens.push_back( TOKEN_KEYWORD("any^") );

            code = string_delete_amount(code,4);
            code = string_kill_whitespace(code);
        }
        else if( string_get_until_or(code," ,)\n") == "void"){   // void
            log_lexer(LEXER_LOG_PREFIX + "Found `void` keyword");
            tokens.push_back( TOKEN_KEYWORD("void") );

            code = string_delete_amount(code,4);
            code = string_kill_whitespace(code);
        }
        else if(string_get_until_or(code," ") == "cast"){        // cast
            log_lexer(LEXER_LOG_PREFIX + "Found `cast` keyword");
            tokens.push_back( TOKEN_KEYWORD("cast") );

            code = string_delete_amount(code,4);
            code = string_kill_whitespace(code);

            tokens.push_back( TOKEN_WORD(string_get_until_or(code, " (")) );
            code = string_delete_until_or(code, " (");
            code = string_kill_whitespace(code);
        }
        else if( is_identifier(string_get_until_or(code, " ,()[]\n.+-*/=&:^<>")) ){
            tokens.push_back( TOKEN_WORD(string_get_until_or(code, " ,()[]\n.+-*/=&:^<>")) );
            code = string_delete_until_or(code, " ,()[]\n.+-*/=&:^<>");
        }

        code = string_kill_whitespace(code);
    }

    if(code == prev){
        log_lexer("Encountered unrecognized operator '" + code.substr(0,1) + "'");

        die("Unrecognized operator '" + code.substr(0,1) + "'");
    }

    lexer_log_tokens(tokens);
    return tokens;
}
