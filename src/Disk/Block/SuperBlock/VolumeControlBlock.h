#pragma once
#include <vector>
#include "../../DiskAllocationMethod.h"

using std::vector;

class VolumeControlBlock
{
    int totalDiskEntries;
    int entriesPerDiskBlock;
    int totalDiskBlock;

    std::vector<bool> freeDataBlocks;

public:
    VolumeControlBlock(int totalDiskEntries, int entriesPerDiskBlock);

    int getTotalDiskEntries() const;
    int getEntriesPerDiskBlock() const;
    int getTotalDiskBlock() const;
    int getTotalFreeDataBlocks() const;

    vector<int> getFreeDataBlocks(int entriesSize, DiskAllocationMethod diskAllocationMethod, int *speed);

    void updateFreeDataBlock(int blockIndex, bool free);
};
