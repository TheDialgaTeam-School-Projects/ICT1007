#include "Disk.h"

#include <iostream>

#include "Block/ContiguousDiskBlock.h"
#include "Block/IndexedDiskBlock.h"
#include "Block/LinkedDiskBlock.h"

using std::cout;

using std::endl;

// ReSharper disable CppUseAuto

void Disk::printDiskIsFull(const int fileName)
{
    cout << "The disk is full and unable to allocate for file " << fileName << endl << endl;
}

void Disk::addContiguousFile(const int fileName, vector<int> &data)
{
    int speed = 0;
    vector<int> freeBlockIndex = getVolumeControlBlock().getFreeDataBlocks(data.size() - 1, Contiguous, &speed);

    const uint64_t directorySize = getVolumeControlBlock().getEntriesPerDiskBlock() - 1;

    if (freeBlockIndex.empty() || getDirectoryBlock().getFilesInformation().size() + 1 > directorySize)
    {
        printDiskIsFull(fileName);
        return;
    }

    cout << "Adding file " << fileName << " and found free ";

    for (uint64_t i = 0; i < freeBlockIndex.size(); i++)
    {
        if (i == 0)
            cout << "B" << freeBlockIndex[i];
        else
            cout << ", B" << freeBlockIndex[i];
    }

    cout << " with " << speed << " access time" << endl << "Added file " << fileName << " at ";

    for (uint64_t i = 0; i < freeBlockIndex.size(); i++)
    {
        ContiguousDiskBlock *block = addDiskBlock<ContiguousDiskBlock>(freeBlockIndex[i]);

        if (i == 0)
            cout << "B" << freeBlockIndex[i] << "(";
        else
            cout << ", B" << freeBlockIndex[i] << "(";

        for (int j = 0; j < block->getBlockSize(); j++)
        {
            if (j + 1 + i * block->getBlockSize() < data.size())
            {
                (*block)[j] = data[j + 1 + i * block->getBlockSize()];

                if (j == 0)
                    cout << data[j + 1 + i * block->getBlockSize()];
                else
                    cout << ", " << data[j + 1 + i * block->getBlockSize()];
            }
        }

        cout << ")";
    }

    cout << endl << endl;

    ContiguousFileInformation *file = getDirectoryBlock().addFile<ContiguousFileInformation>();
    file->setFileName(fileName);
    file->setStartBlockIndex(freeBlockIndex[0]);
    file->setLength(data.size() - 1);
}

void Disk::addLinkedFile(const int fileName, vector<int> &data)
{
    int speed = 0;
    int useableEntries = 0;

    vector<int> freeBlockIndex = getVolumeControlBlock().getFreeDataBlocks(data.size() - 1, Linked, &speed);

    const uint64_t directorySize = getVolumeControlBlock().getEntriesPerDiskBlock() - 1;

    if (freeBlockIndex.empty() || getDirectoryBlock().getFilesInformation().size() + 1 > directorySize)
    {
        printDiskIsFull(fileName);
        return;
    }

    cout << "Adding file " << fileName << " and found free ";

    for (uint64_t i = 0; i < freeBlockIndex.size(); i++)
    {
        if (i == 0)
            cout << "B" << freeBlockIndex[i];
        else
            cout << ", B" << freeBlockIndex[i];
    }

    cout << " with " << speed << " access time" << endl << "Added file " << fileName << " at ";

    const uint64_t blockSize = getVolumeControlBlock().getEntriesPerDiskBlock() - 1;

    if (data.size() - 1 > blockSize)
    {
        useableEntries = (data.size() - 1) / blockSize;
        if ((data.size() - 1) % blockSize > 0)
        {
            useableEntries += 1;
        }
    }
    else if (data.size() - 1 <= blockSize)
    {
        useableEntries += 1;
    }

    uint64_t size = 0;

    for (int i = 0; i < useableEntries; ++i)
    {
        if (i == 0)
            cout << "B" << freeBlockIndex[i] << "(";
        else
            cout << ", B" << freeBlockIndex[i] << "(";

        int z = i;
        LinkedDiskBlock *linkedBlock = addDiskBlock<LinkedDiskBlock>(freeBlockIndex[i]);

        for (int j = 0; j < linkedBlock->getBlockSize() - 1; ++j)
        {
            if (size < data.size() - 1)
            {
                if (j == 0)
                    cout << data[size + 1];
                else
                    cout << ", " << data[size + 1];

                (*linkedBlock)[j] = data[size + 1];
                size++;
            }
        }

        if (size < data.size() - 1)
        {
            z++;
            (*linkedBlock)[linkedBlock->getBlockSize() - 1] = freeBlockIndex[z];
            //if reached last block, add pointer plus 1
            cout << ", B" << freeBlockIndex[z];
        }

        cout << ")";
    }

    cout << endl << endl;

    LinkedFileInformation *file = getDirectoryBlock().addFile<LinkedFileInformation>();
    file->setFileName(data[0]);
    file->setEndBlockIndex(freeBlockIndex[useableEntries - 1]);

    int counter = useableEntries;

    for (int k = 0; k < useableEntries; k++)
    {
        counter--;
    }

    file->setStartBlockIndex(freeBlockIndex[counter]);
}

void Disk::addIndexedFile(const int fileName, vector<int> &data)
{
    int speed = 0;
    vector<int> freeBlockIndex = getVolumeControlBlock().getFreeDataBlocks(data.size() - 1, Indexed, &speed);

    const uint64_t directorySize = getVolumeControlBlock().getEntriesPerDiskBlock() - 1;

    if (freeBlockIndex.empty() || getDirectoryBlock().getFilesInformation().size() + 1 > directorySize || freeBlockIndex.size() - 1 > getVolumeControlBlock().getEntriesPerDiskBlock())
    {
        printDiskIsFull(fileName);
        return;
    }

    cout << "Adding file " << fileName << " and found free ";

    for (uint64_t i = 0; i < freeBlockIndex.size(); i++)
    {
        if (i == 0)
            cout << "B" << freeBlockIndex[i];
        else
            cout << ", B" << freeBlockIndex[i];
    }

    cout << " with " << speed << " access time" << endl << "Added file " << fileName << " at B" << freeBlockIndex[0] << "(";

    for (uint64_t i = 1; i < freeBlockIndex.size(); i++)
    {
        if (i == 1)
            cout << "B" << freeBlockIndex[i];
        else
            cout << ", B" << freeBlockIndex[i];
    }

    cout << ")";

    IndexedDiskBlock *indexedDiskBlock = addDiskBlock<IndexedDiskBlock>(freeBlockIndex[0]);

    for (uint64_t i = 1; i < freeBlockIndex.size(); i++)
    {
        (*indexedDiskBlock)[i - 1] = freeBlockIndex[i];
        cout << ", B" << freeBlockIndex[i] << "(";

        ContiguousDiskBlock *block = addDiskBlock<ContiguousDiskBlock>(freeBlockIndex[i]);

        for (int j = 0; j < block->getBlockSize(); j++)
        {
            if (j + 1 + (i - 1) * block->getBlockSize() < data.size())
            {
                (*block)[j] = data[j + 1 + (i - 1) * block->getBlockSize()];

                if (j == 0)
                    cout << data[j + 1 + (i - 1) * block->getBlockSize()];
                else
                    cout << ", " << data[j + 1 + (i - 1) * block->getBlockSize()];
            }
        }

        cout << ")";
    }

    cout << endl << endl;

    IndexedFileInformation *file = getDirectoryBlock().addFile<IndexedFileInformation>();
    file->setFileName(data[0]);
    file->setIndexBlockIndex(indexedDiskBlock->getBlockIndex());
}

void Disk::addCustomFile(const int fileName, vector<int> &data)
{
    defragDisk();
    addContiguousFile(fileName, data);
}
