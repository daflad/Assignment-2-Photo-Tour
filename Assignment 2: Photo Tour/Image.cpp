//
//  Image.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "Image.h"
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// Init
//
//----------------------------------------------------------------------------------------------

void Image::init(string fp, string dp, int w, int h, int ind) {
    filePath    = fp;
    dirPath     = dp;
    width       = w;
    height      = h;
    matrix      = loadImage();
    orig        = loadImage();
    thumbnail   = loadImage();
    newROI      = false;
    isWarped    = false;
    failed      = false;
    warpAcc     = 0;
    warpInd     = 0;
    index       = ind;
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// Load Image
//
// Load the image based on the supplied filepath
// Check what dimentions user has specifed
// Adjust dimentions to defaults if required
//----------------------------------------------------------------------------------------------
Mat Image::loadImage() {
    
    // load image
    Mat temp;
    if (dirPath[dirPath.length() - 1] != '/') {
        dirPath += "/";
    }
    temp = imread(dirPath + filePath);
    return temp;
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// Resize Image
//
// Even simpler
//----------------------------------------------------------------------------------------------
void Image::resizeImage(float w, float he) {
    if (width != 0 && height != 0) {
        // resize
        resize(matrix, matrix, Size(width, height));
    }
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// Get ROI
//
// Returns a matrix from the specifed ROI
// if roiCorners is not of size 4 is will return
// an empty matrix of no size.
//----------------------------------------------------------------------------------------------
Mat Image::getRoi(vector<float> roiCorners) {
    if (roiCorners.size() == 4) {
        newROI = true;
        return matrix(Rect(Point(roiCorners[0],roiCorners[1]),Point(roiCorners[2],roiCorners[3])));
    } else {
        return Mat();
    }
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// Get ROI
//
// Update thumbnail with latest transformation
//----------------------------------------------------------------------------------------------
void Image::updateThumb() {
    matrix.copyTo(thumbnail);
}
