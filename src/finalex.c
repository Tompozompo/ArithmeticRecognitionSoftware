/*
 * finalex.c
 *
 *  Created on: Aug 10, 2014
 *      Author: Thomas Amon and Zhi Wei Zheng 
 */

#include "ARS.h"
#include "knearest.h"

#define CLASS_COUNT 16
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
				*resized_input, // stores subimages resized to SIZExSIZE 
	 			**samples; // array of sample images
	CvMat** training_matrices;  // array containing the sample data and
										 // classes in 2 seperate matrices
										 // [0] is data and [1] is classes
	char sample_images[256],	// path to the root of the sample images dir
	     *expression,	// the picture converted into an expression
		  *postfix,  // the expression ion postfix form
		  *f,	// loops through the expression string
		  e;	// stores the value of each image temporarily 
	
	strcpy(sample_images, "samples/");	
	
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
	if(train_cvknearest(training_matrices[0], training_matrices[1]) != 0)
	{
		fprintf(stderr, "Error: Failed training kNearest\n");
		exit(1);
	}
	
	// Load the source image
	printf("Loading input image\n");
	input = cvLoadImage(argv[1], 0);
	if(input == NULL)
	{
		fprintf(stderr, "Could not load %s!\n", argv[1]);
		return EXIT_FAILURE;
	}
   cvSaveImage("saves/echo.png", input, NULL);

	
	// convert input to black and white  
	printf("Converting input image to black and white\n");
	bw_input = convert_to_bw(input);
   cvSaveImage("saves/bwimage.png", bw_input, NULL);
	
	//Seperate the image into its operators and operands
	printf("Seperating the image into its operators and operands\n");
	Ops = OpCropper(bw_input, &number_of_images);

	expression = (char*)malloc(3*number_of_images*sizeof(char));
	f = expression;

	char filename[32];	
   printf("Finding closest matches\n");	
	for(i = 0; i < number_of_images; i++)
	{

		sprintf(filename, "saves/seperated_image_%d.png", i);
		cvSaveImage(filename, Ops[i], NULL);
		//scale the input image to SIZExSIZE
		resized_input = scale_image(Ops[i], SIZE);
   	
		//finding closest match in sample data	
		sprintf(filename, "saves/resized_image_%d.png", i);
   	result = find_closest(resized_input, SIZE);
		cvSaveImage(filename, resized_input, NULL);

	
		//store the interpreted character in expression
		e = int_to_ops(result);
		if(isdigit(e))
		{
			*f = e;
			f++;
		}
		else
		{
			*f = ' ';
			f++;
			*f = e;
			f++;
			*f = ' ';
			f++;
		}
	}
	*f = '\0';
	
	printf("Expression identified as: %s \n", expression);

	//make the expressin posfix	
	postfix = in2post(expression);
	if(postfix == NULL)
		return EXIT_FAILURE;
	printf("Post fix expression: %s\n", postfix);
	
	//evaluate the posfix expression
	result = postEval(postfix);
	printf("Answer: %d\n", result);	

	return EXIT_SUCCESS;
}

