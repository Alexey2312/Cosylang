#pragma once

#include <string>


class CosylangType
{
public:
    virtual ~CosylangType() = default;
    virtual std::string getName() {  return "unknow"; };
};

class IntType : public CosylangType
{
    unsigned int size;
public:
    IntType(unsigned int size = 32) : size(size) {}

    std::string getName() override
    {
        return "int" + std::to_string(size);
    }
};

class Float32Type : public CosylangType
{
public:
    std::string getName() override
    {
        return "float32";
    }
};

class Float64Type : public CosylangType
{
public:
    std::string getName() override
    {
        return "float64";
    }

};

class Float80Type : public CosylangType
{
public:
    std::string getName() override
    {
        return "float80";
    }

};

class BoolType : public CosylangType
{
public:

    std::string getName() override
    {
        return "bool";
    }

};

class VoidType : public CosylangType
{
public:

    std::string getName() override
    {
        return "void";
    }
};

class UnknownType : public CosylangType
{
    std::string getName() override
    {
        return "unknow";
    }
};
