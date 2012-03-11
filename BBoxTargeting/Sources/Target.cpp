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
	targetIndex = 3; //set targets as bottom target by default
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
	cvCircle(originImage, cvPoint(leftX(), bottomY()), 2, color, 4, 8, 0);
	cvRectangleR(originImage, boundingBox, color, 2, 8, 0);
	cvDrawContours(originImage, &contour, color, color, 1, 1, 0);

	cvLine(originImage, cvPoint(leftX(), bottomY()), cvPoint(320, bottomY()), color, 1, 8, 0);
	cvLine(originImage, cvPoint(leftX(), bottomY()), cvPoint(leftX(), 240), color, 1, 8, 0);
}

void Target::getNavigationString()
{
	//TODO add functions here to make it work
	offsets[0] = getXDistanceFromAspectRatio();
	offsets[1] = getYDistanceFromBBoxHeight();
	offsets[2] = getAngleFromOffsets();
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

void Target::setTargetIndex(int indx)
{
	targetIndex = indx;
}

int Target::getTargetIndex()
{
	return targetIndex;
}

int Target::leftX()
{
	return boundingBox.x;
}

int Target::rightX()
{
	return boundingBox.x + boundingBox.width;
}

int Target::topY()
{
	return boundingBox.y;
}

int Target::bottomY()
{
	return boundingBox.y + boundingBox.height;
}

int Target::width()
{
	return boundingBox.width;
}

int Target::height()
{
	return boundingBox.height;
}

float Target::getYDistanceFromBBoxHeight()
{
	return 0.0;
}

float Target::getXDistanceFromAspectRatio()
{
	return 0.0;
}

float Target::getAngleFromOffsets()
{
	if(offsets[1] != 0)
		return atan(offsets[0]/offsets[1]);
	else
		return 0.0;
}
