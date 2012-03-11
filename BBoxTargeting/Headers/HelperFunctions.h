/*
 * HelperFunctions.h
 *
 *  Created on: Feb 28, 2012
 * @author Nick Royer
 *
 */

#ifndef HELPERFUNCTIONS_H_
#define HELPERFUNCTIONS_H_
#include <opencv/cv.h>
#include <opencv/cvaux.h>

/**
 * @brief Returns the distance between two points.
 * @param point1 The first point used for the calculation.
 * @param point2 The second point used for the calculation.
 * @return the distance, in pixels, between the two points.
 */
float pointDistance(CvPoint point1, CvPoint point2);
/**
 * @brief Converts a float variable into a string for easier display.
 *
 * Converts a float variable into a string for easier display. Note that this function is limited to
 * 30 characters or fewer for the resulting string.
 * @param flt The float variable desired to be converted to a string.
 * @return The float variable expressed as a string.
 */
char* floatToString(float flt);
/**
 * @brief Returns a basic default font.
 * @param size The desired font size, from 0 to 1 to an arbitrary float. 0.5 is used primarily by this code.
 * @return A pointer to a CvFont object of the desired size.
 */
CvFont* getFont(float size);
/**
 * @brief Calculates the 3D distance from the origin to a point.
 * @param vecX The X-offset of the point from the origin.
 * @param vecY The Y-offset of the point from the origin.
 * @param vecZ The Z-offset of the point from the origin.
 * @return The distance from the origin to the given point, expressed in the same units as the inputs.
 */
float calculateVectorFrom3Offsets(float vecX, float vecY,float vecZ);

/**
 * @brief Converts the dimensions of an Aruco marker (meters) to inches.
 * @param markerCoordinate The dimension in meters.
 * @return The dimension in inches.
 */
float metersToInches(float markerCoordinate);
/**
 * @brief Multiplies two square matrices.
 *
 * Multiplies to matrices. The matrices are expressed as square arrays of dimensions indicated by
 * the dimension parameter. Due to idiosyncracies of the requirements of this function, the first
 * matrix uses doubles while the rest use floats.
 * @param a The first matrix to multiply.
 * @param b The second matrix to multiply.
 * @param result the resulting multiplied matrix.
 * @param dimension The size of each square matrix.
 */
void matrixMultiply(float *a, double *b, float *result, int dimension);

CvPoint3D32f crossProduct(CvPoint3D32f vector1, CvPoint3D32f vector2);

float inchesToMeters(int inches);
float rad2deg(float rads);
float deg2rad(float degrees);
#endif /* HELPERFUNCTIONS_H_ */
