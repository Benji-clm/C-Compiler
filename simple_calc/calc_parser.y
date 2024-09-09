%token INTEGER VARIABLE
%left '+' '-'
%left '*' '/'
%{
  void yyerror(char *);
  int yylex(void);
  int sym[26];  // Symbol table for variables
%}

%%
program:
    program statement '\n'
  |
  ;

statement:
    expr { printf("%d\n", $1); }
  | VARIABLE '=' expr { sym[$1] = $3; }
  ;

expr:
    INTEGER
  | VARIABLE { $$ = sym[$1]; }
  | expr '+' expr { $$ = $1 + $3; }
  | expr '-' expr { $$ = $1 - $3; }
  | expr '*' expr { $$ = $1 * $3; }
  | expr '/' expr { $$ = $1 / $3; }
  | '(' expr ')' { $$ = $2; }
  ;

%%
void yyerror(char *s) {
  fprintf(stderr, "%s\n", s);
}

int main(void) {
  return yyparse();
}
