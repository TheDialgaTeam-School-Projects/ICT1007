#pragma once
#include <map>
#include <string>
#include <vector>

using namespace std;

class tempOpen
{
public:

    vector<map<string, vector<int>>> datas;
    vector<string> openFile();
    void initiateFiles();
    vector<map<string, vector<int>>> getData();
};
