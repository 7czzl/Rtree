#include "stdafx.h"
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include"readFile.h"
using namespace std;

void readFeature(string filename, vector<vector<int>>& Array)
{
	ifstream inFile(filename);
	if (!inFile.is_open())
	{
		fprintf(stderr, "%s not exist\n", filename.c_str());
		return;
	}
	string lineArray;
	vector<int> tempArray;
	getline(inFile, lineArray, 't');
	getline(inFile, lineArray, 't');
	inFile.ignore();
	while (getline(inFile, lineArray))
	{
		stringstream numbersTemp;
		numbersTemp << lineArray;
		string str;
		int number = 0;
		while (getline(numbersTemp, str, ' '))
		{
			stringstream numberTemp;
			numberTemp << str;
			numberTemp >> number;
			tempArray.push_back(number);
		}
		Array.push_back(tempArray);
		tempArray.clear();
		inFile.ignore(5);
	}
	Array.pop_back();
	inFile.close();
}

void readImage(string filename, vector<string> &Array)
{
	ifstream inFile(filename);
	if (!inFile.is_open())
	{
		fprintf(stderr, "%s not exist\n", filename.c_str());
		return;
	}
	string str;
	while (getline(inFile, str))
	{
		Array.push_back(str);
	}
	inFile.close();
}