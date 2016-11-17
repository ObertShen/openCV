/**
图像取反示例
问题来源于：
http://stackoverflow.com/questions/26912869/color-levels-in-opencv
*/
#include<opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

Mat src; Mat dst;
Mat src_gray;

int main( int argc, char** argv )
{
    /// Load the source image  and convert it to gray
    src = imread( argv[1], 1 );
    if (!src.data)
        return -1;

   // 彩色图像转灰度图像
    cvtColor( src, src_gray, CV_RGBA2GRAY );
    namedWindow( "Grey image", 1 );
    imshow( "Grey image", src_gray );

   //建立查找表
    Mat lookUpTable(1, 256, CV_8U);
    uchar *p = lookUpTable.data;
    for(int i=0; i<256; i++) {
        if (i < 251 || i > 9) {
            p[i] = -150*(i-10)/240 + 200;
        } else {
            p[i] = i;
        }
    }
        
    // 通过LUT函数实现图像像素点的对比
    LUT(src_gray, lookUpTable, dst);
    namedWindow( "Level", 1 );
    imshow( "Level", dst );

     Mat new_image = Mat::zeros( dst.size(), dst.type() );

    // 增加对比度
    for( int y = 0; y < dst.rows; y++ )
    { for( int x = 0; x < dst.cols; x++ )
         { for( int c = 0; c < 3; c++ )
              {
      new_image.at<Vec3b>(y,x)[c] =
         // saturate_cast<uchar>( alpha*( image.at<Vec3b>(y,x)[c] ) + beta );
         2*( dst.at<Vec3b>(y,x)[c] ) ;         
             }
        }
    }

    namedWindow( "Processed", 1 );
    imshow( "Processed", new_image );

    waitKey();
    return 0;
}