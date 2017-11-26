#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;

template<typename T>
Mat& matMap(Mat &matrix, std::vector<T> map);

template<typename T>
Mat& matMap(Mat &matrix, T(*map)(T));

/************************/
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