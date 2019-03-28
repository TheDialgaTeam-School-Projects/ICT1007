#pragma once
#include <vector>
#include <string>
#include <map>
using namespace std;
class tempOpen
{
public:

	vector<map<string, vector<int>>> datas;
	vector <string> openFile();
	void initiateFiles();
	vector<map<string, vector<int>>> getData();
};

