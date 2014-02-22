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

using namespace std;

int main(int argc, const char * argv[])
{
    FileUtil fu;
    ThumbPreview tp;
    
    if (fu.checkArgs(argc, argv)) {
        tp.init(&fu.filepaths, argv[9]);
        tp.displayThumbnails();
    }
    return 0;
}

