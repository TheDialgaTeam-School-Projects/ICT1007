#include "VolumeControlBlock.h"

// ReSharper disable CppUseAuto

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

int VolumeControlBlock::getTotalFreeDataBlocks() const
{
    int result = 0;

    for (bool freeDataBlock : freeDataBlocks)
    {
        if (!freeDataBlock)
            continue;

        result++;
    }

    return result;
}

vector<int> VolumeControlBlock::getFreeDataBlocks(const int entriesSize, const DiskAllocationMethod diskAllocationMethod, int *speed)
{
    vector<int> result;
    int currentSize = 0;

    if (diskAllocationMethod == Indexed)
        currentSize -= entriesPerDiskBlock;

    for (auto i = 0; i < totalDiskBlock; i++)
    {
        if (freeDataBlocks[i])
        {
            result.push_back(i);

            if (diskAllocationMethod == Linked)
                currentSize += entriesPerDiskBlock - 1;
            else
                currentSize += entriesPerDiskBlock;
        }
        else
        {
            if (diskAllocationMethod == Contiguous || diskAllocationMethod == Custom)
            {
                result.clear();
                currentSize = 0;
            }
        }

        (*speed)++;

        if (currentSize >= entriesSize)
            return result;
    }

    if (currentSize < entriesSize)
        result.clear();

    return result;
}

void VolumeControlBlock::updateFreeDataBlock(const int blockIndex, const bool free)
{
    freeDataBlocks[blockIndex] = free;
}
