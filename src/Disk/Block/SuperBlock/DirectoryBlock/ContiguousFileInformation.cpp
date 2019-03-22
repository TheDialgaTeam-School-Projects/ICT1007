#include "ContiguousFileInformation.h"

#include <sstream>

using std::stringstream;

int ContiguousFileInformation::getStartBlockIndex() const
{
    return startBlockIndex;
}

void ContiguousFileInformation::setStartBlockIndex(const int startBlockIndex)
{
    this->startBlockIndex = startBlockIndex;
}

int ContiguousFileInformation::getLength() const
{
    return length;
}

void ContiguousFileInformation::setLength(const int length)
{
    this->length = length;
}

void ContiguousFileInformation::reset()
{
    fileName = -1;
    startBlockIndex = -1;
    length = -1;
}

string ContiguousFileInformation::toString() const
{
    if (fileName == -1 || startBlockIndex == -1 || length == -1)
        return "";

    stringstream stringStream;
    stringStream << fileName << "," << startBlockIndex << "," << length;

    return stringStream.str();
}
