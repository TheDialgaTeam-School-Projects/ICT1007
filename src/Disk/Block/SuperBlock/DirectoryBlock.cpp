#include "DirectoryBlock.h"

#include "DirectoryBlock/ContiguousFileInformation.h"
#include "DirectoryBlock/CustomFileInformation.h"
#include "DirectoryBlock/IndexedFileInformation.h"
#include "DirectoryBlock/LinkedFileInformation.h"

DirectoryBlock::DirectoryBlock(const int blockSize, const DiskAllocationMethod diskAllocationMethod)
{
    for (auto i = 0; i < blockSize; i++)
    {
        switch (diskAllocationMethod)
        {
            case Contiguous:
                filesInformation.push_back(new ContiguousFileInformation());
                break;

            case Linked:
                filesInformation.push_back(new LinkedFileInformation());
                break;

            case Indexed:
                filesInformation.push_back(new IndexedFileInformation());
                break;

            case Custom:
                filesInformation.push_back(new CustomFileInformation());
                break;
        }
    }
}

DirectoryBlock::~DirectoryBlock()
{
    for (auto *fileInformation : filesInformation)
        delete fileInformation;

    filesInformation.clear();
}

AbstractFileInformation *DirectoryBlock::operator[](const int index)
{
    return filesInformation[index];
}
