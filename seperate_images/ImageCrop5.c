#include <stdlib.h>
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cv.h>
#include <highgui.h>


int main(int argc, char** argv) {
	IplImage* im_gray = cvLoadImage("written2.png", CV_LOAD_IMAGE_GRAYSCALE);

	IplImage* im_bw = cvCreateImage(cvGetSize(im_gray), IPL_DEPTH_8U, 1);
	cvThreshold(im_gray, im_bw, 128, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	cvSaveImage("written2.png", im_bw,0);
	return 0;
}
