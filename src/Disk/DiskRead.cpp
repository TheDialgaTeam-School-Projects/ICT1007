#include "Disk.h"

#include <iostream>
#include <sstream>
#include "Block/LinkedDiskBlock.h"
#include "Block/IndexedDiskBlock.h"
#include "Block/ContiguousDiskBlock.h"

using std::cout;

using std::endl;

using std::stringstream;

// ReSharper disable CppUseAuto

void Disk::readContiguousFile(const int data)
{
    int index = -1;
    int length = 0;
    int fileName;
    auto info = getDirectoryBlock().getFilesInformation();
    for (int i = 0; i < getDirectoryBlock().getFilesInformation().size(); i++)
    {
        if (data - info[i]->getFileName() < 100 && data - info[i]->getFileName() > 0)
        {
            index = reinterpret_cast<ContiguousFileInformation*>(info[i])->getStartBlockIndex();
            length = reinterpret_cast<ContiguousFileInformation*>(info[i])->getLength();
            fileName = info[i]->getFileName();
        }
    }

    if (index == -1)
    {
        cout << "File with the data " << data << " could not be found" << endl << endl;
        return;
    }

    int count = 1;
    bool found = false;
    do
    {
        count++;
        ContiguousDiskBlock *block = reinterpret_cast<ContiguousDiskBlock*>(diskBlocks[index]);
        for (int i = 0; i < getVolumeControlBlock().getEntriesPerDiskBlock(); i++)
        {
            if ((*block)[i] == data)
            {
                cout << "Read file " << fileName << "(" << data << ") from B" << index << " with " << count << " access time" << endl << endl;
                found = true;
                break;
            }
            length--;
        }
        index++;
    } while (length != 0 && !found);
}

void Disk::readLinkedFile(const int data)
{
    int index = -1;
    int fileName;
    auto info = getDirectoryBlock().getFilesInformation();
    for (int i = 0; i < getDirectoryBlock().getFilesInformation().size(); i++)
    {
        if (data - info[i]->getFileName() < 100 && data - info[i]->getFileName() > 0)
        {
            index = reinterpret_cast<LinkedFileInformation*>(info[i])->getStartBlockIndex();
            fileName = info[i]->getFileName();
        }
    }

    if (index == -1)
    {
        cout << "File with the data " << data << " could not be found" << endl << endl;
        return;
    }

    int count = 1;
    bool found = false;
    do
    {
        count++;
        LinkedDiskBlock *block = reinterpret_cast<LinkedDiskBlock*>(diskBlocks[index]);
        for (int i = 0; i < getVolumeControlBlock().getEntriesPerDiskBlock(); i++)
        {
            if (i != getVolumeControlBlock().getEntriesPerDiskBlock() - 1)

                if ((*block)[i] == data)
                {
                    cout << "Read file " << fileName << "(" << data << ") from B" << index << " with " << count << " access time" << endl << endl;
                    found = true;
                    break;
                }
        }
        index = (*block)[getVolumeControlBlock().getEntriesPerDiskBlock() - 1];
    } while (index != -1 && !found);
}

void Disk::readIndexedFile(const int data)
{
    int index = -1;
    int fileName;
    auto info = getDirectoryBlock().getFilesInformation();
    for (int i = 0; i < getDirectoryBlock().getFilesInformation().size(); i++)
    {
        if (data - info[i]->getFileName() < 100 && data - info[i]->getFileName() > 0)
        {
            index = reinterpret_cast<IndexedFileInformation*>(info[i])->getIndexBlockIndex();
            fileName = info[i]->getFileName();
        }
    }

    if (index == -1)
    {
        cout << "File with the data " << data << " could not be found" << endl << endl;
        return;
    }

    int count = 1;
    IndexedDiskBlock *block = reinterpret_cast<IndexedDiskBlock*>(diskBlocks[index]);
    for (int i = 0; i != getVolumeControlBlock().getEntriesPerDiskBlock(); i++)
    {
        if ((*block)[i] == -1)
            break;

        count++;
        auto indexBlockIndex = (*block)[i];
        ContiguousDiskBlock *block_c = reinterpret_cast<ContiguousDiskBlock*>(diskBlocks[indexBlockIndex]);

        for (int j = 0; j != getVolumeControlBlock().getEntriesPerDiskBlock(); j++)
        {
            if ((*block_c)[j] == data)
            {
                cout << "Read file " << fileName << "(" << data << ") from B" << index << " with " << count << " access time" << endl << endl;
                return;
            }
        }
    }
}

void Disk::readCustomFile(const int data)
{
    readContiguousFile(data);
}
