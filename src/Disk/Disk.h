#pragma once
#include <map>

#include "DiskAllocationMethod.h"
#include "Block/AbstractDiskBlock.h"
#include "Block/SuperBlock/DirectoryBlock.h"
#include "Block/SuperBlock/VolumeControlBlock.h"
#include "Block/SuperBlock/DirectoryBlock/ContiguousFileInformation.h"
#include "Block/SuperBlock/DirectoryBlock/LinkedFileInformation.h"
#include "Block/SuperBlock/DirectoryBlock/IndexedFileInformation.h"
#include "Block/SuperBlock/DirectoryBlock/CustomFileInformation.h"

using std::map;
using std::string;

class Disk
{
    DiskAllocationMethod diskAllocationMethod = Contiguous;
    map<int, AbstractDiskBlock*> diskBlocks;

    static void printDiskIsFull(int fileName);

    void addContiguousFile(int fileName, vector<int> &data);
    void addLinkedFile(int fileName, vector<int> &data);
    void addIndexedFile(int fileName, vector<int> &data);
    void addCustomFile(int fileName, vector<int> &data);

    void readContiguousFile(int data);
    void readLinkedFile(int data);
    void readIndexedFile(int data);
    void readCustomFile(int data);

    void deleteContiguousFile(ContiguousFileInformation *fileInformation, vector<int> &blocksFreed);
    void deleteLinkedFile(LinkedFileInformation *fileInformation, vector<int> &blocksFreed);
    void deleteIndexedFile(IndexedFileInformation *fileInformation, vector<int> &blocksFreed);
    void deleteCustomFile(CustomFileInformation *fileInformation, vector<int> &blocksFreed);

    static void printDiskMapHeader();
    static void printDiskMapValues(int index, int block, const string &value, bool lastBit = false);

    string getDiskEntriesValue(int index);

public:
    Disk(int totalDiskEntries, int entriesPerDiskBlock, DiskAllocationMethod diskAllocationMethod);
    ~Disk();

    /**
     * \brief Get the Volume Control Block.
     * \return Volume Control Block
     */
    VolumeControlBlock &getVolumeControlBlock();

    /**
     * \brief Get the directory structure.
     * \return Directory structure.
     */
    DirectoryBlock &getDirectoryBlock();

    /**
     * \brief Add a file into the disk.
     * \param fileName File name to add into the disk.
     * \param data Data to add into the disk.
     */
    void addFile(int fileName, vector<int> &data);

    /**
     * \brief Read a file from the disk.
     * \param data The data to read from the disk.
     */
    void readFile(int data);

    /**
     * \brief Delete a file from the disk.
     * \param fileName The file to delete from the disk.
     */
    void deleteFile(int fileName);

    /**
     * \brief Get the disk block object from the disk.
     * \tparam TAbstractDiskBlock A class that extends AbstractDiskBlock.
     * \param blockIndex The block number to get the block object.
     * \return The disk block object.
     */
    template <class TAbstractDiskBlock>
    TAbstractDiskBlock *getDiskBlock(int blockIndex);

    /**
     * \brief Add a new disk block object into the specified block number.
     * \tparam TAbstractDiskBlock A class that extends AbstractDiskBlock.
     * \param blockIndex The block number to add the block object.
     * \return The disk block object that has been added into the disk.
     */
    template <class TAbstractDiskBlock>
    TAbstractDiskBlock *addDiskBlock(int blockIndex);

    /**
     * \brief Delete the disk block object from the disk.
     * \param blockIndex The block number to delete the block object.
     * \return true if it delete successfully else false.
     */
    bool deleteDiskBlock(int blockIndex);

    /**
     * \brief Reformat the disk into the selection disk allocation method.
     * \param totalDiskEntries The total disk entries this disk has.
     * \param entriesPerDiskBlock The amount of entries per block.
     * \param diskAllocationMethod The disk allocation method.
     */
    void reformatDisk(int totalDiskEntries, int entriesPerDiskBlock, DiskAllocationMethod diskAllocationMethod);

    /**
     * \brief Defragment the disk.
     */
    void defragDisk();

    /**
     * \brief Print the disk map on the console.
     */
    void printDiskMap();

    /**
     * \brief Dispose any allocated memory in the disk.
     */
    void dispose();

    Disk(const Disk &) = default;
    Disk &operator=(const Disk &) = default;
    Disk(Disk &&) = default;
    Disk &operator=(Disk &&) = default;
};

template <class TAbstractDiskBlock>
TAbstractDiskBlock *Disk::getDiskBlock(const int blockIndex)
{
    // Ensure that TAbstractDiskBlock is a class that extends AbstractDiskBlock.
    static_assert(std::is_base_of<AbstractDiskBlock, TAbstractDiskBlock>::value, "TAbstractDiskBlock requires a class that extends AbstractDiskBlock.");

    // Handle nullptr exception.
    if (diskBlocks.count(blockIndex) == 0)
        return nullptr;

    return reinterpret_cast<TAbstractDiskBlock*>(diskBlocks[blockIndex]);
}

template <class TAbstractDiskBlock>
TAbstractDiskBlock *Disk::addDiskBlock(const int blockIndex)
{
    // Ensure that TAbstractDiskBlock is a class that extends AbstractDiskBlock.
    static_assert(std::is_base_of<AbstractDiskBlock, TAbstractDiskBlock>::value, "TAbstractDiskBlock requires a class that extends AbstractDiskBlock.");

    // Handle out of range exception.
    if (diskBlocks.size() + 1 > getVolumeControlBlock().getTotalDiskBlock())
        return nullptr;

    // Handle nullptr exception.
    if (diskBlocks.count(blockIndex) > 0)
        return nullptr;

    // Create a new block and add into the disk.
    TAbstractDiskBlock *diskBlock = new TAbstractDiskBlock(blockIndex, getVolumeControlBlock().getEntriesPerDiskBlock());
    diskBlocks[blockIndex] = diskBlock;

    // Update the VCB to tell that this block is no longer free to use.
    getVolumeControlBlock().updateFreeDataBlock(blockIndex, false);

    return reinterpret_cast<TAbstractDiskBlock*>(diskBlock);
}
