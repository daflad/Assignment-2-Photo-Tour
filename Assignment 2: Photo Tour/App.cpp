//
//  App.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 24/04/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "App.h"
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//
// Init class vars
//
//----------------------------------------------------------------------------------------------
void App::init(int argc, const char **argv) {
    alligned = false;
    exit = true;
    writeIMG = false;
    writeVID = false;
    roi.init();
    
    if (fu.checkArgs(argc, argv, &roi)) {
        dataSet = fu.loadImages();
        tp.init(dataSet, fu.dirpath);
        readMe();
        exit = false;
    }
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//
// Main function running whilst program active
//
//----------------------------------------------------------------------------------------------
int App::run() {
    
    // Wait for exit call
    while (!exit) {
        
        // Start by arranging thumbs
        tp.arrangeThumbnails(dataSet);
        tp.displayThumbnails();
        
        // Only do allignment if not done yet
        if (!alligned) {
            allign();
        }
        
        // Check the keys
        int wk = waitKey();
        keyCheck(wk);
        
        // Isolate selected images
        vector<int> chosen;
        for (int i = 0; i < dataSet.size(); i++) {
            bool hit = false;
            for (int j = 0; j < tp.scratched.size(); j++) {
                if (tp.scratched[j] == i) {
                    hit = true;
                }
            }
            // Add all non hit & non warped images
            if (!hit && dataSet[i].isWarped) {
                chosen.push_back(i);
            }
        }
        
        if (writeVID) {
            cout << "writing video" << endl;
            vc.writeSequence(dataSet, chosen);
            writeVID = false;
        }
        
        if (writeIMG) {
            cout << "writing images" << endl;
            vc.writeImages(dataSet, chosen, fu.dirpath);
            writeIMG = false;
        }
        
    }
    
    return 0;
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//
// keyCheck
//
// Set bools to control actions based on key strokes
//
//----------------------------------------------------------------------------------------------
void App::keyCheck(int wk) {

    if (wk == 27) {
        exit = true;
    } else if (wk == 105) {
        writeIMG = true;
    } else if (wk == 97) {
        writeVID = true;
    } else if (wk == 115) {
        tp.showUnwarped = !tp.showUnwarped;
    }
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//
// lookForNew
//
// look for new roi's found in search
//
//----------------------------------------------------------------------------------------------
bool App::lookForNew(int ind, bool once) {
    bool lessThan = false;
//    cout << "looking " << ind << endl;
    // For each image
    for (int j = 1; j < dataSet.size(); j++) {
        // find non warped & non failed images
        if (!dataSet[j].isWarped && !dataSet[j].failed && j != ind) {
            // Try to alllign but catch exceptions
            try {
                ia.detectFeaturePoints(j, dataSet, dataSet[ind].roi.image, 0.0025, 0.05);
                ia.extractDescriptors(j, fu.dirpath, dataSet, dataSet[ind].roi.image, 0.7, 16, 0.01, ind);
                ia.pruneResults(16);
                if (ia.ransac(j, dataSet, dataSet[ind].roi.x1, dataSet[ind].roi.y1, dataSet[ind].roi.image, 0.7, ind)) {
                    // if j < less than ind then we have passed the point where
                    // j wil get checked again so check it when you get back!
                    if (j < ind) {
                        lessThan = true;
                    }
                    tp.arrangeThumbnails(dataSet);
                    tp.displayThumbnails();
                    // short delay to see results as they grow
                    waitKey(30);
                    // dont want to get stuck in a loop!!
                    if (j > ind && once) {
                        lookForNew(j, true);
                    } else {
                        lookForNew(j, false);
                    }
                }
            } catch( cv::Exception& e ) {
                dataSet[j].failed = true;
//                const char* err_msg = e.what();
//                std::cout << "exception caught: " << err_msg << std::endl;
            }
        }
    }
    return lessThan;
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//
// Helper to assist recursion.
//
//----------------------------------------------------------------------------------------------
void App::looking() {
    // For each image find ones with rois too look again
    for (int i = 1; i < dataSet.size(); i++) {
        if (dataSet[i].isWarped && !dataSet[i].failed) {
            if (lookForNew(i, true)) {
                looking();
            }
        }
    }
}
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//
// Allign
//
// Perform allignment
//
//----------------------------------------------------------------------------------------------
void App::allign() {

    // Allign images to first ROI
    for (int i = 0; i < dataSet.size(); i++) {
        ia.detectFeaturePoints(i, dataSet, roi.image, 0.025, 0.05);
        ia.extractDescriptors(i, fu.dirpath, dataSet, roi.image, 0.7, 9, 0.01, 0);
        ia.pruneResults(9);
        if (ia.ransac(i, dataSet, roi.x1, roi.y1, roi.image, 0.7, 0)) {
            tp.arrangeThumbnails(dataSet);
            tp.displayThumbnails();
            // Show results
            waitKey(30);
        }
    }
    // Allign images to all other ROIs
    looking();

    // once alligned show all images
    tp.showUnwarped = true;
    tp.arrangeThumbnails(dataSet);
    tp.displayThumbnails();
    // Just incase the user is looking here!
    cout << "All Alligned!" << endl;
    alligned = true;
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//
// Display README to user
//
//----------------------------------------------------------------------------------------------
void App::readMe() {
    String msg;
    msg += "Insructions\n";
    msg += "***********\n\n";
    msg += "1 ~ Select the ROI from the first window.\n";
    msg += "2 ~ Wait for allignment to complete. (All images will be visible)\n";
    msg += "3 ~ Select images to exclude from output.\n";
    msg += "4 ~ Select an output (Video / Image Sequence.\n\n";
    msg += "\tWhen selecting images, clicking the black space\n";
    msg += "\tsurrounding the images will clear the selction\n\n";
    
    msg += "KEY\n";
    msg += "***\n\n";
    msg += "\tesc = exit\n";
    msg += "\ta   = Output Video File\n";
    msg += "\ti   = Output Image Sequence\n";
    msg += "\ts   = Show/Hide Un-Warped Images (Can't use while selecting)\n";
    
    cout << msg << endl;
}

