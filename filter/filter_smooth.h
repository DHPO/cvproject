#ifndef __FILTER_SMOOTH__
#define __FILTER_SMOOTH__

#include "../matrix/matrix_map.h"
#include "../matrix/matrix_conv.h"

class GaussianFilter{
    protected:
        Mat kernel;
    public:
        GaussianFilter(float sigma);
        Mat doFilter(const Mat &);
};

class MeanFilter{
    protected:
        Mat kernel;
    public:
        MeanFilter(int size);
        Mat doFilter(const Mat &);
};

#endif