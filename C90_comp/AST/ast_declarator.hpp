#ifndef AST_DECLARATOR_HPP
#define AST_DECLARATOR_HPP

#include "ast_node.hpp"

class Declarator : public Node
{
private:
    std::string declarator_;

public:

    Declarator(std::string name) : declarator_(name){};

    ~Declarator(){};

    std::string GetName() const;

    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
    
    
};

#endif