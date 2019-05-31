/**
 * @file ImageLinearTransform.cpp
 * @author Xiang Zheng
 * @date 2017.12.26
 * @brief The program completes the gray-scale linear transformation of the image
 */
#include<iostream>
#include<opencv2\opencv.hpp>
using namespace std;
using namespace cv;
int main() {
    Mat img;                                       /**< @variable img input image */
    /**< @brief Read input image file */
	img = imread("test.bmp", 0);
	if (img.empty()) {
		cout << "Can't open file!\n";
		exit(EXIT_FAILURE);
	}
    unsigned char *raw_image = img.data;           /**< @variable raw_image pointer to input image data */
    int width = img.cols;                          /**< @variable width input image width */
    int height = img.rows;                         /**< @variable height input image height */

    Mat new_img;                                   /**< @variable new_img output image */
    /**< @brief create output gray image */
    new_img.create(height, width, CV_8UC1);
    unsigned char* linear_image = new_img.data;    /**< @variable linear_image pointer to output image data */
	memcpy(linear_image, raw_image, width*height * sizeof(unsigned char));

    /**
     * @variable a coefficient 1
     * @variable b coefficient 2
     * @brief Set the pixel gray value before the transformation is x,
     * and the pixel gray value after the transformation is y,
     * then y and x meet the relationship y=a*x+b
     * as long as y does not exceed the grayscale threshold
     */
	double a, b;
	cin >> a >> b;
	for (int i = 0; i < width*height; i++) {
		double temp = (double)linear_image[i] * a + b;
		if (temp > 255)temp = 255;
		if (temp < 0)temp = 0;
		linear_image[i] = (unsigned char)temp;
	}

    /**< @brief display and save the putput image */
	namedWindow("display");
	imshow("display", new_img);
	imwrite("result.bmp", new_img);
    waitKey(0);

	return 0;
}
