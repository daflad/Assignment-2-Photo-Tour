//
//  main.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, const char * argv[])
{
    
    int h = 10;
    int l = 20;
    int t = 40;
    
    // Window & trackbars
    namedWindow("blended", CV_WINDOW_AUTOSIZE);
    createTrackbar("Canny High", "blended", &h, 199);
    createTrackbar("Canny Low", "blended", &l, 199);
    createTrackbar("Detection Threshold", "blended", &t, 99);

    waitKey();
    // insert code here...
    std::cout << "Photo allignment!!\n";
    return 0;
}

