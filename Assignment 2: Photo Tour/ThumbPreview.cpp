//
//  ThumbPreview.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "ThumbPreview.h"

void MouseCallBackFunc(int event, int x, int y, int flags, void* userdata) {
    
    ThumbPreview* tp = static_cast<ThumbPreview*>( userdata );
    
    if (event == CV_EVENT_LBUTTONUP) {
        int index = tp->hitOrMiss(x, y);
        if (index > -1){
            tp->scratched.push_back(index);
            tp->scratchThumbnail();
        } else {
            Mat c = Mat(Size(tp->width,tp->height), tp->thumbs[0].type());
            tp->scratched.clear();
            tp->displayThumbnails(tp->arrangeThumbnails(c));
            
        }
    }
}

void ThumbPreview::init(vector<string>* fp, string dp) {
    number_in_row   = 5;
    firstIndex      = 0;
    spacing         = 20;
    width           = 1400;
    height          = 850;
    dir_path        = dp;
    
    namedWindow("Thumbnail");
    
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
    
    Mat combined = Mat::zeros(Size(width,height), thumbs[0].type());
    combined = arrangeThumbnails(combined);
    displayThumbnails(combined);
}

int ThumbPreview::hitOrMiss(int x, int y) {
    
    float min_dist = width;
    float xOff = thumbs[0].cols / 2;
    float yOff = thumbs[0].rows / 2;
    int index = -1;
    
    for (int i = 0; i < coords.size(); i++) {
        float dx = coords[i].x - x + xOff;
        float dy = coords[i].y - y + yOff;
        float dx2 = dx * dx;
        float dy2 = dy * dy;
        float d = sqrt(dx2 + dy2);
        if (d < min_dist ) {
            min_dist = d;
            index = i;
        }
    }
    
    if (x > coords[index].x + thumbs[index].cols || x < coords[index].x ||
        y > coords[index].y + thumbs[index].rows || y < coords[index].y) {
        index = -1;
    }
    
    return index;
}

void ThumbPreview::displayThumbnails(Mat& combined) {

    setMouseCallback("Thumbnail", MouseCallBackFunc, this);
    imshow("Thumbnail", combined);
    waitKey();
}

Mat& ThumbPreview::arrangeThumbnails(Mat& combined) {
    
    combined = Mat::zeros(Size(width,height), thumbs[0].type());
    
    int xc = spacing;
    int yc = spacing;
    int row_tracker = 0;
    for (int i = 0; i < thumbs.size(); i++) {
        Mat t = thumbs[i];
        t.copyTo(combined(Rect(Point(xc, yc),Point(xc + t.cols, yc + t.rows))));
        coords.push_back(Point(xc, yc));
        if (row_tracker < number_in_row - 1) {
            xc += (t.cols + spacing);
            row_tracker++;
        } else {
            row_tracker = 0;
            xc = spacing;
            yc += (t.rows + spacing);
        }
    }
    return combined;
}

void ThumbPreview::scratchThumbnail() {
    
    Mat combined = Mat(Size(width,height), thumbs[0].type());
    combined = arrangeThumbnails(combined);
    Mat t = combined.clone();
    
    float opacity = 0.5;
    
    for (int i = 0; i < scratched.size(); i++) {
        int index = scratched[i];
        int scaleWidth = thumbs[index].cols;
        int scaleHeight = thumbs[index].rows;
        Point p1 = coords[index];
        Point p2 = Point(p1.x + scaleWidth, p1.y + scaleHeight);
        rectangle(t, p1, p2, Scalar( 0, 0, 50), CV_FILLED, 8, 0);
        rectangle(combined, p1, p2, Scalar( 0, 255, 255), 2, 8, 0);
        line(combined, p2, p1, Scalar( 0, 0, 255), 2, 8, 0);
    }
    addWeighted(t, opacity, combined, 1 - opacity, 0, combined);
    displayThumbnails(combined);
    
}







