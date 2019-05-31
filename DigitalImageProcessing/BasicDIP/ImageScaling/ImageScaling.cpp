/**
 * @file ImageScaling.cpp
 * @author Xiang Zheng
 * @date 2017.12.27
 * @brief The program completes the scaling of the image
 */
#include<iostream>
#include<cmath>
#include<opencv2\opencv.hpp>
using namespace std;
using namespace cv;
int main() {
    const char* input_file_name = "image.bmp";       /**< @variable input_file_name input file name */
    const char* output_file_name = "result.bmp";     /**< @variable output_file_name output file name */
    const unsigned char zero = 0;                    /**< @variable zero zero of the unsigned-char-type */

    Mat img = imread(input_file_name, 0);            /**< @variable img input image */
	if (img.empty()) {
		cout << "Can't open file!\n";
		exit(EXIT_FAILURE);
	}
    unsigned char* raw_image = img.data;             /**< @variable raw_image pointer to the input image data */
    int width = img.cols;                            /**< @variable width input image width */
    int height = img.rows;                           /**< @variable height input image height */
	
    /**< @variable scaling_ratio_1 scaling ratio for the width */
    /**< @variable scaling_ratio_2 scaling ratio for the height */
	double scaling_ratio_1, scaling_ratio_2;
	cin >> scaling_ratio_1 >> scaling_ratio_2;

    int nwidth = static_cast<int>(width*scaling_ratio_1 + 0.5);   /**< @variable nwidth output image width */
    int nheight = static_cast<int>(height*scaling_ratio_2 + 0.5); /**< @variable nheight output image height */
    Mat new_img;                                                  /**< @variable new_img output image */
	new_img.create(nheight, nwidth, CV_8UC1);
    unsigned char* scaling_image = new_img.data;                  /**< @variable scaling_image pointer to the output image data */

	int x0, y0;
	for (int i = 0; i < nheight; i++) {
		for (int j = 0; j < nwidth; j++) {
			x0 = static_cast<int>(i / scaling_ratio_2 + 0.5);
			y0 = static_cast<int>(j / scaling_ratio_1 + 0.5);
			if (x0 >= 0 && x0 < height&&y0 >= 0 && y0 < width) {
				scaling_image[i*nwidth + j] = raw_image[x0*width + y0];
			}
			else {
				scaling_image[i*nwidth + j] = zero;
			}
		}
	}

    /**< @brief dispaly and save the output image */
	namedWindow("display");
	imshow("display", new_img);
	imwrite(output_file_name, new_img);
	waitKey(0);
	return 0;
}
