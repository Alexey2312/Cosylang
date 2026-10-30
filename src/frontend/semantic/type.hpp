#pragma once

#include <string>
#include <vector>

namespace Cosylang::Semantic
{

enum class TypeKind
{
    VOID,
    BOOL,
    INT,
    FLOAT,
    STRING,
    FUNCTION,
    STRUCT,
    MODULE,
    TYPE_ALIAS
};

struct Type
{
    std::string name = "";
    TypeKind kind = TypeKind::VOID;

    std::vector<std::string> convertible_types = {};

    void fillConvertibleTypes(TypeKind current_kind)
    {
        switch (current_kind)
        {
            case TypeKind::BOOL:
            case TypeKind::INT:
            {
                convertible_types = {"int8", "int16", "int32", "int64", "int128"};
                return;
            }
            case TypeKind::FLOAT:
            {
                convertible_types = {"float32", "float64", "float128"};
                return;
            }
            default:
            {
                convertible_types = {};
                return;
            }

        }
    }

    Type(std::string this_name, TypeKind this_kind, std::vector<std::string> this_convertible_types = {})
        : name(this_name), kind(this_kind), convertible_types(this_convertible_types) {fillConvertibleTypes(this_kind);}
};

} //namespace Cosylang::Semantic
