/*
 * Target.cpp
 *
 *  Created on: Feb 21, 2012
 *      Author: frc3928
 */
#include "../Headers/Target.h"
#include "../Headers/HelperFunctions.h"
#include "../Constants.h"
#include <math.h>

using namespace cv;
using namespace Lines;


Target::Target()
{

}

Target::Target(CvSeq* cntr, IplImage* img)
{
	boundingBox = cvBoundingRect(cntr);
	originImage = img;
	contour = *cntr;
}

CvPoint Target::getBoundingBoxPoint1()
{
	return cvPoint(boundingBox.x, boundingBox.y);
}

CvPoint Target::getBoundingBoxPoint2()
{
	return cvPoint(boundingBox.x+boundingBox.width, boundingBox.y+boundingBox.height);
}

CvPoint Target::getCenter()
{
	return cvPoint(boundingBox.x+(0.5*boundingBox.width), boundingBox.y+(0.5*boundingBox.height));
}

void Target::drawTarget(CvScalar color)
{
	cvCircle(originImage, this->getCenter(), 5, color, 2, 8, 0);
	cvRectangleR(originImage, boundingBox, color, 2, 8, 0);
	cvDrawContours(originImage, &contour, color, color, 1, 1, 0);

	cvLine(originImage, getCenter(), cvPoint(320, getCenter().y), color, 1, 8, 0);
	cvLine(originImage, getCenter(), cvPoint(getCenter().x, 240), color, 1, 8, 0);



	//draw height of bbox

}

void Target::getNavigationString()
{
	float x = getCenter().x- 320.0;
	float angle = asin((x*0.608761429)/320); //0.608761429 = sin(37.5 degrees)
	offsets[1] = x/(tan(angle));
	offsets[0] = rad2deg(angle);
}

int Target::getArea()
{
	return boundingBox.width*boundingBox.height;
}

float Target::getRectangularity()
{
	return (cvContourArea(&contour)/getArea())*100;
}

float Target::getAspectRatio()
{
	return ((float) boundingBox.width)/boundingBox.height;
}
