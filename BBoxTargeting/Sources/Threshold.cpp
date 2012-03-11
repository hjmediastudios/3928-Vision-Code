#include "../Headers/Threshold.h"

IplImage* Threshold::threshold_findGreen(IplImage* img, int dilates)
{
	CvSize imgSize = cvGetSize(img);
	IplImage *imgHSV = cvCreateImage(imgSize, 8, 3); //Create an HSV image for more efficient thresholding
	IplImage *imgThreshold = cvCreateImage(imgSize, 8, 1);
	cvCvtColor(img, imgHSV, CV_BGR2HSV); //convert the BGR image to an HSV image stored in imgHSV
	cvInRangeS(imgHSV, cvScalar(THRESHOLD_HUE_MIN, THRESHOLD_SAT_MIN, THRESHOLD_VAL_MIN), cvScalar(THRESHOLD_HUE_MAX, THRESHOLD_SAT_MAX, THRESHOLD_VAL_MAX), imgThreshold);
	//Dilates are sloooow...
	for (int i=0; i<dilates; i++)
		cvDilate(imgThreshold, imgThreshold);

	for(int i=0; i<dilates; i++)
		cvErode(imgThreshold, imgThreshold);

	cvReleaseImage(&imgHSV);
	return imgThreshold;
}

CvSeq* Threshold::findContours(IplImage *image)
{
	CvSeq* contours;
	static CvMemStorage* storage = NULL;
	if (storage == NULL)
		storage = cvCreateMemStorage(0);
	cvFindContours(image, storage, &contours);
	return contours;
}

void Threshold::pickTargets(IplImage* img, CvSeq* contours, CvSize imgSize, Target *tgtSet, int *numTgts)
{

	if (contours->total > 0)
	{
		(*numTgts) = 0;
		while (contours != 0)
		{

			double area = cvContourArea(contours);
			if (area > FILTER_CONTOURS_MINAREA && !CV_IS_SEQ_HOLE(contours)) //filter out holes and too-small contours
			{
				CvRect tempBBox = cvBoundingRect(contours, 1);
				float aspectRatio = (float) tempBBox.width/tempBBox.height;

				if (tempBBox.x > FILTER_CONTOURS_BBOXBORDERTHRESHOLD &&
						(tempBBox.x + tempBBox.width) < (imgSize.width-FILTER_CONTOURS_BBOXBORDERTHRESHOLD) &&
						tempBBox.y > FILTER_CONTOURS_BBOXBORDERTHRESHOLD &&
						(tempBBox.y + tempBBox.height) < (imgSize.height - FILTER_CONTOURS_BBOXBORDERTHRESHOLD) &&
						((float)(tempBBox.width*tempBBox.height)/(imgSize.width*imgSize.height)) < (FILTER_CONTOURS_BBOXMAXAREAPERCENT)) //filter based on distance to img border and, later, if it's too large
				{
					if (aspectRatio >= FILTER_CONTOURS_ASPECTRATIO_MIN && aspectRatio <= FILTER_CONTOURS_ASPECTRATIO_MAX) //filter based on aspect ratio
					{
#if VERBOSITY >= 2
						std::cout << "Found another target!\n";
#endif
						CvSeq* contourPoints = cvConvexHull2((void*)contours, 0, CV_CLOCKWISE, 1);
						tgtSet[*numTgts] = Target::Target(contourPoints, img);
						(*numTgts)++;
					}
				}
			}
			//...
			contours = contours->h_next;
		}
	}

#if VERBOSITY >= 1
	if ((*numTgts) < 1)
	{
		std::cerr << "ERROR: No targets found.\n";
	}
#endif
}

