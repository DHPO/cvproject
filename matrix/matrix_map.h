#ifndef __MATRIX_MAP__
#define __MATRIX_MAP__

#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;

template <typename T>
Mat &matMap(Mat &matrix, std::vector<T> map);

template <typename T>
Mat &matMap(Mat &matrix, T (*map)(T));

/**
 * MatMapper - map element to another through matrix
 * override map method to apply your own one
 */
template <typename T, int channels = 1>
class MatMapper
{
  protected:
    int start;
    int stride;

  public:
    MatMapper(int start = 0, int stride = 1) : start(start), stride(stride){};
    /* To override */
    virtual Vec<T, channels> map(Vec<T, channels>);
    void setStart(int start);
    void setStride(int stride);
    /* To invoke */
    Mat &domap(Mat &matrix);
};

/*
template <typename T, int channels = 1>
class MatOperator
{
    protected:
        int start;
        int stride;
    public:
        MatOperator( int start = 0, int stride = 1) : start(start), stride(stride) {};
        virtual Vec<T, channels> map (Vec<T, channels> d1, Vec<T, channels> d2);
        Mat domap(const Mat &matrix1, const Mat &matrix2);
};*/

    /***************************************/

#include "../expect/expect.h"

template <typename T>
Mat &matMap(Mat &matrix, std::vector<T> map)
{
    /* expect datatype of matrix is 1 byte */
    expect((matrix.type() % 8 <= 1), "MatMap - matrix datatype violate");
    expect(sizeof(T) == 1, "MatMap - datatype violate");
    expect(map.size() == 256, "MatMap - map size is not 256");

    /* go through matrix */
    int channels = matrix.channels();
    int rows = matrix.rows;
    int cols = matrix.cols * channels;

    for (int i = 0; i < rows; i++)
    {
        T *pa = matrix.ptr<T>(i);
        for (int j = 0; j < cols; j++)
        {
            pa[j] = map[pa[j]];
        }
    }

    return matrix;
}

template <typename T>
Mat &matMap(Mat &matrix, T (*map)(T))
{
    /* check whether datatype of matrix matches map */
    int datasize = 1 << ((matrix.type() % 8) >> 1);
    expect(sizeof(T) == datasize, "MatMap - datatype violate");

    /* go through matrix */
    int channels = matrix.channels();
    int rows = matrix.rows;
    int cols = matrix.cols * channels;

    for (int i = 0; i < rows; i++)
    {
        T *pa = matrix.ptr<T>(i);
        for (int j = 0; j < cols; j++)
        {
            pa[j] = map(pa[j]);
        }
    }

    return matrix;
}

template <typename T, int channels>
Mat &MatMapper<T, channels>::domap(Mat &matrix)
{
    expect(stride >= 1, "MatMapper - illegal stride");
    expect(matrix.channels() == channels, "MatMapper - channels violate");
    expect(1 << ((matrix.type() % 8) >> 1) == sizeof(T), "MatMapper - type violate");

    int rows = matrix.rows;
    int cols = matrix.cols;

    for (int i = 0; i < rows; i++)
    {
        T *pa = matrix.ptr<T>(i);
        for (int j = start; j < cols * channels; j += stride)
        {
            auto temp = map(Vec<T, channels>(pa));
            for (int m = 0; m < channels; m++)
                pa[j + m] = temp[m];
        }
    }

    return matrix;
}

template <typename T, int channels>
void MatMapper<T, channels>::setStart(int start)
{
    expect(start >= 0, "MatMapper - illegal start");
    this->start = start;
}

template <typename T, int channels>
void MatMapper<T, channels>::setStride(int stride)
{
    expect(stride >= 1, "MatMapper - illegal stride");
    this->stride = stride;
}

template <typename T, int channels>
Vec<T, channels> MatMapper<T, channels>::map(Vec<T, channels> data) { return data; }

#endif