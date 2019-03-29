#include "Disk.h"

#include <iostream>
#include <iomanip>
#include <sstream>

#include "../Config.h"
#include "Block/ContiguousDiskBlock.h"
#include "Block/SuperDiskBlock.h"
#include "Block/SuperBlock/DirectoryBlock/ContiguousFileInformation.h"
#include <algorithm>

using std::cout;

using std::endl;
using std::fixed;
using std::left;
using std::setprecision;
using std::setw;

using std::pair;

// ReSharper disable CppUseAuto

void Disk::printDiskMapHeader()
{
    cout
        << "=======================================================================================================================" << endl
        << "DISK MAP (For optimal reading, please set your console buffer width to at least 120 in length." << endl
        << "=======================================================================================================================" << endl
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
    const auto blockIndex = index / getVolumeControlBlock().getEntriesPerDiskBlock();

    if (diskBlocks.count(blockIndex) > 0)
        return diskBlocks[blockIndex]->toString(index - blockIndex * getVolumeControlBlock().getEntriesPerDiskBlock());

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
    return getDiskBlock<SuperDiskBlock>(0)->getVolumeControlBlock();
}

DirectoryBlock &Disk::getDirectoryBlock()
{
    return getDiskBlock<SuperDiskBlock>(0)->getDirectoryBlock();
}

void Disk::addFile(const int fileName, vector<int> &data)
{
    switch (diskAllocationMethod)
    {
        case Contiguous:
            addContiguousFile(fileName, data);
            break;

        case Linked:
            addLinkedFile(fileName, data);
            break;

        case Indexed:
            addIndexedFile(fileName, data);
            break;

        case Custom:
            addCustomFile(fileName, data);
            break;
    }
}

void Disk::readFile(const int data)
{
    switch (diskAllocationMethod)
    {
        case Contiguous:
            readContiguousFile(data);
            break;

        case Linked:
            readLinkedFile(data);
            break;

        case Indexed:
            readIndexedFile(data);
            break;

        case Custom:
            readCustomFile(data);
            break;
    }
}

void Disk::deleteFile(const int fileName)
{
    DirectoryBlock &directoryBlock = getDirectoryBlock();

    for (auto i = 0ull; i < directoryBlock.getFilesInformation().size(); i++)
    {
        if (directoryBlock[i]->getFileName() != fileName)
            continue;

        vector<int> blocksFreed;

        switch (diskAllocationMethod)
        {
            case Contiguous:
                deleteContiguousFile(reinterpret_cast<ContiguousFileInformation*>(directoryBlock[i]), blocksFreed);
                break;

            case Linked:
                deleteLinkedFile(reinterpret_cast<LinkedFileInformation*>(directoryBlock[i]), blocksFreed);
                break;

            case Indexed:
                deleteIndexedFile(reinterpret_cast<IndexedFileInformation*>(directoryBlock[i]), blocksFreed);
                break;

            case Custom:
                deleteCustomFile(reinterpret_cast<CustomFileInformation*>(directoryBlock[i]), blocksFreed);
                break;
        }

        directoryBlock.deleteFile(fileName);

        cout << "Deleted file " << fileName << " and freed ";

        for (uint64_t j = 0; j < blocksFreed.size(); j++)
        {
            if (j == 0)
                cout << "B" << blocksFreed[j];
            else
                cout << ", B" << blocksFreed[j];
        }

        cout << endl << endl;

        return;
    }

    cout << "Requested file " << fileName << " could not be found." << endl << endl;
}

bool Disk::deleteDiskBlock(const int blockIndex)
{
    // Block not found.
    if (diskBlocks.count(blockIndex) == 0)
        return false;

    // Block is found. Delete away.
    delete diskBlocks[blockIndex];
    diskBlocks.erase(blockIndex);

    // Update the VCB to tell that this block is free to use.
    getVolumeControlBlock().updateFreeDataBlock(blockIndex, true);
    return true;
}

void Disk::reformatDisk(const int totalDiskEntries, const int entriesPerDiskBlock, const DiskAllocationMethod diskAllocationMethod)
{
    dispose();

    this->diskAllocationMethod = diskAllocationMethod;

    SuperDiskBlock *superBlockData = new SuperDiskBlock(totalDiskEntries, entriesPerDiskBlock, diskAllocationMethod);
    diskBlocks[0] = superBlockData;

    getVolumeControlBlock().updateFreeDataBlock(0, false);

    cout << "Disk has successfully formatted. You may do disk operation now." << endl << endl;
}

void Disk::defragDisk()
{
    // Defrag operation is only available in custom for safety reasons.
    if (diskAllocationMethod != Custom)
        return;

    int currentFreeIndex = 0;

    for (int defragProgress = 0; defragProgress < getVolumeControlBlock().getTotalDiskBlock(); defragProgress++)
    {
        if (diskBlocks.count(defragProgress) > 0)
        {
            // Block exist and in the correct sequence.
            if (defragProgress == currentFreeIndex)
            {
                currentFreeIndex++;
                continue;
            }

            // Block exist but it is not in the correct sequence, move to the correct sequence now.
            ContiguousDiskBlock *block = getDiskBlock<ContiguousDiskBlock>(defragProgress);

            for (uint64_t i = 0; i < getDirectoryBlock().getFilesInformation().size(); i++)
            {
                CustomFileInformation *fileInformation = reinterpret_cast<CustomFileInformation*>(getDirectoryBlock()[i]);

                if (fileInformation->getStartBlockIndex() != defragProgress)
                    continue;

                fileInformation->setStartBlockIndex(currentFreeIndex);
                break;
            }

            // Move to the free block.
            diskBlocks[currentFreeIndex] = block;
            getVolumeControlBlock().updateFreeDataBlock(currentFreeIndex, false);

            // Remove the old block away.
            diskBlocks.erase(defragProgress);
            getVolumeControlBlock().updateFreeDataBlock(defragProgress, true);

            currentFreeIndex++;
        }
    }
}

void Disk::printDiskMap()
{
    printDiskMapHeader();

    int totalDiskEntries = getVolumeControlBlock().getTotalDiskEntries();
    int index = 0;
    int index2 = totalDiskEntries / 3 + (totalDiskEntries % 3 > 0 ? 1 : 0);
    int index3 = index2 * 2;

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

    int diskSpaceLeft = getVolumeControlBlock().getTotalFreeDataBlocks();

    const uint64_t directorySize = getVolumeControlBlock().getEntriesPerDiskBlock() - 1;

    if (getDirectoryBlock().getFilesInformation().size() + 1 > directorySize)
        diskSpaceLeft = 0;

    const double diskSpacePercentage = static_cast<double>(diskSpaceLeft) / static_cast<double>(getVolumeControlBlock().getTotalDiskBlock()) * 100;
    const int totalDiskBlockUsed = getVolumeControlBlock().getTotalDiskBlock() - getVolumeControlBlock().getTotalFreeDataBlocks();
    
    cout
        << endl
        << "=======================================================================================================================" << endl
        << "Disk Information (Volume Control Block)" << endl
        << "=======================================================================================================================" << endl
        << "Total Files added: " << getDirectoryBlock().getFilesInformation().size() << endl
        << "Total Size (Including internal fragmentation): " << totalDiskBlockUsed << " Blocks (" << totalDiskBlockUsed * getVolumeControlBlock().getEntriesPerDiskBlock() << " entries)" << endl
        << "Disk Space Left: " << diskSpaceLeft << " / " << getVolumeControlBlock().getTotalDiskBlock() << " (" << fixed << setprecision(2) << diskSpacePercentage << "%)" << endl
    ;
}

void Disk::dispose()
{
    for (pair<const int, AbstractDiskBlock*> &diskBlock : diskBlocks)
        delete diskBlock.second;

    diskBlocks.clear();
}
