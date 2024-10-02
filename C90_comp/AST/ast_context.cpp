#include "ast_context.hpp"
#include <stdexcept>

Context::Context(){

    // We initalize the register names, following https://github.com/riscv-non-isa/riscv-elf-psabi-doc

    // x0/zero is immutable and should always contain zero
    available_registers_[0].setRegisterId("zero");

    available_registers_[1].setRegisterId("ra");

    // This is the stack pointer
    available_registers_[2].setRegisterId("sp");

    // Do not modify registers tp and gp, signal handlers may rely updon their values.
    available_registers_[3].setRegisterId("gp");
    available_registers_[4].setRegisterId("tp");
    available_registers_[5].setRegisterId("t0");
    available_registers_[6].setRegisterId("t1");
    available_registers_[7].setRegisterId("t2");

    // x8/s0 should be kept for frame pointer
    available_registers_[8].setRegisterId("s0");
    available_registers_[9].setRegisterId("s1");

    // These are arguments registers
    for(size_t i = 10; i <= 17; i++){
        available_registers_[i].setRegisterId("a" + std::to_string(i - 10));
    }

    for(size_t i = 18; i <= 27; i++){
        available_registers_[i].setRegisterId("s" + std::to_string(i - 16));
    }

    available_registers_[28].setRegisterId("t3");
    available_registers_[29].setRegisterId("t4");
    available_registers_[30].setRegisterId("t5");
    available_registers_[31].setRegisterId("t6");


    // Now we move on to the floating-point registers

    for(size_t i = 0; i <= 7; i++){
        float_registers_[i].setRegisterId("ft" + std::to_string(i));
    }

    float_registers_[8].setRegisterId("fs0");
    float_registers_[9].setRegisterId("fs1");

    for(size_t i = 10; i <= 17; i++){
        float_registers_[i].setRegisterId("fa" + std::to_string(i - 10));
    }

    for(size_t i = 18; i <= 27; i++){
        float_registers_[i].setRegisterId("fs" + std::to_string(i - 16));
    }

    float_registers_[28].setRegisterId("ft" + std::to_string(8));
    float_registers_[29].setRegisterId("ft" + std::to_string(9));
    float_registers_[30].setRegisterId("ft" + std::to_string(10));
    float_registers_[31].setRegisterId("ft" + std::to_string(11));


    // Vector Registers

    // for(size_t i = 0; i <= 31; i++){
    //     vector_registers_[i].setRegisterId("")
    // }

    // No calling conventions yet for vector registers

    stack_pointer_ = 0;
}


// Setters

void Context::AddVariable(const std::string &name){

    std::string reg = GetFreeReg();
    AssignVarReg(name, reg);

}


void Context::SetCurrentType(const std::string &type){
    current_type_ = type;
}


void Context::AssignVarReg(const std::string& variable_identifier_, const std::string& register_identifier_){

    if(!register_identifier_.empty()){
        register_map_[variable_identifier_] = register_identifier_;
        for(size_t i = 0; i <= 31; i++){
            if(available_registers_[i].getRegisterId().compare(register_identifier_) == 0){
                available_registers_[i].setVariableId(variable_identifier_);

                VariableLocation loc;
                loc = {register_identifier_, -1, true};

                symbol_table_[variable_identifier_] = {GetCurrentType(), loc};
                return;
            }
        }
    }
    else{
        AllocateStack(variable_identifier_);
    }
}


void Context::AllocateStack(const std::string &name){

    // Allocate 4 bytes (words are 4 bytes in RISC-V 32 bits)
    stack_pointer_ += 4;

    // We use static cast to safely ensure that we have an int32_t value (8 bytes)., in case the stack_pointer_ had issues
    // (which shouldn't happen either as stack_pointer_ is already int32_t)

    VariableLocation loc;
    loc = {"", static_cast<int32_t>(stack_pointer_)};

    symbol_table_[name] = {GetCurrentType(), loc};
}


void Context::SetIsInitialized(const std::string &name, const bool &is_initialized){
    auto it = symbol_table_.find(name);
    if(it != symbol_table_.end()){
        it->second.is_initialized_ = is_initialized;
    }
    else{
        throw std::runtime_error("Variable DNE: " + name);
    }
}


void Context::SetValue(const std::string &name, const VariableValue &value){
    auto it = symbol_table_.find(name);
    if(it != symbol_table_.end()){
        it->second.value_ = value;
    }
    else{
        throw std::runtime_error("Variable DNE: " + name);
    }
}


void Context::SetCurrentArgCount(const int &size){
    current_arg_count = size;
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

// A Get Argument Register function is not needed, as these registers are not saved across call, meaning they will be free again after
// being used for the function. We simply have to, whenever calling a f^n, save the first 8 parameters in the registers then use the stack.

// std::string Context::GetArgReg(){
//     for(size_t i = 10; i <= 17; i++){
//         if(available_registers_[i].isRegFree()){
//             return available_registers_[i].getRegisterId();
//         }
//     }
// }

std::string Context::GetFreeFloatReg(){

    // TODO: DURING SCOPE MANAGEMENT/OPTIMISATION, DECIDE WHAT REGISTERS SHOULD BE USED DEPENDING ON THE SCOPE AND/OR CONTEXT
    // For the moment, we will simply keep a temporary register, t0, aside.

    for(size_t i = 1; i <= 31; i++){
        if(float_registers_[i].isRegFree()){
            return float_registers_[i].getRegisterId();
        }
    }

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


VariableInfo Context::GetVariableInfo(const std::string &name) const{
    auto info = symbol_table_.find(name);
    return info->second;
}


VariableValue Context::GetVariableValue(const std::string &name) const{
    auto it = symbol_table_.find(name);
    if(it != symbol_table_.end()){
        return it->second.value_;
    }
    else{
        throw std::runtime_error("Variable DNE: " + name);
    }
}


int Context::GetCurrentArgCount() const{
    return current_arg_count;
}


// Checkers


bool Context::IsVariableDeclared(const std::string &name) const{
    return symbol_table_.find(name) != symbol_table_.end();
}


bool Context::IsInitialized(const std::string &name) const{
    auto it = symbol_table_.find(name);
    if(it != symbol_table_.end()){
        return it->second.is_initialized_;
    }
    else{
        throw std::runtime_error("Variable DNE: " + name);
    }
}
