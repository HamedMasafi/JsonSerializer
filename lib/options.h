#pragma once

#include <QString>

namespace JsonSerializer {

struct Options
{
    QString key{"_key"};
    QString value{"_value"};
    QString className{"_className"};
    bool exportEnumAsString{false};
};

inline Options options;
} // namespace JsonSerializer