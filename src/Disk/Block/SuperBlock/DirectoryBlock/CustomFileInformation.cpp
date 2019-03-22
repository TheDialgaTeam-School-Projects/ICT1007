#include "CustomFileInformation.h"

#include <sstream>

using std::stringstream;

int CustomFileInformation::getStartBlockIndex() const
{
    return startBlockIndex;
}

void CustomFileInformation::setStartBlockIndex(const int startBlockIndex)
{
    this->startBlockIndex = startBlockIndex;
}

int CustomFileInformation::getLength() const
{
    return length;
}

void CustomFileInformation::setLength(const int length)
{
    this->length = length;
}

void CustomFileInformation::reset()
{
    fileName = -1;
    startBlockIndex = -1;
    length = -1;
}

string CustomFileInformation::toString() const
{
    if (fileName == -1 || startBlockIndex == -1 || length == -1)
        return "";

    stringstream stringStream;
    stringStream << fileName << "," << startBlockIndex << "," << length;

    return stringStream.str();
}
