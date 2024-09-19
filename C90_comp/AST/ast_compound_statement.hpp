#ifndef AST_COMPOUND_STATEMENT_HPP
#define AST_COMPOUND_STATEMENT_HPP

#include "ast_node.hpp"

class CompoundStatement : public Node
{
    private:
        NodeList *declaration_list_;
        NodeList *statement_list_;

    public:

    // constructor
    CompoundStatement (NodeList *declaration_list, NodeList *statement_list) :
        declaration_list_(declaration_list), statement_list_(statement_list) {}

    ~CompoundStatement()
    {
        delete declaration_list_;
        delete statement_list_;
    }

    void EmitRISC(std::ostream &stream, Context &context) const override;
  
    void Print(std::ostream &stream) const override;
};


#endif