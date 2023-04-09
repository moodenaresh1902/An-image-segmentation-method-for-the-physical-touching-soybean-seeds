#pragma once
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

string ReadLine(string filename, int line)
{
	int i = 0;
	string temp;
	fstream file;
	file.open(filename, ios::in);

	if (line <= 0)
	{return "Error 1: The number of lines cannot be 0 or negative.";}

	if (file.fail()) 
	{ return "Error 2: The file does not exist."; }

	while (getline(file, temp) && i < line - 1) { i++; }

	file.close();
	return temp;
}