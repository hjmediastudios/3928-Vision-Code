/*
 * TargetPositionDetermination.h
 *
 *  Created on: Mar 10, 2012
 *      Author: nick
 */

#ifndef TARGETPOSITIONDETERMINATION_H_
#define TARGETPOSITIONDETERMINATION_H_

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "../Headers/Target.h"
#include "../Constants.h"


namespace targetPositionDetermination
{
	int setTargetIndices(Target* targets, int* numTargets, IplImage* img);
	bool targetAHigherThanB(Target a, Target b);

}


#endif /* TARGETPOSITIONDETERMINATION_H_ */
