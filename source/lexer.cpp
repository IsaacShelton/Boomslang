
/*
    (c) 2016 Isaac Shelton

    This file is part of Boomslang.

    Boomslang is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Boomslang is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Boomslang. If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <stdlib.h>
#include "../include/die.h"
#include "../include/log.h"
#include "../include/token.h"
#include "../include/lexer.h"
#include "../include/errors.h"
#include "../include/management.h"

#define LEXER_LOG_PREFIX (to_string(tokens.size()) + " ")

void process_indentation(TokenList& tokens, std::string& code, size_t& i, unsigned int& indentation){
    unsigned int line_indentaion = 0;

    while( is_indent(code, i) ){
        line_indentaion++;
        if(code.substr(i,4) == "    ") { i += 4; }
        else i += 1;
    }

    while(line_indentaion > indentation){
        indentation++;
        tokens.push_back( TOKEN_INDENT );
    }
    while(line_indentaion < indentation){
        indentation--;
        tokens.push_back( TOKEN_DEDENT );
    }
}

TokenList tokenize(std::string code){
    // Converts code into a stream of tokens

    TokenList tokens;
    unsigned int indentation = 0;
    size_t i = 0;
    size_t prev_i = -1;

    process_indentation(tokens, code, i, indentation);
    if(code == ""){
        tokens.push_back(TOKEN_TERMINATE);
        lexer_log_tokens(tokens);
        return tokens;
    }
    if(code[code.length()-1] != '\n') code += "\n";

    // Cached Data
    char beginning_character;
    std::string until_space;
    std::string until_space_newline;
    std::string until_space_newline_close;
    std::string until_space_newline_close_next;
    std::string until_space_open;

    while(i < code.length() and i != prev_i){
        prev_i = i;

        // Remove Newlines and track indentation
        while(code[i] == '\n'){
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found a newline, adding terminate token");
            tokens.push_back( TOKEN_TERMINATE );
            process_indentation(tokens, code, ++i, indentation);
        }

        // Cache common tests
        beginning_character            = code[i];
        until_space                    = string_itertest_until(code, i, ' ');
        until_space_newline            = string_itertest_until_or(code, i, " \n");
        until_space_newline_close      = string_itertest_until_or(code, i, " \n)");
        until_space_newline_close_next = string_itertest_until_or(code, i, " \n),");
        until_space_open               = string_itertest_until_or(code, i, " (");

        if(beginning_character == '"'){ // String Literal
            std::string content;

            i++;
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found string literal, adding string literal token");
            content = string_iter_until(code, i, '"');
            tokens.push_back( TOKEN_STRING_LITERAL(content) );
            i++;
        }
        else if( (int) beginning_character >= 48 and (int) beginning_character <= 57 ){    // Numeric Literal
            std::string number = string_iter_until_or(code, i,",)].\n iuf");
            char suffix = code[i];

            if(suffix == '.'){
                if( (int)code[i+1] >= 48 and (int)code[i+1] <= 57 ){
                    i++;
                    number += "." + string_iter_until_or(code, i, ",)].\n iuf");
                }
            }

            if(suffix == 'f'){
                i++;
                logging_context.log_lexer(LEXER_LOG_PREFIX + "Found number literal, adding number literal token");
                tokens.push_back( TOKEN_FLOAT_LITERAL(number) );
            }
            else if(suffix == 'd'){
                i++;
                logging_context.log_lexer(LEXER_LOG_PREFIX + "Found number literal, adding number literal token");
                tokens.push_back( TOKEN_NUMBER_LITERAL(number) );
            }
            else if(suffix == 'i'){
                i++;
                logging_context.log_lexer(LEXER_LOG_PREFIX + "Found integer literal, adding integer literal token");
                tokens.push_back( TOKEN_INTEGER_LITERAL(number) );
            }
            else if(suffix == 'u'){
                i++;
                logging_context.log_lexer(LEXER_LOG_PREFIX + "Found unsigned integer literal, adding unsigned integer literal token");
                tokens.push_back( TOKEN_UNSIGNED_LITERAL(number) );
            }
            else {
                logging_context.log_lexer(LEXER_LOG_PREFIX + "Found numeric literal, adding numeric literal token");
                tokens.push_back( TOKEN_NUMERIC_LITERAL(number) );
            }
        }
        else if(beginning_character == '.'){                 // Member
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found member operator, adding member token");
            tokens.push_back( TOKEN_MEMBER );
            i++;
        }
        else if(beginning_character == '('){                 // Open
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found open operator, adding open token");
            tokens.push_back( TOKEN_OPEN );
            i++;
        }
        else if(beginning_character == ')'){                 // Close
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found close operator, adding close token");
            tokens.push_back( TOKEN_CLOSE );
            i++;
        }
        else if(beginning_character == '<'){                 // Less Than
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found less than operator, adding less than token");
            tokens.push_back( TOKEN_LESSTHAN );
            i++;
        }
        else if(beginning_character == '>'){                 // Greater Than
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found greater than operator, adding greater than token");
            tokens.push_back( TOKEN_GREATERTHAN );
            i++;
        }
        else if(beginning_character == '['){                 // Square Open
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found opening square bracket, adding sq open token");
            tokens.push_back( TOKEN_SQOPEN );
            i++;
        }
        else if(beginning_character == ']'){                 // Square Close
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found closing square bracket, adding sq close token");
            tokens.push_back( TOKEN_SQCLOSE );
            i++;
        }
        else if(beginning_character == '{'){                 // Curly Open
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found opening curly bracket, adding curly open token");
            tokens.push_back( TOKEN_CURLYOPEN );
            i++;
        }
        else if(beginning_character == '}'){                 // Curly Close
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found closing curly bracket, adding curly close token");
            tokens.push_back( TOKEN_CURLYCLOSE );
            i++;
        }
        else if(beginning_character == '='){                 // Assign
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found equal sign, adding assign token");
            tokens.push_back( TOKEN_ASSIGN );
            i++;
        }
        else if(beginning_character == '+'){                 // Add
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found addition sign, adding add token");
            tokens.push_back( TOKEN_ADD );
            i++;
        }
        else if(beginning_character == '-'){                 // Subtract
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found subtraction sign, adding subtract token");
            tokens.push_back( TOKEN_SUBTRACT );
            i++;
        }
        else if(beginning_character == '*'){                 // Multiply
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found asterisk, adding multiplication token");
            tokens.push_back( TOKEN_MULTIPLY );
            i++;
        }
        else if(beginning_character == '/'){                 // Divide
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found forward slash, adding division token");
            tokens.push_back( TOKEN_DIVIDE );
            i++;
        }
        else if(beginning_character == ','){                 // Next
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found comma, adding next argument token");
            tokens.push_back( TOKEN_NEXT );
            i++;
        }
        else if(beginning_character == '!'){                 // Not
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found exclamation point, adding not token");
            tokens.push_back( TOKEN_NOT );
            i++;
        }
        else if(beginning_character == '&'){                 // Address
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `and` symbol, adding address token");
            tokens.push_back( TOKEN_ADDRESS );
            i++;
        }
        else if(beginning_character == '^'){                 // Pointer
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `exponent` symbol, adding pointer token");
            tokens.push_back( TOKEN_POINTER );
            i++;
        }
        else if(beginning_character == '@'){                 // Macro
            std::string macro;

            i++;
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `at` symbol, adding macro token");
            macro = string_iter_until_or(code, i, " \n");
            tokens.push_back( TOKEN_MACRO(macro) );
        }
        else if(beginning_character == '#'){                 // Line Comment
            i++;
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found hashtag, ignoring comment");
            while(code[i] != '\n') i++;
        }
        else if(until_space == "def"){                       // Method Declaration
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `def` keyword");
            tokens.push_back( TOKEN_KEYWORD("def") );

            i += 3;
            string_iter_kill_whitespace(code, i);

            std::string name = string_iter_until_or(code, i, " (");
            while(name == "static" or name == "public" or name == "private"){
                logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `" + name + "` keyword");
                tokens.push_back( TOKEN_KEYWORD(name) );
                string_iter_kill_whitespace(code, i);
                name = string_iter_until_or(code, i, " (");
            }

            tokens.push_back( TOKEN_WORD(name) );
        }
        else if(until_space == "import"){                    // import
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `import` keyword");
            tokens.push_back( TOKEN_KEYWORD("import") );

            i += 7;
            string_iter_kill_whitespace(code, i);

            if(code[i] == '"'){ // String Literal
                logging_context.log_lexer(LEXER_LOG_PREFIX + "Found string literal, adding string literal token");
                i++;

                std::string content = string_iter_until(code, i, '"');
                tokens.push_back( TOKEN_STRING_LITERAL(content) );
                i++;
            }
            else { // Word
                std::string content = string_iter_until_or(code, i, " \n");
                tokens.push_back( TOKEN_WORD(content) );
                string_iter_kill_whitespace(code, i);
            }
        }
        else if(until_space == "register"){                  // register
            std::string content;
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `register` keyword");
            tokens.push_back( TOKEN_KEYWORD("register") );
            i += 9;

            string_iter_kill_whitespace(code, i);
            content = string_iter_until(code, i, ' ');
            tokens.push_back( TOKEN_WORD(content) );
            string_iter_kill_whitespace(code, i);

            content = string_iter_until(code, i, '\n');
            tokens.push_back( TOKEN_WORD(content) );
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space_newline == "return"){            // return
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `return` keyword");
            tokens.push_back( TOKEN_KEYWORD("return") );
            i += 6;

            string_iter_kill_whitespace(code, i);
        }
        else if(until_space_open == "new"){                  // new
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `new` keyword");
            tokens.push_back( TOKEN_KEYWORD("new") );
            i += 3;

            string_iter_kill_whitespace(code, i);
        }
        else if(until_space_open == "create"){               // create
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `create` keyword");
            tokens.push_back( TOKEN_KEYWORD("create") );

            i += 6;
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space == "delete"){                    // delete
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `delete` keyword");
            tokens.push_back( TOKEN_KEYWORD("delete") );

            i += 6;
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space == "var"){                       // var
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `var` keyword");
            tokens.push_back( TOKEN_KEYWORD("var") );

            i += 3;
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space_open == "if"){                   // if
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `if` keyword");
            tokens.push_back( TOKEN_KEYWORD("if") );

            i += 2;
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space_open == "unless"){               // unless
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `unless` keyword");
            tokens.push_back( TOKEN_KEYWORD("unless") );

            i += 6;
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space_newline == "else"){              // else
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `else` keyword");
            tokens.push_back( TOKEN_KEYWORD("else") );

            i += 4;
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space_newline == "continue"){          // continue
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `continue` keyword");
            tokens.push_back( TOKEN_KEYWORD("continue") );

            i += 8;
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space_newline == "break"){             // break
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `break` keyword");
            tokens.push_back( TOKEN_KEYWORD("break") );

            i += 5;
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space_open == "switch"){               // switch
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `switch` keyword");
            tokens.push_back( TOKEN_KEYWORD("switch") );

            i += 6;
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space_open == "while"){                // while
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `while` keyword");
            tokens.push_back( TOKEN_KEYWORD("while") );

            i += 5;
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space_open == "until"){                // until
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `until` keyword");
            tokens.push_back( TOKEN_KEYWORD("until") );

            i += 5;
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space_newline == "forever"){           // forever
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `forever` keyword");
            tokens.push_back( TOKEN_KEYWORD("forever") );

            i += 7;
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space == "for"){                       // for
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `for` keyword");
            tokens.push_back( TOKEN_KEYWORD("for") );

            i += 3;
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space == "class"){                     // class
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `class` keyword");
            tokens.push_back( TOKEN_KEYWORD("class") );

            i += 5;
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space == "native"){                    // native
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `native` keyword");
            tokens.push_back( TOKEN_KEYWORD("native") );

            i += 6;
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space == "any^"){                      // any^
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `any^` keyword");
            tokens.push_back( TOKEN_KEYWORD("any^") );

            i += 4;
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space_open == "function^"){            // function^
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `function^` keyword");
            tokens.push_back( TOKEN_KEYWORD("function^") );

            i += 9;
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space_newline_close_next == "void"){   // void
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `void` keyword");
            tokens.push_back( TOKEN_KEYWORD("void") );

            i += 4;
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space_newline_close == "public"){      // public
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `public` keyword");
            tokens.push_back( TOKEN_KEYWORD("public") );

            i += 6;
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space_newline_close == "private"){     // private
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `private` keyword");
            tokens.push_back( TOKEN_KEYWORD("private") );

            i += 7;
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space_newline_close == "static"){      // static
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `static` keyword");
            tokens.push_back( TOKEN_KEYWORD("static") );

            i += 6;
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space == "cast"){                      // cast
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `cast` keyword");
            tokens.push_back( TOKEN_KEYWORD("cast") );

            i += 4;
            string_iter_kill_whitespace(code, i);

            std::string type = string_iter_until_or(code, i, " (");
            tokens.push_back( TOKEN_WORD(type) );
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space_newline == "and"){               // and
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `and` keyword");
            tokens.push_back( TOKEN_KEYWORD("and") );

            i += 3;
            string_iter_kill_whitespace(code, i);
        }
        else if(until_space_newline == "or"){                // or
            logging_context.log_lexer(LEXER_LOG_PREFIX + "Found `or` keyword");
            tokens.push_back( TOKEN_KEYWORD("or") );

            i += 2;
            string_iter_kill_whitespace(code, i);
        }
        else if(is_identifier(string_itertest_until_or(code, i, " ,()[]{}\n.+-*/=&:^<>"))){
            std::string word = string_iter_until_or(code, i, " ,()[]{}\n.+-*/=&:^<>");
            tokens.push_back( TOKEN_WORD(word) );
        }

        string_iter_kill_whitespace(code, i);
    }

    if(i == prev_i){
        logging_context.log_lexer("Encountered unrecognized operator '" + code.substr(i,1) + "'");
        die("Unrecognized operator '" + code.substr(i,1) + "'");
    }

    lexer_log_tokens(tokens);
    return tokens;
}
