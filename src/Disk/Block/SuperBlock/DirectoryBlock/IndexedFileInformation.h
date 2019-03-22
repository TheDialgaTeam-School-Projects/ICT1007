#pragma once
#include "AbstractFileInformation.h"

/**
 * \brief This is a file information for indexed allocation method.
 */
class IndexedFileInformation final : public AbstractFileInformation
{
    int indexBlockIndex = -1;

public:
    IndexedFileInformation() = default;

    int getIndexBlockIndex() const;
    void setIndexBlockIndex(int indexBlockIndex);

    void reset() override;

    string toString() const override;
};
