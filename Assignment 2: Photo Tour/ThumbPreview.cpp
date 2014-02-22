//
//  ThumbPreview.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "ThumbPreview.h"

void ThumbPreview::init(vector<string>* fp, string dp) {
    filepaths       = fp;
    number_in_row   = 5;
    firstIndex      = 0;
    spacing         = 20;
    width           = 1280;
    height          = 768;
    dir_path        = dp;
}

void ThumbPreview::displayThumbnails() {
    
    
    vector<Mat> thumbs;
    
    for (int i = 0; i < filepaths->size(); i++) {
        Mat t;
        t = imread(dir_path + filepaths->at(i).c_str());
        cout << t.rows << "\t" << t.cols << endl;
        thumbs.push_back(t);
    }
    
    bool solution = false;
    int scaledWidth;
    int scaledHeight;
    
    while (!solution) {
        scaledWidth = (int)((float)(width - ((number_in_row + 1) * spacing)) / number_in_row);
        float ratio = thumbs.at(0).cols / (float)scaledWidth;
        printf("Rows : %d Ratio : %f", thumbs.at(0).cols, ratio);
        scaledHeight = (int) thumbs.at(0).rows  / ratio;
        printf("%d\n", scaledHeight);
        number_in_column = ((int)thumbs.size() / number_in_row) + 1;
        if (number_in_column * (scaledHeight + spacing) < height) {
            solution = true;
        } else {
            number_in_row++;
        }
    }
    
    printf("width : %d\nheight : %d # in row :%d",scaledWidth, scaledHeight, number_in_row);
    
    Mat combined = Mat(Size(width,height), thumbs[0].type());

    int xc = spacing;
    int yc = spacing;
    int row_tracker = 0;
    for (int i = 0; i < thumbs.size(); i++) {
        resize(thumbs[i], thumbs[i], Size(scaledWidth, scaledHeight));
        thumbs[i].copyTo(combined(Rect(Point(xc, yc),Point(xc + scaledWidth, yc + scaledHeight))));
        if (row_tracker < number_in_row - 1) {
            xc += (scaledWidth + spacing);
            row_tracker++;
        } else {
            row_tracker = 0;
            xc = spacing;
            yc += (scaledHeight + spacing);
        }
        printf("Were at : %d\n", i);
    }

    namedWindow("Thumbnail");
    imshow("Thumbnail", combined);
    waitKey();
}