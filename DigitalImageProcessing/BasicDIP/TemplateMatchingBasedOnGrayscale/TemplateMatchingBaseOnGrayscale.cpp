/**
 * @file TemplateMatchingBaseOnGrayscale.cpp
 * @author Xiang Zheng
 * @date 2017.12.27
 * @brief This program realizes the template matching based on gray scale
 */
#include<iostream>
#include<opencv2\opencv.hpp>
using namespace std;
using namespace cv;
int main() {
    const char* input_file_name = "image.bmp";           /**< @variable input_file_name input file name */
    const char* template_file_name = "template.bmp";     /**< @variable template_file_name template file name */
    const char* output_file_name = "result.bmp";         /**< @variable output_file_name output file name */

    Mat img = imread(input_file_name, 0);                /**< @variable img input image */
	if (img.empty()) {
		cout << "Can't open file " << input_file_name << "!\n";
		exit(EXIT_FAILURE);
	}
    unsigned char* raw_image = img.data;                 /**< @variable raw_image pointer to the input image data */
    int width = img.cols;                                /**< @variable width input iamge width */
    int height = img.rows;                               /**< @variable height input image height */
	
    Mat img_t = imread(template_file_name, 0);           /**< @variable img_t template image */
	if (img_t.empty()) {
		cout << "Can't open file " << template_file_name << "!\n";
		exit(EXIT_FAILURE);
	}
    unsigned char* template_image = img_t.data;          /**< @variable template_image pointer to the template image data */
    int twidth = img_t.cols;                             /**< @variable twidth template image width */
    int theight = img_t.rows;                            /**< @variable theight template image height */

    Mat new_img;                                         /**< @variable new_img output image */
	new_img.create(theight, twidth, CV_8UC1);
    unsigned char* match_image = new_img.data;           /**< @variable matching_image pointer to the output image data */

    double SigmaT = 0.0;                                 /**< @variable SigmaT intermediate variance T */
	for (int i = 0; i < theight; i++) {
		for (int j = 0; j < twidth; j++) {
			SigmaT += pow(static_cast<double>(template_image[i*twidth + j]), 2);
		}
	}

    double MaxR = 0.0;                                   /**< @variable MaxR maximum similarity measure */
    double SigmaS;                                       /**< @variable SigmaS intermediate variance S */
    double SigmaST;                                      /**< @variable SigmaST intermediate variance ST */
    double R;                                            /**< @variable R similarity measure */
	int x0, y0;
	for (int i = 0; i <= height - theight; i++) {
		for (int j = 0; j <= width - twidth; j++) {
			SigmaS = 0.0;
			SigmaST = 0.0;

			for (int m = 0; m < theight; m++) {
				for (int n = 0; n < twidth; n++) {
					SigmaS += pow(static_cast<double>(raw_image[(i + m)*width + j + n]), 2);
					SigmaST += static_cast<double>(raw_image[(i + m)*width + j + n])
						      *static_cast<double>(template_image[m * twidth + n]);
				}
			}

            /**< @brief Calculation of normalized autocorrelation function */
			R = SigmaST / (sqrt(SigmaS)*sqrt(SigmaT));
			if (R > MaxR) {
				MaxR = R;
				x0 = i;
				y0 = j;
			}
		}
	}

    /**< @brief display the matching point coordinates */
	cout << "Mathcing Point Coordinates: (" << y0 << ", " << x0 << ")\n";

	for (int i = 0; i < theight; i++) {
		for (int j = 0; j < twidth; j++) {
			match_image[i*twidth + j] = raw_image[(x0 + i)*width + y0 + j];
		}
	}

    /**< @brief display and save the output image */
	namedWindow("display");
	imshow("display", new_img);
	imwrite(output_file_name, new_img);
	waitKey(0);
	return 0;
}
