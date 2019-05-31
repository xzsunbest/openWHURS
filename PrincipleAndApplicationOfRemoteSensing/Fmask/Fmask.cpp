#include"Fmask.h"

bool Fmask::readLandsatImage(std::array<const char*, 7>& inputFileNames) {
	GDALAllRegister();
	std::array<GDALDataset*, 7> inputDatasets;
	std::cout << "Reading Landsat Image...\n";
	for (int i = 0; i < 7; i++) {
		inputDatasets[i] = (GDALDataset *)GDALOpen(inputFileNames[i], GA_ReadOnly);
		if (inputDatasets[i] == nullptr) {
			std::cout << "Error!Reading Failure: " << inputFileNames[i] << "!\n";
			return false;
		}
	}
	std::cout << "Reading Landsat Image Complete.\n";
	imgSizeX = inputDatasets[0]->GetRasterXSize();
	imgSizeY = inputDatasets[0]->GetRasterYSize();
	calculateTOAReflectance(inputDatasets);
	calculateTOABrightnessTemperature(inputDatasets);
	calculateNDVI();
	calculateNDSI();
	calculateTwater();
	calculateTland();
	calculateLandThreshold();
	return true;
}

bool Fmask::generatePotentialCloudLayer(const char * outputFileName) {
	PCPlayer = new unsigned char[imgSizeX*imgSizeY];
	bool flag = false;
	for (int i = 0; i < imgSizeX*imgSizeY; i++) {
		flag = (potentialCloudPixelTest(i) && waterTest(i) && calculateWaterCloudProbability(i) > 0.5) ||
			(potentialCloudPixelTest(i) && !waterTest(i) && calculateLandCloudProbability(i) > LandThreshold) ||
			(calculateLandTemperatureProbability(i) > 0.7 && !waterTest(i)) ||
			//(calculateLandCloudProbability(i) > 0.99 && !waterTest(i)) ||
			(TOABrightnessTemperatureData[i] < (Tlow - 35.0));
		if (flag)PCPlayer[i] = 255;
		else if (!potentialCloudPixelTest(i)&&hotTest(i)&&whitenessTest(i))PCPlayer[i] = 128;
		//else if (waterTest(i))PCPlayer[i] = 128;
		else PCPlayer[i] = 0;
	}

	/*int tmp = 383 * imgSizeX + 1451;
	std::cerr << potentialCloudPixelTest(tmp) << std::endl;
	std::cerr << TOAReflectanceData[0][tmp] << std::endl;
	std::cerr << TOAReflectanceData[1][tmp] << std::endl;
	std::cerr << TOAReflectanceData[2][tmp] << std::endl;*/

	GDALDataset* outputDataset;
	const char* imageFormat = "GTiff";
	GDALDriver* gdalDriver = GetGDALDriverManager()->GetDriverByName(imageFormat);
	if (gdalDriver == nullptr) {
		std::cout << "Create PCP Layer Image Failure!\n";
		return false;
	}
	std::cout << "Generate PCP Layer Image...\n";
	outputDataset = gdalDriver->Create(outputFileName, imgSizeX, imgSizeY, 1, GDT_Byte, NULL);
	GDALRasterBand* outputRasterBand = outputDataset->GetRasterBand(1);
	outputRasterBand->RasterIO(GF_Write, 0, 0, imgSizeX, imgSizeY, PCPlayer, imgSizeX, imgSizeY, GDT_Byte, 0, 0);
	GDALClose(outputDataset);
	GetGDALDriverManager()->DeregisterDriver(gdalDriver);
	std::cout << "Generate PCP Layer Image Complete.\n";
	return true;
}

void Fmask::calculateTOAReflectance(std::array<GDALDataset*, 7>& inputDatasets) {
	std::cout << "Calculate Top of Atmosphere Reflectance...\n";
	for (int i = 0; i < 6; i++) {
		TOAReflectanceData[i] = new float[imgSizeX*imgSizeY];
		GDALRasterBand* inputRasterBand = inputDatasets[i]->GetRasterBand(1);
		inputRasterBand->RasterIO(GF_Read, 0, 0, imgSizeX, imgSizeY, TOAReflectanceData[i], imgSizeX, imgSizeY, GDT_Float32, 0, 0);
		for (int j = 0; j < imgSizeX*imgSizeY; j++)
			TOAReflectanceData[i][j] = (REFLECTANCE_MULT_BAND[i] * TOAReflectanceData[i][j] + REFLECTANCE_ADD_BAND[i])
			/ std::cos(SUN_ELEVATION);
		GDALClose(inputDatasets[i]);
	}
	std::cout << "Calculate Top of Atmosphere Reflectance Complete.\n";
}

void Fmask::calculateTOABrightnessTemperature(std::array<GDALDataset*, 7>& inputDatasets) {
	std::cout << "Calculate Top of Atmosphere Brightness Temperature...\n";
	TOABrightnessTemperatureData = new float[imgSizeX*imgSizeY];
	GDALRasterBand* inputRasterBand = inputDatasets[6]->GetRasterBand(1);
	inputRasterBand->RasterIO(GF_Read, 0, 0, imgSizeX, imgSizeY, TOABrightnessTemperatureData, imgSizeX, imgSizeY, GDT_Float32, 0, 0);
	for (int i = 0; i < imgSizeX*imgSizeY; i++) {
		TOABrightnessTemperatureData[i] = RADIANCE_MULT_BAND_6 * TOABrightnessTemperatureData[i] + RADIANCE_ADD_BAND_6;
		TOABrightnessTemperatureData[i] = K2_CONSTANT_BAND_6 / std::log(K1_CONSTANT_BAND_6 / TOABrightnessTemperatureData[i] + 1);
		TOABrightnessTemperatureData[i] -= 273.15;
	}
	GDALClose(inputDatasets[6]);
	std::cout << "Calculate Top of Atmosphere Brightness Temperature Complete.\n";
}

void Fmask::calculateNDVI() {
	std::cout << "Calculate NDVI...\n";
	NDVIData = new float[imgSizeX*imgSizeY];
	for (int i = 0; i < imgSizeX*imgSizeY; i++)
		NDVIData[i] = (TOAReflectanceData[3][i] - TOAReflectanceData[2][i])
		/ (TOAReflectanceData[3][i] + TOAReflectanceData[2][i]);
	std::cout << "Calculate NDVI Complete.\n";
}

void Fmask::calculateNDSI() {
	std::cout << "Calculate NDSI...\n";
	NDSIData = new float[imgSizeX*imgSizeY];
	for (int i = 0; i < imgSizeX*imgSizeY; i++)
		NDSIData[i] = (TOAReflectanceData[1][i] - TOAReflectanceData[4][i])
		/ (TOAReflectanceData[1][i] + TOAReflectanceData[4][i]);
	std::cout << "Calculate NDSI Complete.\n";
}

void Fmask::calculateTwater() {
	std::cout << "Calculate Twater...\n";
	Twater = 0.0;
	std::vector<float> water_temperature;
	for (int i = 0; i < imgSizeX*imgSizeY; i++)
		if (clearSkyWaterTest(i))water_temperature.push_back(TOABrightnessTemperatureData[i]);
	std::sort(water_temperature.begin(), water_temperature.end(), std::greater<float>());
	int t = (int)(0.825*water_temperature.size());
	for (int i = 0; i < t; i++)Twater += water_temperature[i];
	Twater /= t;
	std::cout << "Calculate Twater Complete.\n";
}

void Fmask::calculateTland() {
	std::cout << "Calculate Tland...\n";
	Thigh = 0.0;
	Tlow = 0.0;
	std::vector<float> land_temperature;
	for (int i = 0; i < imgSizeX*imgSizeY; i++)
		if (clearSkyLandTest(i))land_temperature.push_back(TOABrightnessTemperatureData[i]);
	std::sort(land_temperature.begin(), land_temperature.end(), std::greater<float>());
	int t = (int)(0.825*land_temperature.size());
	for (int i = 0; i < t; i++)Thigh += land_temperature[i];
	Thigh /= t;
	for (int i = t; i < land_temperature.size(); i++)Tlow += land_temperature[i];
	Tlow /= (land_temperature.size() - t);
	std::cout << "Calculate Tland Complete.\n";
}

void Fmask::calculateLandThreshold() {
	std::cout << "Calculate LandThreshold...\n";
	LandThreshold = 0.0;
	std::vector<float> land_cloud_probability;
	for (int i = 0; i < imgSizeX*imgSizeY; i++)
		land_cloud_probability.push_back(calculateLandCloudProbability(i));
	std::sort(land_cloud_probability.begin(), land_cloud_probability.end(), std::greater<float>());
	int t = (int)(0.825*land_cloud_probability.size());
	for (int i = 0; i < t; i++)LandThreshold += land_cloud_probability[i];
	LandThreshold /= t;
	LandThreshold += 0.2;
	std::cout << "Calculate LandThreshold Complete.\n";
}

float Fmask::calculateWaterCloudProbability(int x) {
	float water_temperature_probability = (Twater - TOABrightnessTemperatureData[x]) / 4.0;
	float brightness_probability = std::min(TOAReflectanceData[4][x], 0.11f) / 0.11;
	return water_temperature_probability * brightness_probability;
}

float Fmask::calculateLandCloudProbability(int x) {
	float meanVis = (TOAReflectanceData[0][x] + TOAReflectanceData[1][x] + TOAReflectanceData[2][x]) / 3.0;
	float whiteness = 0.0;
	for (int i = 0; i < 3; i++)
		whiteness += std::abs((TOAReflectanceData[i][x] - meanVis) / meanVis);
	float land_temperature_probability = (Thigh + 4.0 - TOABrightnessTemperatureData[x]) /
		                                 (Thigh - Tlow + 8.0);
	float modified_NDVI = (std::abs(NDVIData[x]) < 0.8) ? std::abs(NDVIData[x]) : 0.8;
	float modified_NDSI = (std::abs(NDSIData[x]) < 0.8) ? std::abs(NDSIData[x]) : 0.8;
	float variability_probability = 1.0 - std::max(std::max(modified_NDVI, modified_NDSI), whiteness);
	return land_temperature_probability * variability_probability;
}

float Fmask::calculateLandTemperatureProbability(int x) {
	return (Thigh + 4.0 - TOABrightnessTemperatureData[x]) / (Thigh - Tlow + 8.0);
}

inline bool Fmask::basicTest(int x) {
	return ((TOAReflectanceData[5][x] > 0.03) &&
		(TOABrightnessTemperatureData[x] < 27) &&
		(NDSIData[x] < 0.8) &&
		(NDVIData[x] < 0.8));
}

inline bool Fmask::whitenessTest(int x) {
	float meanVis = (TOAReflectanceData[0][x] + TOAReflectanceData[1][x] + TOAReflectanceData[2][x]) / 3.0;
	float whiteness = 0.0;
	for (int i = 0; i < 3; i++)
		whiteness += std::abs((TOAReflectanceData[i][x] - meanVis) / meanVis);
	return (whiteness < 0.7);
}

inline bool Fmask::hotTest(int x) {
	return ((TOAReflectanceData[0][x] - 0.5*TOAReflectanceData[2][x] - 0.08) > 0.0);
	//return true;
}

inline bool Fmask::band45Test(int x) {
	return (TOAReflectanceData[3][x] / TOAReflectanceData[4][x] > 0.75);
}

inline bool Fmask::waterTest(int x) {
	return ((NDVIData[x] < 0.01&&TOAReflectanceData[3][x] < 0.11) ||
		(NDVIData[x] < 0.1&&TOAReflectanceData[3][x] < 0.05));
}

inline bool Fmask::potentialCloudPixelTest(int x) {
	return basicTest(x) && whitenessTest(x) && hotTest(x) && band45Test(x);
}

inline bool Fmask::clearSkyWaterTest(int x) {
	return (waterTest(x) && TOAReflectanceData[5][x] < 0.03);
}

inline bool Fmask::clearSkyLandTest(int x) {
	return (!potentialCloudPixelTest(x) && !waterTest(x));
}