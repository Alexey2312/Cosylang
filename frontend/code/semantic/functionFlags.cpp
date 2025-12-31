#include <string>
#include <vector>
#include <iostream>

class IFlag
{
    std::string flagName = "";
public:
    std::string getName() const { return flagName; }
};

class flagName
{
private:
    std::string name = "";

public:
    flagName(const std::string name) : name(name) {}
    std::string getName() const { return name; }
    bool check(std::string checkingName) const { return checkingName == name; }
};

class FlagsRegister
{
private:
    std::vector<flagName> correctFlags =
    {

    };
public:
    bool isFlagValid(std::string inputFlag)
    {
        if (correctFlags.empty())
        {
            std::cout << "FlagsRegister: No flags registered" << std::endl;
            return false;
        }
        for (auto& flag : correctFlags)
        {
            if (flag.check(inputFlag))
            {
                return true;
            }
        }
        return false;
    }
    void registerFlag(IFlag newFlag)
    {
        correctFlags.push_back(newFlag.getName());
    }
};
