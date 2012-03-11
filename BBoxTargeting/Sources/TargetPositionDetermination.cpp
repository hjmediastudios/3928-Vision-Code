/*
 * TargetPositionDetermination.cpp
 *
 *  Created on: Mar 10, 2012
 *      Author: nick
 */
#include "../Headers/TargetPositionDetermination.h"
#include <iostream>

using namespace std;

int targetPositionDetermination::setTargetIndices(Target* targets, int* numTargets, IplImage* img)
{
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

	cvLine(img, cvPoint(targets[highestTargetIndex].leftX() - 5, targets[highestTargetIndex].topY() - 5),
			cvPoint(targets[highestTargetIndex].rightX() + 5, targets[highestTargetIndex].topY() - 5), CV_RGB(255,150,200), 2, 8, 0);
	cvLine(img, cvPoint(targets[lowestTargetIndex].leftX() - 5, targets[lowestTargetIndex].bottomY() + 5),
			cvPoint(targets[lowestTargetIndex].rightX() + 5, targets[lowestTargetIndex].bottomY() + 5), CV_RGB(255,150,200), 2, 8, 0);
	cvLine(img, cvPoint(targets[leftmostTargetIndex].leftX() - 5, targets[leftmostTargetIndex].topY() - 5),
			cvPoint(targets[leftmostTargetIndex].leftX() - 5, targets[leftmostTargetIndex].bottomY() + 5), CV_RGB(255,150,200), 2, 8, 0);
	cvLine(img, cvPoint(targets[rightmostTargetIndex].rightX() + 5, targets[rightmostTargetIndex].topY() - 5),
			cvPoint(targets[rightmostTargetIndex].rightX() + 5, targets[rightmostTargetIndex].bottomY() + 5), CV_RGB(255,150,200), 2, 8, 0);

	targets[highestTargetIndex].setTargetIndex(0);

	if (leftmostTargetIndex == lowestTargetIndex)
		targets[lowestTargetIndex].setTargetIndex(1);
	else if (rightmostTargetIndex == lowestTargetIndex)
		targets[lowestTargetIndex].setTargetIndex(2);
	else
		targets[lowestTargetIndex].setTargetIndex(3);

	return highestTargetIndex; //assume that the highest target's index is the top target

}

bool targetPositionDetermination::targetAHigherThanB(Target a, Target b)
{
	return true;
}
