//
//  App.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 24/04/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "App.h"

void App::init(int argc, const char **argv) {

    roi.init();
    
    if (fu.checkArgs(argc, argv, &roi)) {
        dataSet = fu.loadImages();
        tp.init(dataSet, fu.dirpath);
    }
    
    ia.init();
}


int App::run() {
    tp.arrangeThumbnails(dataSet);
    tp.displayThumbnails(true);
    
    for (int i = 0; i < dataSet.size(); i++) {
        
        ia.detectFeaturePoints(i, dataSet, roi.image);
        ia.extractDescriptors(i, roi.x1, roi.y1, fu.dirpath, dataSet, roi.image);
        tp.arrangeThumbnails(dataSet);
        tp.displayThumbnails(false);
    }
    tp.displayThumbnails(true);
    
    vc.writeSequence(dataSet);
    
    return 0;

}