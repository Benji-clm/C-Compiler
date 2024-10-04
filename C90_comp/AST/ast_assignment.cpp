#include "ast_assignment.hpp"

void Assignment::EmitRISC(std::ostream &stream, Context &context) const {

    VariableLocation loc = context.GetVariableLoc(lhs_);
    
    if(loc.inRegister){
        stream << "sw a5," << loc.reg << std::endl;
    }

    else {
        stream << "sw a5," << loc.stack_loc << std::endl;
    }

    // Clear the registers after storing result
    stream << "li a5,0" << std::endl;
}

void Assignment::Print(std::ostream &stream) const {
    stream << lhs_ << " = ";
    rhs_->Print(stream);
}
