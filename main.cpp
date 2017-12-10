#include "./filter/filter_difference.h"
#include "./filter/filter_smooth.h"
#include "./matrix/matrix_map.h"
#include "./color/color_colorspace.h"
#include "./color/color_histogram.h"
#include "./morphology/morphology_basic.h"
#include "./matrix/matrix_math.h"

int main()
{
    Mat img = imread("1.png");
    
    // Mat out = LaplacianFilter(2.0).doFilter(img);
    // Mat out = SobelFilter().doFilter(img);
    // Mat out = MeanFilter(20).doFilter(img);
    // Mat out = GrayToHeat().doMap(img);
    // Mat out = HSVToRGBConverter().doMap(RGBToHSVConverter().doMap(img));

    // Mat out = close(img, Mat(10, 10, CV_16SC1, Scalar(1)));
    Mat out = MediumFilter(10).doFilter(img);
    /*class MatSub: public MatOperator<uchar, 1>
    {
        Vec<uchar, 1> op(Vec<uchar, 1> d1, Vec<uchar, 1> d2)
        {
            return Vec<uchar, 1>(d1[0] - d2[0]);
        }
    };
    Mat out = MatSub().doOp(dilate(img, Mat(5, 5, CV_16SC1, kernel_data)), erode(img, Mat(5, 5, CV_16SC1, kernel_data)));

    class MinMax: public Summary<uchar, 1>
    {
        public:
            uchar min = 255;
            uchar max = 0;
            void record(Vec<uchar, 1> data)
            {
                min = min > data[0] ? data[0]: min;
                max = max < data[0] ? data[0]: max;
            }
    };

    MinMax minmax;
    MatReducer<uchar, 1>().doReduce(out, minmax);


    out.convertTo(out, CV_8UC1, 255.0/(minmax.max - minmax.min), - minmax.min/(minmax.max - minmax.min));
*/

    //out.convertTo(out, CV_8UC3);
    imshow("img", out);
    waitKey(0);
    return 0;
}