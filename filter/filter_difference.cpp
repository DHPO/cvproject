#include "./filter_difference.h"
#include "../expect/expect.h"

Mat sobelFilter_gray(const Mat &img, int threshold)
{
    float kernel_data1[] =
        {1, 2, 1,
         0, 0, 0,
         -1, -2, -1};
    float kernel_data2[] =
        {1, 0, -1,
         2, 0, -2,
         1, 0, -1};
    Mat kernel1(3, 3, CV_32FC1, kernel_data1);
    Mat convimg1 = conv(img, kernel1, MIRROR);

    Mat kernel2(3, 3, CV_32FC1, kernel_data2);
    Mat convimg2 = conv(img, kernel2, MIRROR);

    class MatDistance : public MatOperator<short, 1>
    {
      protected:
        int threshold;

      public:
        MatDistance(int threshold = 0) : threshold(threshold){};
        Vec<short, 1> op(Vec<short, 1> d1, Vec<short, 1> d2)
        {
            float distance = sqrt(d1[0] * d1[0] + d2[0] * d2[0]);
            return Vec<short, 1>(distance >= threshold ? distance : 0);
        }
    };

    Mat convimg = MatDistance(threshold).doOp(convimg1, convimg2);
    convimg.convertTo(convimg, CV_8UC1, 1);

    return convimg;
}