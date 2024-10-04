#include "ast_function_definition.hpp"

void FunctionDefinition::EmitRISC(std::ostream &stream, Context &context) const
{
    // Emit assembler directives.
    // TODO: these are just examples ones, make sure you understand
    // the concept of directives and correct them.
    stream << ".text" << std::endl; // mark the following section as code

    std::string F_name = declarator_->GetIdentifier();
    stream << ".globl f " << F_name << std::endl; // mark the "f" function as a global function.

    // Emit the f^n name
    stream << F_name << "." << context.GetFCount(F_name);

    if (compound_statement_ != nullptr)
    {
        compound_statement_->EmitRISC(stream, context);
    }
}

void FunctionDefinition::Print(std::ostream &stream) const
{
    declaration_specifiers_->Print(stream);
    stream << " ";

    declarator_->Print(stream);

    stream << "(";
    if(parameters_list_ != nullptr)
    {
        parameters_list_->Print(stream);
    }

    stream << ") {" << std::endl;

    if (compound_statement_ != nullptr)
    {
        compound_statement_->Print(stream);
    }
    stream << "}" << std::endl;
}
