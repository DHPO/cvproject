#include "testlib.cpp"
#include "../filter/filter_basic.h"
#include "../matrix/matrix_math.h"

TEST_CASE("test uniform", "[filter]")
{
    float data[9] = 
        { 1, 1, 1,
          1, 2, 1,
          1, 1, 1};
    float result[9] = 
        { 0.1, 0.1, 0.1,
          0.1, 0.2, 0.1,
          0.1, 0.1, 0.1};
    Mat m1(3, 3, CV_32FC1, data);
    Mat m2(3, 3, CV_32FC1, result);
    CHECK(matEqual(
        kernelUniform(m1), m2
    ));
}