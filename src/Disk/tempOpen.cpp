#include "tempOpen.h"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

using std::cout;

using std::endl;
using std::stoi;

using std::map;
using std::string;
using std::vector;

vector<string> tempOpen::openFile()
{
    fstream fin;
    fin.open("SampleCSV.csv", ios::in);
    // Read the Data from the file 
    // as String Vector 
    vector<string> row;
    vector<string> myOperations;
    string line, word, temp;
    while (fin >> temp)
    {
        // read an entire row and 
        // store it in a string variable 'line' 
        getline(fin, line);

        // used for breaking words 
        stringstream s(line);

        // read every column data of a row and 
        // store it in a string variable, 'word' 
        while (getline(s, word, ','))
        {
            // add all the column data 
            // of a row to a vector 
            row.push_back(word);
        }
        string currentOp = temp + line;
        myOperations.push_back(currentOp);
    }
    cout << "File is now read!" << endl;
    return myOperations;
}

void tempOpen::initiateFiles()
{
    tempOpen o;
    vector<string> arr = o.openFile(); //contains add,100,101,102,103,104
    //         del,100,102 etc

    vector<map<string, vector<int>>> convertedData;

    for (int i = 0; i < arr.size(); i++)
    {
        string tmp;
        stringstream ss(arr[i]);
        vector<string> words;
        map<std::string, vector<int>> csv;
        vector<int> values; //contains 100,101,102,103,104 for example

        while (getline(ss, tmp, ','))
        {
            words.push_back(tmp);
        }

        for (auto x = 0; x < words.size() - 1; x++)
        {
            values.push_back(stoi(words[x + 1]));
        }

        csv.insert(std::pair<string, vector<int>>(words[0], values));
        convertedData.push_back(csv);
    }

    this->datas = convertedData;
}

vector<map<string, vector<int>>> tempOpen::getData()
{
    return datas;
}
