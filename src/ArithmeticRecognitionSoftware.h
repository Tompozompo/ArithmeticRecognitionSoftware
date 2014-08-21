/*
 * ArithmeticRecognitionSoftware.h
 *
 *  Created on: Aug 10, 2014
 *      Author: Thomas Amon and Zhi Wei Zheng
 */

#ifndef HW4_H_
#define HW4_H_

#include "cv.h"
#include "highgui.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include <ml.h>


typedef struct SampleImage
{
	IplImage* image;
	int classifier;
} SampleImage;

void usage();

char int_to_ops(int i);

IplImage* convert_to_bw(IplImage* image);

IplImage** OpCropper(IplImage* imgSrc, int* ImgNum);

SampleImage* loadSamples(int num_samples, int num_classes, char* dirpath,
		int image_size);

CvMat** createMatrices(SampleImage* samples, int num_samples, int
		num_classes, int image_size);

void train_cvknearest(CvMat* data, CvMat* responses);

int find_closest(IplImage* resized_input, int image_size);

IplImage* preprocessing(IplImage* image, int image_size);

#endif
