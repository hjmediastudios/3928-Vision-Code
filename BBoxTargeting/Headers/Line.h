/**
 * Line.h
 *
 *  Created on: Feb 29, 2012
 *      Author: Nick Royer
 */

#ifndef LINE_H_
#define LINE_H_
#include <opencv/cv.h>
#include "../Headers/HelperFunctions.h"
/**
 * @brief A set of functions and classes for working with mathematical lines.
 *
 * This namespace contains a class for creating
 * Line-based functions.
 */
namespace Lines
{
/**
 *
 * @brief A class representing a line or line segment.
 *
 * A class representing a line or line segment, with functions to perform operations such as intersection, length calculation, and vector math.
 * @author Nick Royer
 */
class Line
{
	public:
		Line(); //!< Constructs an empty Line object.
		/**
		 * @brief Creates a Line object from two points.
		 * @param pt1 The first CvPoint to create the line from.
		 * @param pt2 The second CvPoint to create the line from.
		 */
		Line(CvPoint pt1, CvPoint pt2);
		/**
		 * @brief Returns the intersection point of the current line with a given other line.
		 * @param otherLine The line o intersect the current line with.
		 * @return A CvPoint2D32f floating-value point representing th intersection derived. If no intersection is found, this returns (0, 0).
		 */
		CvPoint2D32f getIntersectionWith(Line otherLine);
		/**
		 * @brief Draws the infinite line representing this Line object on an image.
		 * @param img A pointer to the IplImage the line should be drawn on.
		 * @param color The color to draw the line in.
		 */
		void drawInfiniteLine(IplImage* img, CvScalar color);
		/**
		 * @brief Returns the dot product of the current line and a given other Line object.
		 * @param otherLine The other line to compute the dot product with.
		 * @return The dot product of the current line and the given other line.
		 */
		float dotProductWith(Line otherLine);

		float Slope; //!< The slope of the line.
		float Intercept; //!< The line's Y-intercept
		float Length; //!< The length of the line segment between the two given points. @see pointDistance
		CvPoint point1; //!< The first point used to create the line.
		CvPoint point2; //!< The second point used to create the line.

	private:
		bool isVertical; //!< True if the line is vertical; false otherwise.
};

};

#endif /* LINE_H_ */
