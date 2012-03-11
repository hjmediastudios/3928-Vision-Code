/*
 * Target.h
 *
 *  Created on: Feb 21, 2012
 *      Author: Nick Royer
 */

#ifndef TARGET_H_
#define TARGET_H_
#include <opencv/cv.h>
#include "../Headers/HelperFunctions.h"
#include "../Headers/Line.h"

using namespace Lines;

/**
 * @author Nick Royer
 * @brief A class that represents a single 2012 'Rebound Rumble' rectangular target.
 */
class Target
{
	public:
		/**
		 * Constructs a new Target object with a null contour pointer.
		 */
		Target(); //!< Constructs an empty Target object.
		/**
		 * Constructs a new Target objec using the given pointer to a contour. Ideally, the contour should belong to an actual target.
		 * This method assumes the given contour is correct; no filtering methods are performed.
		 * @param cntr The contour that the target will represent.
		 */
		Target(CvSeq* cntr, IplImage* img); //!< Creates a target from a CvSeq contour.
		/**
		 * Returns the upper-left point of the target's bounding box.
		 * @return A CvPoint representing the upper-left point of the target's bounding box.
		 */
		CvPoint getBoundingBoxPoint1(); //!< Returns the upper left point of the bounding box.
		/**
		 * Returns the lower-right point of the target's bounding box.
		 * @return A CvPoint representing the lower-right point.
		 */
		CvPoint getBoundingBoxPoint2(); //!< Returns the bottom right point of the bounding box.
		/**
		 * Returns the center of the target's bounding box.
		 * @return A CvPoint at the center o the taget's bounding box.
		 */
		CvPoint getCenter(); //!< Gets the center point of the target.

		float offsets[4]; //!< The X, Y, and Z offsets of the target from the camera
		CvSeq contour; //!< The CvSeq contour of the target.

		/**
		 * Draws the target on a provided image, including the target's bounding box, center, axes of rotation, contour, and selected points.
		 * @param img The pointer to an IplImage that the target is to be drawn on.
		 * @param color A CvScalar defining the desired color of the target's bounding box, contour, and points. Note that all X,Y,Z values are rendered in their conventional colors.
		 */
		void drawTarget(CvScalar color); //!< Draws the target on an image

		void getNavigationString();
		int getArea();
		float getAspectRatio();
		float getRectangularity();

	private:
		CvRect boundingBox; //!< The CvRect defining the bounding box of the target.
		IplImage* originImage; //!< A pointer to the IplImage the target originated from.

};


#endif /* TARGET_H_ */
