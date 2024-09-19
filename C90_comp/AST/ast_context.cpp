#include "ast_context.hpp"
#include <stdexcept>

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

int Context::GetVariableLoc(const std::string &name) const{

    auto loc = symbol_table_.find(name);

    if(loc != symbol_table_.end()){
        return loc->second.location;
    }

    else{
        throw std::runtime_error("Variable DNE" + name);
    }
}

bool Context::IsVariableDeclared(const std::string &name) const{
    return symbol_table_.find(name) != symbol_table_.end();
}