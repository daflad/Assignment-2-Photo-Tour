//
//  ImageAllignment.h
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#ifndef __Assignment_2__Photo_Tour__ImageAllignment__
#define __Assignment_2__Photo_Tour__ImageAllignment__

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "Image.h"

using namespace std;
using namespace cv;

class ImageAllignment {
    
public:
    
    int minHessian;
    
    /// Key points for ROI
    vector<KeyPoint>    roikp;
    
    /// Key points for the current image to be comared
    vector<KeyPoint>    imgkp;
    
    void init();
    
    void detectFeaturePoints(int, vector<Image> &images,  Mat roi, float ql);
    
    bool extractDescriptors(int, int, int, string, vector<Image> &images,  Mat roi, float accuracy, int nM);
    
    void pruneResults();
};

#endif /* defined(__Assignment_2__Photo_Tour__ImageAllignment__) */
