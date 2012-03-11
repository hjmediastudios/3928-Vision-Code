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
	cvRectangleR(originImage, boundingBox, color, 2, 8, 0);
	cvDrawContours(originImage, &contour, color, color, 1, 1, 0);

	cvLine(originImage, getCenter(), cvPoint(320, getCenter().y), color, 1, 8, 0);
	cvLine(originImage, getCenter(), cvPoint(getCenter().x, 240), color, 1, 8, 0);
}

void Target::getNavigationString()
{
	float x = getCenter().x- 320.0;
	float y = getCenter().y - 240.0;
	float angleX = asin((x*sin(CAMERA_VIEWING_ANGLE_HALF_X))/320); //0.608761429 = sin(37.5 degrees) //37.5 = half viewing angle in y direction
	float angleY = -1 * asin((y*sin(CAMERA_VIEWING_ANGLE_HALF_Y))/240); //28.1255 = half viewing angle in X-direction

	//offsets[0] = angle offset in X direction
	offsets[0] = rad2deg(angleX);

	/*
	 * offsets[1] = angle offset from center of camera in Y direction, with camera angle offset added to
	 * get the angular Y offset from the center of the camera irrespective of camera rotation
	 */
	offsets[1] = rad2deg(angleY) + CAMERA_ROTATION_AXIS_X;
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
