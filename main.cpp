
#include "stdio.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "create_file.h"
#include "External_matrix.h"
#include "Otsu.h"
#include "Read_txt.h"
#include "MSRetinexCR.h"

using namespace std;
using namespace cv;

string image_format = ".bmp"; // the format of the original images
string image_names = "/home/Linux Version/path.txt"; // this algorithm processes the images according to images' name in this file.


int Line_number = 2;//the number of images 
int count_seeds = 1;// Image counting of soybean seeds

int main()
{
	clock_t start, end;
	start = clock();

	for (int Line = 0; Line < Line_number; Line++)
	{
		string root_img_path = "/home/Linux Version/src/"; // the original images are saved in this file

		string img_names = ReadLine(image_names, Line + 1);
		string file_path = create_file(img_names, root_img_path);
		string original_image_path = root_img_path.append(img_names).append(image_format);
		{
			cout << img_names << " " << "Processing....." << endl;
			string MSRCR_src_path = create_path(count_seeds, create_files(file_path, "MSRCR"), image_format, "MSRCR");
			string binary_src_path = create_path(count_seeds, create_files(file_path, "Binary"), image_format, "Binary");
			MSRetinexCR(original_image_path, MSRCR_src_path, file_path);
			count_seeds = thresh_callback(binary_src_path, MSRCR_src_path, file_path, original_image_path, image_format, count_seeds);
			cout << img_names << " " << "Processing completed." << endl;
			cout << endl;
		}
	}

	end = clock();

	double runing_time = double(end - start) / CLOCKS_PER_SEC;
	cout <<"Total Runing Time:"<<" "<< runing_time << " " << "s" << endl;
	cout << "The Avarage Time for Segmentation a Individual Soybean Seeds:" << " " << double(runing_time/ count_seeds) << " " << "s" << endl;

	return 0;
}
