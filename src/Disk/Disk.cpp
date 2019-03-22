#include "Disk.h"

#include <iostream>
#include <iomanip>

#include "../Config.h"
#include "Block/ContiguousDiskBlock.h"
#include "Block/SuperDiskBlock.h"

using std::cout;

using std::endl;
using std::left;
using std::setw;

void Disk::printDiskMapHeader()
{
    cout
        << "Index" << setw(2) << ' ' << "Block" << setw(2) << ' ' << "Block Data" << setw(10) << ' '
        << setw((CONSOLE_WIDTH - 34 * 3) / 2) << ' '
        << "Index" << setw(2) << ' ' << "Block" << setw(2) << ' ' << "Block Data" << setw(10) << ' '
        << setw((CONSOLE_WIDTH - 34 * 3) / 2) << ' '
        << "Index" << setw(2) << ' ' << "Block" << setw(2) << ' ' << "Block Data" << setw(10) << ' '
        << endl;
}

void Disk::printDiskMapValues(const int index, const int block, const string &value, const bool lastBit)
{
    cout << left << setw(5) << index << setw(2) << ' ' << left << setw(5) << block << setw(2) << ' ' << left << setw(20) << value;

    if (!lastBit)
        cout << setw((CONSOLE_WIDTH - 34 * 3) / 2) << ' ';
    else
        cout << endl;
}

string Disk::getDiskEntriesValue(const int index)
{
    if (index == 0)
        return "VOLUME CONTROL BLOCK";

    return "";
}

Disk::Disk(const int totalDiskEntries, const int entriesPerDiskBlock, const DiskAllocationMethod diskAllocationMethod)
{
    reformatDisk(totalDiskEntries, entriesPerDiskBlock, diskAllocationMethod);
}

Disk::~Disk()
{
    dispose();
}

VolumeControlBlock &Disk::getVolumeControlBlock()
{
    return reinterpret_cast<SuperDiskBlock*>(diskBlocks[0])->getVolumeControlBlock();
}

DirectoryBlock &Disk::getDirectoryBlock()
{
    return reinterpret_cast<SuperDiskBlock*>(diskBlocks[0])->getDirectoryBlock();
}

void Disk::reformatDisk(const int totalDiskEntries, const int entriesPerDiskBlock, const DiskAllocationMethod diskAllocationMethod)
{
    dispose();

    this->diskAllocationMethod = diskAllocationMethod;

    // Step 1: Generate Super Block.
    // Super Block contains Volume Control Block and Directory Block.
    // Directory Block contains file information.
    const auto superBlockData = new SuperDiskBlock(totalDiskEntries, entriesPerDiskBlock, diskAllocationMethod);
    diskBlocks[0] = superBlockData;

    auto volumeControlBlock = superBlockData->getVolumeControlBlock();

    // Step 2: Generate Data Block based on allocation methods.
    for (auto i = 0; i < volumeControlBlock.getTotalDiskBlock() - 1; i++)
    {
        AbstractDiskBlock *dataBlock = nullptr;
        const auto size = volumeControlBlock.getEntriesPerDiskBlock();

        switch (diskAllocationMethod)
        {
            case Contiguous:
                dataBlock = new ContiguousDiskBlock(i + 1, size);
                break;

            case Linked:
                dataBlock = new ContiguousDiskBlock(i + 1, size);
                break;

            case Indexed:
                dataBlock = new ContiguousDiskBlock(i + 1, size);
                break;

            case Custom:
                dataBlock = new ContiguousDiskBlock(i + 1, size);
                break;
        }

        volumeControlBlock.updateFreeDataBlock(i + 1, true);
        diskBlocks[i + 1] = dataBlock;
    }

    cout << "Disk has successfully formatted. You may do disk operation now." << endl;
}

void Disk::printDiskMap()
{
    printDiskMapHeader();

    auto totalDiskEntries = getVolumeControlBlock().getTotalDiskEntries();
    auto index = 0;
    auto index2 = totalDiskEntries / 3 + (totalDiskEntries % 3 > 0 ? 1 : 0);
    auto index3 = index2 * 2;

    while (totalDiskEntries >= 3)
    {
        printDiskMapValues(index, index / getVolumeControlBlock().getEntriesPerDiskBlock(), getDiskEntriesValue(index), false);
        printDiskMapValues(index2, index2 / getVolumeControlBlock().getEntriesPerDiskBlock(), getDiskEntriesValue(index2), false);
        printDiskMapValues(index3, index3 / getVolumeControlBlock().getEntriesPerDiskBlock(), getDiskEntriesValue(index3), true);

        totalDiskEntries -= 3;
        index++;
        index2++;
        index3++;
    }

    if (totalDiskEntries == 1)
        printDiskMapValues(index, index / getVolumeControlBlock().getEntriesPerDiskBlock(), getDiskEntriesValue(index), false);
    else if (totalDiskEntries == 2)
    {
        printDiskMapValues(index, index / getVolumeControlBlock().getEntriesPerDiskBlock(), getDiskEntriesValue(index), false);
        printDiskMapValues(index2, index2 / getVolumeControlBlock().getEntriesPerDiskBlock(), getDiskEntriesValue(index2), false);
    }
}

void Disk::dispose()
{
    for (auto &diskBlock : diskBlocks)
        delete diskBlock.second;

    diskBlocks.clear();
}
