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
    int index = -1;
    int length;
    int count = 0;
    for (int i = 0; i < theUltimateVector.size(); i++)
    {
        if (data - theUltimateVector[i][0] < 100 && data - theUltimateVector[i][0] > 0)
        {
            index = theUltimateVector[i][1];
            length = theUltimateVector[i][2];
            break;
        }
    }

    if (index == -1)
    {
        cout << "File with the data " << data << " could not be found" << endl << endl;
        return;
    }

    count++;
    bool found = false;
    do
    {
        ContiguousDiskBlock *block = reinterpret_cast<ContiguousDiskBlock*>(diskBlocks[index]);
        for (int i = 0; i < getVolumeControlBlock().getEntriesPerDiskBlock(); i++)
        {
            if ((*block)[i] == data)
            {
                cout << "Read file " << data << " from B" << index << " with " << count << " access time" << endl << endl;
                found = true;
                break;
            }
            length--;
        }
        index++;
        count++;
    } while (length != 0 && !found);
}

void Disk::readLinkedFile(const int data)
{
    vector<string> strOperration;
	vector< vector<int>> theUltimateVector;
	string word;

	vector<AbstractFileInformation*> fileInfo = getDirectoryBlock().getFilesInformation();
	for (auto x : fileInfo) {

		strOperration.push_back(x->toString());
	}

	for (int i = 0; i < strOperration.size(); i++) {
		vector<int> myNumbers;
		for (stringstream sst(strOperration[i]); getline(sst, word, ',');) {
			myNumbers.push_back(stoi(word));
		}
		theUltimateVector.push_back(myNumbers);
	}
	int index = -1;
	int count = 0;
	for (int i = 0; i < theUltimateVector.size(); i++) {
		if (data - theUltimateVector[i][0] < 100 && data - theUltimateVector[i][0] > 0) {
			index = theUltimateVector[i][1];
			break;
		}
	}

    if (index == -1)
    {
        cout << "File with the data " << data << " could not be found" << endl << endl;
        return;
    }

	count++;
	bool found = false;
	do {
		LinkedDiskBlock *block = reinterpret_cast<LinkedDiskBlock*>(diskBlocks[index]);
		for (int i = 0; i < getVolumeControlBlock().getEntriesPerDiskBlock(); i++) {
			if (i != getVolumeControlBlock().getEntriesPerDiskBlock() - 1)
	
			if ((*block)[i] == data) {     
				cout << "Read file " << data << " from B" << index << " with " << count << " access time" << endl << endl;
				found = true;
				break;
			}
		}
		index = (*block)[getVolumeControlBlock().getEntriesPerDiskBlock() - 1];
		count++;
	} while (index != -1 && found == false);
}

void Disk::readIndexedFile(const int data)
{
    vector<string> strOperration;
	vector< vector<int>> theUltimateVector;
	string word;

	vector<AbstractFileInformation*> fileInfo = getDirectoryBlock().getFilesInformation();
	for (auto x : fileInfo) {

		strOperration.push_back(x->toString());
	}

	for (int i = 0; i < strOperration.size(); i++) {
		vector<int> myNumbers;
		for (stringstream sst(strOperration[i]); getline(sst, word, ',');) {

			myNumbers.push_back(stoi(word));
		}
		theUltimateVector.push_back(myNumbers);
	}
	int blockIndex = -1;
	int count = 0;
	for (int i = 0; i < theUltimateVector.size(); i++) {
		if (data - theUltimateVector[i][0] < 100 && data - theUltimateVector[i][0] > 0) {
			blockIndex = theUltimateVector[i][1];
			break;
		}
	}

    if (blockIndex == -1)
    {
        cout << "File with the data " << data << " could not be found" << endl << endl;
        return;
    }

	count++;
	IndexedDiskBlock *block = reinterpret_cast<IndexedDiskBlock*>(diskBlocks[blockIndex]);
	for (int i = 0; i != getVolumeControlBlock().getEntriesPerDiskBlock(); i++) {
		if ((*block)[i] == -1)
			break;

		count++;
		auto indexBlockIndex = (*block)[i];
		ContiguousDiskBlock *block_c = reinterpret_cast<ContiguousDiskBlock*>(diskBlocks[indexBlockIndex]);

		for (int j = 0; j != getVolumeControlBlock().getEntriesPerDiskBlock(); j++) {
			if ((*block_c)[j] == data) {
				cout << "Read file " << data << " from B" << indexBlockIndex << " with " << count << " access time" << endl << endl;
				return;
			}
		}
	}
}

void Disk::readCustomFile(const int data)
{
    readContiguousFile(data);
}
