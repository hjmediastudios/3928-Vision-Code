/**
 * Threshold.h
 *
 * @brief A series of functions for thresholding images and detecting targets based on that information.
 *
 * @author Nick Royer
 */
#include <opencv/cv.h>
#include "../Constants.h"
#include "../Headers/Target.h"
#include <sstream>

#ifndef THRESHOLD_H_
#define THRESHOLD_H_

/**
 * @brief A series of functions for performing thresholding and target-selection operations on image.
 */
namespace Threshold
{
		/**
		 * @brief Finds green target-colored objects and converts the color image into a black-and-white thresholded one.
		 *
		 * This function takes an input 3-channel image and thresholds it, converting it into a thresholded black-and-white image
		 * with white being all pixels within the given threshold and black being all pixels outside the given range. Note that the
		 * threshold values are defined in the Constants.h file so as to provide for easier modification of the thresholded image,
		 * and are not passes as arguments into the function.
		 *
		 * @param img The pointer to the input IplImage used for the thresholding operation
		 * @return The pointer to the thresholded IpImage; a 1-channel grayscale image
		 */
		IplImage* threshold_findGreen(IplImage *img, int dilates);

		/**
		 * @brief Finds contours in the thresholded image.
		 *
		 * This function takes in a 1-channel grayscale IplImage and performs a contour-finding operation on it. After finding
		 * the contours; they are returned as a pointer to a CvSequence of contours.
		 *
		 * @param img A pointer to the grayscale IplImage to fin contours in.
		 * @return A pointer to a CvSeq containing the detected contours.
		 */
		CvSeq* findContours(IplImage *img);

		/**
		 * @brief Selects the valid targets from a CvSequence of contours.
		 *
		 * This function takes in a pointer to a CvSeq of contours, then processes the contours to select which contours represent
		 * legitimate Rebound Rumble targets. First, contours that are within other contours or contours that are too small are filtered
		 * out. Then, contours that have passedthstest have a bounding box created. If the bounding box is sufficiently far away from
		 * the image border and does not take up an excessive amount of screen space, the bounding box's aspect ratio is checked. If
		 * this passes the test, a new Target object is created and added to an array of Target objects. All of the thresholding operations
		 * are controlled by values located in the constants file.
		 *
		 * @param contours A pointer to the CvSeq containing the contours to test.
		 * @param imgSize The CvSize object representing the image size, used for border thresholding.
		 * @param tgtSet A pointer to an array of Target objects.
		 * @param numTgts A pointer to an integer variable that keeps track of how many targets there are in the current frame.
		 *
		 * @see Constants.h
		 * @see Target
		 */
		void pickTargets(IplImage* img, CvSeq* contours, CvSize imgSize, Target *tgtSet, int *numTgts);
};
#endif /* THRESHOLD_H_ */
