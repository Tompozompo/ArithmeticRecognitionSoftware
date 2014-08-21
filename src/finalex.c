/*
 * finalex.c
 *
 *  Created on: Aug 10, 2014
 *      Author: Thomas Amon and Zhi Wei Zheng 
 */

//Sample images from TODO link here
#include "ArithmeticRecognitionSoftware.h"

#define CLASS_COUNT 14
#define SAMPLE_COUNT 100
#define SIZE 128

int main(int argc, char ** argv) {
		 int i, // just an iterator
		     result,	//store the integer result
		     number_of_images; // the number of subimages the input image was
			  						  // split into

	IplImage *input, // the source image
				*bw_input, 	// the input image made black and white
				**Ops,	// array that holds the Operator/Operand images
				*resized_input; // stores subimages resized to SIZExSIZE 

	CvMat** training_matrices;  // array containing the sample data and
										 // classes in 2 seperate matrices
										 // [0] is data and [1] is classes

	SampleImage* samples; // array of sample images
	char sample_images[256];	// path to the root of the sample images dir
	
	strcpy(sample_images, "../samples/");	
	
	result = 0;	
	number_of_images = 0;

	if(argc < 2)
	{
		usage();
		exit(0);
	}
	
	// Load the sample images
	printf("Loading sample images\n");
	if ((samples = loadSamples(SAMPLE_COUNT, CLASS_COUNT, sample_images, SIZE)) == NULL ) 
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
	
	// Load the source image
	printf("Loading input image\n");
	input = cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	if(input == NULL)
	{
		fprintf(stderr, "Could not load %s!\n", argv[1]);
		return EXIT_FAILURE;
	}
   cvSaveImage("echo.png", input);

	
	// convert input to black and white  
	printf("Converting the image to black and white");
	bw_input = convert_to_bw(input);
   cvSaveImage("bwimage.png", bw_input, 0);

	Ops = OpCropper(bw_input, &number_of_images);

	char filename[32];	
	for(i = 0; i < number_of_images; i++)
	{
		sprintf(filename, "seperated_image_%d.png", i);
		cvSaveImage(filename, Ops[i]);
	
		//scale the input image to SIZExSIZE
		printf("Scaling input image to size %d x %d\n", SIZE, SIZE);
		resized_input = preprocessing(Ops[i], SIZE);
   	cvSaveImage(filename, resized_input);
   	
		//finding closest match in sample data	
   	printf("Finding closest match\n");	
   	result = find_closest(resized_input, SIZE);
   
   	printf("Number entered: %c\n", int_to_ops(result));
	}


	return EXIT_SUCCESS;
}

