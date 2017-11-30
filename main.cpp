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
    matMap(convimg1, myabs);

    Mat kernel2(3, 3, CV_32FC1, kernel_data2);
    Mat convimg2 = conv(img, kernel2, MIRROR);
    matMap(convimg2, myabs);

    class MatDistance: public MatOperator<short, 1>
    {
        Vec<short, 1> op(Vec<short, 1> d1, Vec<short, 1> d2)
        {
            return Vec<short, 1>(sqrt(d1[0] * d1[0] + d2[0] * d2[0]));
        }
    };
    MatDistance distance;

    class MatAdd: public MatOperator<uchar, 1>
    {
        Vec<uchar, 1> op(Vec<uchar, 1> d1, Vec<uchar, 1> d2)
        {
            int sum = d1[0] + d2[0];
            return Vec<uchar, 1>(sum>255?255:sum);
        }
    };
    MatAdd add;

    Mat convimg = distance.doOp(convimg1, convimg2);
    convimg.convertTo(convimg, CV_8UC1, 0.25);

    Mat out = add.doOp(img, convimg);

    imshow("img", out);
    waitKey(0);
    return 0;
}