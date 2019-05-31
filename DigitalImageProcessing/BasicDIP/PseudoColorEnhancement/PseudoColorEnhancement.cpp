/**
 * @file PseudoColorEnhancement.cpp
 * @author Xiang Zheng
 * @date 2017.12.28
 * @brief This program completes a pseudo color enhancement method for a given image
 */
#include<iostream>
#include<opencv2\opencv.hpp>
using namespace std;
using namespace cv;
int main() {
    const char *input_file_name = "image.bmp";      /**< @variable input_file_name input file name */
    const char *output_file_name = "result.bmp";    /**< @variable output_file_name output file name */

    Mat img = imread(input_file_name, 0);           /**< @variable img input image */
	if (img.empty()) {
		cout << "Can't open file!\n";
		exit(EXIT_FAILURE);
	}
    unsigned char *raw_image = img.data;            /**< @variable raw_image pointer to the input image data */
    int width = img.cols;                           /**< @variable width input image width */
    int height = img.rows;                          /**< @variable height input image height */

    Mat new_img;                                    /**< @variable new_img output image */
	new_img.create(height, width, CV_8UC3);
    unsigned char *color_image = new_img.data;      /**< @variable color_image pointer to the output image data */
	unsigned char tmp = 0;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			tmp = raw_image[i*width + j];
            color_image[(i*width + j) * 3 + 0] = abs(255 - tmp);   /**< @brief Blue channel */
            color_image[(i*width + j) * 3 + 1] = abs(127 - tmp);   /**< @brief Green channel */
            color_image[(i*width + j) * 3 + 2] = abs(0 - tmp);     /**< @brief Red channel */
		}
	}

    /**< @brief display and save the output image */
	namedWindow("display");
	imshow("display", new_img);
	waitKey(0);
	imwrite(output_file_name, new_img);

	return 0;
}
