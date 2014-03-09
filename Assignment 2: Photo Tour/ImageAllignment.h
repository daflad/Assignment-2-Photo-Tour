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

using namespace std;
using namespace cv;

class ImageAllignment {
    
public:
    
    // List of images to be processed
    vector<Mat>         images;
    // ROI to be found in other images
    Mat                 roi;
    // Key points for ROI & current image being comared
    vector<KeyPoint>    roikp;
    vector<KeyPoint>    imgkp;
    
    void init(vector<string>*, Mat*, string);
    
    void detectFeaturePoints(int);
    
    void extractDescriptors(int);
    
    void pruneResults();
};

#endif /* defined(__Assignment_2__Photo_Tour__ImageAllignment__) */
