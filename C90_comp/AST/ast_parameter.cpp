#include "ast_parameter.hpp"

void Parameter::EmitRISC(std::ostream &stream, Context &context) const {

    // Passing the arguments might seem simple, unless you're dealing with *floating points* (again...).
    // Hence, we need to check the type of the arguments, and adjust accoridingly the logic we use to pass the argument.
    
    int arg_count = context.GetCurrentArgCount();
    VariableLocation loc = context.GetVariableLoc(identifier_);

    if(type_ != "float"){

        if(arg_count >= 8){
            
            if(loc.inRegister){
                stream << "mv " << reg << "," << loc.reg;
            }
            else{
                stream << "mv " << reg << "," << loc.stack_loc;
            }
            

            context.SetCurrentArgCount(arg_count--);
        }

        if(arg_count < 8){
            std::string reg = "a" + std::to_string(arg_count);
            arg_count--;

            if(loc.inRegister){
                stream << "mv " << reg << "," << loc.reg;
            }
            else{
                stream << "mv " << reg << "," << loc.stack_loc;
            }
        }

        else{

        }
    }

}

void Parameter::Print(std::ostream &stream) const {

}