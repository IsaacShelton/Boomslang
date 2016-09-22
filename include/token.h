
#ifndef INCLUDED_TOKEN_H
#define INCLUDED_TOKEN_H

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

#include <string>
#include <vector>

#define TOKENINDEX_STRING_LITERAL     0
#define TOKENINDEX_NUMERIC_LITERAL    1
//#define TOKENINDEX_METHOD_CALL      2 // Obsolete
//#define TOKENINDEX_FUNCTION_CALL    3 // Obsolete
#define TOKENINDEX_ASSIGN             4
#define TOKENINDEX_ADD                5
#define TOKENINDEX_SUBTRACT           6
#define TOKENINDEX_MULTIPLY           7
#define TOKENINDEX_DIVIDE             8
#define TOKENINDEX_NEW                9
#define TOKENINDEX_OPEN              10
#define TOKENINDEX_CLOSE             11
#define TOKENINDEX_INDENT            12
#define TOKENINDEX_DEDENT            13
#define TOKENINDEX_MACRO             14
#define TOKENINDEX_KEYWORD           15
#define TOKENINDEX_WORD              16
#define TOKENINDEX_TERMINATE         17
#define TOKENINDEX_NEXT              18
#define TOKENINDEX_SQOPEN            19
#define TOKENINDEX_SQCLOSE           20
#define TOKENINDEX_CURLYOPEN         21
#define TOKENINDEX_CURLYCLOSE        22
#define TOKENINDEX_NOT               23
#define TOKENINDEX_ADDRESS           24
#define TOKENINDEX_MEMBER            25
//#define TOKENINDEX_ADDRESSMEMBER   26 // Obsolete
#define TOKENINDEX_POINTER           27
#define TOKENINDEX_LESSTHAN          28
#define TOKENINDEX_GREATERTHAN       29
#define TOKENINDEX_POINTERMEMBER     30 // Generated
#define TOKENINDEX_HEADERFILE        31 // Generated
#define TOKENINDEX_LIBRARYFILE       32 // Generated
#define TOKENINDEX_SETFILENAME       33 // Generated
#define TOKENINDEX_SETLINENUMBER     34 // Generated
#define TOKENINDEX_DOUBLE_LITERAL    35
#define TOKENINDEX_INTEGER_LITERAL   36
#define TOKENINDEX_UNSIGNED_LITERAL  37
#define TOKENINDEX_RAW_WORD          38 // Generated
#define TOKENINDEX_FLOAT_LITERAL     39
#define TOKENINDEX_SCOPE_MEMBER      40 // Generated

#define TOKEN_STRING_LITERAL(a)   Token{ 0, a}
#define TOKEN_NUMERIC_LITERAL(a)  Token{ 1, a}
//#define TOKEN_METHOD_CALL(a)    Token{ 2, a} // Obsolete
//#define TOKEN_FUNCTION_CALL(a)  Token{ 3, a} // Obsolete
#define TOKEN_ASSIGN              Token{ 4}
#define TOKEN_ADD                 Token{ 5}
#define TOKEN_SUBTRACT            Token{ 6}
#define TOKEN_MULTIPLY            Token{ 7}
#define TOKEN_DIVIDE              Token{ 8}
#define TOKEN_NEW(a)              Token{ 9, a}
#define TOKEN_OPEN                Token{10}
#define TOKEN_CLOSE               Token{11}
#define TOKEN_INDENT              Token{12}
#define TOKEN_DEDENT              Token{13}
#define TOKEN_MACRO(a)            Token{14, a}
#define TOKEN_KEYWORD(a)          Token{15, a}
#define TOKEN_WORD(a)             Token{16, a}
#define TOKEN_TERMINATE           Token{17}
#define TOKEN_NEXT                Token{18}
#define TOKEN_SQOPEN              Token{19}
#define TOKEN_SQCLOSE             Token{20}
#define TOKEN_CURLYOPEN           Token{21}
#define TOKEN_CURLYCLOSE          Token{22}
#define TOKEN_NOT                 Token{23}
#define TOKEN_ADDRESS             Token{24}
#define TOKEN_MEMBER              Token{25}
//#define TOKEN_ADDRESSMEMBER     Token{26} // Obsolete
#define TOKEN_POINTER             Token{27}
#define TOKEN_LESSTHAN            Token{28}
#define TOKEN_GREATERTHAN         Token{29}
#define TOKEN_POINTERMEMBER       Token{30}   // Generated
#define TOKEN_HEADERFILE(a)       Token{31,a} // Generated
#define TOKEN_LIBRARYFILE(a)      Token{32,a} // Generated
#define TOKEN_SETFILENAME(a)      Token{33,a} // Generated
#define TOKEN_SETLINENUMBER(a)    Token{34,a} // Generated
#define TOKEN_NUMBER_LITERAL(a)   Token{35,a}
#define TOKEN_INTEGER_LITERAL(a)  Token{36,a}
#define TOKEN_UNSIGNED_LITERAL(a) Token{37,a}
#define TOKEN_RAW_WORD(a)         Token{38,a} // Generated
#define TOKEN_FLOAT_LITERAL(a)    Token{39,a}
#define TOKEN_SCOPE_MEMBER        Token{40}   // Generated

struct Token {
    unsigned int id;
    std::string data;
};

typedef std::vector<Token> TokenList;

struct TokenContext {
    TokenList& tokens;
    unsigned int& index;
};

std::string token_name(Token token);
std::string token_operator(Token token);
void token_print(Token token);
void lexer_log_tokens(TokenList token);

bool advance_index(unsigned int& index, unsigned int length);
bool retreat_index(unsigned int& index);
void index_increase(TokenContext context);
void index_decrease(TokenContext context);
void token_force(TokenContext context, unsigned int token_id, std::string cant_advance, std::string fail);

#endif // INCLUDED_TOKEN_H
