# Full C Compiler

Now that we have understood the basics, structure and AST of a compiler using a limited subset, it's time to do the full C-Compiler.

For this I will use two files:

- The ANSI C grammar, Lex Specification.
- The Ansi C Yacc grammar.

These are predefined grammars which should include all of C. However, although everything is defined, the AST builder isn't, hence, the first task will be to write code able to produce the AST required. Then, we will need to write up the compiler which will transform our code into Risc-V 64 bits 12.2.0.

