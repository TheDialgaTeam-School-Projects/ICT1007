#pragma once
#include <vector>

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

    void updateFreeDataBlock(int blockIndex, bool free);
};
