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
vector<Image> imgs;

void MouseCallBackFunc(int event, int x, int y, int flags, void* userdata) {
    
    // Retieve the class object as this function is not part of the class
    ThumbPreview* tp = static_cast<ThumbPreview*>( userdata );

    // ******************************************************************** //
    // FOR NOW, CLICKING OUTSIDE AN IMAGE WILL CLEAR THE SCRATCHED ARRAY.   //
    // THIS IS TOO FIDLEY FOR THE FINAL PROGRAM, BETTER TO PROVIDE A BUTTON //
    // OR A KEY PRESS FOR THE USER                                          //
    // ******************************************************************** //
    if (event == CV_EVENT_LBUTTONUP) {
        int index = tp->hitOrMiss(x, y);
        // if we have a hit then add it to the list
        if (index > -1){
            tp->scratched.push_back(index);
            tp->scratchThumbnail(imgs);
        } else {
            tp->scratched.clear();
            tp->arrangeThumbnails(imgs);
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
void ThumbPreview::init(vector<Image> &img, string dp) {
    
    // Default values
    number_in_row   = 5;
    spacing         = 20;
    width           = 1200;
    height          = 700;
    dir_path        = dp;
    MATRIX_TYPE     = img[0].matrix.type();
    showUnwarped    = false;
    combined        = Mat::zeros(Size(width,height), MATRIX_TYPE);
    
    
    // Open Preview window & define callback
    namedWindow("Thumbnail");
    setMouseCallback("Thumbnail", MouseCallBackFunc, this);
    
    // the final width of the thumbnails & an idicator to the solution
    bool solution = false;    
    
    // Keep going till you find an answer
    while (!solution) {
        
        // Start by scaling the image width based on 5 images & given spacing
        scaledWidth = (int)((float)(width - ((number_in_row + 1) * spacing)) / number_in_row);
        // Adjust height accordingly
        scaledHeight = (int) img[0].matrix.rows  / (img[0].matrix.cols / (float)scaledWidth);
        // calulate the number that would end up in a clumn
        number_in_column = ((int)img.size() / number_in_row) + 1;
        // If there is enough space to fit the images vertically there is a solution
        // Otherwise increas the number in the row and repeat.
        if (number_in_column * (scaledHeight + spacing) < height) {
            solution = true;
        } else {
            number_in_row++;
        }
    }
    
    xOff = scaledWidth / 2;
    yOff = scaledHeight / 2;
//    cout << "x :: " << xOff << " y :: " << yOff << endl;
    // Resize each image to the correct size.
    for (int i = 0; i < img.size(); i++) {
        resize(img[i].thumbnail, img[i].thumbnail, Size(scaledWidth, scaledHeight));
    }
    imgs            = img;
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
    if (x > coords[index].x + scaledWidth || x < coords[index].x ||
        y > coords[index].y + scaledHeight || y < coords[index].y) {
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
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// arrangeThumbnails
//
// display thumbnails in allocated location
//
//----------------------------------------------------------------------------------------------
void ThumbPreview::arrangeThumbnails(vector<Image> &img) {
    // Reset large image to remove border from scratch
    combined = Mat::zeros(Size(width,height), MATRIX_TYPE);
    coords.clear();
    // The spacing for images
    int xc = spacing;
    int yc = spacing;
    // The current row being added to
    int row_tracker = 0;
    // For each image
    //      copy image to combined
    //      & update location information
    for (int i = 0; i < img.size(); i++) {
        img[i].updateThumb();
        resize(img[i].thumbnail, img[i].thumbnail, Size(scaledWidth, scaledHeight));
        Mat t = img[i].thumbnail;
        if (img[i].isWarped || showUnwarped) {
            t.copyTo(combined(Rect(Point(xc, yc),Point(xc + t.cols, yc + t.rows))));
        }
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
void ThumbPreview::scratchThumbnail(vector<Image> &img) {
    
    // construct large image
    arrangeThumbnails(img);
    
    Mat t = combined.clone();
    
    // how transparrent should the dulling effect be on the image
    float opacity = 0.5;
    
    // for each image that has been added to the scratch list do what is required.
    for (int i = 0; i < scratched.size(); i++) {
        int index = scratched[i];
        Point p1 = coords[index];
        Point p2 = Point(p1.x + scaledWidth, p1.y + scaledHeight);
        rectangle(t, p1, p2, Scalar( 0, 0, 50), CV_FILLED, 8, 0);
        rectangle(combined, p1, p2, Scalar( 0, 255, 255), 2, 8, 0);
        line(combined, p2, p1, Scalar( 0, 0, 255), 2, 8, 0);
    }
    
    // Combine lines and boxes with final combined image
    addWeighted(t, opacity, combined, 1 - opacity, 0, combined);
    
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// setImage
//
//----------------------------------------------------------------------------------------------
void ThumbPreview::setImage(int index, Mat *matrix, vector<Image> &img) {
    Image temp;
    Image temp2;
    temp.init("", "", 0, 0, index);
    temp.matrix = *matrix;
    temp.updateThumb();
    temp2 = img[index];
    img[index] = temp;
    img.push_back(temp2);
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// setSelection
//
//----------------------------------------------------------------------------------------------
void ThumbPreview::setSelection(int index, bool sel) {
    if (sel) {
        scratched.push_back(index);
    } else {
        scratched.erase(scratched.begin() + index);
    }
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
// getSelection
//
//----------------------------------------------------------------------------------------------
bool ThumbPreview::getSelection(int index) {
    bool in = false;
    for (int i = 0; i < scratched.size(); i++) {
        if (scratched[i] == index) {
            in = true;
            break;
        }
    }
    return in;
}


