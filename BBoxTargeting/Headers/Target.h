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
#include <aruco/aruco.h>

using namespace aruco;
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

		/**
		 * Calculates the position, in inches, of the target's hoop center and stores it in an array of X,Y,Z and vector offsets.
		 * @param numPointsPerSide The number of points desired on the shortest side of the target's contour when spacing the points evenly.
		 * @param crossProductThreshold A scalar value used to filter out non-colinear lines from the target's segment array. A very low number will filter few lines. A very high number will filter out most lines.
		 * @param camParams The Aruco camera parameters used in determining the target's position.
		 * @see spacePointsEvenly
		 * @see calculateSegmentsFromPoints
		 * @see calculateIntersectionsOfLines
		 * @see calculateVanishingPoints
		 * @see calculateCornersFromVanishingPoints
		 * @see calculateSquare
		 * @see calculatePosition
		 */
		void calculateHoopPosition(int numPointsPerSide, int crossProductThreshold, aruco::CameraParameters camParams); //!< Calculates the position of the hoop relative to the camera

		float offsets[4]; //!< The X, Y, and Z offsets of the target from the camera
		CvSeq contour; //!< The CvSeq contour of the target.
		aruco::Marker marker; //!< The Aruco Marker object defining the square portion of the target coplanar with the target and aligned with the target's left edge.
		aruco::Marker getMarker(); //!< Returns the Aruco marker representing the target.

		double modelViewMatrix[16]; //!< The OpenGL model view matrix of the target, in a 3x3 rotation matrix appended to a 3x1 translation matrix.
		int numLines; //!< The number of Line objects contained in lineSet.

		/**
		 * Draws the target on a provided image, including the target's bounding box, center, axes of rotation, contour, and selected points.
		 * @param img The pointer to an IplImage that the target is to be drawn on.
		 * @param color A CvScalar defining the desired color of the target's bounding box, contour, and points. Note that all X,Y,Z values are rendered in their conventional colors.
		 */
		void drawTarget(CvScalar color); //!< Draws the target on an image

		void getNavInfo(float camAngleDegrees);

	private:
		CvRect boundingBox; //!< The CvRect defining the bounding box of the target.

		IplImage* originImage;
		CvPoint pointSet[500]; //!< An array of CvPoints defining the points on the target's contour.
		int numPoints; //!< The number of poins on the tart's contour.

		Line lineSet[500]; //!< An array of Line objects containing the segments between the target's points, and, later, the edges of the target.

		CvPoint2D32f intersectionSet[1000]; //!< The set of floating-point CvPoints defining the intersection of the edges of the target.
		int numIntersections; //!< The number of intersections between the target's edges.
		int intersectionLineIndices[2][1000]; //!< An array that indicates which lines formed each intersection points. The first column contains the first line that formed the given intersection point,the second column the second line, and each row is used for a single intersection point.

		void mergeColinearLines(); //!< Merges all colinear line segments into a single segment; one for each edge of the target.

		CvPoint2D32f vPointArray[2]; //!< An array of CvPoint2D32f's defining the two vanishing points of the target's parallel lines.
		Line vLines[4]; //!< An array of Line objects, two per vanishing point- one for each line that creaed that vanishing point via intersection.

		CvPoint2D32f cornerArray[4]; //!< The four corners of the target, in CvPoint2D32f format.

		/**
		 * @brief A function to space a list of points from a contour in even intervals around the contour.
		 * @param numPointsPerSide The number of points desired on the shortest side of the contour.
		 */
		void spacePointsEvenly(int numPointsPerSide);
		/**
		 * @brief Makes the set of segments on a target from points.
		 * @param crossProductThreshold An integer value defining the threshold for filtering out non-colinear line segments.
		 * @see lineSet
		 */
		void calculateSegmentsFromPoints(int crossProductThreshold);
		/**
		 * @brief Calculates all the intersections of the edges of the target.
		 * @see lineSet
		 * @see intersectionSet
		 */
		void calculateIntersectionsOfLines();
		/**
		 * @brief Calculates vanishing pints from the segments contained in lineSet.
		 * @see lineSet
		 */
		void calculateVanishingPoints();
		/**
		 * @brief Intersects vanishing ponts to get the four corners of the target.
		 * @see vPointArray
		 */
		void calculateCornersFromVanishingPoints();
		/**
		 * @brief Calculates the corners of the square Marker object.
		 * Performs two dicing operations on the four corner points to create an Aruco Marker object, square, centered on the left edge of the target.
		 * @see marker
		 * @see vPointArray
		 */
		void calculateSquare();
		/**
		 * @brief Calculates the position of the target's hoop from the Aruco marker object.
		 * @param camParams The Aruco camera parameters used to determine the target's position.
		 * @see marker
		 * @see offsets
		 */
		void calculatePosition(aruco::CameraParameters camParams);

		float hoopCoordinatesArray[16];

};


#endif /* TARGET_H_ */
