#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

Mat src; Mat dst;
char window_name1[] = "Unprocessed Image";
char window_name2[] = "Processed Image";

int main( int argc, char** argv )
{
    /// Load the source image
    src = imread( argv[1], 1 );
    if (!src.data)
        return -1;

    namedWindow( window_name1, CV_WINDOW_AUTOSIZE);
    imshow("Unprocessed Image", src);

    // 高斯模糊处理 Apply GaussianBlur algorithm
    
    dst = src.clone();
    GaussianBlur( src, dst, Size( 19, 19 ), 0, 0 );
    namedWindow( window_name2, CV_WINDOW_AUTOSIZE);
    imshow("Processed Image", dst); 

    // Apply Canny algorithm  
    // Mat contours; 
    // Canny(dst, contours, 125, 350); 
    // Mat contoursInv;
    // threshold(contours, contoursInv, 128, 255, THRESH_BINARY_INV);  
    // namedWindow(window_name2, WINDOW_AUTOSIZE); 
    // imshow("Processed Image", contoursInv); 

    waitKey();
    return 0;
}

