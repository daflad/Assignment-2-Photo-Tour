//
//  VideoCopmoser.h
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 04/04/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#ifndef __Assignment_2__Photo_Tour__VideoCopmoser__
#define __Assignment_2__Photo_Tour__VideoCopmoser__

#include <iostream>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>  
#include "Image.h"

using namespace std;
using namespace cv;

class VideoCopmoser {
    
public:
    
    bool writeSequence(vector<Image>, vector<int>);
    bool writeImages(vector<Image>, vector<int>, string);
};



#endif /* defined(__Assignment_2__Photo_Tour__VideoCopmoser__) */
