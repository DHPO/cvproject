#include "testlib.cpp"
#include "../Matrix/matrix_math.h"

TEST_CASE("Test math operation of matrix", "[matrix]") {
    SECTION("test add") {
        Mat m1(3, 3, CV_8UC1, Scalar(1));
        Mat m2(3, 3, CV_8UC1, Scalar(1));
        Mat m3(4, 4, CV_8UC1, Scalar(1));
        Mat m4(3, 3, CV_8UC3, Scalar(1));
        CHECK(matEqual(add(m1, m2), Mat(3, 3, CV_8UC1, Scalar(2))));
        CHECK_FALSE(matEqual(add(m1, m2), Mat(4, 4, CV_8UC1, Scalar(2))));
        CHECK_THROWS(add(m1, m3));
        CHECK_THROWS(add(m1, m4));
    }
    SECTION("test sub") {
        Mat m1(3, 3, CV_8UC1, Scalar(1));
        Mat m2(3, 3, CV_8UC1, Scalar(1));
        Mat m3(4, 4, CV_8UC1, Scalar(1));
        Mat m4(3, 3, CV_8UC3, Scalar(1));
        CHECK(matEqual(sub(m1, m2), Mat::zeros(3, 3, CV_8UC1)));
        CHECK_THROWS(sub(m1, m3));
        CHECK_THROWS(sub(m1, m4));
    }
    SECTION("test negative") {
        Mat m1(3, 3, CV_8SC1, Scalar(1));
        Mat m2(3, 3, CV_8SC1, Scalar(-1));
        CHECK(matEqual(negative(m1), m2));
    }
    SECTION("test multiply") {
        Mat m1(3, 3, CV_8UC1, Scalar(1));
        Mat m2(3, 3, CV_8UC1, Scalar(-1));
        CHECK(matEqual(multiply( 2, m1), Mat(3, 3, CV_8UC1, Scalar(2))));
        CHECK(matEqual(multiply( 0, m1), Mat::zeros(3, 3, CV_8UC1)));
    }
}