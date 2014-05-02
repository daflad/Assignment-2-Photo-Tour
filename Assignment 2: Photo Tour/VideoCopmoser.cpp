//
//  VideoCopmoser.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 04/04/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "VideoCopmoser.h"


bool VideoCopmoser::writeSequence(vector<Image> images, vector<int> chosen) {
    
    // The next & previous frames to outputted
    Mat output;
    Mat lastFrame = images[chosen[0]].matrix.clone();

    VideoWriter outputVideo;

    // Setup output file
    outputVideo.open("anim.mov", CV_FOURCC('m', 'p', '4', 'v'), 30, images[0].matrix.size(), true);
//    outputVideo.open("Test.mov", CV_FOURCC('m', 'p', '4', 'v'), 30, Size(300, 200), true);
    
    // Number of frames for each tranition
    int NUM_FRAMES = 90;
    
    float alpha = 0;
    // Increment in alpha per frame
    float aInc = (float)1 / (NUM_FRAMES - 1);

//    resize(lastFrame, lastFrame, Size(300, 200));
    for (int i = 1; i < chosen.size(); i++) {
        for (int j = 0; j < NUM_FRAMES; j++) {
            // Update next frame
            Mat nextFrame = images[chosen[i]].matrix;
//            resize(nextFrame, nextFrame, Size(300, 200));
            // Blend frames
            addWeighted( nextFrame, alpha, lastFrame , 1 - alpha, 0.0, output);
            // Update alpha
            alpha += aInc;
//            resize(output, output, Size(300, 200));
            // Write image to video
            outputVideo.write(output);
        }
        // reset alpha for next transition
        alpha = 0.0f;
        // The current interpolated matrix
        Mat tr;
        Mat ey = Mat::eye(3, 3, images[chosen[i]].trans.type());
        for (int j = 0; j < NUM_FRAMES; j++) {
            // Blend transformation with identidy matrix
            addWeighted( images[chosen[i]].trans, 1 - alpha, ey, alpha, 0.0, tr);
            // Warp image
            warpPerspective(images[chosen[i]].orig, output, tr, images[chosen[i]].matrix.size(), 1);
//            resize(output, output, Size(300, 200));
            //Write to video
            outputVideo.write(output);
            alpha += aInc;
        }
        // reset alpha for next /ransition
        alpha = 0;
        // Update last frame
        lastFrame = output.clone();
    }
    
    // Close video file
    outputVideo.release();
    
    return true;
}

bool VideoCopmoser::writeImages(vector<Image> images, vector<int> chosen, string dp) {

    // Add frame_XX where XX = number to file name
    dp = "frame_";
    
    // add extra 0 infront of single digit numbers
    for (int i = 0; i < chosen.size(); i++) {
        string fp;
        if (i < 10) {
            // less than 10 needs 0 added
            fp = dp + to_string(0) + to_string(i);
        } else {
            fp = dp + to_string(i);
        }
        // Add extention
        fp += ".jpg";

        // Write out
        imwrite(fp.c_str(), images[chosen[i]].matrix);

        // Not sure why but this seems to do the job :D
        waitKey(30);
    }
    return true;
}