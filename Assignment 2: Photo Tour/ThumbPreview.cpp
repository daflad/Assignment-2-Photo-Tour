//
//  ThumbPreview.cpp
//  Assignment 2: Photo Tour
//
//  Genorate and display all the thumbnail previews for a given folder of images
//  Re-size images to fit all within a single screen.
//
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "ThumbPreview.h"

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// mousePressed
//
// Call the appropriate function from the ThumbPreview class
//
//----------------------------------------------------------------------------------------------
void MouseCallBackFunc(int event, int x, int y, int flags, void* userdata) {
    
    // Retieve the class object as this function is not part of the class
    ThumbPreview* tp = static_cast<ThumbPreview*>( userdata );
    
    // ******************************************************************** //
    // FOR NOW, CLICKING OUTSIDE AN IMAGE WILL CLEAR THE SCRATCHED ARRAY.   //
    // THIS IS TOO FIDLEY FOR THE FINAL PROGRAM, BETTER TO PROVIDE A BUTTON //
    // FOR THE USER                                                         //
    // ******************************************************************** //
    if (event == CV_EVENT_LBUTTONUP) {
        int index = tp->hitOrMiss(x, y);
        // if we have a hit then add it to the list
        if (index > -1){
            tp->scratched.push_back(index);
            tp->scratchThumbnail();
        } else {
            tp->scratched.clear();
            tp->arrangeThumbnails();
        }
        tp->displayThumbnails();
    }
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// init
//
// Default values
// Open a window & do the required maths to fit each thumbnail in the view
// Once calcumated resize each image accordingly and to the list
// Display images once complete.
//
//----------------------------------------------------------------------------------------------
void ThumbPreview::init(vector<string>* fp, string dp) {
    
    // Default values
    number_in_row   = 5;
    spacing         = 20;
    width           = 800;
    height          = 600;
    dir_path        = dp;
    
    // Read in each image in folder
    for (int i = 0; i < fp->size(); i++) {
        Mat t;
        t = imread(dir_path + fp->at(i).c_str());
        thumbs.push_back(t);
    }
    
    combined        = Mat::zeros(Size(width,height), thumbs[0].type());
    
    // Open Preview window & define callback
    namedWindow("Thumbnail");
    setMouseCallback("Thumbnail", MouseCallBackFunc, this);
    
    
    
    // the final width of the thumbnails & an idicator to the solution
    bool solution = false;
    int scaledWidth;
    int scaledHeight;
    
    // Keep going till you find an answer
    while (!solution) {
        
        // Start by scaling the image width based on 5 images & given spacing
        scaledWidth = (int)((float)(width - ((number_in_row + 1) * spacing)) / number_in_row);
        // Adjust height accordingly
        scaledHeight = (int) thumbs.at(0).rows  / (thumbs.at(0).cols / (float)scaledWidth);
        // calulate the number that would end up in a clumn
        number_in_column = ((int)thumbs.size() / number_in_row) + 1;
        // If there is enough space to fit the images vertically there is a solution
        // Otherwise increas the number in the row and repeat.
        if (number_in_column * (scaledHeight + spacing) < height) {
            solution = true;
        } else {
            number_in_row++;
        }
    }
    
    // Resize each image to the correct size.
    for (int i = 0; i < thumbs.size(); i++) {
        resize(thumbs[i], thumbs[i], Size(scaledWidth, scaledHeight));
    }
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// hitOrMiss
//
// Calculate weather or not a given mouse click has landed on an image
//
//----------------------------------------------------------------------------------------------
int ThumbPreview::hitOrMiss(int x, int y) {
    
    // The largest dimention available
    float min_dist = width;
    // Distance to the center of thumbnail in x & y respectively
    float xOff = thumbs[0].cols / 2;
    float yOff = thumbs[0].rows / 2;
    // init index to -1
    int index = -1;
    
    // for each image, calculate the euclidean distance.
    // the thumbnail with the smallest distance is the best guess for now.
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
    
    // Check to see if the mouse click was outside the image or inside
    if (x > coords[index].x + thumbs[index].cols || x < coords[index].x ||
        y > coords[index].y + thumbs[index].rows || y < coords[index].y) {
        index = -1;
    }
    
    return index;
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// displayThumbnails
//
// display and wait for key press
//
//----------------------------------------------------------------------------------------------
void ThumbPreview::displayThumbnails() {
    imshow("Thumbnail", combined);
    waitKey();
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// arrangeThumbnails
//
// display thumbnails in allocated location
//
//----------------------------------------------------------------------------------------------
void ThumbPreview::arrangeThumbnails() {
    // Reset large image to remove border from scratch
    combined = Mat::zeros(Size(width,height), thumbs[0].type());
    
    // The spacing for images
    int xc = spacing;
    int yc = spacing;
    // The current row being added to
    int row_tracker = 0;
    // For each image
    //      copy image to combined
    //      & update location information
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
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// scratchThumbnails
//
// Place a red line through, a yellow box arround and dull the clicked image
//
//----------------------------------------------------------------------------------------------
void ThumbPreview::scratchThumbnail() {
    
    // construct large image
    arrangeThumbnails();
    
    Mat t = combined.clone();
    
    // how transparrent should the dulling effect be on the image
    float opacity = 0.5;
    
    // for each image that has been added to the scratch list do what is required.
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
    
    // Combine lines and boxes with final combined image
    addWeighted(t, opacity, combined, 1 - opacity, 0, combined);
    
}







