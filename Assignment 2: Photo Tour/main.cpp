//
//  main.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "FileUtil.h"
#include "ThumbPreview.h"
#include "ImageAllignment.hpp"



using namespace std;

int main(int argc, const char * argv[])
{
    FileUtil fu;
    fu.init();
    ThumbPreview tp;
    
    string dp = argv[9];
    
    if (fu.checkArgs(argc, argv)) {
        tp.init(&fu.filepaths, dp);
        tp.displayThumbnails();
    }
    
    ImageAllignment ia;
    ia.init(&fu.filepaths, &fu.roi.image, dp);
    ia.detectFeaturePoints();
    ia.extractDescriptors();
    
    return 0;
}

