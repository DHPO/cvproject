#include "matrix_math.h"
#include "../expect/expect.h"

bool matEqual(const cv::Mat& a, const cv::Mat& b) {
    if (a.rows != b.rows || a.cols != b.cols || a.channels() != b.channels() || a.type() != b.type())
        return false;
    cv::Mat temp;
    cv::bitwise_xor(a,b,temp);   
    return !(cv::countNonZero(temp) );
}

Mat add(Mat& matrix1, Mat& matrix2)
{
    expect(matrix1.rows == matrix2.rows, "Add - #rows not equal");
    expect(matrix1.cols == matrix2.cols, "Add - #cols not equal");
    expect(matrix1.channels() == matrix2.channels(), "Add - #channels not equal");

    int channels = matrix1.channels();
    int rows = matrix1.rows;
    int cols = matrix1.cols; 
    int datasize = 1 << ((matrix1.type() % 8) >> 1);

    Mat result(rows, cols, matrix1.type());

    for (int i = 0; i < rows; i++) {
        auto *pa = matrix1.ptr(i);
        auto *pb = matrix2.ptr(i);
        auto *pr = result.ptr(i);
        for (int j = 0; j < cols * channels * datasize; j++) {
            pr[j] = pa[j] + pb[j];
        }
    }

    return result;
}

Mat sub(Mat& matrix1, Mat& matrix2)
{
    expect(matrix1.rows == matrix2.rows, "Sub - #rows not equal");
    expect(matrix1.cols == matrix2.cols, "Sub - #rows not equal");
    expect(matrix1.channels() == matrix2.channels(), "Sub - #channels not equal");
    
    int channels = matrix1.channels();
    int rows = matrix1.rows;
    int cols = matrix1.cols;
    int datasize = 1 << ((matrix1.type() % 8) >> 1);
    Mat result(rows, cols, matrix1.type());

    for (int i = 0; i < rows; i++) {
        auto *pa = matrix1.ptr(i);
        auto *pb = matrix2.ptr(i);
        for (int j = 0; j < cols * channels * datasize; j++) {
            pa[j] -= pb[j]; 
        }
    }

    return matrix1;
}

Mat negative(Mat& matrix)
{
    int channels = matrix.channels();
    int rows = matrix.rows;
    int cols = matrix.cols * channels;

    for (int i = 0; i < rows; i++) {
        auto *pa = matrix.ptr(i);
        for (int j = 0; j < cols; j++) {
            pa[j] = -pa[j];
        }
    }

    return matrix;
}


Mat multiply(const double coefficient, Mat& matrix)
{
    int channels = matrix.channels();
    int rows = matrix.rows;
    int cols = matrix.cols * channels;

    for (int i = 0; i < rows; i++) {
        auto *pa = matrix.ptr(i);
        for (int j = 0; j < cols; j++) {
            pa[j] *= coefficient;
        }
    }

    return matrix;
}


Mat matMul(Mat& left, Mat& right)
{
    int leftRow = left.rows;
    int leftCol = left.cols * left.channels();
    int rightRow = right.rows;
    int rightCol = right.cols * right.channels();

    expect(leftCol == rightRow, "MatMul - size mismatch");

    Mat result(leftRow, rightCol, left.type(), Scalar(0));

    for (int i = 0; i < leftRow; i++) {
        for (int j = 0; j < rightCol; j++) {
            double temp = 0;
            for (int k = 0; k < leftCol; k++) {
                /* optimize */
                temp += left.ptr(i)[k] * right.ptr(k)[j];
            }
            result.ptr(i)[j] = temp;
        }
    }

    return result;
}