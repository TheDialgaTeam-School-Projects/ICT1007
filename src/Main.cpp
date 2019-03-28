#include "Main.h"

#include <exception>
#include <iostream>

#include "Config.h"
#include "Disk/Disk.h"
#include "Disk/DiskAllocationMethod.h"
#include "Disk/tempOpen.h"

using std::cout;
using std::cin;
using std::cerr;

using std::endl;

using std::exception;

int main(int argc, char *argv[])
{

    try
    {
        auto entriesPerDiskBlock = 0;
        askEntriesPerDiskBlock(entriesPerDiskBlock);

        auto allocationIndex = -1;
        askDiskAllocationMethod(allocationIndex);

        const auto allocationMethod = static_cast<DiskAllocationMethod>(allocationIndex - 1);

        cout << "We are now formatting the disk based on the above configuration!" << endl;
        Disk disk(DEFAULT_TOTAL_ENTRIES, entriesPerDiskBlock, allocationMethod);
        
		tempOpen Dataset;
		Dataset.initiateFiles();

		vector<map<string, vector<int>>> dataSet = Dataset.getData();

		for (auto i = 0; i < dataSet.size() - 1; i++) {
			for (map<string, vector<int> >::iterator ii = dataSet[i].begin(); ii != dataSet[i].end(); ++ii) {

				if ((*ii).first == "add") {
					disk.addFile((*ii).second[0], (*ii).second);
				}
				else if ((*ii).first == "delete") {
					disk.deleteFile((*ii).second[0]);
				}
				else if ((*ii).first == "read") {
					disk.readFile((*ii).second[0]);
				}

			}

		}
        disk.deleteFile(100);
		disk.printDiskMap();

    }
    catch (exception &exception)
    {
        cerr << "ERROR: " << exception.what() << endl;
    }
}

void askEntriesPerDiskBlock(int &entriesPerDiskBlock)
{
    do
    {
        /*
         * Ask the user for the number of entries per disk block.
         */
        cout << "Input number of entries per disk block: ";

        /*
         * if cin fails or the number of entries per disk block is smaller or equal to 1,
         * print an error message and clear the input buffer before asking again.
         */
        if (!(cin >> entriesPerDiskBlock) || entriesPerDiskBlock <= 1)
        {
            cout << "Number of entries per disk block require to be a positive integer number bigger than 1." << endl;
            cin.clear();
            cin.ignore(LLONG_MAX, '\n');
        }
    } while (entriesPerDiskBlock <= 1);
}

void askDiskAllocationMethod(int &diskAllocationIndex)
{
    do
    {
        /*
         * Ask the user for the disk allocation method.
         */
        cout << "Please choose an allocation method for the disk:" << endl;
        cout << "1. Contiguous" << endl;
        cout << "2. Linked" << endl;
        cout << "3. Indexed" << endl;
        cout << "4. Custom" << endl;

        /*
         * if cin fails or the number of index of disk allocation method is smaller or bigger than 1 and 4 respectively,
         * print an error message and clear the input buffer before asking again.
         */
        if (!(cin >> diskAllocationIndex) || diskAllocationIndex < 1 || diskAllocationIndex > 4)
        {
            cout << "Invalid disk allocation method chosen. Please try again." << endl;
            cin.clear();
            cin.ignore(LLONG_MAX, '\n');
        }
    } while (diskAllocationIndex < 1 || diskAllocationIndex > 4);
}
