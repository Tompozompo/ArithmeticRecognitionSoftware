#include "cv.h"

#ifndef KNEAREST_H_
#define KNEAREST_H_

#ifdef __cplusplus
#include <ml.h>
extern CvKNearest *knn;
extern "C" {
#endif

int train_cvknearest(CvMat* data, CvMat* responses);
int find_closest(IplImage* resized_input, int image_size);

#ifdef __cplusplus
}
#endif

#endif
