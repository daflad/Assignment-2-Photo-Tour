//
//  ROI.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "ROI.h"

// mousePressed
//
// Call the appropriate function from the ROI class
void mousePressed(int event, int x, int y, int flags, void *param) {
    
    ROI *roi = static_cast<ROI *>(param);
    
    if (event == CV_EVENT_LBUTTONDOWN) {
        roi->mouseActions(x, y);
    }
    
}
//----------------------------------------------------------------------------------------------

// Init
//
// Init bools for roi selection
void ROI::init() {
    first = false;
    roi_captured = false;
}
//----------------------------------------------------------------------------------------------

// mouseActions
//
// Flow of control for selecting an ROI
void ROI::mouseActions(int x, int y) {
    
    if(!first && !roi_captured) {
        
        // no click yet set x1, y1
        x1 = x;
        y1 = y;
        
        // mark selection in program & visually
        first = true;
        printf("x1y1 :: %d, %d\n", x, y);
        circle(sourceImage, Point(x1-1,y1-1), 1, Scalar(155,250,150), -1, 8, 0 );
        imshow("ROI", sourceImage);
        
    } else if (!roi_captured && first) {
        
        // First click completed set x2, y2
        x2 = x;
        y2 = y;
        
        // mark selection in program & visually
        roi_captured = true;
        printf("x2y2 :: %d, %d\n", x, y);
        circle(sourceImage, Point(x2+1,y2+1), 1, Scalar(155,250,150), -1, 8, 0 );
        imshow("ROI", sourceImage);
        waitKey();
        image = sourceImage(Rect(Point(x1,y1),Point(x2,y2)));
        imshow("ROI", image);
    } else {
        // Already got ROI do we want another?
        std::cout<<"ROI Captured, Click again to recapture \n"<<std::endl;
        first = false;
        roi_captured = false;
        printf("RESET");
    }
}
//----------------------------------------------------------------------------------------------

// Get ROI from mouse input
// meat of work done above
void ROI::getROI(string sI) {
    namedWindow("ROI", CV_WINDOW_AUTOSIZE);
    sourceImage = imread(sI);
    imshow("ROI", sourceImage);
    setMouseCallback("ROI", mousePressed, this);
    waitKey();
}
//----------------------------------------------------------------------------------------------

// Get ROI from pre defined numbers & display result
void ROI::getROI(string sI, int coords[4]){

    namedWindow("ROI", CV_WINDOW_AUTOSIZE);
    sourceImage = imread(sI);
    x1 = coords[0];
    y1 = coords[1];
    x2 = coords[2];
    y2 = coords[3];
    image = sourceImage(Rect(Point(x1,y1),Point(x2,y2)));
    imshow("ROI", image);
    waitKey();
}