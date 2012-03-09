/*
 * VisualMath.cpp
 *
 *  Created on: Feb 28, 2012
 *      Author: frc3928
 */
#include "../Headers/HelperFunctions.h"
using namespace cv;

float pointDistance(CvPoint point1, CvPoint point2)
{
	return sqrt( ((point2.y - point1.y)*(point2.y - point1.y)) + ((point2.x - point1.x)*(point2.x - point1.x)) );
}

char* floatToString(float flt)
{
    char* str = new char[30];
    sprintf(str, "%.4g", flt );
    return str;
}

float metersToInches(float markerCoordinate)
{
	return markerCoordinate*39.3700787401575;
}


float calculateVectorFrom3Offsets(float vecX, float vecY,float vecZ)
{
	return sqrt( (vecX*vecX) + (vecY*vecY) + (vecZ*vecZ) );
}

void matrixMultiply(float *a, double *b, float *result, int dimension)
{
	for (int i=0; i<dimension; i++)
	{
		for (int j=0; j<dimension; j++)
		{
			result[(i*dimension)+j] = 0;
			for (int k=0; k<dimension; k++)
			{
				result[(i*dimension)+j] += a[(i*dimension)+k]*b[(k*dimension)+j];
			}
		}
	}
}

float inchesToMeters(int inches)
{
	return inches*0.0254;
}

float rad2deg(float rads)
{
	return rads*57.2957795;
}

float deg2rad(float degrees)
{
	return degrees*0.0174532925;
}

CvPoint3D32f crossProduct(CvPoint3D32f a, CvPoint3D32f b)
{
	CvPoint3D32f result;
	result.x = (a.y * b.z) - (a.z * b.y);
	result.y = (a.z * b.x) - (a.x * b.z);
	result.z = (a.x * b.y) - (a.y * b.x);
	return result;
}
