#pragma once
#include "AbstractFileInformation.h"

/**
 * \brief This is a file information for linked allocation method.
 */
class LinkedFileInformation final : public AbstractFileInformation
{
    int startBlockIndex = -1;
    int endBlockIndex = -1;

public:
    LinkedFileInformation() = default;

    int getStartBlockIndex() const;
    void setStartBlockIndex(int startBlockIndex);

    int getEndBlockIndex() const;
    void setEndBlockIndex(int endBlockIndex);

    string toString() const override;
};
