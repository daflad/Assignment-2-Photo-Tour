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

using namespace std;
using namespace cv;

class ImageAllignment {
    
public:
    
    vector<Mat>         images;
    Mat                 roi;
    vector<KeyPoint>    roikp;
    vector<KeyPoint>    imgkp;
    
    void init(vector<string>*, Mat*, string);
    
    void detectFeaturePoints();
    
    void extractDescriptors();
};

#endif /* defined(__Assignment_2__Photo_Tour__ImageAllignment__) */
