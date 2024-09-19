#ifndef AST_FUNCTION_DEFINITION_HPP
#define AST_FUNCTION_DEFINITION_HPP

#include "ast_node.hpp"

class FunctionDefinition : public Node
{
  
private:

    // note that:
    // declaration_specifiers_ correspond to, for example "int", in a function "int f(){...}", its the *return type* of the function
    // declarator_ is the name of the function, for example "f()", it also includes its parameter
    // TODO: IMPLEMENT PARAMETERS OF FUNCTIONS
    // compound_statement_ includes *both* the curly braces, and the statement within. Note that this "statement within"
    // can either be a statement list, a delcaration list, or both.
  
    Node *declaration_specifiers_;
    Node *declarator_;
    Node *compound_statement_;

public:
  
    // constructor
    FunctionDefinition
    (Node *declaration_specifiers,
     Node *declarator,
     Node *compound_statement) :
      declaration_specifiers_(declaration_specifiers),
      declarator_(declarator),
      compound_statement_(compound_statement){};
  
    ~FunctionDefinition()
    {
        delete declaration_specifiers_;
        delete declarator_;
        delete compound_statement_;
    };
  
    void EmitRISC(std::ostream &stream, Context &context) const override;
  
    void Print(std::ostream &stream) const override;
};

#endif
