#include "VolumeControlBlock.h"

VolumeControlBlock::VolumeControlBlock(const int totalDiskEntries, const int entriesPerDiskBlock)
{
    this->entriesPerDiskBlock = entriesPerDiskBlock;
    totalDiskBlock = totalDiskEntries / entriesPerDiskBlock;
    this->totalDiskEntries = totalDiskBlock * entriesPerDiskBlock;
    freeDataBlocks.assign(totalDiskBlock, false);
}

int VolumeControlBlock::getTotalDiskEntries() const
{
    return totalDiskEntries;
}

int VolumeControlBlock::getEntriesPerDiskBlock() const
{
    return entriesPerDiskBlock;
}

int VolumeControlBlock::getTotalDiskBlock() const
{
    return totalDiskBlock;
}

void VolumeControlBlock::updateFreeDataBlock(const int blockIndex, const bool free)
{
    freeDataBlocks[blockIndex] = free;
}
