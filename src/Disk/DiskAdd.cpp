#include "Disk.h"

#include <iostream>

#include "Block/ContiguousDiskBlock.h"
#include "Block/IndexedDiskBlock.h"
#include "Block/LinkedDiskBlock.h"

void Disk::addContiguousFile(const int fileName, vector<int> &data)
{
    int speed = 0;
    vector<int> freeBlockIndex = getVolumeControlBlock().getFreeDataBlocks(data.size() - 1, &speed);

    // 

    for (int i = 0; i < freeBlockIndex.size(); i++)
    {
        ContiguousDiskBlock *block = addDiskBlock<ContiguousDiskBlock>(freeBlockIndex[i]);

        for (int j = 0; j < block->getBlockSize(); j++)
        {
            if (j + 1 + i * block->getBlockSize() < data.size())
                (*block)[j] = data[j + 1 + i * block->getBlockSize()];
        }
    }

    ContiguousFileInformation *file = getDirectoryBlock().addFile<ContiguousFileInformation>();
    file->setFileName(fileName);
    file->setStartBlockIndex(freeBlockIndex[0]);
    file->setLength(data.size() - 1);
}

void Disk::addLinkedFile(const int fileName, vector<int> &data)
{
    int speed = 0;
    int useableEntries = 0;

    //THIS METHOD DOES NOT ALLOCATE ENOUGH BLOCKS FOR LINKED METHOD JIEMING HELP.
    vector<int> freeBlockIndex = getVolumeControlBlock().getFreeDataBlocksForLinked(data.size(), &speed);
    if (freeBlockIndex.size() == 0)
    {
        //add validation
    }
    int blockSize = getVolumeControlBlock().getEntriesPerDiskBlock() - 1;
    if (data.size() - 1 > blockSize)
    {
        useableEntries = (data.size() - 1) / blockSize;
        if ((((data.size() - 1) % blockSize) > 0))
        {
            useableEntries += 1;
        }
    }
    else if (data.size() - 1 <= blockSize)
    {
        useableEntries += 1;
    }

    int size = 0;
    for (int i = 0; i < useableEntries; ++i)
    {
        int z = i;
        LinkedDiskBlock *linkedBlock = addDiskBlock<LinkedDiskBlock>(freeBlockIndex[i]);
        for (int j = 0; j < linkedBlock->getBlockSize() - 1; ++j)
        {
            if (size < data.size() - 1)
            {
                (*linkedBlock)[j] = data[size + 1];
                size++;
            }
        }
        if (size < data.size() - 1)
        {
            z ++;
            (*linkedBlock)[linkedBlock->getBlockSize() - 1] = freeBlockIndex[z];
            //if reached last block, add pointer plus 1
        }
    }

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
    vector<int> freeBlockIndex = getVolumeControlBlock().getFreeDataBlocks(1, &speed);
    for (int i = 0; i < freeBlockIndex.size(); i++)
    {
        IndexedDiskBlock *indexblock = addDiskBlock<IndexedDiskBlock>(freeBlockIndex[i]);

        vector<int> freeBlockIndex2 = getVolumeControlBlock().getFreeDataBlocks(data.size() - 1, &speed);
        for (int j = 0; j < indexblock->getBlockSize(); j++)
        {
            if (j < freeBlockIndex2.size())
                (*indexblock)[j] = freeBlockIndex2[j];
        }

        for (int k = 0; k < freeBlockIndex2.size(); k++)
        {
            ContiguousDiskBlock *block = addDiskBlock<ContiguousDiskBlock>(freeBlockIndex2[k]);

            for (int dataIndex = 0; dataIndex < block->getBlockSize(); dataIndex++)
            {
                if (dataIndex + 1 + k * block->getBlockSize() < data.size())
                    (*block)[dataIndex] = data[dataIndex + 1 + k * block->getBlockSize()];
            }
        }

        IndexedFileInformation *file = getDirectoryBlock().addFile<IndexedFileInformation>();
        file->setFileName(data[0]);
        file->setIndexBlockIndex(indexblock->getBlockIndex());
    }
}

void Disk::addCustomFile(int fileName, vector<int> &data)
{
    //TODO: implement Custom allocation
}
