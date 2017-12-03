#ifndef __FILTER_DIFFERENCE__
#define __FILTER_DIFFERENCE__

#include "../matrix/matrix_conv.h"
#include "../matrix/matrix_map.h"

Mat sobelFilter_gray(const Mat &img, int threshold = 0);

#endif