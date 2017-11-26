#include "./Matrix/matrix_math.h"

int main()
{
    uchar data1[] = {1, 2, 3, 4, 5, 6};
    uchar data2[] = {1, 2, 3, 4, 5, 6};
    Mat m1(2, 3, CV_8UC1, data1);
    Mat m2(3, 2, CV_8UC1, data2);

    std::cout << m1 << std::endl;
    std::cout << m2 << std::endl;
    std::cout << Mat::eye(3, 3, CV_8UC1) * Mat::eye(3, 3, CV_8UC1) << std:: endl;
    std::cout << m1 * m2 << std::endl;
    std::cout << m2 * m1 << std::endl;

    return 0;
}