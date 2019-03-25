#pragma once
#include <string>

using std::string;

class AbstractDiskBlock
{
protected:
    int blockIndex = -1;
    int blockSize = -1;

public:
    AbstractDiskBlock(int blockIndex, int blockSize);
    virtual ~AbstractDiskBlock() = default;

    int getBlockIndex() const;
    int getBlockSize() const;

    virtual string toString(int index) = 0;

    AbstractDiskBlock(const AbstractDiskBlock &) = default;
    AbstractDiskBlock &operator=(const AbstractDiskBlock &) = default;
    AbstractDiskBlock(AbstractDiskBlock &&) = default;
    AbstractDiskBlock &operator=(AbstractDiskBlock &&) = default;
};

using AbstractBlockData = AbstractDiskBlock;
using IBlockData = AbstractDiskBlock;