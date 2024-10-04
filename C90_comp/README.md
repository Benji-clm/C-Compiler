# C90 Compiler to target language RISC V Assembly

In this repository, is a C90 compiler written mainly in C++.

This compiler is only a subset of the programming language C, so far it involves the basic features, emitting the RISC V assembly code for:

- Files containing multiple functions calling each other
- Functions can take 8 or more parameters
- Local Variables
- Storage of variables using either registers or a stack, depending on availability
- Variables of `int` type
- Basic declarations
- Basic arithmetic expressions

<br>

The features that should be implemented next are:

- Variables of `double`, `float`, `char`, `unsigned`, sturcts and pointer types. Note that `double`, `char` and `unsigned` can easily be implemented by modifying and reusing parts of the code, which has been written to be flexible. Floating points will require more work, althoguh the floating point registers have already been declared, and logic to get and set these registers is present.
- Arrays
