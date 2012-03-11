/*
 * TargetPositionDetermination.cpp
 *
 *  Created on: Mar 10, 2012
 *      Author: nick
 */
#include "../Headers/TargetPositionDetermination.h"
#include <iostream>

using namespace std;

void targetPositionDetermination::setTargetIndices(Target* targets, int* numTargets, IplImage* img)
{
	std::cout << "Number of targets: " << *numTargets << endl;

	//Determine the size of the image
	CvSize imgSize = cvGetSize(img);

	int imgHeight = imgSize.height;
	int imgWidth = imgSize.width;

	int currentHighestY = 9000;
	int currentSecondHighestY = 8999;
	int currentLowestY = 0;

	int highestTargetIndex = 0;
	int lowestTargetIndex = 0;
	int secondHighestTargetIndex = 0;

	int leftmostTargetIndex = 0;
	int rightmostTargetIndex = 0;

	int currentRightmostX = 0;
	int currentLeftmostX = 9000;

	for (int i=0; i<(*numTargets); i++)
	{
		//cycle through targets and determine if there are targets higher than the current target or lower
		int currentY = targets[i].getCenter().y;
		int currentX = targets[i].getCenter().x;

		if (currentY < currentHighestY)
		{
			highestTargetIndex = i;
			currentHighestY = currentY;
		}
		else if (currentY < currentSecondHighestY)
		{
			secondHighestTargetIndex = i;
			currentSecondHighestY = currentY;
		}

		if (currentY > currentLowestY)
		{
			lowestTargetIndex = i;
			currentLowestY = currentY;
		}

		if (currentX > currentRightmostX)
		{
			rightmostTargetIndex = i;
			currentRightmostX = currentX;
		}

		if (currentX < currentLeftmostX)
		{
			leftmostTargetIndex = i;
			currentLeftmostX = currentX;
		}
	}

	cvLine(img, cvPoint(targets[highestTargetIndex].getBoundingBoxPoint1().x - 5, targets[highestTargetIndex].getBoundingBoxPoint1().y - 5),
			cvPoint(targets[highestTargetIndex].getBoundingBoxPoint2().x + 5, targets[highestTargetIndex].getBoundingBoxPoint1().y - 5), CV_RGB(255,150,200), 2, 8, 0);
	cvLine(img, cvPoint(targets[lowestTargetIndex].getBoundingBoxPoint1().x - 5, targets[lowestTargetIndex].getBoundingBoxPoint2().y + 5),
			cvPoint(targets[lowestTargetIndex].getBoundingBoxPoint2().x + 5, targets[lowestTargetIndex].getBoundingBoxPoint2().y + 5), CV_RGB(255,150,200), 2, 8, 0);
	cvLine(img, cvPoint(targets[leftmostTargetIndex].getBoundingBoxPoint1().x - 5, targets[leftmostTargetIndex].getBoundingBoxPoint1().y - 5),
			cvPoint(targets[leftmostTargetIndex].getBoundingBoxPoint1().x - 5, targets[leftmostTargetIndex].getBoundingBoxPoint2().y + 5), CV_RGB(255,150,200), 2, 8, 0);
	cvLine(img, cvPoint(targets[rightmostTargetIndex].getBoundingBoxPoint2().x + 5, targets[rightmostTargetIndex].getBoundingBoxPoint1().y - 5),
			cvPoint(targets[rightmostTargetIndex].getBoundingBoxPoint2().x + 5, targets[rightmostTargetIndex].getBoundingBoxPoint2().y + 5), CV_RGB(255,150,200), 2, 8, 0);


	if (highestTargetIndex == lowestTargetIndex && highestTargetIndex == rightmostTargetIndex && highestTargetIndex == rightmostTargetIndex)
	{ //only 1 target case
		targets[highestTargetIndex].setTargetIndex(0);
	}
	else
	{
		if (leftmostTargetIndex == lowestTargetIndex)
		{
			targets[leftmostTargetIndex].setTargetIndex(1);
		}
		else if (leftmostTargetIndex == highestTargetIndex)
		{
			targets[leftmostTargetIndex].setTargetIndex(0);
		} //TODO how to decide between top-right and left-bottom conundrum?

		else if (rightmostTargetIndex == lowestTargetIndex)
		{
			targets[rightmostTargetIndex].setTargetIndex(2);
		}
	}

}

bool targetPositionDetermination::targetAHigherThanB(Target a, Target b)
{
	return true;
}
