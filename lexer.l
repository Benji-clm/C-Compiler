%{
#include <stdio.h>
%}

/* Rules Section */
%%
[a-zA-Z_][a-zA-Z0-9_]*    { printf("IDENTIFIER: %s\n", yytext); }
[0-9]+                     { printf("NUMBER: %s\n", yytext); }
"="                        { printf("EQUALS: %s\n", yytext); }
"+"                        { printf("PLUS: %s\n", yytext); }
";"                        { printf("SEMICOLON: %s\n", yytext); }
[ \t\n]+                   { /* Ignore whitespace and newlines */ }
.                          { printf("UNKNOWN: %s\n", yytext); }
%%

int main(int argc, char **argv) {
    yylex();
    return 0;
}
