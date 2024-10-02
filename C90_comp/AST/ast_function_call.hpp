#ifndef AST_FUNCTION_CALL_HPP
#define AST_FUNCTION_CALL_HPP

#include "ast_node.hpp"


class FunctionCall : public Node
{
private:
    Node *identifier_;
    NodeList *parameters_list_;

public:

    FunctionCall(Node *identifier, NodeList *parameters_list) : 
    identifier_(identifier), parameters_list_(parameters_list){};

    ~FunctionCall(){
        delete identifier_;
        delete parameters_list_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;

};

#endif