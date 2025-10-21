#pragma once

#include "UiDataHolderInterface.hpp"

class UiDataHolder final : public UiDataHolderInterface
{
public:
    void resetDefault() override;
};
