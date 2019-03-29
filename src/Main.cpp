#include "Main.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Config.h"
#include "Disk/Disk.h"
#include "Disk/DiskAllocationMethod.h"

using std::cout;
using std::cin;
using std::cerr;

using std::endl;

using std::exception;
using std::fstream;
using std::ios;
using std::stringstream;

// ReSharper disable CppUseAuto

int main(int argc, char *argv[])
{
    try
    {
        int entriesPerDiskBlock = 0;
        askEntriesPerDiskBlock(entriesPerDiskBlock);

        int allocationIndex = -1;
        askDiskAllocationMethod(allocationIndex);

        const auto allocationMethod = static_cast<DiskAllocationMethod>(allocationIndex - 1);

        cout << "We are now formatting the disk based on the above configuration!" << endl;
        Disk disk(DEFAULT_TOTAL_ENTRIES, entriesPerDiskBlock, allocationMethod);

        vector<map<string, vector<int>>> csvFileInstructions = getCsvFileInstructions("SampleCSV.csv");

        for (map<string, vector<int>> &instruction : csvFileInstructions)
        {
            for (std::pair<string, vector<int>> item : instruction)
            {
                if (item.first == "add")
                    disk.addFile(item.second[0], item.second);
                else if (item.first == "delete")
                    disk.deleteFile(item.second[0]);
                else if (item.first == "read")
                    disk.readFile(item.second[0]);
            }
        }

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

vector<map<string, vector<int>>> getCsvFileInstructions(const string &fileName)
{
    fstream fin;
    fin.open(fileName, ios::in);

    vector<map<string, vector<int>>> result;

    // Loop until end of file.
    while (!fin.eof())
    {
        // Contains a line of instructions: add, 100, 101, 102, 103, 104
        string currentLine;
        getline(fin, currentLine);

        stringstream ss(currentLine);
        map<string, vector<int>> innerResult;

        string instruction;
        vector<int> data;

        getline(ss, instruction, ',');

        if (instruction.empty())
            continue;

        while (!ss.eof())
        {
            string tempData;
            getline(ss, tempData, ',');

            data.push_back(stoi(tempData));
        }

        innerResult[instruction] = data;
        result.push_back(innerResult);
    }

    fin.close();

    return result;
}
