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
	int currentLowestY = 0;

	int highestTargetIndex = 0;
	int lowestTargetIndex = 0;

	for (int i=0; i<(*numTargets); i++)
	{
		//cycle through targets and determine if there are targets higher than the current target or lower
		for (int j=0; j<(*numTargets); j++)
		{
			if (targets[i].getCenter().y < currentHighestY)
			{
				highestTargetIndex = i;
				currentHighestY = targets[i].getCenter().y;
			}

			if (targets[i].getCenter().y > currentLowestY)
			{
				lowestTargetIndex = i;
				currentLowestY = targets[i].getCenter().y;
			}
		}
	}

	for (int i= 0; i<4; i++)
	{
		targetPossibilites[highestTargetIndex][i] = 1;
		targetPossibilites[lowestTargetIndex][i] = 0;
	}

	//print possibilities to debug
	for (int i=0; i<(*numTargets); i++)
	{
		std::cout << "targets[" << i << "]: ";
		for (int j=0; j<4; j++)
		{
			std::cout << targetPossibilites[i][j] << " ";
		}
		std::cout << endl;
	}

}
