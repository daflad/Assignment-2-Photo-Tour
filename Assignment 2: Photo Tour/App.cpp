//
//  App.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 24/04/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "App.h"

void App::init(int argc, const char **argv) {
    fu.init();
    roi.init();
    
    string m = argv[1];
    
    if (m == "-fd") {
        dp = argv[3];
    } else {
        dp = argv[9];
    }
    
    
    if (fu.checkArgs(argc, argv, &roi)) {
        tp.init(&fu.filepaths, dp);
    }
    
    ia.init(&fu.filepaths, &roi.image, dp);
}


int App::run() {
    tp.arrangeThumbnails();
    tp.displayThumbnails();
    
    for (int i = 0; i < ia.images.size(); i++) {
        
        ia.detectFeaturePoints(i);
        ia.extractDescriptors(i, roi.x1, roi.y1, dp);
    }
    
    vc.writeSequence(ia.images);
    
    return 0;

}