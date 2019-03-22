#include "ContiguousDiskBlock.h"

#include <sstream>

using std::stringstream;

ContiguousDiskBlock::ContiguousDiskBlock(const int blockIndex, const int blockSize):
    AbstractDiskBlock(blockIndex, blockSize),
    data(blockSize, -1)
{
}

int &ContiguousDiskBlock::operator[](const int index)
{
    return data[index];
}
