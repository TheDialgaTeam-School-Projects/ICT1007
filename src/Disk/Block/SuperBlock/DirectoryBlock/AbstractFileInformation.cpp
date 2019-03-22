#include "AbstractFileInformation.h"

int AbstractFileInformation::getFileName() const
{
    return fileName;
}

void AbstractFileInformation::setFileName(const int fileName)
{
    this->fileName = fileName;
}
