#ifndef AST_CONTEXT_HPP
#define AST_CONTEXT_HPP

#include "reg.hpp"
#include <unordered_map>
#include <vector>
#include <string>

// An object of class Context is passed between AST nodes during compilation.
// This can be used to pass around information about what's currently being
// compiled (e.g. function scope and variable names).

union VariableValue{
    int int_value;
    float float_value;
    bool bool_value;
};

struct VariableInfo{
    std::string type;
    VariableLocation location;
    VariableValue value_;
    bool is_initialized_;
};

struct VariableLocation{
    std::string reg;
    int32_t stack_loc;
    bool inRegister;
};

class Context
{
private:

    // So far, we will map each variable to their types, assuming their types are strings, namely "int", "float" etc.
    std::unordered_map<std::string, VariableInfo> symbol_table_;
    std::string current_type_;

    // For the location management
    std::vector<Reg> available_registers_;
    std::unordered_map<std::string, std::string> register_map_; //maps variable identifiers to registers

    std::unordered_map<std::string, int> VarPos;
    int32_t stack_pointer_ = 0;
    uint32_t fp_ = 0;

public:


    // Setters

    void AddVariable(const std::string &name, const VariableValue &value, const bool &is_initialized);
    void SetCurrentType(const std::string &type);
    void AssignVarReg(const std::string& variable_identifier_, const std::string& register_identifier_, const VariableValue &value, const bool &is_initialized);
    void AllocateStack(const std::string &name, const VariableValue &value, const bool &is_initialized);

    // Getters

    std::string GetCurrentType() const;
    std::string GetVariableType(const std::string &name) const;
    VariableLocation GetVariableLoc(const std::string &name) const;
    std::string GetFreeReg();


    // Checks

    bool IsVariableDeclared(const std::string &name) const;

    

    

    

    // TODO: Extend context to include scope etc.
};

#endif
