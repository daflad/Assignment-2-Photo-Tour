//
//  ROI.h
//  Assignment 2: Photo Tour
//
//
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
    
    // The image to extract an ROI from
    Mat     sourceImage;
    
    // The extacted ROI in question
    Mat     image;
    
    // x1, y1 - upper left corner
    // x2, y2 - lower right corner
    int     x1, y1,
            x2, y2;
    
    // Flow of control bools
    bool    roi_captured, first;
    
    // init
    void    init();

    // select an ROI with the cursor
    void    getROI(string sourceImage);
    
    // slecet an ROI based on predetermined coordinates
    void    getROI(string sourceImage, int coords[4]);
    
    // what to do if mouse events ocour
    void    mouseActions(int x, int y);
    
};

#endif /* defined(__Assignment_2__Photo_Tour__ROI__) */
