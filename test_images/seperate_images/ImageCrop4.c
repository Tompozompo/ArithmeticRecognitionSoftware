#include <stdlib.h>
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <dirent.h>

IplImage** OpCropper(IplImage* imgSrc, int* main, int* max, int* ImgNum);

int main(int argc, char* argv) {
	IplImage** Ops;													//Initialize array of IplImage to hold the Operator/Operand images
	IplImage* img = cvLoadImage("equation2.png", CV_LOAD_IMAGE_GRAYSCALE);		//Loads the image that contains the equations
	if(img == NULL) {
		printf("The image could not be loaded\n");
		exit(1);
	}
	fprintf(stderr,"%s\n", "new stuff");
	IplImage* im_bw = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 1);
	fprintf(stderr, "%s\n", "newer stuff");
	cvThreshold(img, im_bw, 128, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	fprintf(stderr, "%s\n", "newest stuff");
	int ImgNum = 0;													//Initialize the variable to keep track of the amount of operator/operand images

	//Initialize the number for the croped image dimenions
	int xmin = 0;
	int xmax = 0;
	int ymin = 0;
	int ymax = img->height;

	//Stores the result of the separated images
	Ops = OpCropper(im_bw, &xmin, &xmax, &ImgNum);
	cvSaveImage("out.PNG", Ops[0], NULL);
	fprintf(stderr, "%s\n", "newest stuff");
	cvSaveImage("out2.PNG", Ops[1], NULL);
	fprintf(stderr, "%s\n", "newest stuff");
	cvSaveImage("out3.PNG", Ops[2], NULL);
	fprintf(stderr, "%s\n", "newest stuff");
	cvSaveImage("out4.PNG", Ops[3], NULL);
	fprintf(stderr, "%s\n", "newest stuff");
	cvSaveImage("out5.PNG", Ops[4], NULL);
	fprintf(stderr, "%s\n", "newest stuff");
	cvSaveImage("out6.PNG", Ops[5], NULL);
	fprintf(stderr, "%s\n", "newest stuff");

}

IplImage** OpCropper(IplImage* imgSrc,int* min, int* max, int* ImgNum){
	int i;						//Iterate through column
	int minFound=0;			//Acts as a bool to see if the first minimum value is set
	int count = 0;				//Index through the array of IplImage

	CvRect cropRect;			//Initialize a rectangle
	IplImage* cropImg;		//Initialize a IplImage pointer to hold the cropped image
	IplImage** rv = (IplImage**)malloc(128*sizeof(IplImage*));		//Array to store a maximum of 128 IplImages

	CvMat data;
	CvScalar maxVal=cvRealScalar(imgSrc->height* 255);					//Stores the value of a column that is entirely white
	CvScalar val=cvRealScalar(0);												//Stores the value of the current column

	//For each col sum, if sum < width*255 then we find the min 
	//then continue to end to search the max, if sum< width*255 then is new
	//max

	//Iterate through each column
	for (i=0; i< imgSrc->width; i++){
		cvGetCol(imgSrc, &data, i);
		val= cvSum(&data);

		//Set the min to the initial column containing black
		//This is where the Operator/Operand begins
		if(val.val[0] < maxVal.val[0] && !minFound){
			*min = i;
			minFound = 1;
		}
		//Keeps setting the max value to the column with a black pixel
		else if(val.val[0] < maxVal.val[0] && minFound){
			*max= i;			
		}
		//Once a white pixel is found, we have found the end of the
		//operator/operand
		else if(val.val[0] == maxVal.val[0] && minFound) 
		{
			//Set a rectangle to have the dimension of the cropping area
			cropRect = cvRect(*min, 0, *(max)-*(min), imgSrc->height);
			//Crop the image and put it in the array
			cropImg = cvCreateImage(cvSize(cropRect.width, cropRect.height),imgSrc->depth, imgSrc->nChannels);
			cvSetImageROI(imgSrc, cropRect);
			cvCopy(imgSrc, cropImg, NULL);
			rv[count] = cvCreateImage(cvSize(cropImg->width, cropImg->height), imgSrc->depth,imgSrc->nChannels);
			cvCopy(cropImg, rv[count], NULL);
			cvResetImageROI(imgSrc);

			//Increment the index and amount of images
			count++;	
			(*ImgNum)++;	
			//Minimum is reset to find a new operand/opeartor
			minFound = 0;
		}
	}
	return rv;
}
