#include "ast_parameter.hpp"

void Parameter::EmitRISC(std::ostream &stream, Context &context) const {

    // Passing the arguments might seem simple, unless you're dealing with *floating points* (again...).
    // Hence, we need to check the type of the arguments, and adjust accoridingly the logic we use to pass the argument.
    
    int arg_count = context.GetCurrentArgCount();
    VariableInfo info = context.GetVariableInfo(identifier_);

    if(type_ == "int"){

        if(arg_count >= 8){
            
            if(info.location.inRegister){
                stream << "li a5," << info.value_.int_value << std::endl;
                stream << "sw a5," << 4 * (arg_count - 8) << "(sp)" << std::endl;
            }
            else{
                stream << "lw a5," << info.location.stack_loc << "(s0)" << std::endl;
                stream << "sw a5," << 4 * (arg_count - 8) << "(sp)" << std::endl;
            }
            
            if(arg_count == 8){
                stream << "mv t2,a4" << std::endl;
            }

            context.SetCurrentArgCount(--arg_count);
        }

        if(arg_count < 8){
            std::string reg = "a" + std::to_string(arg_count);

            if(info.location.inRegister){
                stream << "li " << reg << "," << info.value_.int_value << std::endl;
            }
            else{
                stream << "lw " << reg << "," << info.location.stack_loc << "(s0" << std::endl;
            }

            context.SetCurrentArgCount(--arg_count);
        }

        else{

        }
    }

}

void Parameter::Print(std::ostream &stream) const {

}