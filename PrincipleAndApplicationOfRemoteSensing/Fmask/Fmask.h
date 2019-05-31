#pragma once

#include<iostream>
#include<vector>
#include<array>
#include<cmath>
#include<algorithm>
#include<functional>

#include<gdal.h>
#include<gdal_priv.h>
#include<ogr_spatialref.h>

const double REFLECTANCE_MULT_BAND[6] = { 1.2540e-03,     //Band 1
									      2.6207e-03,     //Band 2
                                          2.2303e-03,     //Band 3
                                          2.6995e-03,     //Band 4
                                          1.8278e-03,     //Band 5
                                          2.5372e-03 };   //Band 7
const double REFLECTANCE_ADD_BAND[6] = { -0.003743,       //Band 1
									     -0.007760,       //Band 2
                                         -0.004730,       //Band 3
                                         -0.007353,       //Band 4
                                         -0.007447,       //Band 5
                                         -0.008343, };    //Band 7
const double K1_CONSTANT_BAND_6 = 607.76;
const double K2_CONSTANT_BAND_6 = 1260.56;
const double RADIANCE_MULT_BAND_6 = 5.5375e-02;
const double RADIANCE_ADD_BAND_6 = 1.18243;
const double SUN_ELEVATION = 55.48309511;

class Fmask {
private:
	std::array<float*, 6> TOAReflectanceData;
	float* TOABrightnessTemperatureData;
	float* NDVIData;
	float* NDSIData;
	unsigned char* PCPlayer;
	int imgSizeX;//image width
	int imgSizeY;//image height
	float Twater;
	float Tlow;
	float Thigh;
	float LandThreshold;
	void calculateTOAReflectance(std::array<GDALDataset*, 7>& inputDatasets);
	void calculateTOABrightnessTemperature(std::array<GDALDataset*, 7>& inputDatasets);
	void calculateNDVI();
	void calculateNDSI();
	void calculateTwater();
	void calculateTland();
	void calculateLandThreshold();
	float calculateWaterCloudProbability(int x);
	float calculateLandCloudProbability(int x);
	float calculateLandTemperatureProbability(int x);
	bool basicTest(int x);
	bool whitenessTest(int x);
	bool hotTest(int x);
	bool band45Test(int x);
	bool waterTest(int x);
	bool potentialCloudPixelTest(int x);
	bool clearSkyWaterTest(int x);
	bool clearSkyLandTest(int x);
public:
	Fmask() = default;
	~Fmask() = default;
	bool readLandsatImage(std::array<const char*, 7>& inputFileNames);
	bool generatePotentialCloudLayer(const char* outputFileName);
};