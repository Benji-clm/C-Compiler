%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "calc2.h"

// We start by declaring functions that return pointers of type nodeType. 
// Each of the three function allocates and initializes a node of type nodeType and returns a pointer to that node.
// We later define these functions, but declare them now so that they can be used within the grammar rules.
/* prototypes */
nodeType *opr(int oper, int nops, ...);
nodeType *id(int i);
nodeType *con(int value);
void freeNode(nodeType *p);
int ex(nodeType *p);
int yylex(void);

void yyerror(char *s);
int sym[26]; /* symbol table */
%}

%union {
 int iValue; /* integer value */
 char sIndex; /* symbol table index */
 nodeType *nPtr; /* node pointer */
};

%token <iValue> INTEGER
%token <sIndex> VARIABLE
%token WHILE IF PRINT
%nonassoc IFX
%nonassoc ELSE

%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%type <nPtr> stmt expr stmt_list

%%

program:
    function        { exit(0); }
    ;

function:
        function stmt       { ex($2); freeNode($2); }
    |   /* NULL */
    ;
    
stmt:
    ';'                                 { $$ = opr(';', 2, NULL, NULL); }
    | expr ';'                          { $$ = $1; }
    | PRINT expr ';'                    { $$ = opr(PRINT, 1, $2); }
    | VARIABLE '=' expr ';'             { $$ = opr('=', 2, id($1), $3); }
    | WHILE '(' expr ')' stmt           { $$ = opr(WHILE, 2, $3, $5); }
    | IF '(' expr ')' stmt %prec IFX    { $$ = opr(IF, 2, $3, $5); }
    | IF '(' expr ')' stmt ELSE stmt    { $$ = opr(IF, 3, $3, $5, $7); }
    | '{' stmt_list '}'                 { $$ = $2; }
    ;

stmt_list:
        stmt                { $$ = $1; }
    |   stmt_list stmt      { $$ = opr(';', 2, $1, $2); }
    ;

expr:
    INTEGER                 { $$ = con($1); }
    | VARIABLE              { $$ = id($1); }
    | '-' expr %prec UMINUS { $$ = opr(UMINUS, 1, $2); }
    | expr '+' expr         { $$ = opr('+', 2, $1, $3); }
    | expr '-' expr         { $$ = opr('-', 2, $1, $3); }
    | expr '*' expr         { $$ = opr('*', 2, $1, $3); }
    | expr '/' expr         { $$ = opr('/', 2, $1, $3); }
    | expr '<' expr         { $$ = opr('<', 2, $1, $3); }
    | expr '>' expr         { $$ = opr('>', 2, $1, $3); }
    | expr GE expr          { $$ = opr(GE, 2, $1, $3); }
    | expr LE expr          { $$ = opr(LE, 2, $1, $3); }
    | expr NE expr          { $$ = opr(NE, 2, $1, $3); }
    | expr EQ expr          { $$ = opr(EQ, 2, $1, $3); }
    | '(' expr ')'          { $$ = $2; }
    ;

%%

#define SIZEOF_NODETYPE ((char *)&p->con - (char *)p)

// For a constant value we declare a pointer of type nodeType, then if we are able to allocate memory for the node, we give the pointer to the memory block to p.
// We then copy the information (here the type of node and value of the constant) to p, and we return it.
nodeType *con(int value) {
    nodeType *p;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeCon;
    p->con.value = value;

    return p;
}

nodeType *id(int i) {
    nodeType *p;
    
    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeId;
    p->id.i = i;

    return p;
}

/*
For opr, it is more particular, we make the use of variable arguments, as although we know the operator and the number of operands from the grammar rules,
there can be a varying number of arguments, as different operators might need less or more operators.
-> variable funcions are f^n that can take in a variable (changing) number of arguments. the function opr itself is variable (indicated by "...").
Here is how variable functions work:
 - va_list ap; holds the state of the variable argument list -> it creates a list "ap", which will contain the variable arguments passed to the function.
 - va_start(ap, nops); is used to determine where variable arguments starts (here, they start after argument "int nops").
 - va_arg(ap, nodeType*); ap points to the next argument, nodeType* is the type of the next argument.
  -> The loop retrieves each variable arguments. The pointer p retrieves the different operands passed to the function and stores the pointer to them in an array p->opr.op[i]
 - va_end(ap); simply cleans up the memory used by va_list ap;
*/
nodeType *opr(int oper, int nops, ...) {
    va_list ap;
    nodeType *p;
    int i;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");
    if ((p->opr.op = malloc(nops * sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeOpr;
    p->opr.oper = oper;
    p->opr.nops = nops;
    va_start(ap, nops);
    for (i = 0; i < nops; i++)
        p->opr.op[i] = va_arg(ap, nodeType*);
    va_end(ap);
    return p;
}

// This is the "destructor", which simply cleans up / deallocate the memory once we are done with the AST. It does so recursively,
// for each operator, it goes through all its operands, and calls itself on it then, where if not an operator, deletes the operand, and if an operator, deletes this operators
// operands before deleting itself.
void freeNode(nodeType *p) {
    int i;

    if (!p) return;
    if (p->type == typeOpr) {
        for (i = 0; i < p->opr.nops; i++)
            freeNode(p->opr.op[i]);
        free(p->opr.op);
    }
    free (p);
}

// In case of faulty inputs, error function.
void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}

int main(void) {
    yyparse();
    return 0;
}
