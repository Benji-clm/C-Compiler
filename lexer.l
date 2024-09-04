%{
#include <stdio.h>
%}

/* Rules Section */
%%
"/*"[^*]*\*+([^/*][^*]*\*+)*"/"   { /* Ignore multi-line comments */ }
"//".*                             { /* Ignore single-line comments */ }

/* Keywords */
"if"                               { printf("KEYWORD: %s\n", yytext); }
"else"                             { printf("KEYWORD: %s\n", yytext); }
"while"                            { printf("KEYWORD: %s\n", yytext); }
"for"                              { printf("KEYWORD: %s\n", yytext); }
"return"                           { printf("KEYWORD: %s\n", yytext); }

/* Operators */
"=="                               { printf("OPERATOR: %s\n", yytext); }
"!="                               { printf("OPERATOR: %s\n", yytext); }
"<="                               { printf("OPERATOR: %s\n", yytext); }
">="                               { printf("OPERATOR: %s\n", yytext); }
"&&"                               { printf("OPERATOR: %s\n", yytext); }
"||"                               { printf("OPERATOR: %s\n", yytext); }
"+"                                { printf("PLUS: %s\n", yytext); }
"-"                                { printf("MINUS: %s\n", yytext); }
"*"                                { printf("MULTIPLY: %s\n", yytext); }
"/"                                { printf("DIVIDE: %s\n", yytext); }
"="                                { printf("EQUALS: %s\n", yytext); }

/* Punctuation */
"("                                { printf("LPAREN: %s\n", yytext); }
")"                                { printf("RPAREN: %s\n", yytext); }
"{"                                { printf("LBRACE: %s\n", yytext); }
"}"                                { printf("RBRACE: %s\n", yytext); }
";"                                { printf("SEMICOLON: %s\n", yytext); }
","                                { printf("COMMA: %s\n", yytext); }

/* String and Character Literals */
\"([^"\\]|\\.)*\"                  { printf("STRING_LITERAL: %s\n", yytext); }
\'([^'\\]|\\.)*\'                  { printf("CHAR_LITERAL: %s\n", yytext); }

/* Identifiers */
[a-zA-Z_][a-zA-Z0-9_]*             { printf("IDENTIFIER: %s\n", yytext); }

/* Numbers (Integers and Floating-Point) */
[0-9]+\.[0-9]+                     { printf("FLOAT: %s\n", yytext); }
[0-9]+                             { printf("NUMBER: %s\n", yytext); }

/* Whitespace */
[ \t\n]+                           { /* Ignore whitespace and newlines */ }

/* Catch-all rule for unrecognized characters */
.                                  { printf("UNKNOWN: %s\n", yytext); }
%%

int main(int argc, char **argv) {
    yylex();
    return 0;
}
