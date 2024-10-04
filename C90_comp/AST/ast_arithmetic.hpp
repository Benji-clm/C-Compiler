#ifndef AST_ARITHMETIC_HPP
#define AST_ARITHMETIC_HPP

#include "ast_node.hpp"
#include "ast_constant.hpp"

class Operator : public Node
{
private:

    // When we have an operator, it means we have two operands, hence two child nodes to the operator.
    Node *left_;
    Node *right_;
    std::string operator_;

    mutable IntConstant* left_const;
    mutable IntConstant* right_const;

    mutable std::string right_register;
    mutable std::string left_register;

public:

        // operator name is not allowed -> reserved keyword
        Operator(Node *left, const std::string &op , Node *right) : left_(left), operator_(op), right_(right){};

        ~Operator(){
            delete left_;
            delete right_;
        }
        
    void SetConst() const;
    void LoadOperand(std::ostream &stream, Context &context) const;
    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;

};



#endif