//
//  FileUtil.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "FileUtil.h"

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//
// Check arguments passed at runtime
//
// check useage and report problems
//
//----------------------------------------------------------------------------------------------
bool FileUtil::checkArgs(int argc, const char * argv[], ROI *roi) {
    
    string arg = argv[1];
    if (arg != "-fd" && arg != "-rfd") {
        // Let the user know what they might have done wrong
        cerr << "USAGE :: " << "Photo Tour -fd \"fileName.jpg\" ./" << endl;
        cerr << "USAGE :: " << "Photo Tour -rfd [ x1 y1 x2 y2 ]  \"fileName.jpg\" ./" << endl;
        return false;
    }
    
    // ROI not provided
    if (arg == "-fd") {
        dirpath = argv[3];
        return checkfd(argc, argv, roi);
    }
    
    // ROI provided
    if (arg == "-rfd") {
        dirpath = argv[9];
        return checkrfd(argc, argv, roi);
    }
    
    return false;
};

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//
// checkfd  -- User does not know the ROI in advance
//
// Check that the user has preented 3 arguments precicely.
// If not provide useage instructions.
// Load file peths if possible.
// Prompt user to select ROI in first image.
//
// TODO::
//       What if the ROI dose not complete propperly??
//
//----------------------------------------------------------------------------------------------
bool FileUtil::checkfd(int argc, const char **argv, ROI *roi) {
   
    if (argc != 4) {
        // Let the user know what they might have done wrong
        cerr << "USAGE :: " << "Photo Tour -fd \"fileName.jpg\" ./picture_dir/" << endl;
        return false;
    }
    
    if (getFilePaths(argv)) {
        roi->getROI(argv[2]);
        return true;
    }
    
    return false;
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//
// checkrfd  -- User selects ROI in advance
//
// Check that the user has preented 9 arguments precicely.
// If not provide useage instructions.
// Load file peths if possible.
// Load user selected ROI in first image.
//
// TODO::
//       What if the ROI doe not complete propperly??
//
//----------------------------------------------------------------------------------------------
bool FileUtil::checkrfd(int argc, const char **argv, ROI *roi) {
    
    if (argc != 10) {
        // Let the user know what they might have done wrong
        cerr << "USAGE :: " << "Photo Tour -rfd [ x1 y1 x2 y2 ]  \"fileName.jpg\" ./" << endl;
        return false;
    }

    if (getFilePaths(argv)) {
        int cord[4] = { atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]) };
        return roi->getROI(argv[8], cord);
    }
    
    return false;
}

//----------------------------------------------------------------------------------------------
//
// getFilePaths
//
// Load all file paths into the designated vector for retieval later.
// Present any problems to the user.
//
//----------------------------------------------------------------------------------------------
bool FileUtil::getFilePaths(const char * argv[]) {
    // Retieve the flag inputted, exit if neither.
    string flag = argv[1];
    string dp;
    if (flag == "-fd") {
        dp = argv[3];
    } else if (flag == "-rfd") {
        dp = argv[9];
    } else {
        return false;
    }

    
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (dp.c_str())) != NULL) {
        // list all jpg in a directory
        int c = 0;
        while ((ent = readdir (dir)) != NULL) {
            string name = ent->d_name;
            string ft = ".jpg";
            string FT = ".JPG";
            if (name.find(ft) != string::npos || name.find(FT) != string::npos) {
                filepaths.push_back(ent->d_name);
                c++;
            }
        }
//        printf ("Success :: %d jpg added to list\n", c);
        closedir (dir);
        return true;
    } else {
        // could not open directory
        perror ("ERROR :: Plase provide a valid file path to a folder including jpg images.");
        return false;
    }
    
}

//----------------------------------------------------------------------------------------------
//
// loadImages
//
// Load all images from the file paths provided.
//
//----------------------------------------------------------------------------------------------
vector<Image> FileUtil::loadImages() {
    vector<Image> imList;
    for (int i = 0; i < filepaths.size(); i++) {
        Image img;
        img.init(filepaths[i], dirpath, 0, 0, i);
        imList.push_back(img);
    }
    return imList;
}















