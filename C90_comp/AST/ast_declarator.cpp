#include "ast_declarator.hpp"
#include <stdexcept>

std::string Declarator::GetName() const{
    return declarator_;
}

void Declarator::EmitRISC(std::ostream &stream, Context &context) const{

    // EmitRISC, From here, we have all three, the name of the variable, its type and its location. This means that
    // we are ready to output the RISC V Assembly code, simply implement so. 

    VariableLocation loc = context.GetVariableLoc(declarator_);

    if(context.IsInitialized(declarator_) && context.IsVariableDeclared(declarator_)){
        const VariableInfo& var_info = context.GetVariableInfo(declarator_);
        const VariableValue& var_value = context.GetVariableValue(declarator_);

        // Now we check what type of values was initalized for the specific of the RISC code.
        // If the variable is in a register, we simply output that we loaded it in the register.
        // If its in the stack, we use our temporary register $t0 to load it and store it in the stack.

        if(var_info.type == "int"){
            if(loc.inRegister){
                stream << "li" << loc.reg << ", " << var_value.int_value << std::endl;
            }
            else{
                stream << "li t0, " << var_value.int_value << std::endl;
                stream << "sw t0, " << loc.stack_loc << "(s0)" << std::endl;
            }   
        }

        if(var_info.type == "float"){
            std::string float_reg = context.GetFreeFloatReg();
            if(!float_reg.empty() && loc.inRegister){
                stream << "lui" << loc.reg << ",%hi(.LC0)" << std::endl;
                stream << "flw" << float_reg << ",%lo(.LC0)" << std::endl;
            }
            // TODO: IMPLEMENT STACK STRUCTURE FOR FLOATING POINTS?
            else{
               throw std::runtime_error("No Registers Left for Floating Points. " + declarator_); 
            }
        }
    }
}