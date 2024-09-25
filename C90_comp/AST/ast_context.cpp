#include "ast_context.hpp"
#include <stdexcept>

Context::Context(){

    // We initalize the register names, following https://github.com/riscv-non-isa/riscv-elf-psabi-doc

    // x0/zero is immutable and should always contain zero
    available_registers_[0].setRegisterId("$zero");

    available_registers_[1].setRegisterId("$ra");

    // This is the stack pointer
    available_registers_[2].setRegisterId("$sp");

    // Do not modify registers tp and gp, signal handlers may rely updon their values.
    available_registers_[3].setRegisterId("$gp");
    available_registers_[4].setRegisterId("$tp");
    available_registers_[5].setRegisterId("$t0");
    available_registers_[6].setRegisterId("$t1");
    available_registers_[7].setRegisterId("$t2");

    // x8/s0 should be kept for frame pointer
    available_registers_[8].setRegisterId("$s0");
    available_registers_[9].setRegisterId("$s1");

    for(size_t i = 10; i <= 17; i++){
        available_registers_[i].setRegisterId("$a" + std::to_string(i - 10));
    }

    for(size_t i = 18; i <= 27; i++){
        available_registers_[i].setRegisterId("$s" + std::to_string(i - 16));
    }

    available_registers_[28].setRegisterId("$t3");
    available_registers_[29].setRegisterId("$t4");
    available_registers_[30].setRegisterId("$t5");
    available_registers_[31].setRegisterId("$t6");

    stack_pointer_ = 0;
}


// Setters

void Context::AddVariable(const std::string &name, const VariableValue &value, const bool &is_initialized){

    std::string reg = GetFreeReg();
    AssignVarReg(name, reg, value, is_initialized);

}


void Context::SetCurrentType(const std::string &type){
    current_type_ = type;
}


void Context::AssignVarReg(const std::string& variable_identifier_, const std::string& register_identifier_, const VariableValue &value, const bool &is_initialized){

    if(!register_identifier_.empty()){
        register_map_[variable_identifier_] = register_identifier_;
        for(size_t i = 0; i <= 31; i++){
            if(available_registers_[i].getRegisterId().compare(register_identifier_) == 0){
                available_registers_[i].setVariableId(variable_identifier_);

                VariableLocation loc;
                loc = {register_identifier_, -1, true};

                symbol_table_[variable_identifier_] = {GetCurrentType(), loc, value, is_initialized};
                return;
            }
        }
    }
    else{
        AllocateStack(variable_identifier_, value, is_initialized);
    }
}


void Context::AllocateStack(const std::string &name, const VariableValue &value, const bool &is_initialized){

    // Allocate 4 bytes (words are 4 bytes in RISC-V 32 bits)
    stack_pointer_ += 4;

    // We use static cast to safely ensure that we have an int32_t value (8 bytes)., in case the stack_pointer_ had issues
    // (which shouldn't happen either as stack_pointer_ is already int32_t)

    VariableLocation loc;
    loc = {"", static_cast<int32_t>(stack_pointer_), false};

    symbol_table_[name] = {GetCurrentType(), loc, value, is_initialized};
}





// Getters



std::string Context::GetCurrentType() const{
    return current_type_;
}


std::string Context::GetVariableType(const std::string &name) const{

    auto type = symbol_table_.find(name);

    if(type != symbol_table_.end()){
        return type->second.type;
    }

    else{
        throw std::runtime_error("Variable DNE" + name);
    }
}



std::string Context::GetFreeReg(){

    // First, we want to loop through the temp. registers ($t1 - $t6), I decided for the moment to keep t0 free, so that it can be used to store
    // values easily to the stack.
    for(size_t i = 6; i <= 7; i++){
        if(available_registers_[i].isRegFree()){
            return available_registers_[i].getRegisterId();
        }
    }

    for(size_t i = 28; i < 31; i++){
        if(available_registers_[i].isRegFree()){
            return available_registers_[i].getRegisterId();
        }
    }

    // if none available, we move on to the Callee-Saved ($s1 - $s11), we keep s0/x8 for the frame pointer.
    if(available_registers_[9].isRegFree()){
        return available_registers_[9].getRegisterId();
    }

    for(size_t i = 18; i < 27; i++){
        if(available_registers_[i].isRegFree()){
            return available_registers_[i].getRegisterId();
        }
    }

    // again, if none available, we have overspill, which means we now use the stack

    return "";
}


VariableLocation Context::GetVariableLoc(const std::string &name) const{
    
    auto loc = symbol_table_.find(name);

    if(loc != symbol_table_.end()){
        return loc->second.location;
    }

    else{
        throw std::runtime_error("Variable DNE" + name);
    }
}






// Checkers


bool Context::IsVariableDeclared(const std::string &name) const{
    return symbol_table_.find(name) != symbol_table_.end();
}
