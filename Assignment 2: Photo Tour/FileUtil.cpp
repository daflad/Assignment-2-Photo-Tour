//
//  FileUtil.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "FileUtil.h"

void FileUtil::init() {
    
}

bool FileUtil::checkArgs(int argc, const char * argv[]) {
    
    if (argc > 3) {
        printf("we have %d args\n", argc);
        for (int i = 1; i < argc; i++) {
            cout << argv[i] <<  endl;
        }
    } else {
        cout << "Not enough parameters passed" << endl;
    }
    
    return true;
};