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

	if ((*numTargets) == 1)
	{ //If only one target is seen...
		targets[highestTargetIndex].setTargetIndex(0); //...assume it's the highest one.
	}
	else if ((*numTargets) == 2)
	{ //If two targets are seen...
		if (abs(targets[highestTargetIndex].getCenter().x - targets[lowestTargetIndex].getCenter().x) <= TARGET_INDEX_SELECTION_ACCEPTABLE_ALIGMENT_ERROR)
		{ //...and they're aligned in the same X-direction...
			targets[highestTargetIndex].setTargetIndex(0); //...assume the highest target is the upper target...
			targets[lowestTargetIndex].setTargetIndex(3); //...and the lowest is the lower target.
		}
		else if (abs(targets[rightmostTargetIndex].getCenter().y - targets[leftmostTargetIndex].getCenter().y) <= TARGET_INDEX_SELECTION_ACCEPTABLE_ALIGMENT_ERROR)
		{ //...and they're aligned in the same Y-direction...
			targets[rightmostTargetIndex].setTargetIndex(2); //...assume the rightmost one is the middle right target...
			targets[leftmostTargetIndex].setTargetIndex(1); //...and the leftmost one is the middle left target.
		}
		else if (rightmostTargetIndex == lowestTargetIndex)
		{ //...and the lowest one is the rightmost...
			targets[rightmostTargetIndex].setTargetIndex(2); //...assume the rightmost one is the middle right...
			targets[leftmostTargetIndex].setTargetIndex(0); //...and the leftmost one is the upper target.
		}
		else if (rightmostTargetIndex == highestTargetIndex)
		{ //... and the highest one is rightmost...
			targets[rightmostTargetIndex].setTargetIndex(2); //...assume the rightmost one is the middle right...
			targets[leftmostTargetIndex].setTargetIndex(3); //...and the leftmost one is the bottom target.
		}
		else if (leftmostTargetIndex == lowestTargetIndex)
		{ //...and the leftmost target is lowest...
			targets[leftmostTargetIndex].setTargetIndex(1); //...assume the leftmost one is the middle left...
			targets[rightmostTargetIndex].setTargetIndex(0); //...and the rightmost one is the upper target.
		}
	}

}
