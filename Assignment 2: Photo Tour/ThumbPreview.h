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
#include "Image.h"

using namespace std;
using namespace cv;

class ThumbPreview {
    
public:
    
    /// Number of tumbnails per row & column
    int             number_in_row, number_in_column;
    
    /// Distance between image thumbnils
    int             spacing;
    
    /// Size of containing window
    int             width, height;
    
    // Scaled size of thumbnail
    int             scaledWidth, scaledHeight;
    
    /// Half size of claulated thumbnail size
    float           xOff, yOff;
    
    /// Path to the directory/folder holding the images to be thumbnailed
    string          dir_path;
    
    /// List of positions
    vector<Point>   coords;
    
    /// images clicked
    vector<int>     scratched;
    
    // The final image to display
    Mat             combined;
    int             MATRIX_TYPE;
    
    
    void            init(vector<Image> &img, string);
    
    void            updateThumbnails();
    
    // Render to screen
    void            displayThumbnails();
    
    // Get positions of thumbnails
    void             arrangeThumbnails(vector<Image> &img);
    
    // Did the mouse click get one of the tumbnails?
    int             hitOrMiss(int, int);
    
    // Put a stroke through and highlight a given image
    void            scratchThumbnail(vector<Image> &img);
    
    // Allocate an image to a give screen location (for ordering video)
    void            setImage(int, Mat*, vector<Image>&);
    
    // retieve the clicked image
    bool            getSelection(int);
    
    // assign a selected image
    void            setSelection(int, bool);
    
};


#endif /* defined(__Assignment_2__Photo_Tour__ThumbPreview__) */
