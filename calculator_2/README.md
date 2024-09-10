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



