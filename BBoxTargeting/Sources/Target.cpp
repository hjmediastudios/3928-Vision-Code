/*
 * Target.cpp
 *
 *  Created on: Feb 21, 2012
 *      Author: frc3928
 */
#include "../Headers/Target.h"
#include "../Headers/HelperFunctions.h"
#include <aruco/aruco.h>
#include "../Constants.h"
#include <math.h>

using namespace aruco;
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
	CvSeq* points = cvConvexHull2((void*) cntr, NULL, CV_CLOCKWISE, 1);
	numPoints = points->total;
	if (numPoints > 500)
	{
#if VERBOSITY >= 1
		std::cerr << "ERROR: target has more than 500 points on its contour!\n";
#endif
		exit(1);
	}

	for (int i=0; i<numPoints; i++)
	{
		pointSet[i] = *(CvPoint*) cvGetSeqElem(points, i);
	}

	numIntersections = 0;
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
	cvRectangleR(originImage, boundingBox, color, 1, 8, 0);
	cvDrawContours(originImage, &contour, color, color, 1, 1, 0);

	//	for (int i=0; i < numPoints; i++)
	//	{
	//		cvCircle(originImage, pointSet[i], 1, CV_RGB(150,255,110), 2, 8, 0);
	//	}
	////
	//	for (int i=0; i < numLines; i++)
	//	{
	//		lineSet[i].drawInfiniteLine(originImage, color);
	//	}

	//	cvLine(originImage, cvPointFrom32f(vPointArray[0]), getCenter(), CV_RGB(255, 255, 255), 2, 8, 0);
	//	cvLine(originImage, cvPointFrom32f(vPointArray[1]), getCenter(), CV_RGB(150, 150, 150), 2, 8, 0); //TODO problem finding vPoints!


	cvCircle(originImage, cvPointFrom32f(cornerArray[0]), 2, CV_RGB(0,255,0), 3, 8, 0);
	cvCircle(originImage, cvPointFrom32f(cornerArray[1]), 2, CV_RGB(0,0,255), 3, 8, 0);
	cvCircle(originImage, cvPointFrom32f(cornerArray[2]), 2, CV_RGB(255,0,0), 3, 8, 0);
	cvCircle(originImage, cvPointFrom32f(cornerArray[3]), 2, CV_RGB(255,255,0), 3, 8, 0);

	//draw height of bbox

}

void Target::spacePointsEvenly(int numPointsPerSide)
{
	CvPoint spacedPoints[500];
	spacedPoints[0] = pointSet[0];
	int numSpacedPoints = 1;

	CvPoint pt1;
	CvPoint pt2;

	pt1 = pointSet[0];
	int lineSpacing;

	if (boundingBox.width > boundingBox.height)
		lineSpacing = (int) boundingBox.height/numPointsPerSide;
	else
		lineSpacing = (int) boundingBox.width/numPointsPerSide;

	float runnngPointSeparation = lineSpacing;

	for (int i=0; i<numPoints; i++)
	{

		float distance;
		if (i<(numPoints-1))
		{
			pt2 = pointSet[i+1];
		}
		else
		{
			pt2 = pointSet[0];
		}

		distance = pointDistance(pt1, pt2);

		if (distance > runnngPointSeparation)
		{
			// D2 = (pointDistance-Separation)


			float vecX = (pt2.x - pt1.x);
			float vecY = (pt2.y - pt1.y);

			float scaledVecX = vecX*(runnngPointSeparation/distance);
			float scaledVecY = vecY*(runnngPointSeparation/distance);


			spacedPoints[numSpacedPoints] = cvPoint((int)(pt1.x + scaledVecX), (int)(pt1.y + scaledVecY)); //
			//cvCircle(frame, pt2, 5, colorBlue, 2, 8, 0);
			pt1 = spacedPoints[numSpacedPoints];
			i--;
			numSpacedPoints++;
			runnngPointSeparation = lineSpacing;
			//add good point, subdivide current segment
		}
		else
		{
			runnngPointSeparation = runnngPointSeparation - distance;
			pt1 = pt2;
		}
	}

	numPoints = numSpacedPoints;
	memcpy(pointSet, spacedPoints, sizeof(CvPoint)*numPoints);


}

void Target::calculateSegmentsFromPoints(int crossProductThreshold)
{
	numLines = 0;

	CvPoint ptL;
	CvPoint pt0;
	CvPoint ptR;

	for (int i=0; i<numPoints; i++) //Filter based on angle
	{
		pt0 = pointSet[i];

		if (i<(numPoints-1))
			ptR = pointSet[i+1];
		else
			ptR = pointSet[0];

		if (i>0)
			ptL = pointSet[i-1];
		else
			ptL = pointSet[numPoints - 1];

		Line lineL = Line::Line(pt0, ptL);
		Line lineR = Line::Line(pt0, ptR);

		CvPoint2D32f vectorL;
		CvPoint2D32f vectorR;

		vectorL.x = (ptL.x - pt0.x);
		vectorL.y = (ptL.y - pt0.y);

		vectorR.x = (ptR.x - pt0.x);
		vectorR.y = (ptR.y - pt0.y);

		float zVector = fabs((float) (vectorL.x*vectorR.y) - (vectorL.y*vectorR.x));

		if (zVector < crossProductThreshold)
		{
			lineSet[numLines] = Line::Line(ptL, pt0);
			numLines++;
		}
		//		cvLine(originImage, ptL, pt0, CV_RGB(255,150, 0), 5, 8, 0);
	}

	//	for (int i=0; i<numLines; i++)
	//		cvLine(originImage, lineSet[i].point1, lineSet[i].point2, CV_RGB(0,0,255), 6, 8, 0);


	mergeColinearLines();
}

void Target::mergeColinearLines()
{
	Line lineSet2[500];
	int numGoodLines = 0;

	for (int i=0; i<numLines; i++) //connect large segments
	{
		for (int j = i+1; j<numLines; j++)
		{
			if ((lineSet[j-1].point2.x == lineSet[j].point1.x) && (lineSet[j-1].point2.y == lineSet[j].point1.y) && (j != (numLines-1)))
			{
				//i++;
			}
			else
			{
				lineSet2[numGoodLines] = Line::Line(lineSet[i].point1, lineSet[j-1].point2);
				i = j;
				numGoodLines++;
				break;
			}
		}

	}

	numLines = numGoodLines;
	memcpy(lineSet, lineSet2, sizeof(Line)*numLines);
}

void Target::calculateIntersectionsOfLines()
{
	numIntersections = 0;

	//solve for intersections of lines
	for (int i=0; i<(numLines-1); i++)
	{
		for (int j=(i+1); j<numLines; j++)
		{
			intersectionSet[numIntersections] = lineSet[i].getIntersectionWith(lineSet[j]);
			intersectionLineIndices[0][numIntersections] = i;
			intersectionLineIndices[1][numIntersections] = j;
			numIntersections++;
		}
	}
}

void Target::calculateVanishingPoints()
{
	float absX = 0;
	float absY = 0;
	float absX2 = 0;
	float absY2 = 0;

	int vPointXIndex = 0;
	int vPointYIndex = 0;

	int vPointXIndex2 = 0;
	int vPointYIndex2 = 0;


	//filter vanishing points from intersections
	for (int i=0; i<numIntersections; i++)
	{
		if (fabs(intersectionSet[i].x - (boundingBox.x+(0.5*boundingBox.width))) > absX)
		{
			absX2 = absX;
			vPointXIndex2 = vPointXIndex;
			absX = fabs(intersectionSet[i].x - (boundingBox.x+(0.5*boundingBox.width)));
			vPointXIndex = i;
		}
		else if (fabs(intersectionSet[i].x - (boundingBox.x+(0.5*boundingBox.width))) > absX2)
		{
#if VERBOSITY >= 2
			std::cout << "Set second-best X-vanishing point.\n";
#endif
			absX2 = fabs(intersectionSet[i].x - (boundingBox.x+(0.5*boundingBox.width)));
			vPointXIndex2 = i;
		}




		if (fabs(intersectionSet[i].y - (boundingBox.y+(0.5*boundingBox.height))) > absY)
		{
			absY2 = absY;
			vPointYIndex2 = vPointYIndex;
			absY = fabs(intersectionSet[i].y- (boundingBox.y+(0.5*boundingBox.height)));
			vPointYIndex = i;
		}
		else if (fabs(intersectionSet[i].x - (boundingBox.x+(0.5*boundingBox.width))) > absY2)
		{
#if VERBOSITY >= 2
			std::cout << "Set second-best Y-vanishing point.\n";
#endif
			absY2 = fabs(intersectionSet[i].y - (boundingBox.y+(0.5*boundingBox.height)));
			vPointYIndex2 = i;
		}

	}
	//fix same vanishing-points
	if (vPointXIndex == vPointYIndex)
	{

#if VERBOSITY >= 1
		std::cerr << "ERROR: Vanishing points are identical.\n";
#endif

		if (absX2 > absY2)
		{
			vPointXIndex = vPointXIndex2;
#if VERBOSITY >= 2
			std::cout << "Changing horizontal vanishing point to next-best candidate.\n";
#endif
		}
		else
		{
			vPointYIndex = vPointYIndex2;
#if VERBOSITY >= 2
			std::cout << "Changing vertical vanishing point to next-best candidate.\n";
#endif
		}
	}

	vPointArray[0] = intersectionSet[vPointXIndex]; //horizontal vanishing point
	vPointArray[1] = intersectionSet[vPointYIndex]; //vertical vanishing point

#if VERBOSITY >= 3
	std::cout << "vPoint 0: (" << vPointArray[0].x << ", " << vPointArray[0].y << ")\n";
	std::cout << "vPoint 1: (" << vPointArray[1].x << ", " << vPointArray[1].y << ")\n";
#endif

	//draw lines to vanishing points
	vLines[0] = Line::Line(cvPointFrom32f(vPointArray[0]), lineSet[intersectionLineIndices[0][vPointXIndex]].point1);
	vLines[1] = Line::Line(cvPointFrom32f(vPointArray[0]), lineSet[intersectionLineIndices[1][vPointXIndex]].point1);
	vLines[2] = Line::Line(cvPointFrom32f(vPointArray[1]), lineSet[intersectionLineIndices[0][vPointYIndex]].point1);
	vLines[3] = Line::Line(cvPointFrom32f(vPointArray[1]), lineSet[intersectionLineIndices[1][vPointYIndex]].point1);

	Line tempLine;
	if (vLines[1].point2.y > vLines[0].point2.y) //flip lines to maintain consistency
	{
#if VERBOSITY >= 2
		std::cout << "Flipping horizontal vanishing lines.\n";
#endif
		tempLine = vLines[1];
		vLines[1] = vLines[0];
		vLines[0] = tempLine;
	}

	if (vLines[2].point2.x > vLines[3].point2.x) //flip lines to maintain consistency
	{
#if VERBOSITY >= 2
		std::cout << "Flipping vertical vanishing lines.\n";
#endif
		tempLine = vLines[3];
		vLines[3] = vLines[2];
		vLines[2] = tempLine;
	}
}

void Target::calculateCornersFromVanishingPoints()
{
	cornerArray[0] = vLines[1].getIntersectionWith(vLines[2]); // top left
	cornerArray[1] = vLines[1].getIntersectionWith(vLines[3]); // top right
	cornerArray[2] = vLines[0].getIntersectionWith(vLines[2]); // bottom left
	cornerArray[3] = vLines[0].getIntersectionWith(vLines[3]); //bottom right
}

void Target::calculateSquare()
{
	Line diagonals[2];
	diagonals[0] = Line::Line(cvPointFrom32f(cornerArray[0]), cvPointFrom32f(cornerArray[3]) );
	diagonals[1] = Line::Line(cvPointFrom32f(cornerArray[1]), cvPointFrom32f(cornerArray[2]) );
	CvPoint2D32f centerPoint = diagonals[0].getIntersectionWith(diagonals[1]);
#if VERBOSITY >= 3
	std::cout << "Center point: (" << centerPoint.x << ", " << centerPoint.y <<")\n";
#endif
	Line cuttingLine = Line::Line(cvPointFrom32f(vPointArray[1]), cvPointFrom32f(centerPoint));
	CvPoint2D32f centerCorners[2];
	centerCorners[0] = cuttingLine.getIntersectionWith(vLines[0]);
	centerCorners[1] = cuttingLine.getIntersectionWith(vLines[1]);

	//get right square corners
	diagonals[0] = Line::Line(cvPointFrom32f(centerCorners[1]), cvPointFrom32f(cornerArray[3]) );
	diagonals[1] = Line::Line(cvPointFrom32f(centerCorners[0]), cvPointFrom32f(cornerArray[1]) );
	centerPoint = diagonals[0].getIntersectionWith(diagonals[1]);
	cuttingLine = Line::Line(cvPointFrom32f(vPointArray[1]), cvPointFrom32f(centerPoint));

	cornerArray[1] = cuttingLine.getIntersectionWith(vLines[0]);
	cornerArray[3] = cuttingLine.getIntersectionWith(vLines[1]);
}

void Target::calculatePosition(aruco::CameraParameters camParams)
{
	std::vector<cv::Point2f> ptList;


	for (int i=0; i<4; i++)
	{
		cornerArray[i].x = floor(cornerArray[i].x);
		cornerArray[i].y = floor(cornerArray[i].y);
		std::cout << "   " << i << ": " << cornerArray[i].x << ", " << cornerArray[i].y << endl;
	}

//	cornerArray[0] = cvPoint2D32f(297, 187);
//	cornerArray[1] = cvPoint2D32f(356, 238);
//	cornerArray[2] = cvPoint2D32f(299, 242);
//	cornerArray[3] = cvPoint2D32f(355, 180);


	ptList.push_back(cornerArray[0]);
	ptList.push_back(cornerArray[3]);
	ptList.push_back(cornerArray[1]);
	ptList.push_back(cornerArray[2]);


	marker = aruco::Marker(ptList);

	marker.id = 01;
	marker.calculateExtrinsics(0.4572, camParams);
	marker.glGetModelViewMatrix(modelViewMatrix); //TODO crossProduct

	CvMat mvmMatrix = cvMat(4, 4, CV_64FC2, modelViewMatrix);

	CvMat* mvmPtr = &mvmMatrix;
	std::cerr << "Rows: " << mvmPtr->rows << ", cols: " << mvmPtr->cols << " ";
	double determinant = cvDet(mvmPtr);//TODO ?
	std::cerr << determinant << endl;




	for (int j=0; j<16; j++)
		std::cout << "MVM[" << j <<"]: " << modelViewMatrix[j] << endl;

	/* Model view matrix:
	 * X-component of X axis, Y[x], Z[x], translation[x]
	 * Y-component of X axis, Y[y], Z[y], translation[y]
	 * Z-component of X axis, Y[z], Z[z], translation[z]
	 * 0,					   0,	 0, 		1         <== 1=above is points; 0=above is a vector
	 *
	 * Indices for matrix are going down like so:
	 * 0, 4,  8, 12
	 * 1, 5,  9, 13
	 * 2. 6. 10, 14
	 * 3, 7, 11, 15
	 */


	std::cout << "Translation: \n";
	std::cout << "MVM: " <<  metersToInches(modelViewMatrix[12]) //x offset
											<< " " << metersToInches(modelViewMatrix[13]) //y offset
											<< " " << metersToInches(modelViewMatrix[14]) << "\n"; //z offset

	//Convert from target coordinates to hoop offsets
	float hoopTransformationArray[16] = {1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			inchesToMeters(HOOP_OFFSETFROM_CENTEROFSQUARE_Z), inchesToMeters(HOOP_OFFSETFROM_CENTEROFSQUARE_Y), inchesToMeters(-1*HOOP_OFFSETFROM_CENTEROFSQUARE_X), 1};

	matrixMultiply(hoopTransformationArray, modelViewMatrix, hoopCoordinatesArray, 4);

	std::cout << "0: " << metersToInches(marker.Tvec.at<float>(0, 0)) << endl;
	std::cout << "1: " << metersToInches(marker.Tvec.at<float>(1, 0)) << endl;
	std::cout << "2: " << metersToInches(marker.Tvec.at<float>(2, 0)) << endl;

	marker.Tvec.at<float>(0, 0) = hoopCoordinatesArray[12];
	marker.Tvec.at<float>(1, 0) = hoopCoordinatesArray[13];
	marker.Tvec.at<float>(2, 0) = hoopCoordinatesArray[14]*-1;

	std::cout << "0': " << metersToInches(marker.Tvec.at<float>(0, 0)) << endl;
	std::cout << "1': " << metersToInches(marker.Tvec.at<float>(1, 0)) << endl;
	std::cout << "2': " << metersToInches(marker.Tvec.at<float>(2, 0)) << endl;



}

aruco::Marker Target::getMarker()
{
	return marker;
}

void Target::calculateHoopPosition(int numPointsPerSide, int crossProductThreshold, aruco::CameraParameters camParams)
{
	spacePointsEvenly(numPointsPerSide);
	calculateSegmentsFromPoints(crossProductThreshold);
	calculateIntersectionsOfLines();
	if (numIntersections > 3)
	{
		calculateVanishingPoints();
#if VERBOSITY >= 2
		std::cout << "Calculated vanishing points\n"; fflush(stdout);
#endif
		calculateCornersFromVanishingPoints();
#if VERBOSIY >= 2
		std::cout << "Calculated corners\n"; fflush(stdout);
#endif
		calculateSquare();

		calculatePosition(camParams);
	}
}

void Target::getNavInfo(float camAngleDegrees)
{
	float angle = atan(marker.Tvec.at<float>(0,0)/marker.Tvec.at<float>(2,0));



	std::cout << "Y: " << metersToInches(marker.Tvec.at<float>(1, 0)) << endl;
	float camAngle = deg2rad(camAngleDegrees);

	double camRotationMatrix[16] = {1, 0, 0, 0,
			0, cos(camAngle), -sin(camAngle), 0,
			0, sin(camAngle), cos(camAngle), 0,
			0, 0, 0, 1};

	float newHoopCoordinates[16];
	matrixMultiply(hoopCoordinatesArray, camRotationMatrix, newHoopCoordinates, 4);

	offsets[0] = angle;
	offsets[1] = (-1*newHoopCoordinates[13]) + inchesToMeters(CAMERA_HEIGHT);
	offsets[2] = newHoopCoordinates[14]*cos(angle);

}
