#include<gdal.h>
#include<gdal_priv.h>
#include<Eigen/Dense>
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<cstdint>
#include<complex>
#include<algorithm>
using namespace std;
using namespace Eigen;

typedef Matrix<double, 5, 1> Vector5d;

const double PI = 3.141592653589793;
const double EPS1 = 1e-8;
const double EPS2 = 1e-12;
double listX[6], listY[6], listZ[6];
double t0, dt;
double borderX[5], borderY[5];
double trow, frow, tcol, fcol;
double axis1, axis2;
double GeoTransform[6];
double vecx[4], vecy[4], vecz[4];
int minX, minY, maxX, maxY, outX, outY;

struct satVec {
	double x, y, z, vx, vy, vz, ax, ay, az;
	satVec() = default;
	satVec(double _x, double _y, double _z, double _vx, double _vy, double _vz, double _ax, double _ay, double _az) :
		x(_x), y(_y), z(_z), vx(_vx), vy(_vy), vz(_vz), ax(_ax), ay(_ay), az(_az) {};
};

Vector4d calculateCoefficient(MatrixXd t, Vector5d x) {
	Vector4d a(0, 0, 0, 0);
	Vector5d dx = x;
	double eps = 1.0;
	while (eps > EPS1) {
		Vector4d da = (t.transpose()*t).inverse()*t.transpose()*dx;
		a += da;
		dx = x - t * a;
		eps = max(max(abs(da[0]), abs(da[1])), max(abs(da[2]), abs(da[3])));
	}
	return a;
}

satVec calculateSatelliteVector(double t) {
	double x = vecx[0] + vecx[1] * t + vecx[2] * pow(t, 2) + vecx[3] * pow(t, 3);
	double y = vecy[0] + vecy[1] * t + vecy[2] * pow(t, 2) + vecy[3] * pow(t, 3);
	double z = vecz[0] + vecz[1] * t + vecz[2] * pow(t, 2) + vecz[3] * pow(t, 3);
	double vx = vecx[1] + 2 * vecx[2] * t + 3 * vecx[3] * pow(t, 2);
	double vy = vecy[1] + 2 * vecy[2] * t + 3 * vecy[3] * pow(t, 2);
	double vz = vecz[1] + 2 * vecz[2] * t + 3 * vecz[3] * pow(t, 2);
	double ax = 2 * vecx[2] + 6 * vecx[3] * t;
	double ay = 2 * vecy[2] + 6 * vecy[3] * t;
	double az = 2 * vecz[2] + 6 * vecz[3] * t;
	satVec res(x, y, z, vx, vy, vz, ax, ay, az);
	return res;
}

int main() {
	FILE *fp = fopen("parameter.txt", "rt");
	for (int i = 1; i <= 5; i++)fscanf(fp, "%lf%lf%lf", &listX[i], &listY[i], &listZ[i]);
	fscanf(fp, "%lf%lf", &t0, &dt);
	for (int i = 1; i <= 4; i++)fscanf(fp, "%lf%lf", &borderX[i], &borderY[i]);
	fscanf(fp, "%lf%lf", &trow, &frow);
	fscanf(fp, "%lf%lf", &tcol, &fcol);
	fscanf(fp, "%lf%lf", &axis1, &axis2);
	tcol *= 1e-3;
	fcol *= 1e6;
	axis1 *= 1000;
	axis2 *= 1000;
	fclose(fp);
	Vector5d vecX, vecY, vecZ;
	vecX << listX[1], listX[2], listX[3], listX[4], listX[5];
	vecY << listY[1], listY[2], listY[3], listY[4], listY[5];
	vecZ << listZ[1], listZ[2], listZ[3], listZ[4], listZ[5];
	MatrixXd matT(5, 4);
	matT << 1, 0, 0, 0,
			1, dt, pow(dt, 2), pow(dt, 3),
			1, 2 * dt, pow(2 * dt, 2), pow(2 * dt, 3),
			1, 3 * dt, pow(3 * dt, 2), pow(3 * dt, 3),
			1, 4 * dt, pow(4 * dt, 2), pow(4 * dt, 3);
	Vector4d coeX = calculateCoefficient(matT, vecX);
	Vector4d coeY = calculateCoefficient(matT, vecY);
	Vector4d coeZ = calculateCoefficient(matT, vecZ);
	for (int i = 0; i < 4; i++)vecx[i] = coeX[i];
	for (int i = 0; i < 4; i++)vecy[i] = coeY[i];
	for (int i = 0; i < 4; i++)vecz[i] = coeZ[i];
	GDALAllRegister();
	GDALDataset* imgDEM = (GDALDataset*)GDALOpen("dem.img", GA_ReadOnly);
	GDALDataset* imgSAR = (GDALDataset*)GDALOpen("sar.img", GA_ReadOnly);
	GDALRasterBand* bandDEM = imgDEM->GetRasterBand(1);
	GDALRasterBand* bandSAR = imgSAR->GetRasterBand(1);
	imgDEM->GetGeoTransform(GeoTransform);
	double mindx = 1e9, mindy = 1e9, maxdx = -1e9, maxdy = -1e9;
	for (int i = 1; i <= 4; i++) {
		mindx = min(mindx, borderX[i]);
		mindy = min(mindy, borderY[i]);
		maxdx = max(maxdx, borderX[i]);
		maxdy = max(maxdy, borderY[i]);
	}
	int DEMxsize = bandDEM->GetXSize();
	int DEMysize = bandDEM->GetYSize();
	int SARxsize = bandSAR->GetXSize();
	int SARysize = bandSAR->GetYSize();
	minX = 0;
	maxX = DEMxsize - 1;
	minY = 0;
	maxY = DEMysize - 1;
	outX = maxX - minX + 1;
	outY = maxY - minY + 1;
	uint16_t *output = new uint16_t[outX*outY];
	//complex<uint16_t> *SARdata = new complex<uint16_t>[SARxsize*SARysize];
	uint16_t *SARdata = new uint16_t[SARxsize*SARysize];
	int16_t *DEMdata = new int16_t[DEMxsize*DEMysize];
	bandDEM->RasterIO(GF_Read, 0, 0, DEMxsize, DEMysize, DEMdata, DEMxsize, DEMysize, GDT_Int16, 0, 0);
	bandSAR->RasterIO(GF_Read, 0, 0, SARxsize, SARysize, SARdata, SARxsize, SARysize, GDT_UInt16, 0, 0);
	for (int i = minY; i <= maxY; i++) {
		cout << i - minY << endl;
		for (int j = minX; j <= maxX; j++) {
			double B = (GeoTransform[3] + GeoTransform[4] * j + GeoTransform[5] * i)*PI / 180.0;
			double L = (GeoTransform[0] + GeoTransform[1] * j + GeoTransform[2] * i)*PI / 180.0;
			double h = DEMdata[i*DEMxsize + j];
			double e = sqrt(axis1*axis1 - axis2 * axis2) / axis1;
			double N = axis1 / sqrt(1.0 - pow(e*sin(B), 2));
			double x = (N + h)*cos(B)*cos(L);
			double y = (N + h)*cos(B)*sin(L);
			double z = (N*(1 - e * e) + h)*sin(B);
			double t = 2.0 * dt;
			double delta = 1.0;
			int iter = 0;
			while (iter <= 20 && abs(delta) > EPS2) {
				satVec v = calculateSatelliteVector(t);
				delta = (v.vx * (v.x - x) + v.vy * (v.y - y) + v.vz * (v.z - z)) /
						(v.ax * (v.x - x) + v.ay * (v.y - y) + v.az * (v.z - z) + v.vx * v.vx + v.vy * v.vy + v.vz * v.vz);
				t -= delta;
				++iter;
			}
			satVec s = calculateSatelliteVector(t);
			double R = pow(s.x - x, 2) + pow(s.y - y, 2) + pow(s.z - z, 2);
			double iSAR = (t + t0 - trow)*frow;
			double jSAR = (2 * sqrt(R) / 299792458.458 - tcol)*fcol;
			int isar = static_cast<int>(iSAR), jsar = static_cast<int>(jSAR);
			int hsar = imgSAR->GetRasterYSize(), wsar = imgSAR->GetRasterXSize();
			if (isar<0 || isar + 2>hsar || jsar<0 || jsar + 2>wsar) {
				output[(i - minY)*outX + j - minX] = 0;
				continue;
			}
			double rx = iSAR - isar, ry = jSAR - jsar;
			//double pLU = sqrt(pow(SARdata[isar*SARxsize + jsar].real(), 2) + pow(SARdata[isar*SARxsize + jsar].imag(), 2));
			//double pRU = sqrt(pow(SARdata[(isar + 1)*SARxsize + jsar].real(), 2) + pow(SARdata[(isar + 1)*SARxsize + jsar].imag(), 2));
			//double pLD = sqrt(pow(SARdata[isar*SARxsize + jsar + 1].real(), 2) + pow(SARdata[isar*SARxsize + jsar + 1].imag(), 2));
			//double pRD = sqrt(pow(SARdata[(isar + 1)*SARxsize + jsar + 1].real(), 2) + pow(SARdata[(isar + 1)*SARxsize + jsar + 1].imag(), 2));
			double pLU = SARdata[isar*SARxsize + jsar];
			double pRU = SARdata[(isar + 1)*SARxsize + jsar];
			double pLD = SARdata[isar*SARxsize + jsar + 1];
			double pRD = SARdata[(isar + 1)*SARxsize + jsar + 1];
			double res = (1.0 - ry) * ((1.0 - rx)*pLU + rx * pRU) + ry * ((1.0 - rx)*pLD + rx * pRD);
			uint16_t p = static_cast<uint16_t>(res);
			output[(i - minY)*outX + j - minX] = p;
		}
	}
	GDALDataset* imgOut;
	GDALDriver* gdalDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
	imgOut = gdalDriver->Create("output.tif", outX, outY, 1, GDT_UInt16, NULL);
	imgOut->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, outX, outY, output, outX, outY, GDT_UInt16, 0, 0);
	GDALClose(imgDEM);
	GDALClose(imgSAR);
	GDALClose(imgOut);
	return 0;
}