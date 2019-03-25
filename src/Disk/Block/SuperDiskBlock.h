#pragma once
#include "AbstractDiskBlock.h"
#include "SuperBlock/DirectoryBlock.h"
#include "SuperBlock/VolumeControlBlock.h"

class SuperDiskBlock final : public AbstractDiskBlock
{
    VolumeControlBlock volumeControlBlock;
    DirectoryBlock directoryBlock;

public:
    SuperDiskBlock(int totalDiskEntries, int entriesPerDiskBlock, DiskAllocationMethod diskAllocationMethod);

    VolumeControlBlock &getVolumeControlBlock();
    const VolumeControlBlock &getVolumeControlBlock() const;

    DirectoryBlock &getDirectoryBlock();
    const DirectoryBlock &getDirectoryBlock() const;

    string toString(int index) override;
};

using SuperBlockData = SuperDiskBlock;
