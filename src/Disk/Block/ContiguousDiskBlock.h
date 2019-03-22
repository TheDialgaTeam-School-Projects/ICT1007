#pragma once
#include <vector>

#include "AbstractDiskBlock.h"

using std::vector;

class ContiguousDiskBlock final : public AbstractDiskBlock
{
    vector<int> data;

public:
    ContiguousDiskBlock(int blockIndex, int blockSize);

    int &operator[](int index);
};

using ContiguousBlockData = ContiguousDiskBlock;