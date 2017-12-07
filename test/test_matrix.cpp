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
            matMul(m1, m2), m3));
        CHECK(matEqual(
            matMul(m2, m1), m4));
    }
}

uchar map(uchar data) { return (data >> 2) << 2; }

template <typename T>
class BinaryMap : public MatMapper<T, 1, T, 1>
{
  public:
    Vec<T, 1> map(Vec<T, 1> data)
    {
        data[0] = data[0] < 128 ? 0 : 255;
        return data;
    }
};

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
            matMap(m1, map), m2));
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
            matMap(m1, map), m2));
        CHECK_THROWS(matMap(m3, map));
    }

    SECTION("test matmap - class")
    {
        BinaryMap<uchar> map1;
        BinaryMap<float> map2;
        uchar data1[] = {2, 17, 251, 233};
        uchar result1[] = {0, 0, 255, 255};
        uchar data2[] = {2, 17, 251, 233};
        uchar result2[] = {2, 0, 251, 255};
        float data3[] = {2, 17, 251, 233};
        float result3[] = {0, 0, 255, 255};
        float data4[] = {2, 17, 251, 233};
        float result4[] = {2, 0, 251, 255};
        Mat m1(2, 2, CV_8UC1, data1);
        Mat m2(2, 2, CV_8UC1, result1);
        Mat m3(2, 2, CV_8UC1, data2);
        Mat m4(2, 2, CV_8UC1, result2);
        Mat m5(2, 2, CV_32FC1, data3);
        Mat m6(2, 2, CV_32FC1, result3);
        Mat m7(2, 2, CV_32FC1, data4);
        Mat m8(2, 2, CV_32FC1, result4);
        CHECK(matEqual(
            map1.doMap(m1), m2));
        CHECK_THROWS(matEqual(
            map2.doMap(m1), m2));
        CHECK(matEqual(
            map2.doMap(m5), m6));
        /* test start and stride */
        /* discard */
        /*map1.setStart(1);
        map1.setStride(2);
        map2.setStart(1);
        map2.setStride(2);
        CHECK(matEqual(
            map1.doMap(m3), m4));
        CHECK(matEqual(
            map2.doMap(m7), m8));*/
    }
}

template <typename T, int c>
class MatAdd : public MatOperator<T, c>
{
    Vec<T, c> op(Vec<T, c> d1, Vec<T, c> d2) { return d1 + d2; }
};
TEST_CASE("test matoperator", "[matrix]")
{
    SECTION("test simple function")
    {
        MatAdd<uchar, 1> add;
        Mat m1(3, 3, CV_8UC1, Scalar(1));
        Mat m2(3, 3, CV_8UC1, Scalar(1));
        CHECK(matEqual(add.doOp(m1, m2), Mat(3, 3, CV_8UC1, Scalar(2))));
    }
    SECTION("test different channels")
    {
        MatAdd<uchar, 3> add;
        Mat m1(3, 3, CV_8UC3, Scalar(1, 2, 3));
        Mat m2(3, 3, CV_8UC3, Scalar(1, 2, 3));
        CHECK(matEqual(add.doOp(m1, m2), Mat(3, 3, CV_8UC3, Scalar(2, 4, 6))));
    }
    SECTION("test different data type")
    {
        MatAdd<float, 1> add;
        Mat m1(3, 3, CV_32FC1, Scalar(1.0));
        Mat m2(3, 3, CV_32FC1, Scalar(1.0));
        CHECK(matEqual(add.doOp(m1, m2), Mat(3, 3, CV_32FC1, Scalar(2.0))));
    }
    SECTION("test error case")
    {
        MatAdd<uchar, 1> add;
        /* different size */
        Mat m1(3, 3, CV_8UC1, Scalar(1));
        Mat m2(3, 2, CV_8UC1, Scalar(1));
        CHECK_THROWS(add.doOp(m1, m2));

        /* different type */
        Mat m3(3, 3, CV_32FC1, Scalar(1.0));
        Mat m4(3, 3, CV_8UC1, Scalar(1));
        CHECK_THROWS(add.doOp(m3, m4));

        /* different channel */
        Mat m5(3, 3, CV_8UC3, Scalar(1, 2, 3));
        Mat m6(3, 3, CV_8UC1, Scalar(1));
        CHECK_THROWS(add.doOp(m5, m6));
    }
}

template<typename T, int channels = 1>
class Sum: public Summary<T, channels>
{
    public:
        Vec<T, channels> sum;
        void record(Vec<T, channels> data){sum += data;}
};

TEST_CASE("test MatReducer", "[matrix]")
{
    SECTION("test simple function")
    {
        Sum<uchar, 1> sum;
        Mat m1(3, 3, CV_8UC1, Scalar(1));
        MatReducer<uchar, 1>().doReduce(m1, sum);
        CHECK(sum.sum[0] == 9);
    }
    SECTION("test different channels")
    {
        Sum<uchar, 3> sum;
        Mat m(3, 3, CV_8UC3, Scalar(1, 2, 3));
        MatReducer<uchar, 3>().doReduce(m, sum);
        CHECK(sum.sum[0] == 9);
        CHECK(sum.sum[1] == 18);
        CHECK(sum.sum[2] == 27);
    }
    SECTION("test different types")
    {
        Sum<float, 1> sum;
        Mat m1(3, 3, CV_32FC1, Scalar(1));
        MatReducer<float, 1>().doReduce(m1, sum);
        CHECK(sum.sum[0] == 9);
    }
    SECTION("test error case")
    {
        Sum<uchar, 1> sum;
        /* different type */
        Mat m1(3, 3, CV_32FC1, Scalar(1));
        CHECK_THROWS(MatReducer<uchar, 1>().doReduce(m1, sum));

        /* different channels */
        Mat m2(3, 3, CV_8UC3, Scalar(1, 2, 3));
        CHECK_THROWS(MatReducer<uchar, 1>().doReduce(m2, sum));
    }
}