#include "./matrix/matrix_conv.h"
#include "./matrix/matrix_map.h"
#include "./matrix/matrix_math.h"
#include <cmath>

short myabs(short data){return data >= 0 ? data: -data;}

int main()
{
    Mat img = imread("1.png", 0);
    //resize(img, img, Size(400, 225));
    float kernel_data1[] = 
        { 1,  2,  1,
          0,  0,  0,
         -1, -2, -1};
    float kernel_data2[] = 
        { 1, 0, -1,
          2, 0, -2,
          1, 0, -1};
    Mat kernel1(3, 3, CV_32FC1, kernel_data1);
    Mat convimg1 = conv(img, kernel1, MIRROR);
    //matMap(convimg1, myabs);

    Mat kernel2(3, 3, CV_32FC1, kernel_data2);
    Mat convimg2 = conv(img, kernel2, MIRROR);
    //matMap(convimg2, myabs);

    class MatDistance: public MatOperator<short, 1>
    {
        Vec<short, 1> op(Vec<short, 1> d1, Vec<short, 1> d2)
        {
            return Vec<short, 1>(sqrt(d1[0] * d1[0] + d2[0] * d2[0]));
        }
    };

    class ToRed: public MatMapper<uchar, 1, uchar, 3>
    {
        Vec<uchar, 3> map(Vec<uchar, 1> data)
        {
            if (data[0] > 80)
                return Vec<uchar, 3>(0, 0, 255);
            else
                return Vec<uchar, 3>(0, 0, 0);
        }
    };

    class ToRGB: public MatMapper<uchar, 1, uchar, 3>
    {
        Vec<uchar, 3> map(Vec<uchar, 1> data)
        {
            return Vec<uchar, 3>(data[0], data[0], data[0]);
        }
    };

    class MatAdd: public MatOperator<uchar, 3>
    {
        Vec<uchar, 3> op(Vec<uchar, 3> d1, Vec<uchar, 3> d2)
        {
            int sum0 = d1[0] + d2[0];
            int sum1 = d1[1] + d2[1];
            int sum2 = d1[2] + d2[2];
            sum0 = sum0 > 255? 255: sum0;
            sum1 = sum1 > 255? 255: sum1;
            sum2 = sum2 > 255? 255: sum2;
            return Vec<uchar, 3>(sum0, sum1, sum2);
        }
    };


    Mat convimg = MatDistance().doOp(convimg1, convimg2);
    convimg.convertTo(convimg, CV_8UC1, 1);
    Mat out = MatAdd().doOp(ToRGB().domap(img), ToRed().domap(convimg));

    imshow("img", out);
    waitKey(0);
    return 0;
}