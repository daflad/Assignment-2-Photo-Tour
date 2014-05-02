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
    
    // Compute descriptors
    Mat descriptors1, descriptors2;
    // Find matches between descriptors
    FlannBasedMatcher matcher;
    vector<vector< DMatch >> matches1;
    vector<vector< DMatch >> matches2;
    vector< DMatch > good_matches;
    void init();
    
    void detectFeaturePoints(int, vector<Image> &images,  Mat roi, float ql, float k);
    
    void extractDescriptors(int, string, vector<Image> &images,  Mat roi, float accuracy, int nM, float sig, int roiInd);
    
    void pruneResults(int nM);
    
    bool ransac(int ind, vector<Image> &images, int x1, int y1, Mat roi, float accuracy, int roiInd);
    
    void storeInfo(vector<Image> &images, vector<Point2f> scene_corners, Mat H, int ind, int roiInd, int numInliers, vector<uchar> mask);
    
//1. Detect feature points in the ROI of the first image and everywhere in the second image.
//2. Extract a descriptor for each feature point.
//3. Find the best matches between feature points in both images.
//4. (optionally) Prune the matches with one or both of the suggested methods (Sections 2.5 and 2.6).
//5. Use the RANSAC algorithm to find a homography (projective transform) mapping ROI from one image
//   to another. Optionally, you can use your own RANSAC restricted to similarity transformations to
//   improve the results (Section 2.9)
//6. Use the number of inliners returned by the RANSAC algorithm as the measure of the quality of the
//   match. If there are too few inliners, the match is probably incorrect and needs to be ignored.
//   You can automatically exclude such images from the results.

};

#endif /* defined(__Assignment_2__Photo_Tour__ImageAllignment__) */
