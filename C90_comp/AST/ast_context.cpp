#include "ast_context.hpp"
#include <stdexcept>

Context::Context(){

    // We initalize the register names, following https://github.com/riscv-non-isa/riscv-elf-psabi-doc
    available_registers_[0].setRegisterId("$zero");
    available_registers_[1].setRegisterId("$ra");
    available_registers_[2].setRegisterId("$sp");
    available_registers_[3].setRegisterId("$gp");
    available_registers_[4].setRegisterId("$tp");
    available_registers_[5].setRegisterId("$t0");
    available_registers_[6].setRegisterId("$t1");
    available_registers_[7].setRegisterId("$t2");
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



void Context::AddVariable(const std::string &name, const std::string &type, int location){
    symbol_table_[name] = {type, location};
}

void Context::SetCurrentType(const std::string &type){
    current_type_ = type;
}

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

bool Context::IsVariableDeclared(const std::string &name) const{
    return symbol_table_.find(name) != symbol_table_.end();
}

void Context::AssignVarReg(const std::string& variable_identifier_, const std::string& register_identifier_){
    if(!register_identifier_.empty()){
        register_map_[variable_identifier_] = register_identifier_;
        for(size_t i = 0; i <= 31; i++){
            if(available_registers_[i].getRegisterId().compare(register_identifier_) == 0){
                available_registers_[i].setVariableId(variable_identifier_);
                symbol_table_[variable_identifier_] = {GetCurrentType(), -1, true};
                return;
            }
        }
    }
    else{
        AllocateStack(variable_identifier_);
    }
}

std::string Context::GetFreeReg(){

    // First, we want to loop through the temp. registers ($t0 - $t6), 
    for(size_t i = 5; i <= 7; i++){
        if(available_registers_[i].isRegFree()){
            return available_registers_[i].getRegisterId();
        }
    }

    for(size_t i = 28; i < 31; i++){
        if(available_registers_[i].isRegFree()){
            return available_registers_[i].getRegisterId();
        }
    }

    // if none available, we move on to the Callee-Saved ($s0 - $s11)
    for(size_t i = 8; i < 9; i++){
        if(available_registers_[i].isRegFree()){
            return available_registers_[i].getRegisterId();
        }
    }

    for(size_t i = 18; i < 27; i++){
        if(available_registers_[i].isRegFree()){
            return available_registers_[i].getRegisterId();
        }
    }

    // again, if none available, we have overspill, which means we now use the stack

    return "";
}

void Context::AllocateStack(const std::string &name){

    // Allocate 8 bytes (words are 8 bytes in RISC-V 64 bits)
    stack_pointer_ += 8;

    // We use static cast to safely ensure that we have an int64_t value (8 bytes)., in case the stack_pointer_ had issues
    // (which shouldn't happen either as stack_pointer_ is already int64_t)
    symbol_table_[name] = {GetCurrentType(), static_cast<int64_t>(stack_pointer_), false};
}

VariableLocation Context::GetVariableLoc(const std::string &name) const{

    VariableLocation loc;
    auto stack = symbol_table_.find(name);
    auto reg = register_map_.find(name);

    if(stack != symbol_table_.end() && stack->second.location != -1){
        loc = {"", static_cast<int64_t>(stack->second.location), false};
        return loc;
    }
    
    if(reg != register_map_.end()){
        loc = {reg->second, static_cast<int64_t>(-1), true};
        return loc;
    }

    else{
        throw std::runtime_error("Variable DNE" + name);
    }
}