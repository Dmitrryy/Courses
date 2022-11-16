%option c++
/* TODO */
%option noyywrap nounput noinput batch debug
%option yylineno
%option yyclass="Scanner"

%{

#include "compiler.cpp.h"
#include "scanner.h"
#include <iostream>

#define YY_USER_ACTION updateLocation();

%}

WS              [ \r\t\v]+
ENDL            [\n]+
UNOP            [\-\+]
DOT             [\.]

DIGIT           [0-9]
DIGIT_NON_ZERO  [1-9]
DOUBLE          {DIGIT}+{DOT}?{DIGIT}*
INTEGER         {DIGIT_NON_ZERO}{DIGIT}*
LETTER          [a-zA-Z_]
LETTER_OR_DIGIT [a-zA-Z0-9_]

IDENTIFIER      {LETTER}{LETTER_OR_DIGIT}*
CONST_STRING    [\"].*[\"]

COMMENT1        "//".*

%%
                            /* Сопоставляем каждой лексеме её константу для парсера */
{WS}                        { /* DO NOTHING */  }
{ENDL}                      { /* DO NOTHING */  }
{COMMENT1}                  { /* DO NOTHING */  }

"class"                     return yy::parser::token_type::KW_CLASS;
"this"                      return yy::parser::token_type::KW_THIS;
"while"                     return yy::parser::token_type::KW_WHILE;
"if"                        return yy::parser::token_type::KW_IF;
"return"                    return yy::parser::token_type::KW_RETURN;
"method"                    return yy::parser::token_type::KW_METHOD;

"="                         return yy::parser::token_type::ASSIGN;
"+"                         return yy::parser::token_type::PLUS;
"-"                         return yy::parser::token_type::MINUS;
"*"                         return yy::parser::token_type::MUL;
"/"                         return yy::parser::token_type::DIV;

">"                         return yy::parser::token_type::GREATER;
"<"                         return yy::parser::token_type::LESS;
"<="                         return yy::parser::token_type::LLESS;
">="                         return yy::parser::token_type::LGREATER;
"=="                        return yy::parser::token_type::EQUAL;
"!="                        return yy::parser::token_type::NONEQUAL;

";"                         return yy::parser::token_type::SCOLON;
":"                         return yy::parser::token_type::COLON;
","                         return yy::parser::token_type::COMMA;
"("                         return yy::parser::token_type::LPARENTHESES;
")"                         return yy::parser::token_type::RPARENTHESES;
"{"                         return yy::parser::token_type::LBRACE;
"}"                         return yy::parser::token_type::RBRACE;

{INTEGER}                   return yy::parser::token_type::INTEGER_LITERAL;
{DIGIT}                     return yy::parser::token_type::INTEGER_LITERAL;
{IDENTIFIER}                return yy::parser::token_type::IDENTIFIER;
{DOT}                       return yy::parser::token_type::DOT;
.			                return yy::parser::token_type::ERROR;

%%