#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include "External_matrix.h"
using namespace cv;

int thresh_callback_particular_for_watered(Mat water_binary_result,  string file_path, int count, Mat original_src)
{
    Mat src_gray;

	Mat threshold_output;
	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;

	///转化成灰度图像并进行平滑
	cvtColor(water_binary_result, src_gray, CV_BGR2GRAY);
	int thresh = myOtsu(src_gray);

	threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);

	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	
	vector<vector<Point>> contours_poly(contours.size());
	vector<Rect>boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());

	Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);

	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), (0, 0, 255), 2, 8, 0);
		minEnclosingCircle(contours_poly[i], center[i], radius[i]);

		Mat dst;
		Mat mask = Mat::zeros(original_src.size(), CV_8UC1);

		drawContours(mask, contours, i, Scalar::all(255), -1);

		minEnclosingCircle(contours_poly[i], center[i], radius[i]);

		circle(drawing, center[i], radius[i], Scalar::all(255), -1);

		
		original_src.copyTo(dst, drawing);

		int x0 = 0, y0 = 0, w0 = 0, h0 = 0;
		x0 = boundRect[i].x;
		y0 = boundRect[i].y;
		w0 = boundRect[i].width;
		h0 = boundRect[i].height;

		if (boundRect[i].width > 100 && boundRect[i].height > 100)
		{
			Mat ROI = dst(Rect(x0, y0, w0, h0));

			string save_single_bean = file_path;

			string save_single_bean_path = save_single_bean.append(to_string(count)).append(".bmp");

			Mat dst_img_rsize(227, 227, threshold_output.type());
			resize(ROI, dst_img_rsize, dst_img_rsize.size(), 0, 0, INTER_LINEAR);

			imwrite(save_single_bean_path, dst_img_rsize);

			count++;
		}
	}
	return count;
}


int Watershed_algorithm(Mat src, Mat binary_src, int count,  string file_path, Mat MSRCR_src)
{
	if (src.empty())
	{
		printf("Can not load Image...");
	}
	
	Mat distImg;
	distanceTransform(binary_src, distImg, DIST_L1, 3, 5);

	Mat kernel = Mat::ones(9, 9, CV_8UC1);

	erode(distImg, distImg, kernel);
	
	Mat dist_8u;
	distImg.convertTo(dist_8u, CV_8U);
	vector<vector<Point>> contours;
	findContours(dist_8u, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

	Mat marker = Mat::zeros(src.size(), CV_32SC1);

	for (int i = 0; i < contours.size(); i++)
	{
		drawContours(marker, contours, static_cast<int>(i), Scalar(static_cast<int>(i) + 1), -1);
	}

	circle(marker, Point(5, 5), 3, Scalar(255, 255, 255));

	watershed(src, marker);
	Mat water = Mat::zeros(marker.size(), CV_8UC1);
	marker.convertTo(water, CV_8UC1);
	bitwise_not(water, water, Mat());

	vector<Vec3b> colors;
	for (size_t i = 0; i < contours.size(); i++)
	{
		int r = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0, 255);
		int b = theRNG().uniform(0, 255);
		colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	}
	
	Mat dst = Mat::zeros(marker.size(), CV_8UC3);
	for (int row = 0; row < marker.rows; row++) {
		for (int col = 0; col < marker.cols; col++) {
			int index = marker.at<int>(row, col);
			if (index > 0 && index <= static_cast<int>(contours.size()))
			{
				dst.at<Vec3b>(row, col) = colors[index - 1];
			}
			else 
			{
				dst.at<Vec3b>(row, col) = Vec3b(0, 0, 0);
			}
		}
	}


	for (int j = 0; j < contours.size(); j++)
	{
		int index_number = j + 1;
		Mat dst_copy = Mat::zeros(marker.size(), CV_8UC3);
		for (int row = 0; row < marker.rows; row++)
		{
			for (int col = 0; col < marker.cols; col++)
			{
				int index_1 = marker.at<int>(row, col);

				if (index_1 > 0 && index_1 <= static_cast<int>(contours.size()) && (index_1 == index_number))
				{
					dst_copy.at<Vec3b>(row, col) = colors[index_1 - 1];
				}
				else 
				{
					dst_copy.at<Vec3b>(row, col) = Vec3b(0, 0, 0);
				}
			}
		}

		string seeds_result_path = file_path;

		string seeds_result_path_append = seeds_result_path.append("Individual_Seeds/");

		Mat water_binary_result;

		dst_copy.copyTo(water_binary_result, binary_src);

		count = thresh_callback_particular_for_watered(water_binary_result, seeds_result_path_append, count, MSRCR_src);
	}
	return count;
}
