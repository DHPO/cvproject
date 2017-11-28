#include "./matrix/matrix_conv.h"

int main()
{
    Mat img = imread("1.png");
    resize(img, img, Size(800, 600));
    Mat pad = padding(img, 200, MIRROR);
    imshow("img", pad);
    waitKey(0);
    return 0;
}