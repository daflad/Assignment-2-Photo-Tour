//
//  ROI.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "ROI.h"
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// mousePressed
//
// Call the appropriate function from the ROI class
//
//----------------------------------------------------------------------------------------------
void mousePressed(int event, int x, int y, int flags, void *param) {
    
    // Retieve the class object as this function is not part of the class
    ROI *roi = static_cast<ROI *>(param);
    
    // If there is an event, better do some recording.
    if (event == CV_EVENT_LBUTTONDOWN) {
        roi->mouseActions(x, y);
    }
    
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//
// Init
//
// Init bools for roi selection
//
//----------------------------------------------------------------------------------------------
void ROI::init() {
    // Flow control bools.
    first = false;
    roi_captured = false;
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// mouseActions
//
// Flow of control for selecting an ROI
//----------------------------------------------------------------------------------------------
void ROI::mouseActions(int x, int y) {
    
    // Nothing clicked
    if(!first && !roi_captured) {
        
        // no click yet set x1, y1
        x1 = x;
        y1 = y;
        
        // mark selection in program & visually
        first = true;

        circle(sourceImage, Point(x1-1,y1-1), 1, Scalar(155,250,150), -1, 8, 0 );
        imshow("ROI", sourceImage);
        
        // Got first point
    } else if (!roi_captured && first) {
        
        // First click completed set x2, y2
        x2 = x;
        y2 = y;
        
        // mark selection in program & visually
        roi_captured = true;
        
        circle(sourceImage, Point(x2+1,y2+1), 1, Scalar(155,250,150), -1, 8, 0 );
        imshow("ROI", sourceImage);
        waitKey();
        image = sourceImage(Rect(Point(x1,y1),Point(x2,y2)));
        imshow("ROI", image);
        
        // An oportunity to reselect the ROI.
    } else {
        // Already got ROI do we want another?
        std::cout<<"ROI Captured, Click again to recapture \n"<<std::endl;
        first = false;
        roi_captured = false;
    }
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//
// Get ROI from mouse input
// meat of work done above
//
//----------------------------------------------------------------------------------------------
void ROI::getROI(string sI) {
    namedWindow("ROI", CV_WINDOW_AUTOSIZE);
    sourceImage = imread(sI);
    imshow("ROI", sourceImage);
    setMouseCallback("ROI", mousePressed, this);
    waitKey();
    if (!roi_captured) {
        getROI(sI);
    }
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//
// Get ROI from pre defined numbers & display result
//
// TODO ::
//         There is a chance here that x1, x2, y1 & y2 might not
//         have been set if interface is not used corrctly
//
//----------------------------------------------------------------------------------------------
bool ROI::getROI(string sI, int coords[4]){
    
    namedWindow("ROI", CV_WINDOW_AUTOSIZE);
    sourceImage = imread(sI);
    x1 = coords[0];
    y1 = coords[1];
    x2 = coords[2];
    y2 = coords[3];
    if (x1 > 0 && y1 > 0 && x2 > 0 && y2 > 0 && x1 < sourceImage.cols && y1 < sourceImage.rows
        && x2 < sourceImage.cols && y2 < sourceImage.rows) {
        image = sourceImage(Rect(Point(x1,y1),Point(x2,y2)));
        imshow("ROI", image);
        waitKey();
        return true;
    } else {
        cout << "Error :: The coordinates of the ROI provided are greater " <<
        "than the dimentions of the image" << endl;
        imshow("ROI", sourceImage);
        waitKey();
        return false;
    }
    
    
    
}