#include "VolumeControlBlock.h"

VolumeControlBlock::VolumeControlBlock(const int totalDiskEntries, const int entriesPerDiskBlock)
{
    this->entriesPerDiskBlock = entriesPerDiskBlock;
    totalDiskBlock = totalDiskEntries / entriesPerDiskBlock;
    this->totalDiskEntries = totalDiskBlock * entriesPerDiskBlock;
    freeDataBlocks.assign(totalDiskBlock, true);
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

vector<int> VolumeControlBlock::getFreeDataBlocks(const int entriesSize, int *speed)
{
    vector<int> result;
    auto currentSize = 0;

    for (auto i = 0; i < totalDiskBlock; i++)
    {
        if (freeDataBlocks[i])
        {
            result.push_back(i);
            currentSize += entriesPerDiskBlock;
        }
        else
        {
            result.clear();
            currentSize = 0;
        }

        (*speed)++;

        if (currentSize >= entriesSize)
            return result;
    }

    return result;
}

vector<int> VolumeControlBlock::getFreeDataBlocksForLinked(const int entriesSize, int *speed)
{
    vector<int> result;
    auto currentSize = 0;

    for (auto i = 0; i < totalDiskBlock; i++)
    {
        if (freeDataBlocks[i])
        {
            result.push_back(i);
            currentSize += entriesPerDiskBlock - 1;
        }

        (*speed)++;

        if (currentSize >= entriesSize)
            return result;
    }

    return result;
}

void VolumeControlBlock::updateFreeDataBlock(const int blockIndex, const bool free)
{
    freeDataBlocks[blockIndex] = free;
}
