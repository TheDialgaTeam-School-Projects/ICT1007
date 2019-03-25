#pragma once
#include <vector>

#include "../../DiskAllocationMethod.h"
#include "../AbstractDiskBlock.h"
#include "DirectoryBlock/AbstractFileInformation.h"

using std::vector;

class DirectoryBlock final : public AbstractDiskBlock
{
    vector<AbstractFileInformation*> filesInformation;

public:
    DirectoryBlock(int blockSize, DiskAllocationMethod diskAllocationMethod);
    ~DirectoryBlock();

    vector<AbstractFileInformation*> &getFilesInformation();
    const vector<AbstractFileInformation*> &getFilesInformation() const;

    template<class TAbstractFileInformation>
    TAbstractFileInformation *addFile();

    bool deleteFile(int fileName);

    string toString(int index) override;

    AbstractFileInformation *operator[](int index);

    DirectoryBlock(const DirectoryBlock &) = default;
    DirectoryBlock &operator=(const DirectoryBlock &) = default;
    DirectoryBlock(DirectoryBlock &&) = default;
    DirectoryBlock &operator=(DirectoryBlock &&) = default;
};

template <class TAbstractFileInformation>
TAbstractFileInformation *DirectoryBlock::addFile()
{
    static_assert(std::is_base_of<AbstractFileInformation, TAbstractFileInformation>::value, "TAbstractFileInformation requires a class that extends AbstractFileInformation.");

    if (filesInformation.size() + 1 > blockSize)
        return nullptr;

    AbstractFileInformation *fileInformation = new TAbstractFileInformation();
    filesInformation.push_back(fileInformation);

    return reinterpret_cast<TAbstractFileInformation*>(fileInformation);
}
