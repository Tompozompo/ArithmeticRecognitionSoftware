/*
 * ArithmeticRecognitionSoftware.cpp
 *
 *  Created on: Aug 10, 2014
 *      Author: Thomas Amon and Zhi Wei Zheng
 */

#include "ArithmeticRecognitionSoftware.h"
#include <math.h>
#include <dirent.h>

/**
 * Loads an image (stored as an IplImage struct) for each
 * filename provided.
 * @param numImages		The number of images to load
 * @param filenames		A list of strings of the filenames to load
 */
SampleImage* loadSamples(int numSamples, int numClasses, char* dirpath, int
		image_size)
{
	SampleImage* rv; // the return result
	int i, // used for looping
		 j, // used for looping
		 k; // used for indexing 
	DIR *dir;  //used to find all files
	struct dirent *ent;  
	
	rv =(SampleImage*)malloc(numClasses*numSamples*sizeof(SampleImage));
	k = 0;
   for(i = 0; i < numClasses; i++)	
	{
		char* dirname = (char*)malloc((strlen(dirpath)+4)*sizeof(char));
		char extention[4];
		sprintf(extention, "%d/", i);
		strcpy(dirname, dirpath);
		strcat(dirname, extention);
		if ((dir = opendir (dirname)) == NULL) 
		{
			fprintf(stderr, "Could not open dir %s\n", dirname);
			return NULL;
		}
	   for(j = 0; j < numSamples+2; j++)	 //need to add 2 because . and ..
		{
			char fullpath[256]; //stores the full path
			ent = readdir(dir);
			if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
				continue;
			strcpy(fullpath, dirname);
			strcat(fullpath, ent->d_name);
			rv[k].image = preprocessing(cvLoadImage(fullpath, 0), image_size,
					image_size);
			rv[k].classifier = i;
			if(rv[k].image == 0) 
			{
				fprintf(stderr, "Error: couldn't load image %s\n", ent->d_name);
				return NULL;
			}
			k++;
		}
		closedir(dir);
	}
	
	return rv;
}

CvMat** createMatrices(SampleImage* samples, int num_samples, int
		num_classes, int image_size)
{
	CvMat** res; // the result		
	CvMat *train_data, //matrix of the train data
   	   *train_classes, //matrix of the training classes 
		   row, // the row to place the image
			data, // the holds a 32 bit version of the total image
      	row_header,	// header of compressed matrix	
      	*row1; // stores the compressed matrix
	int i, //used for looping
		 j, //used for looping
		 k; //used for indexing
	res = (CvMat**)malloc(2*sizeof(CvMat*));
	train_data = cvCreateMat(num_classes*num_samples, image_size*image_size, CV_32FC1);
	train_classes = cvCreateMat(num_classes*num_samples, 1, CV_32FC1);
	res[0] = train_data;
	res[1] = train_classes;

	k = 0;
   for(i = 0; i < num_classes; i++)	
	{
	   for(j = 0; j < num_samples; j++)	
		{
			//Set the value of this class
      	cvGetRow(train_classes, &row, k);
      	cvSet(&row, cvRealScalar(i));
      	//Set data 
      	cvGetRow(train_data, &row, k);
      
      	IplImage* img = cvCreateImage(cvSize( image_size, image_size ), IPL_DEPTH_32F, 1 );
      	//convert 8 bits image to 32 float image
      	cvConvertScale(samples[k].image, img, 0.0039215, 0);
     		//store the new image in data 
      	cvGetSubRect(img, &data, cvRect(0, 0, image_size, image_size));
      	
      	//coverts a size x size martix to a 1 x (size*size)
      	row1 = cvReshape(&data, &row_header, 0, 1);
			//copy the compressed matrix into its correct row
      	cvCopy(row1, &row, NULL);
			k++;
		}
	}
	return res;
}

CvKNearest *knn;
void train_cvknearest(CvMat* data, CvMat* responses)
{
	knn = new CvKNearest(data, responses, 0, false, 10);
}

int find_closest(IplImage* resized_input, int image_size)
{
	int K = 10;
   CvMat *nearest, //used by find_nearest
			data, // the holds a 32 bit version of the total image
      	row_header,	// header of compressed matrix	
      	*row1; // stores the compressed matrix
   
	float result;
   
   nearest = cvCreateMat(1, K, CV_32FC1);

   //Set data 
   IplImage* img32 = cvCreateImage( cvSize( image_size, image_size), 
				IPL_DEPTH_32F, 1);
   //convert 8 bits image to 32 float image
   cvConvertScale(resized_input, img32, 0.0039215, 0);
	//store the new image in data
	cvGetSubRect(img32, &data, cvRect(0,0, image_size, image_size));
   //coverts a size x size martix to a 1 x (size*size)
   row1 = cvReshape( &data, &row_header, 0, 1 );
  	//find that result! 
   result = knn->find_nearest(row1, K, 0, 0, nearest, 0);
   
   return result;
}

void findX(IplImage* imgSrc,int* min, int* max){
	int i;
	int minFound=0;
	CvMat data;
	CvScalar maxVal=cvRealScalar(imgSrc->height* 255);
	CvScalar val=cvRealScalar(0);
	//For each col sum, if sum < width*255 then we find the min 
	//then continue to end to search the max, if sum< width*255 then is new max
	for (i=0; i < imgSrc->width; i++){
		cvGetCol(imgSrc, &data, i);
		val= cvSum(&data);
		if(val.val[0] < maxVal.val[0]){
			*max= i;
			if(!minFound){
				*min= i;
				minFound= 1;
			}
		}
	}
}

void findY(IplImage* imgSrc,int* min, int* max){
	int i;
	int minFound=0;
	CvMat data;
	CvScalar maxVal=cvRealScalar(imgSrc->width * 255);
	CvScalar val=cvRealScalar(0);
	//For each col sum, if sum < width*255 then we find the min 
	//then continue to end to search the max, if sum< width*255 then is new max
	for (i=0; i< imgSrc->height; i++){
		cvGetRow(imgSrc, &data, i);
		val= cvSum(&data);
		if(val.val[0] < maxVal.val[0]){
			*max=i;
			if(!minFound){
				*min= i;
				minFound= 1;
			}
		}
	}
}
CvRect findBB(IplImage* imgSrc){
	CvRect aux;
	int xmin, xmax, ymin, ymax;
	xmin=xmax=ymin=ymax=0;

	findX(imgSrc, &xmin, &xmax);
	findY(imgSrc, &ymin, &ymax);
	
	aux=cvRect(xmin, ymin, xmax-xmin, ymax-ymin);
	
	return aux;
	
}

IplImage* preprocessing(IplImage* imgSrc,int new_width, int new_height){
	IplImage* result;
	IplImage* scaledResult;

	CvMat data;
	CvMat dataA;
	CvRect bb;//bounding box
	CvRect bba;//boundinb box maintain aspect ratio
	
	//convert the image to black and white
	/*
	IplImage* bw_image = cvCreateImage(cvGetSize(imgSrc), IPL_DEPTH_8U, 1);
	cvThreshold(imgSrc, bw_image, 128, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	cvSaveImage(bw_image);
	*/

	//Find bounding box
	bb=findBB(imgSrc);
	
	//Get bounding box data and no with aspect ratio, the x and y can be corrupted
	cvGetSubRect(imgSrc, &data, cvRect(bb.x, bb.y, bb.width, bb.height));
	//Create image with this data with width and height with aspect ratio 1 
	//then we get highest size betwen width and height of our bounding box
	int size = (bb.width>bb.height)?bb.width:bb.height;
	result=cvCreateImage( cvSize( size, size ), 8, 1 );
	cvSet(result,CV_RGB(255,255,255),NULL);
	//Copy de data in center of image
	int x = (int)floor((float)(size-bb.width)/2.0f);
	int y = (int)floor((float)(size-bb.height)/2.0f);
	
	cvGetSubRect(result, &dataA, cvRect(x,y,bb.width, bb.height));
	cvCopy(&data, &dataA, NULL);
	//Scale result
	scaledResult=cvCreateImage( cvSize( new_width, new_height ), 8, 1 );
	cvResize(result, scaledResult, CV_INTER_NN);
	
	//Return processed data
	return scaledResult;
}

