#include "SuperDiskBlock.h"

SuperDiskBlock::SuperDiskBlock(const int totalDiskEntries, const int entriesPerDiskBlock, const DiskAllocationMethod diskAllocationMethod):
    AbstractDiskBlock(0, 0),
    volumeControlBlock(totalDiskEntries, entriesPerDiskBlock),
    directoryBlock(entriesPerDiskBlock - 1, diskAllocationMethod)
{
    blockSize = volumeControlBlock.getEntriesPerDiskBlock();
}

VolumeControlBlock &SuperDiskBlock::getVolumeControlBlock()
{
    return volumeControlBlock;
}

const VolumeControlBlock &SuperDiskBlock::getVolumeControlBlock() const
{
    return volumeControlBlock;
}

DirectoryBlock &SuperDiskBlock::getDirectoryBlock()
{
    return directoryBlock;
}

const DirectoryBlock &SuperDiskBlock::getDirectoryBlock() const
{
    return directoryBlock;
}

string SuperDiskBlock::toString(const int index)
{
    if (index == 0)
        return "VOLUME CONTROL BLOCK";

    return directoryBlock.toString(index - 1);
}
