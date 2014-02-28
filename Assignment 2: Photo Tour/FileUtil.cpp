//
//  FileUtil.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "FileUtil.h"
#include <dirent.h>

void FileUtil::init() {
    roi.init();
}

// Check arguments passed at runtime
//
// check useage and report problems
bool FileUtil::checkArgs(int argc, const char * argv[]) {
    
    string arg = argv[1];
    if (arg != "-fd" && arg != "-rfd") {
        // Let the user know what they might have done wrong
        cerr << "USAGE :: " << "Photo Tour -fd \"fileName.jpg\" ./" << endl;
        cerr << "USAGE :: " << "Photo Tour -rfd [ x1 y1 x2 y2 ]  \"fileName.jpg\" ./" << endl;
        return false;
    }
    
    if (arg == "-fd") {
         return checkfd(argc, argv);
    }
    
    if (arg == "-rfd") {
        return checkrfd(argc, argv);
    }
    
    return true;
};
//----------------------------------------------------------------------------------------------

bool FileUtil::checkfd(int argc, const char **argv) {
    if (argc != 4) {
        // Let the user know what they might have done wrong
        cerr << "USAGE :: " << "Photo Tour -fd \"fileName.jpg\" ./picture_dir/" << endl;
        return false;
    }
    
    getFilePaths(argv);
    
    
    roi.getROI(argv[8]);
    
    return true;
}
//----------------------------------------------------------------------------------------------

bool FileUtil::checkrfd(int argc, const char **argv) {
    
    if (argc != 10) {
        // Let the user know what they might have done wrong
        cerr << "USAGE :: " << "Photo Tour -rfd [ x1 y1 x2 y2 ]  \"fileName.jpg\" ./" << endl;
        return false;
    }

    getFilePaths(argv);
    
    int cord[4] = { atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]) };
    roi.getROI(argv[8], cord);
    
    return true;
}
//----------------------------------------------------------------------------------------------

void FileUtil::getFilePaths(const char * argv[]) {
    string dp = argv[9];
    
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (dp.c_str())) != NULL) {
        // list all jpg in a directory
        int c = 0;
        while ((ent = readdir (dir)) != NULL) {
            string name = ent->d_name;
            string ft = ".jpg";
            if (name.find(ft) != string::npos) {
                filepaths.push_back(ent->d_name);
                c++;
            }
        }
        printf ("%d jpg added to list\n", c);
        closedir (dir);
    } else {
        // could not open directory
        perror ("ERROR");
    }
    
}















