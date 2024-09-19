#include "ast_declarator.hpp"

std::string Declarator::GetName() const{
    return declarator_;
}

void Declarator::EmitRISC(std::ostream &stream, Context &context) const{

    std::string type = context.GetCurrentType();

    // Needs Implementation in Context
    int location = context.AllocateRegister();

    context.AddVariable(declarator_, type, location);

    // EmitRISC, From here, we have all three, the name of the variable, its type and its location. This means that
    // we are ready to output the RISC V Assembly code, simply implement so. 

}