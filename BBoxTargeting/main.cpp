#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "./Headers/Threshold.h"
#include "./Headers/Target.h"
#include <sstream>
#include <aruco/aruco.h>

//#define CAMERA_USED
using namespace aruco;

IplImage *frame; //!< A pointer to the currently-evaluated frame.
CvSize frameSize; //!< A CvSize object containing the frame's dimensions.
IplImage *frameThreshed; //!< A pointer to the thresholded (B & W) version of the current frame.
CvCapture *camera = NULL; //!< A pointer to the current capture device.
CameraParameters camParameters; //!< The Aruco camera parameters of the current camera.
CvFont font1; //!< The font used for all drawing on the current frame.

float movingCameraAverageForHighestTarget[OFFSETS_SAMPLESPERREADING][3]; //!< The number of samples taveraged together to provide a moving-camera average for he highest target's position
float movingCameraRunningSum[3] = {0, 0, 0}; //!< The running sums (X, Y, Z) of the highest target's offsets.
int currentSample = 0; //!< The current sample for the running average. @see movingCameraAveragForHighestTarget
int highestTargetIndex = 0; //!< The index index in targetSet of the highest target in the view. @see targetSet

CvSeq* contours; //!< A pointer to the CvSeq object containing the current set of contours in the image.
Target targetSet[50]; //!< A set of Target objects containing all valid targets visible in the current view. @see Target::Target
int numTargets = 0; //!< The number of valid targets visible in the current view. @see targetSet
//CvMemStorage* storage;
int angle = 20 ;

int main()
{
	//Find camera and capture from it.

	if (camera == NULL)
	{
#if USE_CAM == 0
		camera = cvCaptureFromAVI("TestImages/capture1.avi");
#else
		camera = cvCaptureFromCAM(CAMERA_INDEX);	//TODO Why?
#endif
		//		storage = cvCreateMemStorage(0);

		if (!camera)
		{
#if VERBOSITY >= 1
			std::cerr << "ERROR: Camera" << CAMERA_INDEX << " not found.\n";
#endif
		}
		else
		{
			//Set camera properties
			cvSetCaptureProperty(camera, CV_CAP_PROP_BRIGHTNESS, 0.00784); //2 in GUVCview
			cvSetCaptureProperty(camera, CV_CAP_PROP_CONTRAST, 0.2); //51 in GUVCview
			cvSetCaptureProperty(camera, CV_CAP_PROP_SATURATION, 0.498); //127 in GUVCview
			cvSetCaptureProperty(camera, CV_CAP_PROP_GAIN, 0); //0 in GUVC view
			cvSetCaptureProperty(camera, CV_CAP_PROP_AUTO_EXPOSURE, 0); //Have to set through GUVCview as 0, manual mode !!!!
			cvSetCaptureProperty(camera, CV_CAP_PROP_TEMPERATURE, 10000); //Have to set through GUVCview as 10000 !!!
			cvSetCaptureProperty(camera, CV_CAP_PROP_EXPOSURE, 166); //Have to set through GUVCview as 166 !!!
			cvSetCaptureProperty(camera, CV_CAP_PROP_SHARPNESS, 224); //Have to set through GUVCview as 224 !!!
			//Backlight compensation 0, have to set through GUVCview
			cvSetCaptureProperty(camera, CV_CAP_PROP_FRAME_WIDTH, 640);
			cvSetCaptureProperty(camera, CV_CAP_PROP_FRAME_HEIGHT, 480);
			camParameters.readFromXMLFile("intrinsics.yml");
			cvInitFont(&font1,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, 0.5,0.5,0,1);
			memset(movingCameraAverageForHighestTarget, 0, sizeof(float)*3*OFFSETS_SAMPLESPERREADING);


#if VERBOSITY >= 2
			std::cout << "Finished setting camera parameters.\n";
#endif
		}
	}

	frame = cvQueryFrame(camera);
	frameSize = cvGetSize(frame);


	//Main image-processing loop
	while (true)
	{
		frame = cvQueryFrame(camera);

		if (!frame)
		{
#if VERBOSITY >= 1
			std::cerr << "ERROR: Couldn't query frame. Quitting.\n";
#endif
			exit(0);
		}
		frameThreshed = Threshold::threshold_findGreen(frame, 5);
//		cvShowImage("Threshold", frameThreshed);
		contours = Threshold::findContours(frameThreshed);
		cvReleaseImage(&frameThreshed);
		//cvShowImage("Contours", frame);
		if (contours != NULL)
		{
			numTargets = 0;
			//cvDrawContours(frame, contours, CV_RGB(255,0,0), CV_RGB(255,0,0), 1, 8, 0);
			Threshold::pickTargets(frame, contours, frameSize, targetSet, &numTargets);
			if (numTargets> 0) //Run the following code if targets have been found
			{
				//Pick highest target

				for (int i=0; i<numTargets; i++) //Cycle through targets.
				{
//					if (&(targetSet[i].contour) != NULL)
//					{
						targetSet[i].calculateHoopPosition(FILTER_CONTOURS_POINTSPERCONTOURSIDE, FILTER_POINTS_CROSSPRODUCTTHRESHOLD, camParameters);
//					}

					targetSet[i].drawTarget(CV_RGB(255 - ((255/numTargets)*i), (255/numTargets)*i, 100));

					drawAxis(frame, targetSet[i].marker, camParameters);
					std::cerr << i << "\n";

					targetSet[i].getNavInfo(CAMERA_ROTATION_AXIS_X);
					//std::cout << targetSet[i].offsets[0] << " " << targetSet[i].offsets[1] << " " << targetSet[i].offsets[2] << endl;
					cvPutText(frame, floatToString(rad2deg(targetSet[i].offsets[0])), cvPoint(targetSet[i].getBoundingBoxPoint2().x+1,targetSet[i].getBoundingBoxPoint1().y), &font1, CV_RGB(255,255,255));
					cvPutText(frame, floatToString(metersToInches(targetSet[i].offsets[1])), cvPoint(targetSet[i].getBoundingBoxPoint2().x+1,targetSet[i].getBoundingBoxPoint1().y+10), &font1, CV_RGB(0,255,255));
					cvPutText(frame, floatToString(metersToInches(targetSet[i].offsets[2])), cvPoint(targetSet[i].getBoundingBoxPoint2().x+1,targetSet[i].getBoundingBoxPoint1().y+24), &font1, CV_RGB(255,255,0));
//					cvPutText(frame, floatToString(targetSet[i].getBoundingBoxPoint2().y - targetSet[i].getBoundingBoxPoint1().y - 0.0), cvPoint(targetSet[i].getCenter().x ,targetSet[i].getBoundingBoxPoint1().y - 10), &font1, CV_RGB(255,0,0));
//					cvPutText(frame, floatToString(targetSet[i].numLines - 0.0), cvPoint(targetSet[i].getCenter().x ,targetSet[i].getBoundingBoxPoint1().y - 24), &font1, CV_RGB(0,150,0));

					break; //TODO Get me out of here!
				}


				//				for (int sumIndex = 0; sumIndex<3; sumIndex++)
				//									{
				//										movingCameraRunningSum[sumIndex] -= movingCameraAverageForHighestTarget[currentSample][sumIndex];
				//										movingCameraAverageForHighestTarget[currentSample][sumIndex] = targetSet[i].offsets[sumIndex];
				//										movingCameraRunningSum[sumIndex] += movingCameraAverageForHighestTarget[currentSample][sumIndex];
				//									}
				//
				//									currentSample++;
				//
				//									if (currentSample >= OFFSETS_SAMPLESPERREADING)
				//									{
				//										currentSample = 0;
				//				#if VERBOSITY >= 1
				//										std::cout << "Finished " << OFFSETS_SAMPLESPERREADING << " sample.\n";
				//				#endif
				//									}


//				highestTargetIndex = 0;
//				int highestCenterY = 9000;
//				int currY;
//				for (int i=0; i<numTargets; i++)
//				{
//					currY = targetSet[i].getCenter().y;
//					if (currY <= highestCenterY && targetSet[i].offsets[0] != 0)
//					{
//						highestCenterY = currY;
//						highestTargetIndex = i;
//					}
//				}
				//Draw info about highest target
				//				cvPutText(frame, floatToString(metersToInches(movingCameraRunningSum[1]/OFFSETS_SAMPLESPERREADING)), cvPoint(targetSet[highestTargetIndex].getBoundingBoxPoint2().x+1,targetSet[highestTargetIndex].getBoundingBoxPoint1().y+24), &font1, CV_RGB(0,255,0));
				//				cvPutText(frame, floatToString(metersToInches(movingCameraRunningSum[2]/OFFSETS_SAMPLESPERREADING)), cvPoint(targetSet[highestTargetIndex].getBoundingBoxPoint2().x+1,targetSet[highestTargetIndex].getBoundingBoxPoint1().y+38), &font1, CV_RGB(0,0,255));
				//				cvPutText(frame, floatToString(metersToInches(calculateVectorFrom3Offsets(movingCameraRunningSum[0]/OFFSETS_SAMPLESPERREADING, movingCameraRunningSum[1]/OFFSETS_SAMPLESPERREADING,movingCameraRunningSum[2]/OFFSETS_SAMPLESPERREADING))), cvPoint(targetSet[highestTargetIndex].getBoundingBoxPoint2().x+1,targetSet[highestTargetIndex].getBoundingBoxPoint1().y+52), &font1, cvScalarAll(255));


#if VERBOSITY >= 1
				if (highestTargetIndex >= numTargets)
					std::cerr << "ERROR: No highest target found." << endl;
#endif



				//Draw highest target
//				cvRectangle(frame, targetSet[highestTargetIndex].getBoundingBoxPoint1(), targetSet[highestTargetIndex].getBoundingBoxPoint2(), CV_RGB(0,255,0), 2, 8, 0);

			}

		}

		//draw crosshairs
//		cvLine(frame, cvPoint(320, 200), cvPoint(320,280), CV_RGB(255,0,0), 1, 8, 0);
//		cvLine(frame, cvPoint(280, 240), cvPoint(360,240), CV_RGB(255,0,0), 1, 8, 0);
//		cvCircle(frame, cvPoint(320,240), 30, CV_RGB(255, 0, 0), 1, 8, 0);

		cvShowImage("Targets", frame);
		//Wait before moving on
		int key = cvWaitKey(0);
		if (key == 27)
		{
			exit(0);
		}




	}
	// TODO Release images to prevent memory leaks
	//	cvReleaseImage(&frame);
	//	cvReleaseImage(&frameThreshed);

	return 0;
}
