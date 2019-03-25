#include "LinkedFileInformation.h"

#include <sstream>

using std::stringstream;

int LinkedFileInformation::getStartBlockIndex() const
{
    return startBlockIndex;
}

void LinkedFileInformation::setStartBlockIndex(const int startBlockIndex)
{
    this->startBlockIndex = startBlockIndex;
}

int LinkedFileInformation::getEndBlockIndex() const
{
    return endBlockIndex;
}

void LinkedFileInformation::setEndBlockIndex(const int endBlockIndex)
{
    this->endBlockIndex = endBlockIndex;
}

string LinkedFileInformation::toString() const
{
    if (fileName == -1 || startBlockIndex == -1 || endBlockIndex == -1)
        return "";

    stringstream stringStream;
    stringStream << fileName << "," << startBlockIndex << "," << endBlockIndex;

    return stringStream.str();
}
