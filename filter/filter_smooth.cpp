#include "./filter_smooth.h"
#include "./filter_basic.h"
#include "../color/color_gray_rgb.h"

GaussianFilter::GaussianFilter(float sigma)
{
    int size = int(sigma * 3) * 2 + 1;
    int halfsize = (size + 1) / 2;

    float *data = new float[size * size];

    float sigma2 = sigma * sigma;
    float one_divide_two_sigma_square = 0.5 / sigma2;
    float coefficient = -1.0 / (2 * 3.14159 * sigma2);

    for (int i = 0; i < halfsize; i++)
    {
        for (int j = 0; j < halfsize; j++)
        {
            /* calculate */
            float x2y2 = i * i + j * j;
            float x2y2_2sigma2 = x2y2 * one_divide_two_sigma_square;
            float value = coefficient * exp(-x2y2_2sigma2);
            /* assign */
            data[(halfsize - 1 + i) * size + (halfsize - 1 + j)] = value;
            data[(halfsize - 1 - i) * size + (halfsize - 1 + j)] = value;
            data[(halfsize - 1 + i) * size + (halfsize - 1 - j)] = value;
            data[(halfsize - 1 - i) * size + (halfsize - 1 - j)] = value;
            data[(halfsize - 1 + j) * size + (halfsize - 1 + i)] = value;
            data[(halfsize - 1 + j) * size + (halfsize - 1 - i)] = value;
            data[(halfsize - 1 - j) * size + (halfsize - 1 + i)] = value;
            data[(halfsize - 1 - j) * size + (halfsize - 1 - i)] = value;
        }
    }

    Mat temp = Mat(size, size, CV_32FC1, data);
    this->kernel = kernelUniform(temp).clone();
    delete[] data;
}

Mat GaussianFilter::doFilter(const Mat &matrix)
{
    Mat kernel = this->kernel;
    if (matrix.channels() == 3)
        kernel = GrayToRGB<float>().domap(kernel);

    return conv(matrix, kernel, MIRROR);
}

MeanFilter::MeanFilter(int size)
{
    this->kernel = Mat(size, size, CV_32FC1, Scalar(1.0 / (size * size))).clone();
}

Mat MeanFilter::doFilter(const Mat &matrix)
{
    Mat kernel = this->kernel;
    if (matrix.channels() == 3)
        kernel = GrayToRGB<float>().domap(kernel);

    return conv(matrix, kernel, MIRROR);
}