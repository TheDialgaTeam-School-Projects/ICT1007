#pragma once
#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

/**
 * \brief Ask the user for the number of entries per disk block.
 * \param entriesPerDiskBlock Reference variable that contains the number of entries per disk block.
 */
void askEntriesPerDiskBlock(int &entriesPerDiskBlock);

/**
 * \brief Ask the user for the disk allocation method.
 * \param diskAllocationIndex Reference variable that contains the disk allocation index.
 */
void askDiskAllocationMethod(int &diskAllocationIndex);


/**
 * \brief Get the CSV file instructions.
 * \param fileName File path of the csv file to read.
 * \return A list of instructions to call.
 */
vector<map<string, vector<int>>> getCsvFileInstructions(const string &fileName);