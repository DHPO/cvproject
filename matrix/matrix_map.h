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
    Mat domap(const Mat &matrix);
};


template <typename T, int channels = 1>
class MatOperator
{
    protected:
        int start;
        int stride;
    public:
        MatOperator( int start = 0, int stride = 1) : start(start), stride(stride) {};
        /* To override */
        virtual Vec<T, channels> op (Vec<T, channels> d1, Vec<T, channels> d2);
        void setStart(int start);
        void setStride(int stride);
        /* To invoke */
        Mat doOp(const Mat &matrix1, const Mat &matrix2);
};

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
Mat MatMapper<T, channels>::domap(const Mat &matrix)
{
    expect(stride >= 1, "MatMapper - illegal stride");
    expect(matrix.channels() == channels, "MatMapper - channels violate");
    expect(1 << ((matrix.type() % 8) >> 1) == sizeof(T), "MatMapper - type violate");

    int rows = matrix.rows;
    int cols = matrix.cols;

    Mat result = matrix.clone();
    for (int i = 0; i < rows; i++)
    {
        const T *pa = matrix.ptr<T>(i);
        T *pr = result.ptr<T>(i);
        for (int j = start * channels; j < cols * channels; j += stride * channels)
        {
            auto temp = map(Vec<T, channels>(pa + j));
            for (int m = 0; m < channels; m++)
                pr[j + m] = temp[m];
        }
    }

    return result;
}

template <typename T, int channels>
Mat MatOperator<T, channels>::doOp(const Mat &matrix1, const Mat &matrix2)
{
    expect(stride >= 1, "MatOperator - illegal stride");
    expect(matrix1.channels() == channels, "MatOperator - channels violate");
    expect(matrix2.channels() == channels, "MatOperator - channels violate");
    expect(1 << ((matrix1.type() % 8) >> 1) == sizeof(T), "MatOperator - type violate");
    expect(matrix1.type() == matrix2.type(), "MatOperator - type violate");
    expect(matrix1.rows == matrix2.rows && matrix1.cols == matrix2.cols, "MatOperator - size not matches");

    int rows = matrix1.rows;
    int cols = matrix1.cols;

    Mat result(rows, cols, matrix1.type());

    for (int i = 0; i < rows; i++)
    {
        const T *pa = matrix1.ptr<T>(i);
        const T *pb = matrix2.ptr<T>(i);
        T *pr = result.ptr<T>(i);
        for (int j = start * channels; j < cols * channels; j += stride * channels)
        {
            auto data1 = Vec<T, channels>(pa + j);
            auto data2 = Vec<T, channels>(pb + j);
            auto temp = op(data1, data2);
            for (int m = 0; m < channels; m++)
                pr[j + m] = temp[m];
        }
    }

    return result;
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
void MatOperator<T, channels>::setStart(int start)
{
    expect(start >= 0, "MatOperator - illegal start");
    this->start = start;
}

template <typename T, int channels>
void MatOperator<T, channels>::setStride(int stride)
{
    expect(stride >= 1, "MatOperator - illegal stride");
    this->stride = stride;
}

template <typename T, int channels>
Vec<T, channels> MatMapper<T, channels>::map(Vec<T, channels> data) { return data; }

template <typename T, int channels>
Vec<T, channels> MatOperator<T, channels>::op(Vec<T, channels> data1, Vec<T, channels> data2) { return data1; }

#endif