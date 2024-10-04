#include "ast_arithmetic.hpp"
#include "ast_identifier.hpp"
#include "ast_constant.hpp"

void Operator::SetConst() const
{
    left_const = dynamic_cast<IntConstant*>(left_);
    right_const = dynamic_cast<IntConstant*>(right_);
}

void Operator::LoadOperand(std::ostream &stream, Context &context) const
{

        left_register = context.GetArgReg();
        right_register = context.GetArgReg();

        SetConst();

        if(!left_const) {
            std::string left_name_ = left_->GetIdentifier();

            if (context.IsVariableDeclared(left_name_)) {
                VariableLocation left_loc = context.GetVariableLoc(left_name_);

                if(left_loc.inRegister){
                    stream << "mv " << left_register << "," << left_loc.reg << std::endl;
                }

                else{
                    stream << "lw " << left_register << "," << left_loc.stack_loc << "(s0)" << std::endl;
                }
                
            } 
            
            else {
                throw std::runtime_error("Variable not declared: " + left_name_);
            }
        }


        if(!right_const) {
            std::string right_name_ = right_->GetIdentifier();

            if (context.IsVariableDeclared(right_name_)) {
                VariableLocation right_loc = context.GetVariableLoc(right_name_);

                if(right_loc.inRegister){
                    stream << "mv " << right_register << "," << right_loc.reg << std::endl;
                }

                else{
                    stream << "lw " << right_register << "," << right_loc.stack_loc << "(s0)" << std::endl;
                }
                
            } 
            
            else {
                throw std::runtime_error("Variable not declared: " + right_name_);
            }
        }

}


void Operator::EmitRISC(std::ostream &stream, Context &context) const
{

    if(operator_ == "+"){

        LoadOperand(stream, context);
        
        if(left_const && !right_const){
            stream << "addi a5," << right_register << left_const->GetValue() << std::endl;
        }

        if(!left_const && right_const){
            stream << "addi a5," << left_register << right_const->GetValue() << std::endl;
        }

        if(!left_const && !right_const){
            stream << "add a5," << left_register << "," << right_register << std::endl;
        }

        if(left_const && right_const){
            int result = left_const->GetValue() + right_const->GetValue();
            stream << "li a5," << result << std::endl;
        }

        
    }


    // Substraction


    else if(operator_ == "-"){

        LoadOperand(stream, context);
        
        if(left_const && !right_const){
            stream << "li " << left_register << "," << left_const->GetValue() << std::endl;
            stream << "sub a5," << left_register << "," << right_register << std::endl;
        }

        if(!left_const && right_const){
            stream << "add a5," << left_register << ",-" << left_const->GetValue() << std::endl; 
        }

        if(!left_const && !right_const){
            stream << "sub a5," << left_register << "," << right_register << std::endl;
        }

        if(left_const && right_const){
            int result = left_const->GetValue() - right_const->GetValue();
             stream << "li a5," << result << std::endl;
        }
    }

    else if(operator_ == "*"){


        LoadOperand(stream, context);

        // If either one of the values are constants (not variables), we can use left shifts on the other variable for more eff.


        if(left_const && !right_const){
            int mult = left_const->GetValue() / 2;
            int rem = left_const->GetValue() % 2;

            if(rem == 0){
                stream << "slli" << "a5," << right_register << "," << mult << std::endl;
            }
            else{
                stream << "slli a5," << right_register << "," << mult << std::endl;
                stream << "add a5,a5," << right_register << std::endl;
            }

        }

        if(!left_const && right_const){
            int mult = right_const->GetValue() / 2;
            int rem = right_const->GetValue() % 2;

            if(rem == 0){
                stream << "slli" << "a5," << left_register << "," << mult << std::endl;
            }
            else{
                stream << "slli a5," << left_register << "," << mult << std::endl;
                stream << "add a5,a5," << left_register << std::endl;
            }
        }

        if(!left_const && !right_const){
            stream << "mul a5," << left_register << "," << right_register << std::endl;
        }

        if(left_const && right_const){
            int result = left_const->GetValue() * right_const->GetValue();
            stream << "li a5," << result << std::endl;
        }
    }


    else if(operator_ == "/"){

        
        LoadOperand(stream, context);

        if(left_const && !right_const){
            stream << "li " << left_register << "," << left_const->GetValue() << std::endl;
            stream << "div a5," << left_register << "," << right_register << std::endl;
        }

        if(!left_const && right_const){

            int divi = right_const->GetValue();

            if(divi == 2){

                // We isolate the sign
                stream << "srli a4," << left_register << ",31" << std::endl;

                // Add the bias (Two's Complement!)
                stream << "add " << left_register << "," << left_register << ",a4" << std::endl;

                // Right shift by 1 (division by two)
                stream << "srai a5," << left_register << ",1" << std::endl;
            } else {
                stream << "li a4," << divi << std::endl;
                stream << "div a5," << left_register << ",a4" << std::endl;
            }

        }

        if(!left_const && !right_const){
            stream << "div a5," << left_register << "," << right_register << std::endl;
        }

        if(left_const && right_const){
            int result = left_const->GetValue() / right_const->GetValue();
            stream << "li a5," << result << std::endl;
        }

    }


    else if(operator_ == "%"){


        LoadOperand(stream, context);

        if(left_const && !right_const){
            stream << "li " << left_register << "," << left_const->GetValue() << std::endl;
            stream << "rem a5," << left_register << "," << right_register << std::endl; 
        }

        if(!left_const && right_const){
            int divi = right_const->GetValue();

            if(divi == 2){

                stream << "srai a5," << left_register << ",31" << std::endl;
                stream << "srli a5,a5,31" << std::endl; 

                stream << "add " << left_register << "," << left_register << ",a5" << std::endl;

                // The andi checks the last bit, if it is one then we must have a remainder
                stream << "andi " << left_register << ",1" << std::endl;

                stream << "sub a5," << left_register << ",a5" << std::endl;

            }
            else {
                stream << "li " << right_register << "," << right_const->GetValue() << std::endl;
                stream << "rem a5," << left_register << "," << right_register << std::endl;
            }

        }

        if(!left_const && !right_const){
            stream << "rem a5," << left_register << "," << right_register << std::endl;
        }

        if(left_const && right_const){
            int result = left_const->GetValue() % right_const->GetValue();
            stream << "li a5," << result << std::endl;
        }

    }

}