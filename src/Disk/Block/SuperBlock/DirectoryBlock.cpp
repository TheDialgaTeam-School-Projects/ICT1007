#include "DirectoryBlock.h"

// ReSharper disable CppUseAuto

DirectoryBlock::DirectoryBlock(const int blockSize, const DiskAllocationMethod diskAllocationMethod):
    AbstractDiskBlock(0, blockSize)
{
}

DirectoryBlock::~DirectoryBlock()
{
    for (auto *fileInformation : filesInformation)
        delete fileInformation;

    filesInformation.clear();
}

vector<AbstractFileInformation*> &DirectoryBlock::getFilesInformation()
{
    return filesInformation;
}

const vector<AbstractFileInformation*> &DirectoryBlock::getFilesInformation() const
{
    return filesInformation;
}

bool DirectoryBlock::deleteFile(const int fileName)
{
    for (uint64_t i = 0; i < filesInformation.size(); i++)
    {
        AbstractFileInformation *fileInformation = filesInformation[i];

        if (fileInformation->getFileName() != fileName)
            continue;

        delete fileInformation;
        filesInformation.erase(filesInformation.begin() + i);

        return true;
    }

    return false;
}

string DirectoryBlock::toString(const int index)
{
    if (index >= static_cast<const int>(filesInformation.size()))
        return "";

    return filesInformation[index]->toString();
}

AbstractFileInformation *DirectoryBlock::operator[](const int index)
{
    return filesInformation[index];
}
