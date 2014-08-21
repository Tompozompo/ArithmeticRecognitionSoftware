/*
 * finalex.c
 *
 *  Created on: Aug 10, 2014
 *      Author: Thomas Amon and Zhi Wei Zheng 
 */

// Images from http://press.liacs.nl/mirflickr/#sec_download
#include "ArithmeticRecognitionSoftware.h"

#define CLASS_COUNT 10
#define SAMPLE_COUNT 100 
#define SIZE 128 

int main(int argc, char ** argv) {
	int numberOfImages, // the number of images to load for training 
			numColumns, // the number of horizontal images in the result
			numRows, // the number of vertical images in the result
			i, // just an iterator
		   result,
			size;
	IplImage *input, // the source image
			*res, // the resulting mosaic image
			*smallRes, // the resulting mosaic image that is only 2x the size of the original
			*resized_input;
	IplImage **timages, // the small images loaded from the disk
			**subImages, // small images created from the source image
			**closest; // the pictures in timates closest to the subImages
	CvMat** training_matrices;  // array containing the sample data and
										 // classes in 2 seperate matrices
										 // [0] is data and [1] is classes
	SampleImage* samples; //array of sample images

	result = 0;	
	char filepath[256];
	strcpy(filepath, "../OCR/");

	if(argc < 2)
	{
		printf("Error: Need atleast 2 commands.\n");
		//TODO Usage print
		exit(0);
	}
	
	// Load the source image
	printf("Loading input image\n");
	input = cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
   cvSaveImage("echo.png", input);

	if(input == NULL)
	{
		fprintf(stderr, "Could not load %s!\n", argv[1]);
		return EXIT_FAILURE;
	}

   IplImage* bw_input = cvCreateImage(cvGetSize(input), IPL_DEPTH_8U, 1);
   cvThreshold(input, bw_input, 128, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

   cvSaveImage("bwimage.png", bw_input, 0);
	
	// Scale the input image
	printf("Scaling input image to size %d x %d\n", SIZE, SIZE);

	resized_input = preprocessing(bw_input, SIZE, SIZE);

   cvSaveImage("resizedimage.png", resized_input);
	
	// Load the sample images
	printf("Loading sample images\n");
	if ((samples = loadSamples(SAMPLE_COUNT, CLASS_COUNT, filepath, SIZE)) == NULL ) 
	{
		fprintf(stderr, "Could not load images!\n");
		return EXIT_FAILURE;
	}

	//Create matrices of the sample images
	printf("Creating matrices\n");	
	training_matrices = createMatrices(samples, SAMPLE_COUNT, CLASS_COUNT,
			SIZE);
	
	//Training with sample data
	printf("Training with sample data\n");	
	train_cvknearest(training_matrices[0], training_matrices[1]);
	
	//finding closest match in sample data	
	printf("Finding closest match\n");	
	result = find_closest(resized_input, SIZE);

	printf("Number entered: %d\n", result);

	return EXIT_SUCCESS;
}

