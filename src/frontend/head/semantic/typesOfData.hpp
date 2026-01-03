#pragma once

#include <llvm/Support/Casting.h>
#include <string>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>


class CosylangType
{
public:

    virtual std::string getName();
    virtual llvm::Type* getThisTypeForLLVM(llvm::LLVMContext& context);
};

class Int : public CosylangType
{
    unsigned int size;
public:
    Int(unsigned int size = 32) : size(size) {}

    std::string getName() override
    {
        return "Int";
    }

    llvm::Type* getThisTypeForLLVM(llvm::LLVMContext& context) override
    {
        return llvm::Type::getIntNTy(context, size);
    }

};

class Float32 : public CosylangType
{
public:
    std::string getName() override
    {
        return "Float32";
    }

    llvm::Type* getThisTypeForLLVM(llvm::LLVMContext& context) override
    {
        return llvm::Type::getFloatTy(context);
    }
};

class Float64 : public CosylangType
{
public:
    std::string getName() override
    {
        return "Float64";
    }

    llvm::Type* getThisTypeForLLVM(llvm::LLVMContext& context) override
    {
        return llvm::Type::getDoubleTy(context);
    }
};

class Float80 : public CosylangType
{
public:
    std::string getName() override
    {
        return "Float80";
    }

    llvm::Type* getThisTypeForLLVM(llvm::LLVMContext& context) override
    {
        return llvm::Type::getX86_FP80Ty(context);
    }
};

class Bool : public CosylangType
{
public:

    std::string getName() override
    {
        return "Int";
    }

    llvm::Type* getThisTypeForLLVM(llvm::LLVMContext& context) override
    {
        return llvm::Type::getInt1Ty(context);
    }
};
