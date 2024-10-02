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
    // TODO: SWITCH FROM A SIMPLE "std::string" TO enum LIKE IN THE PREVIOUS COMPILER (calc2), THIS WOULD ADD ROBUSTNESS
    std::string type;
    VariableLocation location;
    bool is_initialized_;
    VariableValue value_;
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
    std::vector<Reg> float_registers_;
    // std::vector<Reg> vector_registers_;

    std::unordered_map<std::string, std::string> register_map_; //maps variable identifiers to registers
    std::unordered_map<std::string, std::string> float_regmap_;

    std::unordered_map<std::string, int> VarPos;
    int32_t stack_pointer_ = 0;
    uint32_t fp_ = 0;

    int current_arg_count;

public:


    // Setters

    void AddVariable(const std::string &name);
    void SetCurrentType(const std::string &type);
    void AssignVarReg(const std::string& variable_identifier_, const std::string& register_identifier_);
    void AllocateStack(const std::string &name);
    void SetIsInitialized(const std::string &name, const bool &is_initialized);
    void SetValue(const std::string &name, const VariableValue &value);
    void SetCurrentArgCount(const int &size);

    // Getters

    std::string GetCurrentType() const;
    std::string GetVariableType(const std::string &name) const;
    VariableLocation GetVariableLoc(const std::string &name) const;
    std::string GetFreeReg();
    std::string GetArgReg();
    std::string GetFreeFloatReg();
    VariableInfo GetVariableInfo(const std::string &name) const;
    VariableValue GetVariableValue(const std::string &name) const;
    int GetCurrentArgCount() const;


    // Checks

    bool IsVariableDeclared(const std::string &name) const;
    bool IsInitialized(const std::string &name) const;

    

    

    

    // TODO: Extend context to include scope etc.
};

#endif
