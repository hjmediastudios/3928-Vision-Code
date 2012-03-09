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
	#define VERBOSITY 0 //!< A value denoting the output level on the console.
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

#define FILTER_CONTOURS_BBOXBORDERTHRESHOLD 5 //!< The minimum number of pixels a target's bounding box can be near to the edges of the image for the contour to be considered as a target. @see pickTargets
#define FILTER_CONTOURS_BBOXMAXAREAPERCENT 0.4 //!< The maximum percentage of the screen that can be coved by a contour's bounding box before it won't be considered as a target. @see pickTargets

#define FILTER_CONTOURS_MINAREA 200 //!< The minimum value of a contour's area for it to be selected as a target. @see pickTargets
#define FILTER_CONTOURS_ASPECTRATIO_MIN 0  //!< The minimum aspect ratio of a potential target's bounding box. @see pickTargets
#define FILTER_CONTOURS_ASPECTRATIO_MAX 2 //!< The maximum aspect ratio of a potential target's bounding box. @see pickTargets
#define FILTER_CONTOURS_POINTSPERCONTOURSIDE 6 //!< The number of points on each target's contour's shortest side. @see Target::spacePointsEvenly

#define FILTER_POINTS_CROSSPRODUCTTHRESHOLD 30 //20 //!< The threshold value used for filtering non-colinear lines on contours. @see Target::calculateSegmentsFromPoints

#define OFFSETS_SAMPLESPERREADING 32 //!< The number of samples to track in the past for moving-camera averages of target offsets.

#define HOOP_OFFSETFROM_CENTEROFSQUARE_X 3//!< The X-offset (in meters) of the center of the hoop from the target's marker center. 3
#define HOOP_OFFSETFROM_CENTEROFSQUARE_Y 15.5//!< The Y-offset (in meters) of the center of the hoop from the target's marker center. 11
#define HOOP_OFFSETFROM_CENTEROFSQUARE_Z 11//!< The Z-offset (in meters) of the center of the hoop from the target's marker center. 15.5

#define CAMERA_ROTATION_AXIS_X 20 //14.6209 //!< The vertical axis angle offset of the camera on the robot.
#define CAMERA_HEIGHT 19.5 //!< The camera's height on the robot.
	//Cosmetic attributes

#endif /* CONSTANTS_H_ */
