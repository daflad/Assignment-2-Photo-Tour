//
//  FileUtil.h
//  Assignment 2: Photo Tour
//
//
//
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#ifndef __Assignment_2__Photo_Tour__FileUtil__
#define __Assignment_2__Photo_Tour__FileUtil__

#include <iostream>
#include "Image.h"

using namespace std;

class FileUtil {

public:

    // Setup class vars
    void init();
    
    // Check input arguments and take appropriate action
    bool checkArgs(int argc, const char * argv[]);

    // Load all imges in a given array of file paths
    Image* loadImages(string* filePaths);
    
    
    
private:


};

#endif /* defined(__Assignment_2__Photo_Tour__FileUtil__) */
