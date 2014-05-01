//
//  VideoCopmoser.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 04/04/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "VideoCopmoser.h"


bool VideoCopmoser::writeSequence(vector<Image> images, vector<int> chosen) {
    
    Mat output;
    
    VideoWriter outputVideo;
    outputVideo.open("Test.mov", CV_FOURCC('m', 'p', '4', 'v'), 30, images[0].matrix.size(), true);
    
    int NUM_FRAMES = 120;
    
    float aInc = (float)1 / NUM_FRAMES;
    
    float alpha = 0;
    
    
    
    for (int i = 1; i < chosen.size(); i++) {
        images[chosen[i-1]].matrix.copyTo(output);
        for (int j = 0; j < NUM_FRAMES; j++) {
            addWeighted( images[chosen[i]].matrix, alpha, images[chosen[i - 1]].matrix, 1 - alpha, 0.0, output);
            alpha += aInc;
            outputVideo.write(output);
        }
        alpha = 0.0f;
        Mat tr;
        Mat ey = Mat::eye(3, 3, images[chosen[i]].trans.type());
        for (int j = 0; j < NUM_FRAMES; j++) {
            addWeighted( images[chosen[i]].trans, 1 - alpha, ey, alpha, 0.0, tr);
            warpPerspective(images[chosen[i]].orig, output, tr, images[chosen[i]].matrix.size(), 1);
            outputVideo.write(output);
            alpha += aInc;
        }
        alpha = 0;
//        for (int j = 0; j < NUM_FRAMES; j++) {
//            addWeighted( images[chosen[i]].trans, alpha, ey, 1 - alpha, 0.0, tr);
//            warpPerspective(images[chosen[i]].orig, output, tr, images[chosen[i]].matrix.size(), 1);
//            outputVideo.write(output);
//            alpha += aInc;
//        }
//        alpha = 0;
    }
    
    outputVideo.release();
    
    return true;
}

bool VideoCopmoser::writeImages(vector<Image> images, vector<int> chosen, string dp) {

    dp = "frame_";
    
    for (int i = 0; i < chosen.size(); i++) {
        string fp;
        if (i < 10) {
            fp = dp + to_string(0) + to_string(i);
        } else {
            fp = dp + to_string(i);
        }
        fp += ".jpg";
        cout << fp << endl;
        imwrite(fp.c_str(), images[chosen[i]].matrix);
        waitKey(30);
    }
    return true;
}