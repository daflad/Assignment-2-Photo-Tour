//
//  ImageAllignment.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "ImageAllignment.hpp"

void ImageAllignment::init(vector<string>* fp, Mat *m, string dp) {
    
    roi = *m;
    
    for (int i = 0; i < fp->size(); i++) {
        Mat t;
        t = imread(dp + fp->at(i).c_str());
        images.push_back(t);
    }
}

void ImageAllignment::detectFeaturePoints() {
        
    Ptr<FeatureDetector> fd = FeatureDetector::create("SURF");
    
    fd->detect(roi, roikp);
    
    drawKeypoints(roi, roikp, roi, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    
    fd->detect(images[1], imgkp);
    
    drawKeypoints(images[1], imgkp, images[1], Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    
    imshow("ROI", roi);
    namedWindow("NewFeats");
    imshow("NewFeats", images[1]);
    waitKey();
}

void ImageAllignment::extractDescriptors() {
    Ptr<DescriptorExtractor> de = DescriptorExtractor::create("SURF");
    Mat descriptors1, descriptors2;
    de->compute(roi, roikp, descriptors1);
    de->compute(images[1], imgkp, descriptors2);
    
    FlannBasedMatcher matcher;
    vector< DMatch > matches;
    matcher.match( descriptors1, descriptors2, matches );
    
    double max_dist = 0; double min_dist = 100;
    
    //-- Quick calculation of max and min distances between keypoints
    for (int i = 0; i < descriptors1.rows; i++) {
        double dist = matches[i].distance;
        if ( dist < min_dist ) {
            min_dist = dist;
        }
        if( dist > max_dist ) {
            max_dist = dist;
        }
    }
    
    printf("-- Max dist : %f \n", max_dist );
    printf("-- Min dist : %f \n", min_dist );
    
    Mat img_matches;
    drawMatches( roi, roikp, images[1], imgkp, matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    
    //-- Show detected matches
    imshow( "Good Matches", img_matches );
    
    waitKey(0);
}



