#pragma once
#include <iostream>
#include "string.h"

using namespace std;
using namespace cv;


Mat Matrix_multiply(Mat binary_img_Mat, Mat water_img_Mat)
{
	Mat result;
	result = binary_img_Mat.clone();
	int rowNumber = result.rows;
	int colNumber = result.cols;
	for (int i = 0; i < rowNumber; i++)
	{
		for (int j = 0; j < colNumber; j++)
		{
			result.at<Vec3b>(i, j)[0] = (binary_img_Mat.at<Vec3b>(i, j)[0] * water_img_Mat.at<Vec3b>(i, j)[0]) / 256;
			result.at<Vec3b>(i, j)[1] = (binary_img_Mat.at<Vec3b>(i, j)[1] * water_img_Mat.at<Vec3b>(i, j)[1]) / 256;
			result.at<Vec3b>(i, j)[2] = (binary_img_Mat.at<Vec3b>(i, j)[2] * water_img_Mat.at<Vec3b>(i, j)[2]) / 256;
		}
	}
	return result;

}


