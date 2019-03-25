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

    VolumeControlBlock &getVolumeControlBlock();
    DirectoryBlock &getDirectoryBlock();

    void addFile(int fileName, vector<int> &data);
    void readFile(int data);
    void deleteFile(int fileName);

    void reformatDisk(int totalDiskEntries, int entriesPerDiskBlock, DiskAllocationMethod diskAllocationMethod);
    void printDiskMap();
    void dispose();

    Disk(const Disk &) = default;
    Disk &operator=(const Disk &) = default;
    Disk(Disk &&) = default;
    Disk &operator=(Disk &&) = default;
};
