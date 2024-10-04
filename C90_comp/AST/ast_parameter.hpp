#ifndef AST_PARAMETER_HPP
#define AST_PARAMETER_HPP

#include "ast_node.hpp"

class Parameter : public Node
{
private:
    std::string identifier_;
    std::string type_;

public:
    Parameter(const std::string &identifier, const std::string &type) : type_(type), identifier_(identifier) {}

    ~Parameter();

    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;


};


#endif