// enum is a user-defined data type, we give it names to represent the three different values of enum, namely our three node types, we also assign an alias "nodeEnum" to enum
typedef enum
{
    typeCon,
    typeId,
    typeOpr
} nodeEnum;

// We proceed to define structure names for each of the three node types
    /* constants */
typedef struct
{
    int value; /* value of constant */
} conNodeType;

    /* identifiers */
typedef struct
{
    int i; /* subscript to sym array */
} idNodeType;

// The operator nodetype is special, in that it hols:
// - the operator as an integer code
// - the number of *operands* the operator holds (i.e. a + b -> + operator takes two operands)
// - An array of pointers to the operands.
    /* operators */
typedef struct
{
    int oper;                /* operator */
    int nops;                /* number of operands */
    struct nodeTypeTag **op; /* operands */
} oprNodeType;

// Finally, and most importantly, we define the primary structure (the core data type), to represent any of the three node types in our AST.
// We declare a nodeEnum, with identifier "type", and this can take the value of only one of the three NodeType. We also give this structure the name "nodeType"
typedef struct nodeTypeTag
{
    nodeEnum type; /* type of node */
    union
    {
        conNodeType con; /* constants */
        idNodeType id;   /* identifiers */
        oprNodeType opr; /* operators */
    };
} nodeType;

// This is the symbol table which holds the values for the 26, a-z, possible variables.
extern int sym[26];
