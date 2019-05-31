/**
 * @file raw2bmp.cpp
 * @author Xiang Zheng
 * @date 2017.12.25
 * @brief This program converts RAW format image to BMP Format image.
 * @note The program is valid only for grayscale-type RAW format image.
 *       The width and the height of the image must be specified by the user.
 */
#include<iostream>
#include<Windows.h>
using namespace std;
int main() {
    const char* input_file_name = "test.raw";   /**< @variable input_file_name input file name */
    const char* output_file_name = "test.bmp";  /**< @variable output_file_name output file name */
    const int width = 1642;                     /**< @variable width image width */
    const int height = 1095;                    /**< @variable height image height */
    const BYTE zero = 0;                        /**< @variable zero zero of unsigned-char-type */

    /**< @brief Open file */
	FILE *fin = fopen(input_file_name, "rb");
	if (!fin) {
		cout << "Can't open file!\n";
		exit(EXIT_FAILURE);
	}
	
    /**< @brief Read RAW file */
	BYTE *raw_image = new BYTE[width*height*3];
	fread(raw_image, sizeof(BYTE), width*height*3, fin);
	fclose(fin);

	FILE *fout = fopen(output_file_name, "wb");

    BITMAPFILEHEADER bmfHdr;                    /**< @variable bmfHdr bitmap file header */
    BITMAPINFOHEADER bmpHdr;                    /**< @variable bmpHdr bitmap info header */
    RGBQUAD rgb[256];                           /**< @variable rgb color table */

    /**< @brief Set bitmap file header */
    bmfHdr.bfType = 'MB';
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;

    /**< @brief Set bitmap info header */
	bmpHdr.biSize = 40;
	bmpHdr.biWidth = width;
	bmpHdr.biHeight = height;
	bmpHdr.biPlanes = 1;
	bmpHdr.biCompression = BI_RGB;
	bmpHdr.biXPelsPerMeter = 0;
	bmpHdr.biYPelsPerMeter = 0;
	bmpHdr.biClrUsed = 0;
	bmpHdr.biClrImportant = 0;

	bmpHdr.biBitCount = 8;
	bmfHdr.bfOffBits = 14 + 40 + 256 * 4;
	bmpHdr.biSizeImage = (((width * 8) + 31) / 32 * 4)*height;
	bmfHdr.bfSize = bmfHdr.bfOffBits + bmpHdr.biSizeImage;

    /**< @brief Set color table */
	for (int i = 0; i < 256; i++) {
		rgb[i].rgbBlue = i;
		rgb[i].rgbGreen = i;
		rgb[i].rgbRed = i;
		rgb[i].rgbReserved = 0;
	}

    /**< @brief Write BMP file header */
	fwrite(&bmfHdr, sizeof(bmfHdr), 1, fout);
	fwrite(&bmpHdr, sizeof(bmpHdr), 1, fout);
	fwrite(rgb, sizeof(rgb), 1, fout);

    /**< @brief Write BMP file */
	int width_bytes = bmpHdr.biSizeImage / height;
	for (int i = 0; i < height ; i++) {
		fwrite(raw_image + width * (height - i - 1), sizeof(BYTE), width, fout);
		if (width != width_bytes) {
			for (int j = 0; j < width_bytes - width; j++) {
				fwrite(&zero, sizeof(BYTE), 1, fout);
			}
		}
	}
    /**< @brief Close BMP file */
	fclose(fout);
	cout << "Program Complete.\n";
	getchar();
	return 0;
}
