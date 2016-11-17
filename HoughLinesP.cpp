/**
用OpenCV中的HoughP算法找出图像中的直线
Reference: http://docs.opencv.org/2.4/modules/imgproc/doc/feature_detection.html?highlight=hough#cv2.HoughLinesP
*/
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    Mat src, dst, color_dst;
    if( argc != 2 || !(src=imread(argv[1], 0)).data)
        return -1;

    Canny( src, dst, 50, 200, 3 );
    // 灰度图像转彩色图像
    cvtColor( dst, color_dst, CV_GRAY2BGR );


#if 0
    vector<Vec2f> lines;
    HoughLines( dst, lines, 1, CV_PI/180, 100);

    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0];
        float theta = lines[i][1];
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        Point pt1(cvRound(x0 + 1000*(-b)),
                  cvRound(y0 + 1000*(a)));
        Point pt2(cvRound(x0 - 1000*(-b)),
                  cvRound(y0 - 1000*(a)));
        line( color_dst, pt1, pt2, Scalar(0,0,255), 3, 8 );
    }
#else

    vector<Vec4i> lines;
    // 此处对找出直线的最小长度进行了限制（>150）
    HoughLinesP( dst, lines, 1, CV_PI/180, 80, 150, 10);
    for( size_t i = 0; i < lines.size(); i++ )
    {
        line( color_dst, Point(lines[i][0], lines[i][1]),
            Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
        cout << "line "<< i + 1 << " Start Point: (" << lines[i][0] << "," << lines[i][1]  << ") " ; 
        cout << "End Point: (" << lines[i][2] << "," << lines[i][3]  << ")" << endl;         
    }
#endif
    namedWindow( "Source", 1 );
    imshow( "Source", src );

    namedWindow( "Detected Lines", 1 );
    imshow( "Detected Lines", color_dst );

    waitKey(0);
    return 0;
}