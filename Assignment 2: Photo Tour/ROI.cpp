//
//  ROI.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "ROI.h"

void mousePressed(int event, int x, int y, int flags, void *param) {
    
    ROI *roi = static_cast<ROI *>(param);
    
    if (event == CV_EVENT_LBUTTONDOWN) {
            
        if(!roi->first && !roi->roi_captured) {
            
            roi->x1 = x;
            roi->y1 = y;
            roi->first = true;
            circle(roi->sourceImage, Point(roi->x1-1,roi->y1-1), 1, Scalar(255,50,50), -1, 8, 0 );
            imshow("ROI", roi->sourceImage);
            printf("x1y1 :: %d, %d\n", x, y);
            
        } else if (!roi->roi_captured && roi->first) {
            
            roi->x2 = x;
            roi->y2 = y;
            roi->roi_captured = true;
            printf("x2y2 :: %d, %d\n", x, y);
            roi->image = roi->sourceImage(Rect(Point(roi->x1,roi->y1),Point(roi->x2,roi->y2)));
            imshow("ROI", roi->image);
        } else {
            std::cout<<"ROI Captured, Click again to recapture \n"<<std::endl;
            roi->first = false;
            roi->roi_captured = false;
            printf("RESET");
        }
        
    }
    
}

void ROI::init() {
    first = false;
    roi_captured = false;
}

void ROI::getROI(string sI) {
    namedWindow("ROI", CV_WINDOW_AUTOSIZE);
    sourceImage = imread(sI);
    imshow("ROI", sourceImage);
    setMouseCallback("ROI", mousePressed, this);
    waitKey();
}

void ROI::getROI(string sourceImage, int coords[4]){
    
}