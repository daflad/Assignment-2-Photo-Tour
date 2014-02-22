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
    
    int             number_in_row;
    int             number_in_column;
    int             firstIndex;
    int             spacing;
    int             width;
    int             height;
    
    string          dir_path;
    
    vector<Mat> thumbs;
    
    void init(vector<string>* fp, string);
    
    void displayThumbnails();
    
    void setImage(int, Mat*);
    void getSelection(int);
    void setSelection(int);
    
};

#endif /* defined(__Assignment_2__Photo_Tour__ThumbPreview__) */
