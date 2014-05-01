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

void App::lookForNew() {

    cout << "looking" << endl;
    for (int i = 0; i < dataSet.size(); i++) {
        if (dataSet[i].newROI) {
            for (int j = 0; j < dataSet.size(); j++) {
                if (!dataSet[j].newROI && j != i && !dataSet[j].isWarped) {
                    ia.detectFeaturePoints(j, dataSet, dataSet[i].roi.image, 0.09);
                    if (ia.extractDescriptors(j, dataSet[i].roi.x1, dataSet[i].roi.y1, fu.dirpath, dataSet, dataSet[i].roi.image, 0.99, 4)) {
                        tp.arrangeThumbnails(dataSet);
                        tp.displayThumbnails();
                        waitKey(30);
                        lookForNew();
                    }
                }
            }
        }
    }
}

void App::allign() {

    
    for (int i = 0; i < dataSet.size(); i++) {
        
        ia.detectFeaturePoints(i, dataSet, roi.image, 0.04);
        ia.extractDescriptors(i, roi.x1, roi.y1, fu.dirpath, dataSet, roi.image, 0.8, 4);
        tp.arrangeThumbnails(dataSet);
        tp.displayThumbnails();
        waitKey(30);
    }
    lookForNew();
    tp.displayThumbnails();
    cout << "All Alligned!" << endl;
    alligned = true;
}
