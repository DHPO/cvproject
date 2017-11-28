#include "./matrix_conv.h"

Mat padding(const Mat& matrix, int paddingSize, PaddingMethod method)
{
    if (method == NONE)
        return matrix;

    int channels = matrix.channels();
    int originCols = matrix.cols;
    int originRows = matrix.rows;

    Mat result = Mat(originRows + 2 * paddingSize, originCols + 2 * paddingSize, matrix.type());

    /* copy matrix */
    for (int r = 0; r < originRows; r++) {
        const uchar *optr = matrix.ptr<uchar>(r);
        uchar *rptr = result.ptr<uchar>(r + paddingSize);
        for (int c = 0; c < originCols * channels; c++) {
            rptr[c + paddingSize * channels] = optr[c];
        }
        if (method == SAME) {
            for (int c = 0; c < paddingSize; c++) {
                for (int i = 0; i < channels; i++) {
                    rptr[c * channels + i] = rptr[paddingSize * channels + i];
                }
            }
            for (int c = paddingSize + originCols; c < originCols + 2 * paddingSize; c++) {
                for (int i = 0; i < channels; i++) {
                    rptr[c * channels + i] = rptr[(paddingSize + originCols - 1) * channels + i];
                }
            }
        }
        else {
            for (int c = 0; c < paddingSize; c++) {
                for (int i = 0; i < channels; i++) {
                    rptr[c * channels + i] = rptr[(2 * paddingSize - c)* channels + i];
                }
            }
            for (int c = paddingSize + originCols; c < originCols + 2 * paddingSize; c++) {
                for (int i = 0; i < channels; i++) {
                    rptr[c * channels + i] = rptr[(2 * (paddingSize + originCols - 1) - c) * channels + i];
                }
            }
        }
    }

    for (int r = 0; r < paddingSize; r++) {
        const uchar *optr;
        if (method == SAME) 
            optr = result.ptr<uchar>(paddingSize);
        else 
            optr = result.ptr<uchar>(2 * paddingSize - r);
        uchar *rptr = result.ptr<uchar>(r);
        for (int c = 0; c < (originCols + 2 * paddingSize) * channels; c++) {
            rptr[c] = optr[c];
        }
    }

    for (int r = originRows + paddingSize; r < originRows + 2 * paddingSize; r++) {
        const uchar *optr;
        if (method == SAME) 
            optr = result.ptr<uchar>(paddingSize + originRows - 1);
        else 
            optr = result.ptr<uchar>(2 * (paddingSize + originRows - 1) - r);
        uchar *rptr = result.ptr<uchar>(r);
        for (int c = 0; c < (originCols + 2 * paddingSize) * channels; c++) {
            rptr[c] = optr[c];
        }
    }

    return result;
}

Mat conv(const Mat &matrix, const Mat &kernel, int stride)
{
    return matrix;
}