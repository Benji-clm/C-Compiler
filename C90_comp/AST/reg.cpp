#include "reg.hpp"

// Setters

void Reg::setVariableId(const std::string& var_id){
    variable_identifier_ = var_id;
}

void Reg::setRegisterId(const std::string& reg_id){
    register_identifier_ = reg_id;
}

void Reg::setFP(const uint32_t& FP){
    fp_ = FP;
}

// Getters

std::string Reg::getVariableId() const{
    return variable_identifier_;
}

std::string Reg::getRegisterId() const{
    return register_identifier_;
}

std::string Reg::getFP() const{
    return std::to_string(fp_);
}

// check for free register

bool Reg::isRegFree() const{
    return variable_identifier_.empty();
}