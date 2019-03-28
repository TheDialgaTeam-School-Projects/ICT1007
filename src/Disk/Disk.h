#pragma once
#include <map>

#include "DiskAllocationMethod.h"
#include "Block/AbstractDiskBlock.h"
#include "Block/SuperBlock/DirectoryBlock.h"
#include "Block/SuperBlock/VolumeControlBlock.h"
#include "Block/SuperBlock/DirectoryBlock/ContiguousFileInformation.h"
#include "Block/SuperBlock/DirectoryBlock/LinkedFileInformation.h"
#include "Block/SuperBlock/DirectoryBlock/IndexedFileInformation.h"
#include "Block/SuperBlock/DirectoryBlock/CustomFileInformation.h"

using std::map;
using std::string;

class Disk
{
    DiskAllocationMethod diskAllocationMethod = Contiguous;
    map<int, AbstractDiskBlock*> diskBlocks;

    void deleteContiguousFile(ContiguousFileInformation *fileInformation, vector<int> &blocksFreed);
    void deleteLinkedFile(LinkedFileInformation *fileInformation, vector<int> &blocksFreed);
    void deleteIndexedFile(IndexedFileInformation *fileInformation, vector<int> &blocksFreed);
    void deleteCustomFile(CustomFileInformation *fileInformation, vector<int> &blocksFreed);

    static void printDiskMapHeader();
    static void printDiskMapValues(int index, int block, const string &value, bool lastBit = false);

    string getDiskEntriesValue(int index);

public:
    Disk(int totalDiskEntries, int entriesPerDiskBlock, DiskAllocationMethod diskAllocationMethod);
    ~Disk();

    VolumeControlBlock &getVolumeControlBlock();
    DirectoryBlock &getDirectoryBlock();

    void addFile(int fileName, vector<int> &data);
    void readFile(int data);
    void deleteFile(int fileName);

    template <class TAbstractDiskBlock>
    TAbstractDiskBlock *addDiskBlock(int blockIndex);
    bool deleteDiskBlock(int blockIndex);

    void reformatDisk(int totalDiskEntries, int entriesPerDiskBlock, DiskAllocationMethod diskAllocationMethod);
    void printDiskMap();
    void dispose();

    Disk(const Disk &) = default;
    Disk &operator=(const Disk &) = default;
    Disk(Disk &&) = default;
    Disk &operator=(Disk &&) = default;
};

template <class TAbstractDiskBlock>
TAbstractDiskBlock *Disk::addDiskBlock(const int blockIndex)
{
    static_assert(std::is_base_of<AbstractDiskBlock, TAbstractDiskBlock>::value, "TAbstractDiskBlock requires a class that extends AbstractDiskBlock.");

    if (static_cast<int>(diskBlocks.size()) + 1 > getVolumeControlBlock().getTotalDiskBlock())
        return nullptr;

    if (diskBlocks.count(blockIndex) > 0)
        return nullptr;

    AbstractDiskBlock *diskBlock = new TAbstractDiskBlock(blockIndex, getVolumeControlBlock().getEntriesPerDiskBlock());
    diskBlocks[blockIndex] = diskBlock;

    getVolumeControlBlock().updateFreeDataBlock(blockIndex, false);

    return reinterpret_cast<TAbstractDiskBlock*>(diskBlock);
}
