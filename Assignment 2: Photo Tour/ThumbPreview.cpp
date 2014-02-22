//
//  ThumbPreview.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "ThumbPreview.h"

void ThumbPreview::init(vector<string>* fp, string dp) {
    number_in_row   = 5;
    firstIndex      = 0;
    spacing         = 20;
    width           = 1400;
    height          = 850;
    dir_path        = dp;
    
    for (int i = 0; i < fp->size(); i++) {
        Mat t;
        t = imread(dir_path + fp->at(i).c_str());
        thumbs.push_back(t);
    }
    
    bool solution = false;
    int scaledWidth;
    int scaledHeight;
    
    while (!solution) {
        scaledWidth = (int)((float)(width - ((number_in_row + 1) * spacing)) / number_in_row);

        scaledHeight = (int) thumbs.at(0).rows  / (thumbs.at(0).cols / (float)scaledWidth);

        number_in_column = ((int)thumbs.size() / number_in_row) + 1;
        if (number_in_column * (scaledHeight + spacing) < height) {
            solution = true;
        } else {
            number_in_row++;
        }
    }
    
    for (int i = 0; i < thumbs.size(); i++) {
        resize(thumbs[i], thumbs[i], Size(scaledWidth, scaledHeight));
    }
}

void ThumbPreview::displayThumbnails() {
    
    Mat combined = Mat(Size(width,height), thumbs[0].type());

    int xc = spacing;
    int yc = spacing;
    int row_tracker = 0;
    for (int i = 0; i < thumbs.size(); i++) {
        Mat t = thumbs[i];
        t.copyTo(combined(Rect(Point(xc, yc),Point(xc + t.cols, yc + t.rows))));
        if (row_tracker < number_in_row - 1) {
            xc += (t.cols + spacing);
            row_tracker++;
        } else {
            row_tracker = 0;
            xc = spacing;
            yc += (t.rows + spacing);
        }
    }

    namedWindow("Thumbnail");
    imshow("Thumbnail", combined);
    waitKey();
}









