#include "ast_function_call.hpp"

void FunctionCall::EmitRISC(std::ostream &stream, Context &context) const
{
    if (parameters_list_ != nullptr)
    {
        int size = parameters_list_->GetSize();
        context.SetCurrentArgCount(size);

        // need to implemente "addi dest, src, imm" style (dest = src + imm)
        // For the moment, we assume non float values, meaning 4 bytes per value:

        stream << "addi a4,s0," << -4 * size << std::endl;

        // NodeList already implements going through the list, and calling emit risc on each node.
        parameters_list_->EmitRISCreverse(stream, context);

        stream << "call" << identifier_ << "." << context.GetFCount() << std::endl;
        context.IncrementFCount();
    }
}