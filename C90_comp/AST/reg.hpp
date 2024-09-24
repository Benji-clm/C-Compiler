#ifndef reg_hpp
#define reg_hpp

#include <string>
#include <cstdint>

class Reg{
private:
    std::string variable_identifier_;
    std::string register_identifier_;

    // We are using RISC-V 64 bits, hence we use an unsigned 64 bit number to store the addresses.
    // Addresses should all be 64 bits length in the used RISC-V, hence we need to ensure using uint that the
    // memory addresses stored in the frame pointer always have fixed size.
    uint64_t fp_;

public:
    Reg(const std::string& reg_id_);

    void setVariableId(const std::string& var_id);
    void setRegisterId(const std::string& reg_id);
    void setFP(const uint64_t& FP);

    std::string getVariableId() const;
    std::string getRegisterId() const;
    std::string getFP() const;

    bool isRegFree() const;

};

#endif