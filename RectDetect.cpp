/**
Find Rectangular (and circle) in image
*/
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

Mat src; Mat dst;
Mat src_gray;
int thresh = 100;
int targetRect = 0;
int max_thresh = 255;
long min_area=1000;
RNG rng(12345);

void thresh_callback(int, void* )
{
  Mat threshold_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using Threshold
  threshold( src_gray, threshold_output, thresh, max_thresh, THRESH_BINARY );
  /// Find contours
  findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Approximate contours to polygons + get bounding rects and circles
  vector<vector<Point> > contours_poly( contours.size() );
  vector<Rect> boundRect( contours.size() );
  // vector<Point2f>center( contours.size() );
  // vector<float>radius( contours.size() );

  for( int i = 0; i < contours.size(); i++ )
     { approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
       boundRect[i] = boundingRect( Mat(contours_poly[i]) );
       // minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
     }


  /// Draw polygonal contour + bonding rects + circles
  Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
  {
    // Filter small rectangular
    if (min_area >  boundRect[i].width *  boundRect[i].height)
    {
       // max_area = boundRect[i].width *  boundRect[i].height;
       // targetRect = i;
       continue;
    }
    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    // drawContours( drawing, contours_poly, targetRect, color, 1, 8, vector<Vec4i>(), 0, Point() );
    rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
    // circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
     cout << "Rectangular: "<< i + 1 << " Point: " << boundRect[i].tl() << "," << boundRect[i].br()  << endl ; 
  }

  /// Show in a window
  namedWindow( "Contours", 1 );
  imshow( "Contours", drawing );
}

int main( int argc, char** argv )
{
    /// Load the source image  and convert it to gray
    src = imread( argv[1], 1 );
    if (!src.data)
        return -1;

    /// Create Window
    namedWindow( "Unprocessed Image", 1);
    imshow("Unprocessed Image", src);

    // 降噪处理 使用 MeanShift  进行图像预处理
    // 不用blur方法，以免降低待检测的边缘强度
    // blur( src_gray, src_gray, Size(10,10));
    pyrMeanShiftFiltering(src, dst, 25, 10);

    /// Convert image to gray to find contours
    cvtColor( dst, src_gray, CV_BGR2GRAY );
  
    namedWindow( "Processed Image", 1);
    imshow("Processed Image", dst);

    createTrackbar( " Threshold:", "Source", &thresh, max_thresh, thresh_callback );
    thresh_callback( 0, 0 );

    waitKey();
    return 0;
}