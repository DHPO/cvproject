#include "./filter/filter_difference.h"

int main()
{
    Mat img = imread("1.png", 0);
    
    Mat out = sobelFilter_gray(img, 256);

    imshow("img", out);
    waitKey(0);
    return 0;
}