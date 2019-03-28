#pragma once
#include <vector>

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

    vector<int> getFreeDataBlocks(int entriesSize, int *speed);
    vector<int> getFreeDataBlocksForLinked(int entriesSize, int *speed);

    void updateFreeDataBlock(int blockIndex, bool free);
};
