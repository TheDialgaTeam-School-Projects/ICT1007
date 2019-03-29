#include "LinkedDiskBlock.h"

#include <sstream>

#include "AbstractDiskBlock.h"

using std::stringstream;

LinkedDiskBlock::LinkedDiskBlock(const int blockIndex, const int blockSize) :
    AbstractDiskBlock(blockIndex, blockSize),
    data(blockSize, -1)
{
}

string LinkedDiskBlock::toString(const int index)
{
    if (data[index] == -1)
        return "";

    stringstream stringStream;

    if (index + 1 == blockSize)
        stringStream << "B";

    stringStream << data[index];

    return stringStream.str();
}

int &LinkedDiskBlock::operator[](const int index)
{
    return data[index];
}
