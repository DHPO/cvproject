#ifndef __MATRIX_MAP__
#define __MATRIX_MAP__

#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;

template<typename T>
Mat& matMap(Mat &matrix, std::vector<T> map);

template<typename T>
Mat& matMap(Mat &matrix, T(*map)(T));

class MatMapper{
    protected:
        int start;
        int stride;
    public:
        MatMapper(int start = 0, int stride = 1):start(start),stride(stride){};
        /* To override */
        virtual void map(uchar&);
        /* To override */
        virtual void map(uchar&, uchar&, uchar&);
        /* To override */
        virtual void map(uchar&, uchar&, uchar&, uchar&);
        void setStart(int start);
        void setStride(int stride);
        /* To invoke */
        Mat& domap(Mat &matrix);
};

/***************************************/

#include "../expect/expect.h"

template<typename T>
Mat& matMap(Mat &matrix, std::vector<T> map)
{
    /* expect datatype of matrix is 1 byte */
    expect((matrix.type() % 8 <= 1), "MatMap - matrix datatype violate");
    expect(sizeof(T) == 1, "MatMap - datatype violate");
    expect(map.size() == 256, "MatMap - map size is not 256");

    /* go through matrix */
    int channels = matrix.channels();
    int rows = matrix.rows;
    int cols = matrix.cols * channels;

    for (int i = 0; i < rows; i++) {
        T *pa = matrix.ptr<T>(i);
        for (int j = 0; j < cols; j++) {
            pa[j] = map[pa[j]];
        }
    }

    return matrix;
}

template<typename T>
Mat& matMap(Mat &matrix, T(*map)(T))
{
    /* check whether datatype of matrix matches map */
    int datasize = 1 << ((matrix.type() % 8) >> 1);
    expect(sizeof(T) == datasize, "MatMap - datatype violate");

    /* go through matrix */
    int channels = matrix.channels();
    int rows = matrix.rows;
    int cols = matrix.cols * channels;

    for (int i = 0; i < rows; i++) {
        T *pa = matrix.ptr<T>(i);
        for (int j = 0; j < cols; j++) {
            pa[j] = map(pa[j]);
        }
    }

    return matrix;
}

Mat& MatMapper::domap(Mat &matrix)
{
    expect(stride >= 1, "MatMapper - illegal stride");
    expect(matrix.type() % 8 <= 1, "MatMapper - unsupported data type");

    int channels = matrix.channels();
    int rows = matrix.rows;
    int cols = matrix.cols * channels;

    for (int i = 0; i < rows; i++) {
        uchar *pa = matrix.ptr<uchar>(i);
        for (int j = start; j < cols; j+=stride*channels) {
            if (channels == 1)
                map(pa[j]);
            else if (channels == 3)
                map(pa[j], pa[j+1], pa[j+2]);
            else if (channels == 4)
                map(pa[j], pa[j+1], pa[j+2], pa[j+3]);
        }
    }

    return matrix;
}

void MatMapper::setStart(int start)
{
    expect(start >= 0, "MatMapper - illegal start");
    this->start = start;
}

void MatMapper::setStride(int stride)
{
    expect(stride >= 1, "MatMapper - illegal stride");
    this->stride = stride;
}

void MatMapper::map(uchar& data) { }

void MatMapper::map(uchar& data1, uchar& data2, uchar& data3) { }

void MatMapper::map(uchar& data1, uchar& data2, uchar& data3, uchar& data4) { }

#endif