#pragma once
#include <iostream>
#include "string"
#include "sys/stat.h"
#include <iostream>
#include <direct.h>
#include <io.h>
#include <string>

using namespace std;

string create_files(string folderPath, string FolderPath)
{
	if (FolderPath == "Binary") { folderPath.append(FolderPath); }
	if (FolderPath == "Individual_Seeds") { folderPath.append(FolderPath); }
	if (FolderPath == "MSRCR") { folderPath.append(FolderPath); }

	const char * mystr = folderPath.c_str();
	mkdir(mystr,S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
	string str = mystr;
	return str.append("/");
}

//创建文件夹保存图像
string create_file(string img_name, string root_img_path)
{
	string folderPath = root_img_path;
	folderPath.append(img_name);
	const char * mystr = folderPath.c_str();
	mkdir(mystr,S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
	string str = mystr;
	return str.append("/");
}