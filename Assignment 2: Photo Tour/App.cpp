//
//  App.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 24/04/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "App.h"

void App::init(int argc, const char **argv) {
    alligned = false;
    exit = false;
    writeIMG = false;
    writeVID = false;
    roi.init();
    
    if (fu.checkArgs(argc, argv, &roi)) {
        dataSet = fu.loadImages();
        tp.init(dataSet, fu.dirpath);
    }
    
    ia.init();
}


int App::run() {
    
    while (!exit) {
        
        tp.arrangeThumbnails(dataSet);
        tp.displayThumbnails();
        
        if (!alligned) {
            allign();
        }
        
        int wk = waitKey();
        keyCheck(wk);
        
        vector<int> chosen;
        
        for (int i = 0; i < dataSet.size(); i++) {
            bool hit = false;
            for (int j = 0; j < tp.scratched.size(); j++) {
                if (tp.scratched[j] == i) {
                    hit = true;
                }
            }
            if (!hit) {
                chosen.push_back(i);
            }
        }
        
        if (writeVID) {
            cout << "writing video" << endl;
            writeVideo(chosen);
            writeVID = false;
        }
        
        if (writeIMG) {
            cout << "writing images" << endl;
            writeImages(chosen);
            writeIMG = false;
        }
        
    }
    
    return 0;
}

void App::keyCheck(int wk) {
    
    cout << "Key " << wk << endl;
    
    if (wk == 27) {
        exit = true;
    } else if (wk == 105) {
        writeIMG = true;
    } else if (wk == 97) {
        writeVID = true;
    }
}

void App::writeVideo(vector<int> chosen) {
    vc.writeSequence(dataSet, chosen);
}

void App::writeImages(vector<int> chosen) {
    vc.writeImages(dataSet, chosen, fu.dirpath);
}

bool App::lookForNew(int ind, bool once) {
    bool lessThan = false;
    cout << "looking " << ind << endl;
    for (int j = 1; j < dataSet.size(); j++) {
        if (!dataSet[j].isWarped && !dataSet[j].failed && j != ind) {
            try {
                ia.detectFeaturePoints(j, dataSet, dataSet[ind].roi.image, 0.0025, 0.05);
                if (ia.extractDescriptors(j, dataSet[ind].roi.x1, dataSet[ind].roi.y1, fu.dirpath, dataSet, dataSet[ind].roi.image, 0.7, 16, 0.01, ind)) {
                    if (j < ind) {
                        lessThan = true;
                    }
                    tp.arrangeThumbnails(dataSet);
                    tp.displayThumbnails();
                    waitKey(30);
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

void App::looking() {
    for (int i = 1; i < dataSet.size(); i++) {
        if (dataSet[i].isWarped && !dataSet[i].failed) {
            if (lookForNew(i, true)) {
                looking();
            }
        }
    }
}

void App::allign() {

    
    for (int i = 0; i < dataSet.size(); i++) {
        ia.detectFeaturePoints(i, dataSet, roi.image, 0.0025, 0.05);
        if (ia.extractDescriptors(i, roi.x1, roi.y1, fu.dirpath, dataSet, roi.image, 0.7, 9, 0.01, 0)) {
            tp.arrangeThumbnails(dataSet);
            tp.displayThumbnails();
            waitKey(30);
        }
    }
    
    looking();
    
//    for (int i = 1; i < dataSet.size(); i++) {
//        ia.detectFeaturePoints(dataSet[i].warpInd, dataSet, roi.image, 0.0025, 0.1);
//        if (ia.extractDescriptors(dataSet[i].warpInd, dataSet[dataSet[i].warpInd].roi.x1, dataSet[dataSet[i].warpInd].roi.y1, fu.dirpath, dataSet, roi.image, 0.67, 30, 0.01, dataSet[i].warpInd)) {
//            tp.arrangeThumbnails(dataSet);
//            tp.displayThumbnails();
//            waitKey(30);
//        }
//    }
    tp.displayThumbnails();
    cout << "All Alligned!" << endl;
    alligned = true;
}
