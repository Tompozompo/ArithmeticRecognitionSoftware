#include "knearest.h"

int find_closest(IplImage* resized_input, int image_size)
{
	int K = 10;
   CvMat *nearest, //used by find_nearest
      	row_header,	// header of compressed matrix	
      	*row1; // stores the compressed matrix
   
	float result;
   
   nearest = cvCreateMat(1, K, CV_32FC1);

   //Set up the image so it can be looked for
   IplImage* img32 = cvCreateImage( cvSize( image_size, image_size), 
				IPL_DEPTH_32F, 1);

   //convert 8 bits image to 32 float image
   cvConvertScale(resized_input, img32, 0.0039215, 0);

   //coverts a size x size martix to a 1 x (size*size)
   row1 = cvReshape(img32, &row_header, 0, 1 );

  	//find that result! 
   result = knn->find_nearest(row1, K, 0, 0, nearest, 0);
   return result;
}
