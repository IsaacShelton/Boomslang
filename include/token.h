
#ifndef INCLUDED_TOKEN_H
#define INCLUDED_TOKEN_H

#include <string>
#include <vector>

#define TOKENINDEX_STRING_LITERAL     0
#define TOKENINDEX_NUMERIC_LITERAL    1
#define TOKENINDEX_METHOD_CALL        2 // Obsolete
#define TOKENINDEX_FUNCTION_CALL      3 // Obsolete
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
#define TOKENINDEX_ADDRESSMEMBER     26
#define TOKENINDEX_POINTER           27
#define TOKENINDEX_LESSTHAN          28
#define TOKENINDEX_GREATERTHAN       29

#define TOKEN_STRING_LITERAL(a)  Token{ 0, a}
#define TOKEN_NUMERIC_LITERAL(a) Token{ 1, a}
#define TOKEN_METHOD_CALL(a)     Token{ 2, a} // Obsolete
#define TOKEN_FUNCTION_CALL(a)   Token{ 3, a} // Obsolete
#define TOKEN_ASSIGN             Token{ 4}
#define TOKEN_ADD                Token{ 5}
#define TOKEN_SUBTRACT           Token{ 6}
#define TOKEN_MULTIPLY           Token{ 7}
#define TOKEN_DIVIDE             Token{ 8}
#define TOKEN_NEW(a)             Token{ 9, a}
#define TOKEN_OPEN               Token{10}
#define TOKEN_CLOSE              Token{11}
#define TOKEN_INDENT             Token{12}
#define TOKEN_DEDENT             Token{13}
#define TOKEN_MACRO(a)           Token{14, a}
#define TOKEN_KEYWORD(a)         Token{15, a}
#define TOKEN_WORD(a)            Token{16, a}
#define TOKEN_TERMINATE          Token{17}
#define TOKEN_NEXT               Token{18}
#define TOKEN_SQOPEN             Token{19}
#define TOKEN_SQCLOSE            Token{20}
#define TOKEN_CURLYOPEN          Token{21}
#define TOKEN_CURLYCLOSE         Token{22}
#define TOKEN_NOT                Token{23}
#define TOKEN_ADDRESS            Token{24}
#define TOKEN_MEMBER             Token{25}
#define TOKEN_ADDRESSMEMBER      Token{26}
#define TOKEN_POINTER            Token{27}
#define TOKEN_LESSTHAN           Token{28}
#define TOKEN_GREATERTHAN        Token{29}

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
