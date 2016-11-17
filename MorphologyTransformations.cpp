/**
References:
http://docs.opencv.org/2.4/doc/tutorials/imgproc/opening_closing_hats/opening_closing_hats.html
*/
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

Mat src, dst, src_gray;

int morph_elem = 0;
int morph_size = 0;
int morph_operator = 0;
int const max_operator = 4;
int const max_elem = 2;
int const max_kernel_size = 21;

/** Function Headers */
void Morphology_Operations( int, void* );

int main( int argc, char** argv )
{
    /// Load the source image  and convert it to gray
    src = imread( argv[1], 1 );
    if (!src.data)
        return -1;
    cvtColor( src, src_gray, CV_RGB2GRAY );

     /// Create Window
    namedWindow( "Unprocessed Image", 1);
    imshow("Unprocessed Image", src_gray);

    /// Create window
   namedWindow( "Select", 1 );

   /// Create Trackbar to select Morphology operation
   createTrackbar("Operator:\n 0: Opening - 1: Closing \n 2: Gradient - 3: Top Hat \n 4: Black Hat", "Select", &morph_operator, max_operator, Morphology_Operations );

   /// Create Trackbar to select kernel type
   createTrackbar( "Element:\n 0: Rect - 1: Cross - 2: Ellipse", "Select",
                 &morph_elem, max_elem,
                 Morphology_Operations );

   /// Create Trackbar to choose kernel size
   createTrackbar( "Kernel size:\n 2n +1", "Select",
                 &morph_size, max_kernel_size,
                 Morphology_Operations );

   /// Default start
   Morphology_Operations( 0, 0 );

   waitKey(0);
   return 0;

}

 /**
  * @function Morphology_Operations
  */
void Morphology_Operations( int, void* )
{
    // Since MORPH_X : 2,3,4,5 and 6
    int operation = morph_operator + 2;

    Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );

    /// Apply the specified morphology operation
    morphologyEx( src_gray, dst, operation, element );
    imshow( "Select", dst );
  }