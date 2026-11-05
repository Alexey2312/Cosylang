#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

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
    TYPE_ALIAS,
    AUTO
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

    bool isConvertible(std::string second_type)
    {
        fillConvertibleTypes(kind);
        return std::find(convertible_types.begin(), convertible_types.end(), second_type) != convertible_types.end();
    }

    void defineTypeKindByName()
    {
        if (name == "int8" || name == "int16" || name == "int32" || name == "int64" || name == "int128")
        {
            kind = TypeKind::INT;
        }
        else if (name == "float32" || name == "float64" || name == "float128")
        {
            kind = TypeKind::FLOAT;
        }
        else if (name == "string")
        {
            kind = TypeKind::STRING;
        }
        else if (name == "bool")
        {
            kind = TypeKind::BOOL;
        }
        else if (name == "auto")
        {
            kind = TypeKind::AUTO;
        }
        else if (name == "void")
        {
            kind = TypeKind::VOID;
        }
        else
        {
            std::cerr << "Cannot define type kind by name! Name: " + name + "\n";
        }
    }

    Type(std::string this_name, TypeKind this_kind = TypeKind::VOID, std::vector<std::string> this_convertible_types = {})
        : name(this_name), kind(this_kind), convertible_types(this_convertible_types)
        {
            fillConvertibleTypes(this_kind);
            if (this_kind == TypeKind::VOID)
            {
                defineTypeKindByName();
            }
        }
};

} //namespace Cosylang::Semantic
