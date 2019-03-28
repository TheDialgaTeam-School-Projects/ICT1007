#include "Disk.h"

#include <iostream>
#include <iomanip>
#include <sstream>

#include "../Config.h"
#include "Block/ContiguousDiskBlock.h"
#include "Block/IndexedDiskBlock.h"
#include "Block/LinkedDiskBlock.h"
#include "Block/SuperDiskBlock.h"
#include "Block/SuperBlock/DirectoryBlock/ContiguousFileInformation.h"
#include "tempOpen.h"

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
    return reinterpret_cast<SuperDiskBlock*>(diskBlocks[0])->getVolumeControlBlock();
}

DirectoryBlock &Disk::getDirectoryBlock()
{
    return reinterpret_cast<SuperDiskBlock*>(diskBlocks[0])->getDirectoryBlock();
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

            //    
            //case Custom:
            //    readCustomFile(data);
            //    break;
    }
}

void Disk::readContiguousFile(const int data)
{
    vector<string> strOperration;
    vector<vector<int>> theUltimateVector;
    string word;

    vector<AbstractFileInformation*> fileInfo = getDirectoryBlock().getFilesInformation();
    vector<AbstractBlockData>::iterator i;

    for (auto &x : fileInfo)
    {
        strOperration.push_back(x->toString());
    }

    for (int i = 0; i < strOperration.size(); i++)
    {
        vector<int> myNumbers;
        for (stringstream sst(strOperration[i]); getline(sst, word, ',');)
        {
            myNumbers.push_back(stoi(word));
        }
        theUltimateVector.push_back(myNumbers);
    }

    for (int i = 0; i < theUltimateVector.size(); i++)
    {
        for (int j = 0; j < 1; j++)
        {
            int max = theUltimateVector[i][j] + theUltimateVector[i][j + 2];
            if (data <= max && data >= theUltimateVector[i][j])
            {
                cout << "read file " << theUltimateVector[i][j] << "(" << data << ") from block" << theUltimateVector[i][j + 1] << endl;
                break;
            }
        }
    }
}

void Disk::readLinkedFile(const int data)
{
    vector<string> strOperration;
    vector<vector<int>> theUltimateVector;
    string word;

    vector<AbstractFileInformation*> fileInfo = getDirectoryBlock().getFilesInformation();
    for (auto x : fileInfo)
    {
        strOperration.push_back(x->toString());
    }

    for (int i = 0; i < strOperration.size(); i++)
    {
        vector<int> myNumbers;
        for (stringstream sst(strOperration[i]); getline(sst, word, ',');)
        {
            myNumbers.push_back(stoi(word));
        }
        theUltimateVector.push_back(myNumbers);
    }
    int index;
    for (int i = 0; i < theUltimateVector.size(); i++)
    {
        if (data - theUltimateVector[i][0] < 100 && data - theUltimateVector[i][0] > 0)
        {
            index = theUltimateVector[i][1];
            break;
        }
    }
    do
    {
        LinkedDiskBlock *block = reinterpret_cast<LinkedDiskBlock*>(diskBlocks[index]);
        for (int i = 0; i < getVolumeControlBlock().getEntriesPerDiskBlock(); i++)
        {
            index = (*block)[getVolumeControlBlock().getEntriesPerDiskBlock() - 1];
            if ((*block)[i] == data)
            {
                cout << "Read file " << to_string(data) << " from B" << to_string(index) << endl;
                break;
            }
        }
    } while (index != -1);
}

void Disk::readIndexedFile(const int data)
{
    vector<string> strOperration;
    vector<vector<int>> theUltimateVector;
    string word;

    vector<AbstractFileInformation*> fileInfo = getDirectoryBlock().getFilesInformation();
    for (auto x : fileInfo)
    {
        strOperration.push_back(x->toString());
    }

    for (int i = 0; i < strOperration.size(); i++)
    {
        vector<int> myNumbers;
        for (stringstream sst(strOperration[i]); getline(sst, word, ',');)
        {
            myNumbers.push_back(stoi(word));
        }
        theUltimateVector.push_back(myNumbers);
    }
    int blockIndex;
    for (int i = 0; i < theUltimateVector.size(); i++)
    {
        if (data - theUltimateVector[i][0] < 100 && data - theUltimateVector[i][0] > 0)
        {
            blockIndex = theUltimateVector[i][1];
        }
    }
    IndexedDiskBlock *block = reinterpret_cast<IndexedDiskBlock*>(diskBlocks[blockIndex]);
    for (int i = 0; i != getVolumeControlBlock().getEntriesPerDiskBlock(); i++)
    {
        if ((*block)[i] == -1)
            break;

        auto indexBlockIndex = (*block)[i];

        ContiguousDiskBlock *block_c = reinterpret_cast<ContiguousDiskBlock*>(diskBlocks[indexBlockIndex]);

        for (int i = 0; i != getVolumeControlBlock().getEntriesPerDiskBlock(); i++)
        {
            if ((*block_c)[i] == data)
            {
                cout << "Read file " << to_string(data) << " from B" << to_string(indexBlockIndex) << endl;
            }
        }
    }
    // cout << "File to be read not found" << endl;
}

void Disk::deleteFile(const int fileName)
{
    auto &directoryBlock = getDirectoryBlock();

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

        for (auto j = 0; j < static_cast<int>(blocksFreed.size()); j++)
        {
            if (j == 0)
                cout << "B" << blocksFreed[j];
            else
                cout << ", B" << blocksFreed[j];
        }

        cout << endl;

        return;
    }

    cout << "Requested file " << fileName << " could not be found." << endl;
}

bool Disk::deleteDiskBlock(const int blockIndex)
{
    for (auto &diskBlock : diskBlocks)
    {
        if (diskBlock.first != blockIndex)
            continue;

        delete diskBlock.second;
        diskBlocks.erase(blockIndex);
        getVolumeControlBlock().updateFreeDataBlock(blockIndex, true);
        return true;
    }

    return false;
}

void Disk::reformatDisk(const int totalDiskEntries, const int entriesPerDiskBlock, const DiskAllocationMethod diskAllocationMethod)
{
    dispose();

    this->diskAllocationMethod = diskAllocationMethod;

    const auto superBlockData = new SuperDiskBlock(totalDiskEntries, entriesPerDiskBlock, diskAllocationMethod);
    diskBlocks[0] = superBlockData;

    getVolumeControlBlock().updateFreeDataBlock(0, false);

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
