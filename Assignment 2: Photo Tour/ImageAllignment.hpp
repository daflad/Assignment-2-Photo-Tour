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
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/nonfree/nonfree.hpp>

using namespace std;
using namespace cv;

class ImageAllignment {
    
public:
    
    vector<Mat>         images;
    Mat                 roi;
    vector<KeyPoint>    roikp;
    vector<KeyPoint>    imgkp;
    
    void init(vector<string>*, Mat*, string);
    
    void detectFeaturePoints(int);
    
    void extractDescriptors(int);
};

#endif /* defined(__Assignment_2__Photo_Tour__ImageAllignment__) */
