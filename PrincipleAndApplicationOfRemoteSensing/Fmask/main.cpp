#include"Fmask.h"

int main() {
	/*std::array<const char*, 7> inputs = {
		"LT05_L1TP_123039_20110912_20161006_01_T1_B1.TIF",
		"LT05_L1TP_123039_20110912_20161006_01_T1_B2.TIF",
		"LT05_L1TP_123039_20110912_20161006_01_T1_B3.TIF",
		"LT05_L1TP_123039_20110912_20161006_01_T1_B4.TIF",
		"LT05_L1TP_123039_20110912_20161006_01_T1_B5.TIF",
		"LT05_L1TP_123039_20110912_20161006_01_T1_B7.TIF",
		"LT05_L1TP_123039_20110912_20161006_01_T1_B6.TIF" };
	const char* output = "LT05_L1TP_123039_20110912_20161006_01_T1_CLOUD.TIF";*/
	std::array<const char*, 7> inputs = {
		"band1.tif",
		"band2.tif",
		"band3.tif",
		"band4.tif",
		"band5.tif",
		"band7.tif",
		"band6.tif" };
	const char* output = "band_cloud.tif";
	Fmask fmask;
	fmask.readLandsatImage(inputs);
	fmask.generatePotentialCloudLayer(output);
	getchar();
	return 0;
}