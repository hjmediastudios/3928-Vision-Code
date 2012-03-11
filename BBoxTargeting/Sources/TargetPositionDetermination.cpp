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
	int targetPossibilites[*numTargets][4];

	int imgHeight = imgSize.height;
	int imgWidth = imgSize.width;

	int currentHighestY = 9000;
	int currentSecondHighestY = 8999;
	int currentLowestY = 0;

	int highestTargetIndex = 0;
	int lowestTargetIndex = 0;

	int secondHighestTargetIndex = 0;

	for (int i=0; i<(*numTargets); i++)
	{
		//cycle through targets and determine if there are targets higher than the current target or lower
		for (int j=0; j<(*numTargets); j++)
		{
			int currentY = targets[i].getCenter().y;
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
		}
	}

	//Assign target indices based on this information.
	if (highestTargetIndex == lowestTargetIndex)
	{
		//Assume the only target seen is the highest target if only one target is seen
		targets[highestTargetIndex].setTargetIndex(0);
	}
	else
	{
		if (abs(targets[highestTargetIndex].getCenter().x - targets[lowestTargetIndex].getCenter().x) < TARGET_INDEX_SELECTION_ACCEPTABLE_ALIGMENT_ERROR)
		{
			//If the highest and lowest targets seen are aligned in the same x-direction, set the top one to top target and the bottom to bottom
			targets[highestTargetIndex].setTargetIndex(0);
			targets[lowestTargetIndex].setTargetIndex(3);
		}
		else
		{

		}
	}

}
