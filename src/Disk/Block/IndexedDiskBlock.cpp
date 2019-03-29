#include "IndexedDiskBlock.h"

#include <sstream>

using std::stringstream;

IndexedDiskBlock::IndexedDiskBlock(const int blockIndex, const int blockSize):
    AbstractDiskBlock(blockIndex, blockSize),
    data(blockSize, -1)
{
}

string IndexedDiskBlock::toString(const int index)
{
    if (data[index] == -1)
        return "";

    stringstream stringStream;
    stringStream << "B" << data[index];

    return stringStream.str();
}

int &IndexedDiskBlock::operator[](const int index)
{
    return data[index];
}
