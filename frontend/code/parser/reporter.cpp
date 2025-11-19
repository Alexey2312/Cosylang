#include "../../head/reporter.hpp"
#include <iostream>

void Reporter::report(ReportType type, const std::string& message)
{
    switch (type)
    {
        case ReportType::INFO:
            infoReports.push_back(message);
            break;
        case ReportType::WARNING:
            warningReports.push_back(message);
            break;
        case ReportType::ERROR:
            errorReports.push_back(message);
            break;
        default:
            throw std::runtime_error("Invalid report type to send " + message);
            break;
    }
}

void Reporter::printReports()
{
    std::cout << "\v INFO: \n" << std::endl;
    for (const auto& report : infoReports)
    {
        std::cout << report << "\n" << std::endl;
    }
    std::cout << "\v WARNING: \n" << std::endl;
    for (const auto& report : warningReports)
    {
        std::cout << report << "\n" << std::endl;
    }
     std::cout << "\v ERRORS: \n" << std::endl;
    for (const auto& report : errorReports)
    {
        std::cout << report << "\n" << std::endl;
    }
}
