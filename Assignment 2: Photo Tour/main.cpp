//
//  main.cpp
//  Assignment 2: Photo Tour
//
//  Created by Stephen John Russell on 22/02/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include <iostream>
#include "App.h"

int main(int argc, const char * argv[]) {
    
    App app;
    app.readMe();
    app.init(argc, argv);
    return app.run();
}