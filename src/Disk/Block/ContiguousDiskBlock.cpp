#include "ContiguousDiskBlock.h"

#include <sstream>

using std::stringstream;

ContiguousDiskBlock::ContiguousDiskBlock(const int blockIndex, const int blockSize):
    AbstractDiskBlock(blockIndex, blockSize),
    data(blockSize, -1)
{
}

string ContiguousDiskBlock::toString(const int index)
{
    if (data[index] == -1)
        return "";

    stringstream stringStream;
    stringStream << data[index];

    return stringStream.str();
}

int &ContiguousDiskBlock::operator[](const int index)
{
    return data[index];
}
