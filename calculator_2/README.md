# Calculator

This folder contains a significantly more complex calculator than the previous simple_calc. Major changes will include control constructs such as **if-else** and **while** statements. In addition, I will start working on constructing a syntax tree during parsing. After parsing, we walk the syntax tree to produce the ouput. This calculator will have three versions:
- An interpreter that executes statements during the tree walk.
- A compiler that generates code for a hypothetical stack-based machine.
- A version that generates a synatx tree of the original program.

This calculator, for example, should be able to perform the program:

```
x = 0;
while (x < 3) {
  print x;
  x = x + 1;
}
```

<br>

With ouptut of the **interpreter** being:

```
0
1
2
```

<br>

The **compiler**:

```
        push    0
        pop     x
L000:
        push    x
        push    3
        compLT
        jz      L001
        push    x
        print
        push    x
        push    1
        add
        pop     x
        jmp     L000
L001:
```

<br>

The **syntax tree** (graph):

```
Graph 0:

    [=]
     |
   |----|
   |    |
 id(X) c(0)

Graph 1:

               while
                 |
     |----------------|
     |                |
    [<]              [;]
     |                |
   |----|     |----------|
   |    |     |          |
 id(X) c(3) print       [=]
              |          |
              |     |-------|
              |     |       |
            id(X) id(X)    [+]
                            |
                          |----|
                          |    |
                        id(X) c(1)

```

<br>

## How to Run it

To Run this calculator, it works similarly as for the previous one, start by creater the lexical analyser and the parser, (files lex.yy.c, y.tab.c and y.tab.h), then depending on which output you want, compile with the wanted version of the calculator.

```
flex lexer.l
yacc -d parser.y
gcc lex.yy.c y.tab.c <version>.c -o calc // replace <version> with either interpreter, compiler or graph.
./calc
```

<br>

## Include File

We start by creating **calc2.h**, to provide the correct headers and data structures that will be used in several other files. It is an essential part of to manage the parse tree and symbols in the program.
In this file we define:
- The **types** of nodes of our tree (constant values, identifier (variables) and operators.
- The **structures** for each node type
- A **nodeTypeTag structure**, in which we use `union`, allowing us to use a single memory cell location to store any of the three data types we have (only one type at a time however).
- A **symbol table** (similarly as for the simpler calculator).

<br>

## Lexer Analysis

For this section, it once again follows a very similar pattern as the previous calculator, the same Regex is used for the variable names and the digits. We however add a great deal of operation, for single character operation, we can simply copy verbatim the character itself. For operators using 2 or more characters, we require special handling, as Flex/Lex works by matching the _longest possible input_, hence we need explicit rules to ensure that they are being matched as a single token. These specific rules return distinct tokens for these operations:

```
">="        return GE;
"<="        return LE;
"=="        return EQ;
"!="        return NE;
"while"     return WHILE;
"if"        return IF;
"else"      return ELSE;
"print"     return PRINT;
```

<br>

## Parser

In the `parser.y` file, we start by including the headers, the symbol table and the the prototypes definitions in the prologue section. Then, in the definitions section we start with the union, as mentionned earlier, it defines the types of semantic values that tokens and nonterminals can have. We also declare all our tokens, and write the opearator precedence and associativity declations:

```
%token <iValue> INTEGER
%token <sIndex> VARIABLE
%token WHILE IF PRINT
%nonassoc IFX
%nonassoc ELSE

%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS
```

Note how we use `%nonassoc UMINUS`, (unary minus, which negates the values of the operand); this has the highest precedence, and we later write in the rules that it must precede an `expr`.  We also note how operands have higher precedence than If, else ... statements, this allows code such as `while(x < y + 3)` to work:

1. The lexer converts the input `y+3` into the token stream `VARIABLE + INTEGER`
2. The parser matches the tokens to the grammar rules, and constructrs the AST by creating three nodes: identifier - `y`, constant - `3`, operator - `+` which links `y` and `3` as its operands
3. Once the AST is constructed, the `ex()` function is used to **execute** and **evalutate** the AST, which traverses the tree and performs the appropriate actions recursively based on the nodes.

We then proceed to write the **Grammar Rules and Actions**, this contains the rule explaining of the action code builds an AST nodes representing statements and expressions.

Finally we write the **Post-Grammar Code**, which are the functions defining how to create the different types of our AST's nodes.

/quote test



