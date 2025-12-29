#pragma once

#include <string>
#include <stdexcept>

class IThrowError
{
public:
    virtual void throwError(const std::string& message);
};

class ThrowStdError : public IThrowError
{
public:
    void throwError(const std::string& message) override
    {
        throw std::runtime_error(message);
    };
};
