//
//  ImageAllignment.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "ImageAllignment.h"
void printParams( cv::Algorithm* algorithm ) {
    std::vector<std::string> parameters;
    algorithm->getParams(parameters);
    
    for (int i = 0; i < (int) parameters.size(); i++) {
        std::string param = parameters[i];
        int type = algorithm->paramType(param);
        std::string helpText = algorithm->paramHelp(param);
        std::string typeText;
        
        switch (type) {
            case cv::Param::BOOLEAN:
                typeText = "bool";
                break;
            case cv::Param::INT:
                typeText = "int";
                break;
            case cv::Param::REAL:
                typeText = "real (double)";
                break;
            case cv::Param::STRING:
                typeText = "string";
                break;
            case cv::Param::MAT:
                typeText = "Mat";
                break;
            case cv::Param::ALGORITHM:
                typeText = "Algorithm";
                break;
            case cv::Param::MAT_VECTOR:
                typeText = "Mat vector";
                break;
            }
        std::cout << "Parameter '" << param << "' type=" << typeText << " help=" << helpText << std::endl;
    }
}


//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// init
//
// Default values
// Read each image into vector
//
//----------------------------------------------------------------------------------------------
void ImageAllignment::init() {
    
    minHessian = 400;

}



//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// detectFeaturePoints
//
// Detect feature points in the ROI of the first image and everywhere in the second image.
//
//----------------------------------------------------------------------------------------------
void ImageAllignment::detectFeaturePoints(int ind, vector<Image> images,  Mat roi) {

    Ptr<FeatureDetector> fd = FeatureDetector::create("GFTT");

    printParams(fd);
    
//    fd->set("minDistance", 0.5);
    fd->set("qualityLevel", 0.04);
//    fd->set("nfeatures", 3);
    fd->set("useHarrisDetector", true);
    fd->set("k", 0.04);
    

    fd->detect(roi, roikp);
//    
//    drawKeypoints(roi, roikp, roi, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
//
    Mat temp;
    cvtColor(images[ind].matrix,temp,CV_BGR2GRAY);
    fd->detect(temp , imgkp);
    
//    drawKeypoints(images[ind], imgkp, images[ind], Scalar::all(-1), DrawMatchesFlags::DEFAULT);
//    
//    imshow("ROI", roi);
//    namedWindow("NewFeats");
//    imshow("NewFeats", images[ind]);

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
void ImageAllignment::extractDescriptors(int ind, int x1, int y1, string dp, vector<Image> images,  Mat roi) {
    
    Ptr<DescriptorExtractor> de = DescriptorExtractor::create("SIFT");

//    de->set("threshold", 300);
    
    // Compute descriptors
    Mat descriptors1, descriptors2;
    de->compute(roi, roikp, descriptors1);
    de->compute(images[ind].matrix, imgkp, descriptors2);
    
    // Find matches between descriptors
    FlannBasedMatcher matcher;
    vector<vector< DMatch >> matches1;
    vector<vector< DMatch >> matches2;
    matcher.knnMatch(descriptors1, descriptors2, matches1, 2);
    matcher.knnMatch(descriptors2, descriptors1, matches2, 2);
    printf("%d matches found ROI -> IMG\n", (int)matches1.size());
    printf("%d matches found IMG -> ROI\n", (int)matches2.size());
    
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

    // Debug
//    printf("-- Max dist : %f \n", max_dist );
//    printf("-- Min dist : %f \n", min_dist );
    vector<vector< DMatch >> symMatch;
    std::vector< DMatch > good_matches;
    bool enoughMatches = false;
    int numMatch;
    double inc = diff / 10;
    double mult = min_dist + inc;
    while (!enoughMatches) {
        for( int i = 0; i < matches1.size(); i++ ) {
            for( int j = 0; j < matches2.size(); j++ ) {
                if (matches1[i].size() == 2) {
                    if (matches1[i][0].queryIdx == matches2[j][0].queryIdx) {
                        if( matches1[i][0].distance < min_dist + mult) {
                            good_matches.push_back( matches1[i][0]);
                        }
                    }
                }
            }
        }
        numMatch = (int)good_matches.size();
        if (numMatch > 9) {
            enoughMatches = true;
        } else {
            mult += inc;
            good_matches.clear();
        }
    }
    printf("%d good matches found\n", (int)good_matches.size());
    
    // For debug, remove from final animation
    Mat img_matches;
    drawMatches( roi, roikp, images[ind].matrix, imgkp, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    //-- Localize the object from img_1 in img_2
    std::vector<Point2f> obj;
    std::vector<Point2f> scene;
    
    for( size_t i = 0; i < good_matches.size(); i++ ) {
        //-- Get the keypoints from the good matches
        obj.push_back( roikp[ good_matches[i].queryIdx ].pt );
        scene.push_back( imgkp[ good_matches[i].trainIdx ].pt );
    }
    
    Mat H = findHomography( obj, scene, RANSAC );
    
    //-- Get the corners from the image_1 ( the object to be "detected" )
    std::vector<Point2f> obj_corners(4);
    obj_corners[0] = Point(0,0); obj_corners[1] = Point( roi.cols, 0 );
    obj_corners[2] = Point( roi.cols, roi.rows ); obj_corners[3] = Point( 0, roi.rows );
    std::vector<Point2f> scene_corners(4);
    
    perspectiveTransform( obj_corners, scene_corners, H);
    
    
    //-- Draw lines between the corners (the mapped object in the scene - image_2 )
    Point2f offset( (float)roi.cols, 0);
    line( img_matches, scene_corners[0] + offset, scene_corners[1] + offset, Scalar(0, 255, 0), 4 );
    line( img_matches, scene_corners[1] + offset, scene_corners[2] + offset, Scalar( 0, 255, 0), 4 );
    line( img_matches, scene_corners[2] + offset, scene_corners[3] + offset, Scalar( 0, 255, 0), 4 );
    line( img_matches, scene_corners[3] + offset, scene_corners[0] + offset, Scalar( 0, 255, 0), 4 );
    
    
    // Display
//    imshow( "Good Matches", img_matches );
    
//    waitKey(0);
    
    Mat of = Mat::eye(3,3, CV_64F);
    of.at<double>(0,2) = -x1;
    of.at<double>(1,2) = -y1;
    
    H *= of;
    
    warpPerspective(images[ind].matrix, images[ind].matrix, H, images[ind].matrix.size(), WARP_INVERSE_MAP);
    string fp;
    dp += "frames/Frame_";
    
    if (ind < 10) {
        fp = dp + to_string(0) + to_string(ind);
    } else {
        fp = dp + to_string(ind);
    }
    fp += ".jpg";
    imwrite(fp.c_str(), images[ind].matrix);
//    imshow("Good Matches", images[ind]);
//    waitKey(0);
}