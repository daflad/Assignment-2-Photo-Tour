//
//  ThumbPreview.h
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#ifndef __Assignment_2__Photo_Tour__ThumbPreview__
#define __Assignment_2__Photo_Tour__ThumbPreview__

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class ThumbPreview {
    
public:
    
    // Number of tumbnails per row & column
    int             number_in_row;
    int             number_in_column;
    
    // Distance between image thumbnils
    int             spacing;
    
    // Size of containing window
    int             width;
    int             height;
    
    // Path to the directory/folder holding the images to be thumbnailed
    string          dir_path;
    
    // List of images, positions & clicked
    vector<Mat>     thumbs;
    vector<Point>   coords;
    vector<int>     scratched;
    
    // The final image to display
    Mat             combined;
    
    void            init(vector<string>* fp, string);
    
    // Render to screen
    void            displayThumbnails();
    
    // Get positions of thumbnails
    void             arrangeThumbnails();
    
    // Did the mouse click get one of the tumbnails?
    int             hitOrMiss(int, int);
    
    // Put a stroke through and highlight a given image
    void            scratchThumbnail();
    
    // Allocate an image to a give screen location (for ordering video)
    void            setImage(int, Mat*);
    
    // retieve the clicked image
    void            getSelection(int);
    
    // assign a selected image
    void            setSelection(int);
    
};


#endif /* defined(__Assignment_2__Photo_Tour__ThumbPreview__) */
