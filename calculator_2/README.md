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

With ouptut of the **interpreter** being:

```
0
1
2
```

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
