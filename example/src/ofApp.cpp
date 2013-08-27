#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    image.loadImage("img.jpg");
    
    dst = image.getPixelsRef();
    
    imgWarp.setup(dst.getWidth(), dst.getHeight(), 50);
    
    tex.allocate(dst);
    tex.loadData(dst);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
     tex.draw(0,0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    ctrl.back().second = ofPoint(x,y);
    imgWarp.calcDelta(ctrl);
    imgWarp.genNewImg(image.getPixelsRef(),dst,1);
    tex.loadData(dst);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    ctrl.push_back(make_pair( ofPoint(x,y),ofPoint(x,y)));
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
