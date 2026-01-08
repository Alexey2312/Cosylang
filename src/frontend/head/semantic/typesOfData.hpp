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

class IntType : public CosylangType
{
    unsigned int size;
public:
    IntType(unsigned int size = 32) : size(size) {}

    std::string getName() override
    {
        return "int" + std::to_string(size);
    }

    llvm::Type* getThisTypeForLLVM(llvm::LLVMContext& context) override
    {
        return llvm::Type::getIntNTy(context, size);
    }

};

class Float32Type : public CosylangType
{
public:
    std::string getName() override
    {
        return "float32";
    }

    llvm::Type* getThisTypeForLLVM(llvm::LLVMContext& context) override
    {
        return llvm::Type::getFloatTy(context);
    }
};

class Float64Type : public CosylangType
{
public:
    std::string getName() override
    {
        return "float64";
    }

    llvm::Type* getThisTypeForLLVM(llvm::LLVMContext& context) override
    {
        return llvm::Type::getDoubleTy(context);
    }
};

class Float80Type : public CosylangType
{
public:
    std::string getName() override
    {
        return "float80";
    }

    llvm::Type* getThisTypeForLLVM(llvm::LLVMContext& context) override
    {
        return llvm::Type::getX86_FP80Ty(context);
    }
};

class BoolType : public CosylangType
{
public:

    std::string getName() override
    {
        return "bool";
    }

    llvm::Type* getThisTypeForLLVM(llvm::LLVMContext& context) override
    {
        return llvm::Type::getInt1Ty(context);
    }
};

class VoidType : public CosylangType
{
public:

    std::string getName() override
    {
        return "void";
    }

    llvm::Type* getThisTypeForLLVM(llvm::LLVMContext& context) override
    {
        return llvm::Type::getVoidTy(context);
    }
};
