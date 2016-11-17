/**
用直方图均衡处理图片后用Hough算法找出图像中的直线
问题来源于：
http://stackoverflow.com/questions/26912869/color-levels-in-opencv
*/
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

Mat src; Mat dst;
Mat src_gray; Mat raw_img; Mat src_lab;

int normalization(vector<Vec4i> lines) 
{
   return 0;
}

int main( int argc, char** argv )
{
    /// Load the source image  and convert it to gray
    src = imread( argv[1], 1 );
    if (!src.data)
        return -1;
    cvtColor( src, src_gray, CV_RGB2GRAY );
    // Use Histogram Equalization to preprocess the image
    equalizeHist( src_gray, dst );
    
    /*cvtColor(src, src_lab, CV_BGR2Lab);
    // Extract the L channel
    vector<cv::Mat> lab_planes(3);
    split(src_lab, lab_planes); // now we have the L image in lab_planes[0]
    
    // apply the CLAHE algorithm to the L channel
    Ptr<cv::CLAHE> clahe = createCLAHE();
    clahe->setClipLimit(4);
    Mat labDst;
    clahe->apply(lab_planes[0], labDst);

    // Merge the the color planes back into an Lab image
    labDst.copyTo(lab_planes[0]);
    merge(lab_planes, src_lab);

   // convert back to GRAY
   cvtColor(src_lab, dst, CV_Lab2RGB);
   cvtColor(dst, dst, CV_RGB2GRAY);
   namedWindow( "CLAHE", 1 );
   imshow( "CLAHE", dst );
   */

    // Use GaussianBlur to preprocess the img
    GaussianBlur( dst, dst, Size( 7, 7), 0, 0 );
    namedWindow( "GaussianBlur", 1 );
    imshow( "GaussianBlur", dst );

    Canny( dst, dst, 50, 200, 3 );
    // convert GRAY to BGR
    cvtColor( dst, raw_img, CV_GRAY2BGR );

    vector<Vec4i> lines;
    // 此处对找出直线的最小长度进行了限制（>150）
    HoughLinesP( dst, lines, 1, CV_PI/180, 80, 150, 10);
    double length1 = 0;  double length2 = 0; double angle = 0;
    long multipleResult = 0;
    for( size_t i = 0; i < lines.size(); i++ )
    {   
        // 筛选掉向量夹角较小的相邻直线
        if (i > 0) 
        {
           length1 =  sqrt((lines[i-1][0] - lines[i-1][2])*(lines[i-1][0] - lines[i-1][2]) + (lines[i-1][1] - lines[i-1][3])*(lines[i-1][1] - lines[i-1][3]) );
           length2 =  sqrt((lines[i][0] - lines[i][2])*(lines[i][0] - lines[i][2]) + (lines[i][1] - lines[i][3])*(lines[i][1] - lines[i][3]) );
           multipleResult = (lines[i-1][0] - lines[i-1][2])*(lines[i][0] - lines[i][2]) +  (lines[i-1][1] - lines[i-1][3])*(lines[i][1] - lines[i][3]);
           angle = multipleResult/(length1*length2);
           if (angle > 0.8) {
               continue;
           }
        }
        line( raw_img, Point(lines[i][0], lines[i][1]),
            Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );    
        cout << "line "<< i + 1 << " Start Point: (" << lines[i][0] << "," << lines[i][1]  << ") " ; 
        cout << "End Point: (" << lines[i][2] << "," << lines[i][3]  << ")" << endl;         
    }
    
    namedWindow( "Detected Lines", 1 );
    imshow( "Detected Lines", raw_img );

    waitKey();
    return 0;
}