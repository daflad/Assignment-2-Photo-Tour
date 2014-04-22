//
//  main.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include <iostream>
#include "FileUtil.h"
#include "ThumbPreview.h"
#include "ImageAllignment.h"
#include "VideoCopmoser.h"


using namespace std;

int main(int argc, const char * argv[]) {
    
    FileUtil fu;
    fu.init();
    ThumbPreview tp;
    VideoCopmoser vc;
    
    string m = argv[1];
    string dp;
    
    if (m == "-fd") {
        dp = argv[3];
    } else {
        dp = argv[9];
    }

    
    if (fu.checkArgs(argc, argv)) {
        tp.init(&fu.filepaths, dp);
    }
    
    ImageAllignment ia;
    ia.init(&fu.filepaths, &fu.roi.image, dp);
    
    
    for (int i = 0; i < ia.images.size(); i++) {

        ia.detectFeaturePoints(i);
        ia.extractDescriptors(i, fu.roi.x1, fu.roi.y1, dp);
    }
    
    vc.writeSequence(ia.images);

    
    return 0;
}