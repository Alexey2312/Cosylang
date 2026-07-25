#pragma once

#include <string_view>
#include <vector>
#include <optional>
#include <functional>
#include <iostream>
#include <chrono>

namespace Silteli
{

static constexpr std::string_view RED = "\033[31m";
static constexpr std::string_view GREEN = "\033[1;32m";
static constexpr std::string_view BLUE = "\033[1;34m";
static constexpr std::string_view RESET = "\033[0m";
static constexpr std::string_view YELLOW = "\033[1;33m";

#define UNIT_TEST(testName) \
    void testOf_##testName(); \
    static Silteli::TestRegistrar registrarFor_##testName(Silteli::TestInfo(#testName, testOf_##testName)); \
    void testOf_##testName()

class TestInfo
{
public:
    std::string_view name;
    std::function<void()> testFunction;

    TestInfo(std::string_view thisName, std::function<void()> thisFunction) : name(std::move(thisName)), testFunction(std::move(thisFunction)) {}
};

class TestManager
{
    static inline std::vector<TestInfo> tests = {};
    static inline unsigned int currentTest = 0;
    static inline unsigned int passedTests = 0;
    static inline unsigned int failedTests = 0;
public:
    static void registerTest(TestInfo newTest)
    {
        tests.push_back(std::move(newTest));
    }

    static void runTests()
    {
        if (tests.empty())
        {
            return;
        }

        currentTest = 0;
        passedTests = 0;
        failedTests = 0;

        std::cout << "\033[1;36m" << "=======================TESTS=======================" << "\033[0m" << "\n";

        for (TestInfo& test : tests)
        {
            currentTest++;
            std::cout << "---------------------------------------------------" << "\n";
            std::chrono::time_point start = std::chrono::high_resolution_clock::now();
            test.testFunction();
            std::chrono::time_point end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = end - start;
            std::cout << YELLOW << "Total test time: "<< elapsed.count() << " ms" << RESET << "\n";
        }
        std::cout << "---------------------------------------------------" << "\n";

        std::cout << "Passed: " << passedTests << ", Failed: " << failedTests << "\n";

        std::cout << "\033[1;36m" << "====================END OF TESTS===================" << "\033[0m" << "\n";
    }

    static unsigned int getCurrentTest()
    {
        return currentTest;
    }

    static std::optional<std::string_view> getTestNameByID(unsigned int id)
    {
        if (tests.size() >= id)
        {
            return tests.at(id - 1).name;
        }
        return std::nullopt;
    }

    static void addPassedTest()
    {
        passedTests++;
    }
    static void addFailedTest()
    {
        failedTests++;
    }

};

class TestRegistrar
{
public:
    TestRegistrar(TestInfo test)
    {
        TestManager::registerTest(std::move(test));
    }
};

class ResultPrinter
{

public:
    static void printResultOfTest(std::string_view nameOfTestingUnit, bool isPassed)
    {
        std::cout << BLUE << nameOfTestingUnit << " test: " << (isPassed ? GREEN : RED) << (isPassed ? "[PASSED]" : "[FAILED!]") << RESET << "\n";
    }
};

inline void expect(bool expression)
{
    if (TestManager::getTestNameByID(TestManager::getCurrentTest()).has_value())
    {
        expression ? TestManager::addPassedTest() : TestManager::addFailedTest();
        ResultPrinter::printResultOfTest(TestManager::getTestNameByID(TestManager::getCurrentTest()).value(), expression);
    }
    else
    {
        std::cout << "Unknown test!" << "\n";
    }
}

}
