#include "../reporter.hpp"

class ReporterHolderForParser
{
    static inline Reporter reporter = Reporter();
public:

    static inline void setReporter(Reporter& reporter)
    {
        ReporterHolderForParser::reporter = reporter;
    }

    static inline Reporter& getReporter()
    {
        return reporter;
    }
};
