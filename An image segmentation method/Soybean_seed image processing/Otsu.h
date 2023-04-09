#pragma once
#include <iostream>
#include <time.h>

using namespace std;
using namespace cv;

int myOtsu(Mat & src)
{
	int th;
	const int GrayScale = 256;
	int pixCount[GrayScale] = { 0 };
	int pixSum = src.cols * src.rows;
	float pixPro[GrayScale] = { 0 };
	float w0, w1, u0tmp, u1tmp, u0, u1, deltaTmp, deltaMax = 0;

	for (int i = 0; i < src.cols; i++)
	{
		for (int j = 0; j < src.rows; j++)
		{
			pixCount[src.at<uchar>(j, i)]++;
		}
	}

	for (int i = 0; i < GrayScale; i++)
	{
		pixPro[i] = pixCount[i] * 1.0 / pixSum;
	}

	for (int i = 0; i < GrayScale; i++)
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = deltaTmp = 0;
		for (int j = 0; j < GrayScale; j++)
		{
			if (j <= i)
			{
				w0 += pixPro[j];
				u0tmp += j * pixPro[j];
			}
			else
			{
				w1 += pixPro[j];
				u1tmp += j * pixPro[j];
			}
		}
		u0 = u0tmp / w0;
		u1 = u1tmp / w1;
		deltaTmp = (float)(w0 *w1* pow((u0 - u1), 2)); 
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			th = i;
		}
	}
	return th;
}
