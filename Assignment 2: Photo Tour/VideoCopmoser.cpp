//
//  VideoCopmoser.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 04/04/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "VideoCopmoser.h"


bool VideoCopmoser::writeSequence(vector<Image> images) {
    
    Mat output;
    
    VideoWriter outputVideo;
    outputVideo.open("Test.mov", CV_FOURCC('m', 'p', '4', 'v'), 30, images[0].matrix.size(), true);
    
    int NUM_FRAMES = 120;
    
    float aInc = (float)1 / NUM_FRAMES;
    
    float alpha = 0;
    
    
    for (int i = 1; i < images.size(); i++) {
        output = images[i-1].matrix;
        for (int j = 0; j < NUM_FRAMES; j++) {
            addWeighted( images[i].matrix, alpha, images[i - 1].matrix, 1 - alpha, 0.0, output);
            alpha += aInc;
            outputVideo.write(output);
        }
        alpha = 0;
    }
    
    outputVideo.release();
    
    return true;
}