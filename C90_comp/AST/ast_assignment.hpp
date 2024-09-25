#ifndef AST_ASSIGNMENT_HPP
#define AST_ASSIGNMENT_HPP

#include "ast_node.hpp"
#include <string>

class Assignment : public Node {
private:
    std::string lhs_;   // The variable being assigned to
    Node* rhs_;         // The expression being assigned

public:
    Assignment(const std::string& lhs, Node* rhs) : lhs_(lhs), rhs_(rhs) {}

    ~Assignment() {
        delete rhs_;
    }

    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
};

#endif
