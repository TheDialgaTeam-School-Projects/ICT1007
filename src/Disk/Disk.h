#pragma once
#include <map>

#include "DiskAllocationMethod.h"
#include "Block/AbstractDiskBlock.h"
#include "Block/SuperBlock/DirectoryBlock.h"
#include "Block/SuperBlock/VolumeControlBlock.h"

using std::map;
using std::string;

class Disk
{
    DiskAllocationMethod diskAllocationMethod = Contiguous;
    map<int, AbstractDiskBlock*> diskBlocks;

    static void printDiskMapHeader();
    static void printDiskMapValues(int index, int block, const string &value, bool lastBit = false);

    string getDiskEntriesValue(int index);

public:
    Disk(int totalDiskEntries, int entriesPerDiskBlock, DiskAllocationMethod diskAllocationMethod);
    ~Disk();

    VolumeControlBlock &getVolumeControlBlock();
    DirectoryBlock &getDirectoryBlock();

    void reformatDisk(int totalDiskEntries, int entriesPerDiskBlock, DiskAllocationMethod diskAllocationMethod);
    void printDiskMap();
    void dispose();

    Disk(const Disk &) = default;
    Disk &operator=(const Disk &) = default;
    Disk(Disk &&) = default;
    Disk &operator=(Disk &&) = default;
};
