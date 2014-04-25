//
//  App.h
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 24/04/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#ifndef __Assignment_2__Photo_Tour__App__
#define __Assignment_2__Photo_Tour__App__

#include <iostream>
#include "FileUtil.h"
#include "ThumbPreview.h"
#include "ImageAllignment.h"
#include "VideoCopmoser.h"

using namespace std;

class App {
    
public:
    
    void init(int argc, const char * argv[]);
    
    int run();
    
private:
    
    string          dp;
    vector<Image>   dataSet;
    FileUtil        fu;
    ROI             roi;
    ThumbPreview    tp;
    VideoCopmoser   vc;
    ImageAllignment ia;
    
};

#endif /* defined(__Assignment_2__Photo_Tour__App__) */
