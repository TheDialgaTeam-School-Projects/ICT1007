#pragma once
#include <vector>

#include "AbstractDiskBlock.h"

using std::vector;

class IndexedDiskBlock final : public AbstractDiskBlock
{
    vector<int> data;

public:
    IndexedDiskBlock(int blockIndex, int blockSize);

    string toString(int index) override;

    int &operator[](int index);
};

using IndexBlockData = IndexedDiskBlock;
