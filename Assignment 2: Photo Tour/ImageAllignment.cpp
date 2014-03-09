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
// init
//
// Default values
// Read each image into vector
//
//----------------------------------------------------------------------------------------------
void ImageAllignment::init(vector<string>* fp, Mat *m, string dp) {
    
    roi = *m;
    
    for (int i = 0; i < fp->size(); i++) {
        Mat t;
        t = imread(dp + fp->at(i).c_str());
        images.push_back(t);
    }
}



//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// detectFeaturePoints
//
// Detect feature points in the ROI of the first image and everywhere in the second image.
//
//----------------------------------------------------------------------------------------------
void ImageAllignment::detectFeaturePoints(int ind) {
        
    Ptr<FeatureDetector> fd = FeatureDetector::create("GFTT");
    
    fd->detect(roi, roikp);
    
    drawKeypoints(roi, roikp, roi, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    
    fd->detect(images[ind], imgkp);
    
    drawKeypoints(images[ind], imgkp, images[ind], Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    
    imshow("ROI", roi);
    namedWindow("NewFeats");
    imshow("NewFeats", images[ind]);

}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// extractDescriptors
//
// Extract a descriptor for each feature point.
//
//  TODO:
//       Make interface for user to chose algorithm at run time
//
//----------------------------------------------------------------------------------------------
void ImageAllignment::extractDescriptors(int ind) {
    
    Ptr<DescriptorExtractor> de = DescriptorExtractor::create("SURF");

    // The following lines need to be uncommented / commented based on the algorithm used.
    
    // SURF variables
//    de->set("hessianThreshold", 300);
//    de->set("upright", true);
    
    // SIFT variables
//    de->set("contrastThreshold", 3);
//    de->set("edgeThreshold", 300);
//    de->set("sigma", 1);

    // Compute descriptors
    Mat descriptors1, descriptors2;
    de->compute(roi, roikp, descriptors1);
    de->compute(images[ind], imgkp, descriptors2);
    
    // Find matches between descriptors
    FlannBasedMatcher matcher;
    vector< DMatch > matches;
    matcher.match( descriptors1, descriptors2, matches );
    printf("%d matches found\n", (int)matches.size());
    
    // Calculation of max and min distances between keypoints
    double max_dist = 0; double min_dist = 100;
    for (int i = 0; i < descriptors1.rows; i++) {
        double dist = matches[i].distance;
        if ( dist < min_dist ) {
            min_dist = dist;
        }
        if( dist > max_dist ) {
            max_dist = dist;
        }
    }

    // Debug
//    printf("-- Max dist : %f \n", max_dist );
//    printf("-- Min dist : %f \n", min_dist );
    
    std::vector< DMatch > good_matches;
    
    for( int i = 0; i < descriptors1.rows; i++ ) {
        if( matches[i].distance < min_dist*1.5) {
            good_matches.push_back( matches[i]);
        }
    }
//    printf("%d matches good found\n", (int)good_matches.size());
    
    // For debug, remove from final animation
    Mat img_matches;
    drawMatches( roi, roikp, images[ind], imgkp, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    //-- Localize the object
    std::vector<Point2f> obj;
    std::vector<Point2f> scene;
    
    for( int i = 0; i < good_matches.size(); i++ ) {
        //-- Get the keypoints from the good matches
        obj.push_back( roikp[ good_matches[i].queryIdx ].pt );
        scene.push_back( imgkp[ good_matches[i].trainIdx ].pt );
    }
    
    Mat H = findHomography( obj, scene, CV_RANSAC );
    
    //-- Get the corners from the image_1 ( the object to be "detected" )
    std::vector<Point2f> obj_corners(4);
    obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( images[ind].cols, 0 );
    obj_corners[2] = cvPoint( images[ind].cols, images[ind].rows ); obj_corners[3] = cvPoint( 0, images[ind].rows );
    std::vector<Point2f> scene_corners(4);
    
    perspectiveTransform( obj_corners, scene_corners, H);
    
    //-- Draw lines between the corners (the mapped object in the scene - image_2 )
    line( img_matches, scene_corners[0] + Point2f( images[ind].cols, 0), scene_corners[1] + Point2f( images[ind].cols, 0), Scalar(0, 255, 0), 4 );
    line( img_matches, scene_corners[1] + Point2f( images[ind].cols, 0), scene_corners[2] + Point2f( images[ind].cols, 0), Scalar( 0, 255, 0), 4 );
    line( img_matches, scene_corners[2] + Point2f( images[ind].cols, 0), scene_corners[3] + Point2f( images[ind].cols, 0), Scalar( 0, 255, 0), 4 );
    line( img_matches, scene_corners[3] + Point2f( images[ind].cols, 0), scene_corners[0] + Point2f( images[ind].cols, 0), Scalar( 0, 255, 0), 4 );
    
    // Display
    imshow( "Good Matches", img_matches );
    
    waitKey(0);
}