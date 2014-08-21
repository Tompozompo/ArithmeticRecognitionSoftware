#include "knearest.h"

CvKNearest *knn;
int train_cvknearest(CvMat* data, CvMat* responses)
{
	knn = new CvKNearest(data, responses, 0, false, 10);
	if(knn == NULL)
		return 1;
	return 0;
}
