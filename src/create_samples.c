/*
 * create_samples.c
 *
 * This is used to create sample files.
 * Not used by finalex at all, just used to make the sample files
 * that are used by finalex.
 *
 * Adapted from the main from this github :
 * https://github.com/damiles/basicOCR 
 */


#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <ctype.h>

IplImage* imagen;
int red,green,blue;
IplImage* screenBuffer;
int drawing;
int r,last_x, last_y;

void draw(int x,int y){
	//Draw a circle where is the mouse
	cvCircle(imagen, cvPoint(x,y), r, CV_RGB(red,green,blue), -1, 4, 0);
	//Get clean copy of image
	screenBuffer=cvCloneImage(imagen);
	cvShowImage( "Demo", screenBuffer );
}

void drawCursor(int x, int y){
	//Get clean copy of image
	screenBuffer=cvCloneImage(imagen);
	//Draw a circle where is the mouse
	cvCircle(screenBuffer, cvPoint(x,y), r, CV_RGB(0,0,0), 1, 4, 0);
}

void on_mouse( int event, int x, int y, int flags, void* param )
{
	last_x=x;
	last_y=y;
	drawCursor(x,y);
    //Select mouse Event
	if(event==CV_EVENT_LBUTTONDOWN)
        {
			drawing=1;
			draw(x,y);
		}
    else if(event==CV_EVENT_LBUTTONUP)
		{
			//drawing=!drawing;
			drawing=0;
		}
	else if(event == CV_EVENT_MOUSEMOVE  &&  flags & CV_EVENT_FLAG_LBUTTON)
		{
			if(drawing)
				draw(x,y);
		}
}



int main( int argc, char** argv )
{
	char filename[8];
	char path[64];
	char fullpath[72];
	char t;
	int i = 0;
	int j = 0;
	if(argc < 3)
	{
		printf("Usage: ./a.out path character start\n"
				"path -- the path to the location of your sample files\n"
				"character -- the character you're drawing\n"
				"start -- the number you want the files to start saving at\n\n"
				"Sample: ./create_sample samples/5/ 5 12\n"
				);

		exit(1);
	}

    printf("Hot keys: \n"
	"\tc - reset\n"
	"\tr - reset and save image\n"
	"\t+ - cursor radio ++\n"
	"\t- - cursor radio --\n"
        "\tESC - quit the program\n");
	drawing=0;
	r=10;
	red=green=blue=0;
	last_x=last_y=red=green=blue=0;
	//Create image
	imagen=cvCreateImage(cvSize(128,128),IPL_DEPTH_8U,1);
	//Set data of image to white
	cvSet(imagen, CV_RGB(255,255,255),NULL);
	//Image we show user with cursor and other artefacts we need
	screenBuffer=cvCloneImage(imagen);
	
	//Create window
    	cvNamedWindow( "Demo", 0 );

	cvResizeWindow("Demo", 128,128);
	//Create mouse CallBack
	cvSetMouseCallback("Demo",&on_mouse, 0 );

	//Main Loop

	strcpy(path, argv[1]);
	t = argv[2][0];
	j = atoi(argv[3]);
	while(j >= 10)
	{
		j=j-10;
		i++;
	}
	bzero(fullpath, 0);

    for(;;)
    {
		int c;

        cvShowImage( "Demo", screenBuffer );
        c = cvWaitKey(10);
        if( (char) c == 27 )
            break;
	if( (char) c== '+' ){
		r++;
		drawCursor(last_x,last_y);
	}
	if( ((char)c== '-') && (r>1) ){
		r--;
		drawCursor(last_x,last_y);
	}
	if( (char)c== 'c'){
		cvSet(imagen, cvRealScalar(255),NULL);
		drawCursor(last_x,last_y);
	}
	if( (char)c== 'r'){
		sprintf(filename, "%c%d%d.pbm", t, i, j);
		strcpy(fullpath, path);
		strcat(fullpath, filename);
		cvSaveImage(fullpath, imagen, NULL);
		printf("%s\n", fullpath);
		bzero(fullpath, 0);
		j++;
		if(j == 10)
		{
			i++;
			j = 0;
		}
		cvSet(imagen, cvRealScalar(255),NULL);
		drawCursor(last_x,last_y);
	}
    }

    cvDestroyWindow("Demo");

    return 0;
}

#ifdef _EiC
main(1,"mouseEvent.c");
#endif
