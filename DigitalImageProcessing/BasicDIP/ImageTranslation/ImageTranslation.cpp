/**
 * @file ImageTranslation.cpp
 * @author Xiang Zheng
 * @date 2017.12.26
 * @brief The program completes the translation of the image
 */
#include<iostream>
#include<cmath>
#include<opencv2\opencv.hpp>
using namespace std;
using namespace cv;
int main() {
    const char* input_file_name = "image.bmp";           /**< @variable input_file_name input file name */
    const char* output_file_name = "result.bmp";         /**< @variable output_file_name output file name */
    const unsigned char zero = 0;                        /**< @variable zero zero of unsigned-char type */

    Mat img = imread(input_file_name, 0);                /**< @variable img input image */
	if (img.empty()) {
		cout << "Can't open file!\n";
		exit(EXIT_FAILURE);
	}
    unsigned char* raw_image = img.data;                 /**< @variable raw_image pointer to the input image data */
    int width = img.cols;                                /**< @variable width input image width */
    int height = img.rows;                               /**< @variable height input image height */
	
    /**< @variable translation_offset_1 translation offset downward */
    /**< @variable translation_offset_2 translation offset toward right */
	int translation_offset_1, translation_offset_2;
	cin >> translation_offset_1 >> translation_offset_2;

    Mat new_img;                                         /**< @variable new_img output image */
	new_img.create(height, width, CV_8UC1);
    unsigned char* translation_image = new_img.data;     /**< @variable translation_image pointer to the output image data */

	int x0, y0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			x0 = i - translation_offset_1;
			y0 = j - translation_offset_2;
			if (x0 >= 0 && x0 < height&&y0 >= 0 && y0 < width) {
                /**< @brief transtalte the pixel gray value */
				translation_image[i*width + j] = raw_image[x0*width + y0];
			}
			else {
                /**< @brief set the pixel gray value to zero */
				translation_image[i*width + j] = zero;
			}
		}
	}

    /**< @brief display and save the output image */
	namedWindow("display");
	imshow("display", new_img);
	imwrite(output_file_name, new_img);
	waitKey(0);
	return 0;
}
