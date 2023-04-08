#pragma once
#include "create_img.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <mutex>
#include <iostream>
#include <cstdlib>
#include "create_file.h"
#include "Otsu.h"
#include "Watershed.h"

using namespace cv;
using namespace std;

int thresh_callback_open_operation(Mat binary_src_Mat, Mat original_src_Mat, string file_path,  int count)
{
	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;

	findContours(binary_src_Mat, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<vector<Point>> contours_poly(contours.size());
	vector<Rect>boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());

	Mat drawing = Mat::zeros(original_src_Mat.size(), CV_8UC1);

	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));

		Mat dst;

		int x0 = boundRect[i].x;
		int y0 = boundRect[i].y;
		int w0 = boundRect[i].width;
		int h0 = boundRect[i].height;

		if (boundRect[i].width > 100 && boundRect[i].height > 100)
		{
			minEnclosingCircle(contours_poly[i], center[i], radius[i]);

			circle(drawing, center[i], radius[i], Scalar::all(255), -1);

			original_src_Mat.copyTo(dst, drawing);

			Mat ROI = dst(Rect(x0, y0, w0, h0));

			string save_single_bean_path = create_path(count, create_files(file_path, "Individual_Seeds"), ".bmp", "Individual_Seeds");

			Mat dst_img_rsize(227, 227, original_src_Mat.type()); 
			resize(ROI, dst_img_rsize, dst_img_rsize.size(), 0, 0, INTER_LINEAR);

			imwrite(save_single_bean_path, dst_img_rsize);

			count++;
		}
	}
	return count;
}


int thresh_callback(string binary_src_path, string MSRCR_src_path, string file_path,  string original_drawing_path,string image_format, int count)
{
	 Mat src_gray; Mat MSRCR_src;

	RNG rng(12345);

	Mat binary_output;
	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;

	MSRCR_src = imread(MSRCR_src_path);


	cvtColor(MSRCR_src, src_gray, CV_BGR2GRAY);

	int binary_thresh = myOtsu(src_gray);

	threshold(src_gray, binary_output, binary_thresh, 255, THRESH_OTSU);

	imwrite(binary_src_path, binary_output);

	findContours(binary_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));


	vector<vector<Point>> contours_poly(contours.size());
	vector<Rect>boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		Mat mask_drawing = Mat::zeros(binary_output.size(), CV_8UC1);

		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
		
		Mat dst, mask_binary;

		Mat mask = Mat::zeros(MSRCR_src.size(), CV_8UC1);

		drawContours(mask, contours, i, Scalar::all(255), -1);
		int x0 = boundRect[i].x;
		int y0 = boundRect[i].y;
		int w0 = boundRect[i].width;
		int h0 = boundRect[i].height;

		if ((boundRect[i].width > 95 && boundRect[i].height > 95) || (boundRect[i].width > 115 && boundRect[i].height > 95) || (boundRect[i].width > 95 && boundRect[i].height > 115))
		{
			// processing the physical touching seeds on the masking image
			if (boundRect[i].width > 300 || boundRect[i].height > 300)
			{
				MSRCR_src.copyTo(dst, mask);

				binary_output.copyTo(mask_binary, mask);
				
				// morphological operation for processing the physical touching seeds on the masking image
				{
					Mat element = getStructuringElement(MORPH_RECT, Size(13, 13)); // kernel size
					morphologyEx(mask_binary, mask_binary, MORPH_RECT, element);
                    count = thresh_callback_open_operation(mask_binary, MSRCR_src, file_path, count);
				}

				// watered algorithm for processing the physical touching seeds on the masking image
				{
					//count = Watershed_algorithm(dst, mask_binary, count, file_path, MSRCR_src);// watered algorithm
				}
				
			}
			else 
			{
				minEnclosingCircle(contours_poly[i], center[i], radius[i]);

			    circle(mask_drawing, center[i], radius[i], Scalar::all(255), -1, 8, 0);

				MSRCR_src.copyTo(dst, mask_drawing);

				Mat ROI = dst(Rect(x0, y0, w0, h0));
				
				string save_single_bean_path = create_path(count, create_files(file_path, "Individual_Seeds"), image_format, "Individual_Seeds");

				Mat dst_img_rsize(227, 227, binary_output.type());
				resize(ROI, dst_img_rsize, dst_img_rsize.size(), 0, 0, INTER_LINEAR);

				imwrite(save_single_bean_path, dst_img_rsize);

				cout << save_single_bean_path << endl;

				count++;
			}
		}
	}
	return count;
}