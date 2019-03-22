#pragma once
#include "AbstractFileInformation.h"

/**
 * \brief This is a file information for contiguous allocation method.
 */
class ContiguousFileInformation final : public AbstractFileInformation
{
    int startBlockIndex = -1;
    int length = -1;

public:
    ContiguousFileInformation() = default;

    int getStartBlockIndex() const;
    void setStartBlockIndex(int startBlockIndex);

    int getLength() const;
    void setLength(int length);

    void reset() override;

    string toString() const override;
};
