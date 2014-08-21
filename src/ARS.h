/*
 * ArithmeticRecognitionSoftware.h
 *
 *  Created on: Aug 10, 2014
 *      Author: Thomas Amon and Zhi Wei Zheng
 */

#ifndef ARS_H_
#define ARS_H_

#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void usage();

char int_to_ops(int i);

IplImage* convert_to_bw(IplImage* image);

IplImage** OpCropper(IplImage* imgSrc, int* ImgNum);

IplImage** loadSamples(int num_samples, int num_classes, char* dirpath,
		int image_size);

CvMat** createMatrices(IplImage** samples, int num_samples, int
		num_classes, int image_size);

IplImage* scale_image(IplImage* image, int image_size);

#endif
