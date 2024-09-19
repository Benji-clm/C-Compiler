#include "ast_type_specifier.hpp"

void TypeSpecifier::EmitRISC(std::ostream &stream, Context &context) const {
    context.SetCurrentType(type_);
}

void TypeSpecifier::Print(std::ostream &stream) const
{
    stream << type_;
}
