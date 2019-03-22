#pragma once
#include <vector>

#include "../../DiskAllocationMethod.h"
#include "DirectoryBlock/AbstractFileInformation.h"

using std::vector;

class DirectoryBlock
{
    vector<AbstractFileInformation*> filesInformation;

public:
    DirectoryBlock(int blockSize, DiskAllocationMethod diskAllocationMethod);
    ~DirectoryBlock();

    AbstractFileInformation *operator[](int index);

    DirectoryBlock(const DirectoryBlock &) = default;
    DirectoryBlock &operator=(const DirectoryBlock &) = default;
    DirectoryBlock(DirectoryBlock &&) = default;
    DirectoryBlock &operator=(DirectoryBlock &&) = default;
};
