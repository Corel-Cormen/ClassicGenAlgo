#pragma once

#include <QStringView>

class FunctionInterface
{
public:
    virtual ~FunctionInterface() = default;

    virtual inline QStringView name() const = 0;
};
