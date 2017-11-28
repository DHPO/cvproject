#include "testlib.cpp"
#include "../matrix/matrix_math.h"
#include "../matrix/matrix_map.h"

TEST_CASE("Test math operation of matrix", "[matrix]")
{
    SECTION("test add")
    {
        Mat m1(3, 3, CV_8UC1, Scalar(1));
        Mat m2(3, 3, CV_8UC1, Scalar(1));
        Mat m3(4, 4, CV_8UC1, Scalar(1));
        Mat m4(3, 3, CV_8UC3, Scalar(1));
        CHECK(matEqual(add(m1, m2), Mat(3, 3, CV_8UC1, Scalar(2))));
        CHECK_FALSE(matEqual(add(m1, m2), Mat(4, 4, CV_8UC1, Scalar(2))));
        CHECK_THROWS(add(m1, m3));
        CHECK_THROWS(add(m1, m4));
    }

    SECTION("test sub")
    {
        Mat m1(3, 3, CV_8UC1, Scalar(1));
        Mat m2(3, 3, CV_8UC1, Scalar(1));
        Mat m3(4, 4, CV_8UC1, Scalar(1));
        Mat m4(3, 3, CV_8UC3, Scalar(1));
        CHECK(matEqual(sub(m1, m2), Mat::zeros(3, 3, CV_8UC1)));
        CHECK_THROWS(sub(m1, m3));
        CHECK_THROWS(sub(m1, m4));
    }

    SECTION("test negative")
    {
        Mat m1(3, 3, CV_8SC1, Scalar(1));
        Mat m2(3, 3, CV_8SC1, Scalar(-1));
        CHECK(matEqual(negative(m1), m2));
    }

    SECTION("test multiply")
    {
        Mat m1(3, 3, CV_8UC1, Scalar(1));
        Mat m2(3, 3, CV_8UC1, Scalar(-1));
        CHECK(matEqual(
            multiply(2, m1), Mat(3, 3, CV_8UC1, Scalar(2))));
        CHECK(matEqual(
            multiply(0, m1), Mat::zeros(3, 3, CV_8UC1)));
    }

    SECTION("test matMul - simple case")
    {
        Mat m1 = Mat::eye(3, 3, CV_8UC1);
        uchar data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        Mat m2(3, 3, CV_8UC1, data);

        CHECK(matEqual(
            matMul(m1, m2), m2));
        CHECK(matEqual(
            m2, matMul(m1, m2)));
    }

    SECTION("test matMul - different size")
    {
        Mat m1 = Mat::eye(3, 3, CV_8UC1);
        uchar data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        Mat m2(3, 4, CV_8UC1, data);

        CHECK(matEqual(
            matMul(m1, m2), m2));
        CHECK(matEqual(
            m2, matMul(m1, m2)));
    }

    SECTION("test matMul - different order")
    {
        uchar data1[] = {1, 2, 3, 4, 5, 6};
        uchar data2[] = {1, 2, 3, 4, 5, 6};
        uchar data3[] = {22, 28, 49, 64};
        uchar data4[] = {9, 12, 15, 19, 26, 33, 29, 40, 51};
        Mat m1(2, 3, CV_8UC1, data1);
        Mat m2(3, 2, CV_8UC1, data2);
        Mat m3(2, 2, CV_8UC1, data3);
        Mat m4(3, 3, CV_8UC1, data4);
        CHECK(matEqual(
            matMul(m1, m2), m3
        ));
        CHECK(matEqual(
            matMul(m2, m1), m4
        ));
    }
}

uchar map(uchar data){ return (data >> 2) << 2; }
TEST_CASE("test map operation of matrix", "[matrix]")
{
    SECTION("test matmap - vector")
    {
        std::vector<uchar> map(256);
        uchar data[] = {2, 17, 251, 0};
        uchar result[] = {0, 0, 0, 0};
        Mat m1(2, 2, CV_8UC1, data);
        Mat m2(2, 2, CV_8UC1, result);
        Mat m3(2, 2, CV_16UC1, data);
        CHECK(matEqual(
            matMap(m1, map), m2
        ));
        CHECK_THROWS(matMap(m3, map));
    }

    SECTION("test matmap - function")
    {
        uchar data[] = {2, 17, 251, 0};
        uchar result[] = {0, 16, 248, 0};
        Mat m1(2, 2, CV_8UC1, data);
        Mat m2(2, 2, CV_8UC1, result);
        Mat m3(2, 2, CV_16UC1, data);
        CHECK(matEqual(
            matMap(m1, map), m2
        ));
        CHECK_THROWS(matMap(m3, map));
    }

    SECTION("test matmap - class")
    {
        class BinaryMap: public MatMapper {
            public:
                void map(uchar &data){data = data < 128 ? 0: 255;}
        };
        BinaryMap map;
        uchar data1[] = {2, 17, 251, 233};
        uchar result1[] = {0, 0, 255, 255};
        uchar data2[] = {2, 17, 251, 233};
        uchar result2[] = {2, 0, 251, 255};
        Mat m1(2, 2, CV_8UC1, data1);
        Mat m2(2, 2, CV_8UC1, result1);
        Mat m3(2, 2, CV_8UC1, data2);
        Mat m4(2, 2, CV_8UC1, result2);
        CHECK(matEqual(
            map.domap(m1), m2
        ));
        /* test start and stride */
        map.setStart(1);
        map.setStride(2);
        CHECK(matEqual(
            map.domap(m3), m4
        ));
    }
}