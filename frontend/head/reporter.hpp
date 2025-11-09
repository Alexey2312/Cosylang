#pragma once

#include <string>
#include <vector>

class Reporter
{
private:
    std::vector<std::string> errorReports;
    std::vector<std::string> warningReports;
    std::vector<std::string> infoReports;
public:
    enum ReportType { INFO, WARNING, ERROR };
    void report(ReportType type, const std::string& message);
    void printReports();
};
