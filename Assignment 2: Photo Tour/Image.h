//
//  Image.h
//  Assignment 2: Photo Tour
//
//  Class defining an image in terms of this assignment
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#ifndef __Assignment_2__Photo_Tour__Image__
#define __Assignment_2__Photo_Tour__Image__

#include <iostream>
#include <opencv2/opencv.hpp>
#include "ROI.h"

using namespace cv;

// A one place store for all versions of an image
// Allows for a single vector to hold all image data
class Image {
    
public:
    
    /// System location of image
    string  filePath, dirPath;
    
    // Resolution of full matrix
    //
    /// NOTE ABOUT WIDTH & HEIGHT
    /// default action is to scale the original
    /// height & width proportionately so that
    /// a new width of 500 and corresponding
    /// height is produced.
    int     width, height;

    /// Full resolution version of image
    Mat     matrix;
    
    /// The ROI of the image
    ROI     roi;
    
    /// Low resolution version of image
    Mat     thumbnail;
    
    /// Init the image with it's file path & load Image
    void    init(string filePath, string dirPath, int width, int height);
    
    // Helpers to set class images
    
    /// get the roi using functions from
    Mat     getRoi(vector<float> roiCorners);
    
    /// Resize the image to a given set of dimentions
    /// Can be used for thumbnail too
    void    resizeImage(float width, float height);
    
private:
    /// file path not included as required for init.
    Mat     loadImage();
};

#endif /* defined(__Assignment_2__Photo_Tour__Image__) */
