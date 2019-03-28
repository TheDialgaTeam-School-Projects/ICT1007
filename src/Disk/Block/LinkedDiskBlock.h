#pragma once
#include <vector>

#include "AbstractDiskBlock.h"

using std::vector;

class LinkedDiskBlock final : public AbstractDiskBlock
{
    vector<int> data;

public:
    LinkedDiskBlock(int blockIndex, int blockSize);

    string toString(int index) override;

    int &operator[](int index);
};

using LinkedBlockData = LinkedDiskBlock;
