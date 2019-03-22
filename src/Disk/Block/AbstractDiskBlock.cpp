#include "AbstractDiskBlock.h"

AbstractDiskBlock::AbstractDiskBlock(const int blockIndex, const int blockSize)
{
    this->blockIndex = blockIndex;
    this->blockSize = blockSize;
}

int AbstractDiskBlock::getBlockIndex() const
{
    return blockIndex;
}

int AbstractDiskBlock::getBlockSize() const
{
    return blockSize;
}
