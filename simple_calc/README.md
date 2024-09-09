# Simple Calculator

This folder contains a very simple calculator, this calculator is able to:

- Perform the four operations: +,-,* and /
- Create one letter variables capable of being used to store values and perform operations.
- Using correct, non-ambiguate grammar whilst keeping correct precedence for our operators.
 - Recognise brackets, which will change the precendence of operations.

<br>

## Lexical Analyis

First, we need to create a .l (Flex) file to produce our **lexical analyzer**. For this, the definition remains simple (only needs two headers + declaring a function to handle parsing errors).
We could also include Definition names for our regexes making the rules section simpler, but due to the already simple nature of our calculator, it is not yet necessary.

In the Rules section, we simply define the **Regexes** for variables names (any single lowercase letter), our numbers (any possible integer), our operators (+,-,*, / and ()), and how to deal with whitespaces and errors.

We end with the code section where we add **yywrap()** function, which is called when the lexer reaches the end of the input and tells it that the input is finsihed and that it should stop (we return 1, a return 0 would indiate more inputs to come).

<br>

## Parsing

Then, we need our .y (Yacc) file, which will mainly be here to 'understand' the output of our lexer.

We start with a token section, declaring Integers ad Variables that the lexer will recognise and pass to the parser. Right below it we the declare the **associativity** and **precedence** of the four operators.

We then have a **prologue section**, this is code that is verbatim copied to the parser (verbtaim meaning copied as it is written here, no changes made). It includes the standard headers, declares the yyerror and yylex functions and declares and array of integers _sym_ used as a **symbol table** to store values of variables (we use 26 as we decided to use the 26 letters of the alphabet as our varaibles names, each index corresponding to a letter).

Then we have the **grammar rules**, which is what will be used by Yacc to parse the input, it essentially defines how to deal with the expressions, operators and variables. 

Finally, we write the function for error handling **yyerror()**, and the main function which initiates the parsing process by calling **yyparse()**

<br>

## Run the programs

In the **Cygwin64 Terminal** (for windows, Linux requires a different method), run the following commands:

```
flex calc_lexer.l
// this creates lex.yy.c
yacc -d calc_parser.y
// this creates y.tab.c and y.tab.h
gcc -o calculator y.tab.c lex.yy.c -lfl
// this comiles both our lexer and parser together to create our executable file, calculator.exe
./calculator
// Finally run the calculator.
```
