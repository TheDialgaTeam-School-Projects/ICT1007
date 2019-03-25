#include "IndexedFileInformation.h"

#include <sstream>

using std::stringstream;

int IndexedFileInformation::getIndexBlockIndex() const
{
    return indexBlockIndex;
}

void IndexedFileInformation::setIndexBlockIndex(const int indexBlockIndex)
{
    this->indexBlockIndex = indexBlockIndex;
}

string IndexedFileInformation::toString() const
{
    if (fileName == -1 || indexBlockIndex == -1)
        return "";

    stringstream stringStream;
    stringStream << fileName << "," << indexBlockIndex;

    return stringStream.str();
}
