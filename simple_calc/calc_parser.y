%token INTEGER VARIABLE
%left '+' '-'
%left '*' '/'

%{
  #include <stdio.h>
  #include <stdlib.h>
  void yyerror(char *);
  int yylex(void);
  int sym[26];
%}

%%

program:
    program statement '\n'
    |
    ;

statement:
    expr                    { printf("%d\n", $1); }
    | VARIABLE '=' expr     { sym[$1] = $3; }
    ;

expr:
    INTEGER
    | VARIABLE          { $$ = sym[$1]; }
    | expr '+' expr     { $$ = $1 + $3; }
    | expr '-' expr     { $$ = $1 - $3; }
    | expr '*' expr     { $$ = $1 * $3; }
    | expr '/' expr     { $$ = $1 / $3; }
    | '(' expr ')'      { $$ = $2; }
    ;

%%
void yyerror(char *s) {
  fprintf(stderr, "%s\n", s);
  return 0;
}

int main(void) {
  return yyparse();
  return 0;
}
