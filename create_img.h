#pragma once
#include <iostream>
#include <string>
using namespace std;


string create_path(int number, string file_path, string image_format,string image_type)
{
	string image_file_path;

	if (image_type=="Individual_Seeds")
	{
		image_file_path.append(file_path).append(to_string(number)).append(image_format);
		return image_file_path;
	}
	else if (image_type == "Binary")
	{
		image_file_path.append(file_path).append("Binary").append(image_format);
		return image_file_path;
	}
	else if (image_type == "MSRCR")
	{
		image_file_path.append(file_path).append("MSRCR").append(image_format);
		return image_file_path;
	}
}