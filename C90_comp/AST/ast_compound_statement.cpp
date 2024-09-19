#include "ast_compound_statement.hpp"

void CompoundStatement::EmitRISC(std::ostream &stream, Context &context) const
{
    if (declaration_list_ != nullptr)
    {
        declaration_list_->EmitRISC(stream, context);
    }

    if (statement_list_ != nullptr)
    {
        statement_list_->EmitRISC(stream, context);
    }
}

void CompoundStatement::Print(std::ostream &stream) const
{
    // print the opening curly brace
    stream << "{" << std::endl;

    if (declaration_list_ != nullptr)
    {
        declaration_list_->Print(stream);
    }

    if (statement_list_ != nullptr)
    {
        statement_list_->Print(stream);
    }

    stream << "}" << std::endl;
}