#pragma once

#include "UiData.h"

class UiDataHolderInterface
{
public:
    virtual ~UiDataHolderInterface() = default;

    UiData& getRef() { return uiData; }
    const UiData& getRef() const { return uiData; }

    virtual void resetDefault() = 0;

protected:
    UiData uiData = {};
};
