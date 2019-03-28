#include "Disk.h"

#include "Block/LinkedDiskBlock.h"
#include "Block/IndexedDiskBlock.h"

void Disk::deleteContiguousFile(ContiguousFileInformation *fileInformation, vector<int> &blocksFreed)
{
    auto currentBlockIndex = fileInformation->getStartBlockIndex();
    auto totalFileLength = fileInformation->getLength();

    while (totalFileLength > 0)
    {
        auto *diskBlock = diskBlocks[currentBlockIndex];
        totalFileLength -= diskBlock->getBlockSize();

        delete diskBlock;
        diskBlocks.erase(currentBlockIndex);

        getVolumeControlBlock().updateFreeDataBlock(currentBlockIndex, true);

        blocksFreed.push_back(currentBlockIndex);

        currentBlockIndex++;
    }
}

void Disk::deleteLinkedFile(LinkedFileInformation *fileInformation, vector<int> &blocksFreed)
{
    int index = fileInformation->getStartBlockIndex();
    vector<int> toDelete;
    toDelete.push_back(fileInformation->getStartBlockIndex());

    do
    {
        LinkedDiskBlock *block = reinterpret_cast<LinkedDiskBlock*>(diskBlocks[index]);

        index = (*block)[getVolumeControlBlock().getEntriesPerDiskBlock() - 1];

        if (index != -1)
            toDelete.push_back(index);
    } while (index != -1);

    do
    {
        index = toDelete.back();
        toDelete.pop_back();

        auto *diskBlock = diskBlocks[index];
        delete diskBlock;
        diskBlocks.erase(index);

        getVolumeControlBlock().updateFreeDataBlock(index, true);

        blocksFreed.push_back(index);
    } while (toDelete.size() != 0);
}

void Disk::deleteIndexedFile(IndexedFileInformation *fileInformation, vector<int> &blocksFreed)
{
    IndexedDiskBlock *block = reinterpret_cast<IndexedDiskBlock*>(diskBlocks[fileInformation->getIndexBlockIndex()]);

    for (int i = 0; i != getVolumeControlBlock().getEntriesPerDiskBlock(); i++)
    {
        if ((*block)[i] == -1)
            break;

        auto *diskBlock = diskBlocks[(*block)[i]];
        delete diskBlock;
        diskBlocks.erase((*block)[i]);

        getVolumeControlBlock().updateFreeDataBlock((*block)[i], true);
        blocksFreed.push_back((*block)[i]);
    }

    delete block;
    diskBlocks.erase(fileInformation->getIndexBlockIndex());
}

void Disk::deleteCustomFile(CustomFileInformation *fileInformation, vector<int> &blocksFreed)
{
    auto currentBlockIndex = fileInformation->getStartBlockIndex();
    auto totalFileLength = fileInformation->getLength();

    while (totalFileLength > 0)
    {
        auto *diskBlock = diskBlocks[currentBlockIndex];
        totalFileLength -= diskBlock->getBlockSize();

        delete diskBlock;
        diskBlocks.erase(currentBlockIndex);

        getVolumeControlBlock().updateFreeDataBlock(currentBlockIndex, true);

        blocksFreed.push_back(currentBlockIndex);

        currentBlockIndex++;
    }
}
