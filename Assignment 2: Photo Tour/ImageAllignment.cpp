//
//  ImageAllignment.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "ImageAllignment.h"

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// detectFeaturePoints
//
// Detect feature points in the ROI of the first image and everywhere in the second image.
//
//----------------------------------------------------------------------------------------------
void ImageAllignment::detectFeaturePoints(int ind, vector<Image> &images,  Mat roi, float ql, float k) {
    
    // Select Feature detector & clear key point arrays
    Ptr<FeatureDetector> fd = FeatureDetector::create("GFTT");
    roikp.clear();
    imgkp.clear();
    
    // Set params for GFTT
    fd->set("qualityLevel", ql);
    fd->set("useHarrisDetector", true);
    fd->set("k", k);
    
    // Convert images to B&W
    // Detect Key points in both directions
    cvtColor(roi, roi, CV_BGR2GRAY);
    fd->detect(roi, roikp);
    
    Mat temp;
    cvtColor(images[ind].orig, temp, CV_BGR2GRAY);
    fd->detect(temp , imgkp);
    
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// extractDescriptors
//
// Extract a descriptor for each feature point.
//
//----------------------------------------------------------------------------------------------
void ImageAllignment::extractDescriptors(int ind, string dp, vector<Image> &images,  Mat roi, float accuracy, int nM, float sig, int roiInd) {
    
    Ptr<DescriptorExtractor> de = DescriptorExtractor::create("SIFT");

    // Set params for SIFT
    de->set("sigma", sig);
    de->compute(roi, roikp, descriptors1);
    de->compute(images[ind].matrix, imgkp, descriptors2);
    
    // Find matches in both directions
    matcher.knnMatch(descriptors1, descriptors2, matches1, 2);
    matcher.knnMatch(descriptors2, descriptors1, matches2, 2);
//    Debug
//    printf("%d matches found ROI -> IMG\n", (int)matches1.size());
//    printf("%d matches found IMG -> ROI\n", (int)matches2.size());

}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// pruneResults
//
// Reduce the number of matches.
//
//----------------------------------------------------------------------------------------------
void ImageAllignment::pruneResults(int nM) {
    // Calculation of max and min distances between keypoints
    double max_dist = 0; double min_dist = 100;
    for (int i = 0; i < descriptors1.rows; i++) {
        double dist = matches1[i][0].distance;
        if ( dist < min_dist ) {
            min_dist = dist;
        }
        if( dist > max_dist ) {
            max_dist = dist;
        }
    }
    
    double diff = max_dist - min_dist;
    
    
    // Increase the tollerance until unough mathces have been found
    bool enoughMatches = false;
    int numMatch;
    // There will be 100 increments in the hope of finding matches
    double inc = diff / 100;
    double mult = min_dist + inc;
    while (!enoughMatches) {
        good_matches.clear();
        // for each match in either direction if present in both lists (symetic testing) check NNDR
        for( int i = 0; i < matches1.size(); i++ ) {
            for( int j = 0; j < matches2.size(); j++ ) {
                if (matches1[i][0].queryIdx == matches2[j][0].queryIdx) {
                    if( matches1[i][0].distance < min_dist + mult) {
                        good_matches.push_back( matches1[i][0]);
                    }
                }
            }
        }
        // if not enough matche increas NNDR
        numMatch = (int)good_matches.size();
        if (numMatch >= nM) {
            enoughMatches = true;
        } else {
            // Once diff has been spanned, exit
            if (mult >= diff) {
                enoughMatches = true;
            }
            mult += inc;
        }
    }
//    debug
//    printf("%d good matches found\n", (int)good_matches.size());
}


//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// RANSAC
//
// RANSAC to get perspecticve
//
//----------------------------------------------------------------------------------------------
bool ImageAllignment::ransac(int ind, vector<Image> &images, int x1, int y1, Mat roi, float accuracy, int roiInd) {

    // keypoint holders
    std::vector<Point2f> obj;
    std::vector<Point2f> scene;
    
    for( size_t i = 0; i < good_matches.size(); i++ ) {
        // extract keypoints
        obj.push_back( roikp[ good_matches[i].queryIdx ].pt );
        scene.push_back( imgkp[ good_matches[i].trainIdx ].pt );
    }
    
    // mask to hold inlier indicators
    vector<uchar> mask;
    
    // Calculate translation matrix
    Mat H = findHomography( obj, scene, mask, CV_RANSAC, 3);
    
    // 4 points of ROI
    vector<Point2f> obj_corners(4);
    obj_corners[0] = Point(0,0);
    obj_corners[1] = Point( roi.cols, 0 );
    obj_corners[2] = Point( roi.cols, roi.rows );
    obj_corners[3] = Point( 0, roi.rows );
    
    // New ROI points
    vector<Point2f> scene_corners(4);
    perspectiveTransform( obj_corners, scene_corners, H);
    
    // Offset to middle
    Mat of = Mat::eye(3,3, CV_64F);
    of.at<double>(0,2) = -x1;
    of.at<double>(1,2) = -y1;
    H *= of;
    
    // Calculet number of inliers
    int numInliers = 0;
    for (int i = 0; i < mask.size(); i++) {
        if ((int)mask[i] == 1) {
            numInliers++;
        }
    }
    
    // Check number of inliers against accuracy
    // Check that this warp is better than last, if there is a last
    if ((float)numInliers > (float)mask.size()*accuracy && ((float)numInliers / mask.size()) >= images[ind].warpAcc) {
        if (scene_corners[0].x > 0 && scene_corners[0].y > 0 && scene_corners[2].x > 0 && scene_corners[2].y > 0) {
            if (scene_corners[0].x < images[ind].matrix.cols && scene_corners[0].y < images[ind].matrix.rows
                && scene_corners[2].x < images[ind].matrix.cols && scene_corners[2].y < images[ind].matrix.rows) {
                
                // gather info
                storeInfo(images, scene_corners, H, ind, roiInd, numInliers, mask);
                
                // Perform warp
                warpPerspective(images[ind].orig, images[ind].matrix, H, images[ind].matrix.size(), 1);
            }
        }
    }
    
    // Return warp satus.
    return images[ind].isWarped;
}


//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// storeInfo
//
// Helper to store info on warp
//
//----------------------------------------------------------------------------------------------
void ImageAllignment::storeInfo(vector<Image> &images, vector<Point2f> scene_corners, Mat H, int ind, int roiInd, int numInliers, vector<uchar> mask) {
    // Coords of ROI
    vector<float> roi_co;
    roi_co.push_back(scene_corners[0].x);
    roi_co.push_back(scene_corners[0].y);
    roi_co.push_back(scene_corners[2].x);
    roi_co.push_back(scene_corners[2].y);
    // Update ROI for image & store location of TL
    images[ind].roi.image = images[ind].getRoi(roi_co);
    images[ind].roi.x1 = roi_co[0];
    images[ind].roi.y1 = roi_co[1];
    // Remember that image warped
    images[ind].isWarped = true;
    // Store Matrix for video
    images[ind].trans = H;
    // Store Accuracy & index from which the image was matched
    images[ind].warpAcc = ((float)numInliers / mask.size());
    images[ind].warpInd = roiInd;
}






