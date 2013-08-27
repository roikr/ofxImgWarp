//
//  ofxImgWarp.h
//  mlsExample
//
//  Created by Roee Kremer on 7/15/13.
//
//

#pragma once

#include "ofMain.h"


class ofxImgWarp {
public:
    void setup(int width,int height,int gridSize);
    void calcDelta(vector<pair<ofPoint,ofPoint> > &ctrl);
    void genNewImg(ofPixels &src,ofPixels &dst, float transRatio);
    
private:
    
    int gridSize;
    ofFloatPixels rDx,rDy;
};