#include "ast_function_definition.hpp"

void FunctionDefinition::EmitRISC(std::ostream &stream, Context &context) const
{
    // Emit assembler directives.
    // TODO: these are just examples ones, make sure you understand
    // the concept of directives and correct them.
    stream << ".text" << std::endl; // mark the following section as code
    stream << ".globl f" << std::endl; // mark the "f" function as a global function.

    // Emit the f^n name
    declarator_->EmitRISC(stream, context);

    // The code below has been moved to *ast_function_call*. As it passes the arguments to the function when calling a function
    // Function definition should have different logic -> TODO

    // if (parameters_list_ != nullptr)
    // {
    //     int size = parameters_list_->GetSize();
    //     context.SetCurrentArgCount(size);

    //     // need to implemente "addi dest, src, imm" style (dest = src + imm)
    //     // For the moment, we assume non float values, meaning 4 bytes per value:

    //     stream << "addi a4,s0," << -4 * size << std::endl;

    //     // NodeList already implements going through the list, and calling emit risc on each node.
    //     parameters_list_->EmitRISCreverse(stream, context);

    //     stream << "call" << declarator_ << "." << context.GetFCount() << std::endl;
    //     context.IncrementFCount();
    // }


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
