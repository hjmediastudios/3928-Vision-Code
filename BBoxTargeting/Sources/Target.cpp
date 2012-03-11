/*
 * Target.cpp
 *
 *  Created on: Feb 21, 2012
 *      Author: frc3928
 */
#include "../Headers/Target.h"
#include "../Headers/HelperFunctions.h"
#include "../Constants.h"

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

void Target::sendNavigationString()
{
	//TODO add functions here to make it work
	groundDistance = getGroundDistanceFromArea(boundingBox.width * boundingBox.height * 1.0);
	char buffer[500];

	sprintf(buffer, "%f;%f", 0.0 + getCenter().x, groundDistance);
	if(!writeToSocket(buffer))
	{
#if VERBOSITY >= 1
		printf("ERROR: Problem in sending packet to C-RIO.\n");
#endif
	}
	else
	{
#if VERBOSITY >= 1
		printf("Sent data %s\n", buffer);
#endif
	}
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

float Target::getGroundDistanceFromArea(float area)
{
	return pow((double) area, -0.5598084)*20293.5996;

}
