#pragma once
#include"opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "MSRCR.h"
#include "MSRCR_Function.h"
#include <iostream>
#include <string>
#include "create_img.h"

using namespace cv;
using namespace std;


int MSRetinexCR(string path, string image_save_path,string file_path)
{
	vector<double> sigema;
	vector<double> weight;
	for (int i = 0; i < 3; i++)
		weight.push_back(1. / 3);
	
	sigema.push_back(30);
	sigema.push_back(150);
	sigema.push_back(300);

	//    string path = "/home/grq/style.mp4";
	//    VideoCapture video(path);
	//    Mat frame, imgdst;
	//    Msrcr msrcr;
	//    video >> frame;
	//    while(!frame.empty())
	//    {
	//        video >> frame;
	//        imshow("SRCR", frame);
	//        msrcr.MultiScaleRetinex(frame, imgdst, weight, sigema, 128, 128);
	//        imshow("dst", imgdst);
	//        waitKey(20);
	//    }

	char key;
	Mat img, imgdst;
	Msrcr msrcr;
	img = imread(path);
	if (img.empty())
	{
		cout << "未读取到" << path << "这张图片，请查看此图是否已保存在相应的文件夹中！" << endl;

	}

	//imshow("Frame", img);
	msrcr.MultiScaleRetinexCR(img, imgdst, weight, sigema, 128, 128);
	//imshow("dst", imgdst);

	imwrite(image_save_path, imgdst);

	//key = (char)cvWaitKey(0);

	return 0;
}

