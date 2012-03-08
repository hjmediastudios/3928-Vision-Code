/*
 * Line.cpp
 *
 *  Created on: Feb 29, 2012
 *      Author: frc3928
 */
#include "../Headers/Line.h"

Lines::Line::Line()
{

}

Lines::Line::Line(CvPoint pt1, CvPoint pt2)
{
	if (pt1.x == pt2.x)
	{
		Slope = ((float) (pt2.y - pt1.y)) / ((pt2.x+1) - pt1.x);
		isVertical = true;
	}
	else
	{
		Slope = ((float) (pt2.y - pt1.y)) / (pt2.x - pt1.x);
		isVertical = false;
	}

	Intercept = pt1.y - (Slope*pt1.x);

	point1 = pt1;
	point2 = pt2;
	Length = pointDistance(point1, point2);
}

CvPoint2D32f Lines::Line::getIntersectionWith(Line otherLine)
{
	double intersectionX;
	double intersectionY;

	if (Slope == otherLine.Slope)
	{
#if VERBOSITY == 1
		std::cerr << "ERROR: Slopes parallel\n";
#endif
		intersectionX = (Intercept - otherLine.Intercept) / (otherLine.Slope - Slope + 0.0000001);
	}
	else
	{
		intersectionX = (Intercept - otherLine.Intercept) / (otherLine.Slope - Slope);
	}
	intersectionY = (Slope * intersectionX) + Intercept;

	CvPoint2D32f intersectionPoint = cvPoint2D32f(intersectionX, intersectionY);
	return intersectionPoint;
}

float Lines::Line::dotProductWith(Line otherLine)
{
	return (float) (point2.x - point1.x)*(otherLine.point2.x - otherLine.point1.x) + (point2.y - point1.y)*(otherLine.point2.x - otherLine.point1.x);
}

void Lines::Line::drawInfiniteLine(IplImage* img, CvScalar color)
{

	CvPoint pt1 = cvPointFrom32f(cvPoint2D32f(-1000, -1000*Slope + Intercept));
	CvPoint pt2 = cvPointFrom32f(cvPoint2D32f(1000, 1000*Slope + Intercept));
	if (isVertical == false)
		cvLine(img, pt1, pt2, color, 1, 8, 0);
	else
		cvLine(img, cvPoint(point1.x, 0), cvPoint(point1.x, 1000), color, 1, 8, 0);
}


