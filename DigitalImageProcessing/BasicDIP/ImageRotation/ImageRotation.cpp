/**
 * @file ImageRotation.cpp
 * @author Xiang Zheng
 * @date 2017.12.27
 * @brief The program completes the rotation of the image
 * @note Here the image rotation origin is the image central point
 */
#include<iostream>
#include<cmath>
#include<opencv2\opencv.hpp>
using namespace std;
using namespace cv;
int main() {
    const char* input_file_name = "image.bmp";          /**< @variable input_file_name input file name */
    const char* output_file_name = "result.bmp";        /**< @variable output_file_name output file name */
    const double PI = 3.141592653589793;                /**< @variable PI circular contant */
    const unsigned char zero = 0;                       /**< @variable zero zero of the unsigned-char-type */

    Mat img = imread(input_file_name, 0);               /**< @variable img input iamge */
	if (img.empty()) {
		cout << "Can't open file!\n";
		exit(EXIT_FAILURE);
	}
    unsigned char* raw_image = img.data;                /**< @variable raw_image pointer to the input image data */
    int width = img.cols;                               /**< @variable input image width */
    int height = img.rows;                              /**< @variable input image height */
    double degree;                                      /**< @variable degree clockwise rotation angle in degree */
	cin >> degree;
    double theta = degree / 180.0*PI;                   /**< @variable theta clockwise rotation angle in radian */
    /**< @variable output image width */
	int nwidth = static_cast<int>(width*cos(theta) + height * sin(theta));
    /**< @variable output image height */
	int nheight = static_cast<int>(width*sin(theta) + height * cos(theta));
    Mat new_img;                                        /**< @variable new_img output image */
	new_img.create(nheight, nwidth, CV_8UC1);
    unsigned char* rotation_image = new_img.data;       /**< @variable rotation_image pointer to the output image data */

    /**< @variable rotation_offset_1 rotation offset constant 1 */
	double rotation_offset_1 = -0.5*(nwidth - 1)*cos(theta) - 0.5*(nheight - 1)*sin(theta) + 0.5*(width - 1);
	/**< @variable rotation_offset_2 rotation offset constant 2 */
	double rotation_offset_2 = 0.5*(nwidth - 1)*sin(theta) - 0.5*(nheight - 1)*cos(theta) + 0.5*(height - 1);

	int x0, y0;
	for (int i = 0; i < nheight; i++) {
		for (int j = 0; j < nwidth; j++) {
			x0 = static_cast<int>(i*cos(theta) - j * sin(theta) + rotation_offset_2 + 0.5);
			y0 = static_cast<int>(i*sin(theta) + j * cos(theta) + rotation_offset_1 + 0.5);
			if (x0 >= 0 && x0 < height&&y0 >= 0 && y0 < width) {
                /**< @brief rotate the pixel gray value */
				rotation_image[i*nwidth + j] = raw_image[x0*width + y0];
			}
			else {
                /**< @brief set the pixel gray value to zero */
				rotation_image[i*nwidth + j] = zero;
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
