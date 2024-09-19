#ifndef AST_CONTEXT_HPP
#define AST_CONTEXT_HPP

#include <unordered_map>
#include <string>

// An object of class Context is passed between AST nodes during compilation.
// This can be used to pass around information about what's currently being
// compiled (e.g. function scope and variable names).

struct VariableInfo{
    std::string type;
    int location;
};

class Context
{
private:

    // So far, we will map each variable to their types, assuming their types are strings, namely "int", "float" etc.
    std::unordered_map<std::string, VariableInfo> symbol_table_;
    std::string current_type_;

public:

    void AddVariable(const std::string &name, const std::string &type, int location);

    void SetCurrentType(const std::string &type);

    std::string GetCurrentType() const;
    
    std::string GetVariableType(const std::string &name) const;

    int GetVariableLoc(const std::string &name) const;

    bool IsVariableDeclared(const std::string &name) const;

    // TODO: Extend context to include scope etc.
};

#endif
