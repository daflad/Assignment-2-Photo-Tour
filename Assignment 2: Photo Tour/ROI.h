//
//  ROI.h
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#ifndef __Assignment_2__Photo_Tour__ROI__
#define __Assignment_2__Photo_Tour__ROI__

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class ROI {
    
public:
        
    Mat sourceImage;
    Mat image;
    
    int x1, x2, y1, y2;
    
    bool roi_captured, first;
    
    void init();
    
    void getROI(string sourceImage);
    
    void getROI(string sourceImage, int coords[4]);
    
    void mouseActions(int x, int y);
    
};

#endif /* defined(__Assignment_2__Photo_Tour__ROI__) */
