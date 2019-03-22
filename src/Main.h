#pragma once

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
