#include "./filter/filter_difference.h"
#include "./filter/filter_smooth.h"
#include "./matrix/matrix_map.h"

int main()
{
    Mat img = imread("1.png");
    
    // Mat out = LaplacianFilter(2.0).doFilter(img);
    // Mat out = SobelFilter().doFilter(img);
    Mat out = MeanFilter(20).doFilter(img);

    /*class MinMax: public Summary<float, 1>
    {
        public:
            float min = 0;
            float max = 0;
            void record(Vec<float, 1> data)
            {
                min = min > data[0] ? data[0]: min;
                max = max < data[0] ? data[0]: max;
            }
    };

    MinMax minmax;
    MatReducer<float, 1>().doReduce(out, minmax);


    out.convertTo(out, CV_8UC1, 255.0/(minmax.max - minmax.min), - minmax.min/(minmax.max - minmax.min));
*/

    out.convertTo(out, CV_8UC3);
    imshow("img", out);
    waitKey(0);
    return 0;
}