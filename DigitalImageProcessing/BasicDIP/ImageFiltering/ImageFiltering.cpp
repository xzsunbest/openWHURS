/**
 * @file ImageFiltering.cpp
 * @author Xiang Zheng
 * @date 2017.12.27
 * @brief This program uses several different template operators,
 * such as mean filter operator, Prewitt operator, Sobel operator
 * and Laplacian enhancement operator to realize
 * the low-pass filtering and high-pass filtering
 * in the image space domain.
 */
#include<iostream>
#include<opencv2\opencv.hpp>
using namespace std;
using namespace cv;
const char mat_mean_filter[3 * 3] = { 1,1,1,1,1,1,1,1,1 };    /**< @variable mat_mean_filter mean filter operator */
const char mat_Prewitt_1[3 * 3] = { -1,-1,-1,0,0,0,1,1,1 };   /**< @variable mat_Prewitt_1 Prewitt operator 1 */
const char mat_Prewit_2[3 * 3] = { -1,0,1,-1,0,1,-1,0,1 };    /**< @variable mat_Prewitt_2 Prewitt operator 2 */
const char mat_Sobel_1[3 * 3] = { -1,-2,-1,0,0,0,1,2,1 };     /**< @variable mat_Sobel_1 Sobel operator 1 */
const char mat_Sobel_2[3 * 3] = { -1,0,1,-2,0,2,-1,0,1 };     /**< @variable mat_Sobel_2 Sobel operator 2 */
const char mat_Laplacian[3 * 3] = { 0,-1,0,-1,5,-1,0,-1,0 };  /**< @variable mat_Laplacian Laplacian enhancement operator */
char temp[3 * 3];                                             /**< @variable temp storage operator */
int main() {
    const char* input_file_name = "image.bmp";                /**< @variable input_file_name input file name */
    const char* output_file_name = "result.bmp";              /**< @variable output_file_name output file name */

    Mat img = imread(input_file_name, 0);                     /**< @variable img input image */
	if (img.empty()) {
		cout << "Can't open file!\n";
		exit(EXIT_FAILURE);
	}
    unsigned char* raw_image = img.data;                      /**< @variable raw_image pointer to the input image data */
    int width = img.cols;                                     /**< @variable width input image width */
    int height = img.rows;                                    /**< @variable height input image height */

    Mat new_img;                                              /**< @variable new_img output image */
	new_img.create(height, width, CV_8UC1);
    unsigned char* filter_image = new_img.data;               /**< @variable filter_image pointer to the output image data */

    /**< @brief Select the operator */
	cout << "1. Mean Filter\t2. Prewitt_1\t3. Prewitt_2\n";
	cout << "4. Sobel_1\t5. Sobel_2\t6. Laplacian\n";
	cout << "Input the mat number: ";
	int t, sum = 0;
	cin >> t;
	while (t < 1 || t > 6) {
		cout << "Error!Input again: ";
		cin >> t;
	}
	switch (t) {
	case 1:
		memcpy(&temp, mat_mean_filter, sizeof(temp)); break;
	case 2:
		memcpy(&temp, mat_Prewitt_1, sizeof(temp)); break;
	case 3:
		memcpy(&temp, mat_Prewit_2, sizeof(temp)); break;
	case 4:
		memcpy(&temp, mat_Sobel_1, sizeof(temp)); break;
	case 5:
		memcpy(&temp, mat_Sobel_2, sizeof(temp)); break;
	case 6:
		memcpy(&temp, mat_Laplacian, sizeof(temp)); break;
	default:
		break;
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			sum += temp[i * 3 + j] >= 0 ? temp[i * 3 + j] : -temp[i * 3 + j];
		}
	}
    /**< @brief Convolution operations */
	for (int i = 1; i <= height - 2; i++) {
		for (int j = 1; j <= width - 2; j++) {
			int res = 0;
			for (int m = i - 1; m <= i + 1; m++) {
				for (int n = j - 1; n <= j + 1; n++) {
					res += raw_image[m*width + n] * temp[(m - i + 1) * 3 + n - j + 1];
				}
			}
			if (t != 6)res /= sum;
			filter_image[i*width + j] = res >= 0 ? res : -res;
		}
	}
    /**< @brief Special judge for edge pixel */
	for (int i = 0; i <= width - 1; i++)filter_image[0 * width + i] = raw_image[0 * width + i];
	for (int i = 0; i <= width - 1; i++)filter_image[(height - 1) * width + i] = raw_image[(height - 1) * width + i];
	for (int i = 0; i <= height - 1; i++)filter_image[i * width + 0] = raw_image[i * width + 0];
	for (int i = 0; i <= height - 1; i++)filter_image[i * width + width - 1] = raw_image[i * width + width - 1];

    /**< @brief display and save the output image */
	namedWindow("display");
	imshow("display", new_img);
	imwrite(output_file_name, new_img);
	waitKey(0);
	return 0;
}
