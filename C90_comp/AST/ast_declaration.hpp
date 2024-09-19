/*

CANCEL THESE FILES -> NodeList already implements correctly all these logics and is flexible enough to be reused

*/



#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "ast_node.hpp"
#include <vector>

class Declaration
{
    protected:
        std::vector<Node *> branches_;
};


class DeclarationList : public Node
{

    private:

    // contains the list of declarations
    std::vector<Node*> declarations_;

    public:

    DeclarationList (Node* first_declaration) : declarations_({first_declaration}) {} 

    ~DeclarationList()
    {
        for (auto decl : declarations_)
        {
            delete decl;
        }
    }

    void PushBackDecl(Node *item);
    virtual void EmitRISC(std::ostream &stream, Context &context) const override;
    virtual void Print(std::ostream &stream) const override;
};


#endif