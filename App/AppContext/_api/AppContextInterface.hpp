#pragma once

class AppContextInterface
{
public:
    virtual ~AppContextInterface() = default;

    virtual int run() = 0;
    virtual void quit() = 0;
};
