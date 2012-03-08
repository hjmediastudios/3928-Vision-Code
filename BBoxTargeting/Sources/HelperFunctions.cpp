/*
 * VisualMath.cpp
 *
 *  Created on: Feb 28, 2012
 *      Author: frc3928
 */
#include "../Headers/HelperFunctions.h"
using namespace cv;
using namespace aruco;

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

void drawAxis(IplImage *Image, aruco::Marker &m,const aruco::CameraParameters &CP)
{
	Mat objectPoints (4,3,CV_32FC1);
		objectPoints.at<float>(0,0)=0;
		objectPoints.at<float>(0,1)=0;
		objectPoints.at<float>(0,2)=0;
		objectPoints.at<float>(1,0)=m.ssize;
		objectPoints.at<float>(1,1)=0;
		objectPoints.at<float>(1,2)=0;
		objectPoints.at<float>(2,0)=0;
		objectPoints.at<float>(2,1)=m.ssize;
		objectPoints.at<float>(2,2)=0;
		objectPoints.at<float>(3,0)=0;
		objectPoints.at<float>(3,1)=0;
		objectPoints.at<float>(3,2)=m.ssize;

		vector<Point2f> imagePoints;
		cv::projectPoints( objectPoints, m.Rvec, m.Tvec, CP.CameraMatrix,CP.Distorsion,   imagePoints);

		//draw lines of different colors
		cvLine(Image,imagePoints[0],imagePoints[1],Scalar(255,0,0,255),1,CV_AA);
		cvLine(Image,imagePoints[0],imagePoints[2],Scalar(0,255,0,255),1,CV_AA);
		cvLine(Image,imagePoints[0],imagePoints[3],Scalar(0,0,255,255),1,CV_AA);
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
