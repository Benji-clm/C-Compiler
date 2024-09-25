#include "ast_declarator.hpp"

std::string Declarator::GetName() const{
    return declarator_;
}

void Declarator::EmitRISC(std::ostream &stream, Context &context) const{

    context.AddVariable(declarator_);

    // EmitRISC, From here, we have all three, the name of the variable, its type and its location. This means that
    // we are ready to output the RISC V Assembly code, simply implement so. 

    VariableLocation loc = context.GetVariableLoc(declarator_);

    // We have only declared a variable, but we haven't actually given it any value,
    // therefore no need emit RISC-V code, we simply resever the register/stack space in the context.
}