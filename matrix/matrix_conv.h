#ifndef __MATRIX_CONVOLUTION__
#define __MATRIX_CONVOLUTION__

#include <opencv2/opencv.hpp>
using namespace cv;

Mat conv(const Mat &matrix, const Mat &kernel, int stride = 1);

enum PaddingMethod {NONE, SAME, MIRROR};
Mat padding(const Mat &matrix, int paddingSize, PaddingMethod method);
#endif