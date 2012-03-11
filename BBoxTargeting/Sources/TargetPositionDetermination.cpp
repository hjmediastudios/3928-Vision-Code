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
	//Determine the size of the image
	CvSize imgSize = cvGetSize(img);
	int imgHeight = imgSize.height;
	int imgWidth = imgSize.width;

	std::cout << "Number of targets: " << *numTargets << endl;

}
