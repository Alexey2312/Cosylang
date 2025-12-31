#pragma once
#include <string>
#include <any>
#include <vector>
#include <optional>
#include <stdexcept>

class IType;
class TypeFactory;

class IType
{
private:
    std::string name;
public:
    IType(std::string name) : name(name) {}

    std::string getName() const { return name; }
    void setName(std::string newName) { name = newName; }
};

class Int32Type : public IType
{
public:
    Int32Type() : IType("int32")
    {
    }
};

class AnyType : public IType
{
    std::any concreteType = std::any();
public:
    AnyType() : IType("any")
    {
    }

    std::any getConcreteType() { return concreteType; }
    void setConcreteType(std::any newType) { concreteType = newType; }
};

class IsType
{
    friend TypeFactory;
    std::string name;
    IType thisType;
    std::optional<std::vector<std::string>> names = std::nullopt;
public:
    IsType(std::string name, IType thisType) : name(name), thisType(thisType) {}
    IsType(std::vector<std::string> names, IType thisType) : thisType(thisType), names(names) {}

    bool checkTypeName(std::string typeName)
    {
        if (names.has_value())
        {
            for (const auto& name : names.value())
            {
                if (name == typeName)
                {
                    return true;
                }
            }
            return false;
        }
        return name == typeName;
    }
    IType createThisType()
    {
        return thisType;
    }
    void setTypeName(IType newType)
    {
        thisType = newType;
    }
    void setThisType(IType newType)
    {
        thisType = newType;
    }
};

class IsIntType : public IsType
{
    std::vector<std::string> names = {"int32", "int64", "int128"};
public:
    std::vector<std::string> getNames() const
    {
        return names;
    }

    IsIntType() : IsType(getNames(), Int32Type())
    {

    }
};

class TypeFactory
{
    static std::vector<IsType> isTypes;
public:
    static void initIsTypes()
    {
        isTypes.clear();
        isTypes.push_back(IsIntType());
        /* add more types here */
    }

    static IType createType(std::string typeName)
    {
        initIsTypes();
        for (auto& isType : isTypes)
        {
            if (isType.checkTypeName(typeName))
            {
                return isType.createThisType();
            }
        }
        throw std::runtime_error("Semantic error: Type " + typeName + " not found");
    }

    static void addIsType(IsType isType)
    {
        isTypes.push_back(isType);
    }

};
