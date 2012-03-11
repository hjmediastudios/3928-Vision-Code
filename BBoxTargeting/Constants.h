/*
 * Constants.h
 * @brief A series of physical constants and thresholding value.
 * @version 0.9.1
 * @author Nick Royer
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_
#include <opencv/cv.h>
#include <opencv/highgui.h>

/**
 * @brief Constants not actually used in the running of the program, but rather for debugging purposes
 */
struct DebugConstants
{
	#define USE_CAM 1 //!< Indicates whether the capture device is a camera (1) or provided video file.
	#define VERBOSITY 1 //!< A value denoting the output level on the console.
};

/**
 * @brief Some constants.
 *
 * A lot of constants.
 */

#define CAMERA_INDEX 1//!< The system index of the default camera to use.

#define THRESHOLD_HUE_MIN 34 //!< The minimum hue value for thresholding a image to find targets
#define THRESHOLD_HUE_MAX 79 //!< The maximum hue value for thresholding a image to find targets
#define THRESHOLD_SAT_MIN 133 //!< The minimum saturation value for thresholding a image to find targets
#define THRESHOLD_SAT_MAX 255 //!< The maximum saturation value for thresholding a image to find targets
#define THRESHOLD_VAL_MIN 63 //!< The minimum value value for thresholding a image to find targets
#define THRESHOLD_VAL_MAX 255 //!< The maximum value value for thresholding a image to find targets
#define THRESHOLD_DILATES 4

#define FILTER_CONTOURS_BBOXBORDERTHRESHOLD 5 //!< The minimum number of pixels a target's bounding box can be near to the edges of the image for the contour to be considered as a target. @see pickTargets
#define FILTER_CONTOURS_BBOXMAXAREAPERCENT 0.4 //!< The maximum percentage of the screen that can be coved by a contour's bounding box before it won't be considered as a target. @see pickTargets

#define FILTER_CONTOURS_MINAREA 200 //!< The minimum value of a contour's area for it to be selected as a target. @see pickTargets
#define FILTER_CONTOURS_ASPECTRATIO_MIN 0  //!< The minimum aspect ratio of a potential target's bounding box. @see pickTargets
#define FILTER_CONTOURS_ASPECTRATIO_MAX 2 //!< The maximum aspect ratio of a potential target's bounding box. @see pickTargets
#define FILTER_CONTOURS_POINTSPERCONTOURSIDE 6 //!< The number of points on each target's contour's shortest side. @see Target::spacePointsEvenly

#define FILTER_POINTS_CROSSPRODUCTTHRESHOLD 30 //20 //!< The threshold value used for filtering non-colinear lines on contours. @see Target::calculateSegmentsFromPoints

#define TARGET_INDEX_SELECTION_ACCEPTABLE_ALIGMENT_ERROR 25

#define CAMERA_VIEWING_ANGLE_HALF_X 37.5
#define CAMERA_VIEWING_ANGLE_HALF_Y 28.125

#define CAMERA_ROTATION_AXIS_X 0 //14.6209 //!< The vertical axis angle offset of the camera on the robot.
#define CAMERA_HEIGHT_OFF_GROUND 33 //!< The camera's height on the robot.

#define TOP_TARGET_HEIGHT_FROM_BOTTOM_TO_GROUND 63

#endif /* CONSTANTS_H_ */
