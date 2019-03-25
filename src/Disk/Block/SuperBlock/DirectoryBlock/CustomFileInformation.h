#pragma once
#include "AbstractFileInformation.h"

/**
 * \brief This is a file information for custom allocation method.
 */
class CustomFileInformation final : public AbstractFileInformation
{
    int startBlockIndex = -1;
    int length = -1;

public:
    CustomFileInformation() = default;

    int getStartBlockIndex() const;
    void setStartBlockIndex(int startBlockIndex);

    int getLength() const;
    void setLength(int length);

    string toString() const override;
};
