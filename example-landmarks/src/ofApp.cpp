#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofLog(OF_LOG_NOTICE, "INIT SETUP");
    // All examples share data files from example-data, so setting data path to this folder
    // This is only relevant for the example apps
    ofSetDataPathRoot("../../../data/");//ofFile(__BASE_FILE__).getEnclosingDirectory()+
    
    // Setup grabber
    grabber.setup(1280,720);
    
    // Setup tracker
    tracker.setup();
    ofLog(OF_LOG_NOTICE, "END SETUP");

}

//--------------------------------------------------------------
void ofApp::update(){
    grabber.update();
    
    // Update tracker when there are new frames
    if(grabber.isFrameNew()){
        tracker.update(grabber);
    }
    
   // ofLog(OF_LOG_NOTICE, "instance",instance);

}

//--------------------------------------------------------------
void ofApp::draw(){
    // Draw camera image
    grabber.draw(0, 0);
    
    // Draw tracker landmarks
    tracker.drawDebug();
    
    // Draw estimated 3d pose
    tracker.drawDebugPose();
    vector<int> teste_0;
    int test = 42;
    vector <ofxFaceTracker2Instance> instances = tracker.getInstances();
    ofSetColor(255,255,255);    //set te color to white

    if (tracker.size() > 0 ){
        ofLog(OF_LOG_NOTICE, "got a detected face");
        ofxFaceTracker2Instance instance = tracker.getInstances()[0];
        vector<ofVec2f> points =  instance.getLandmarks().getImagePoints();
        int listL = points.size();
        
        ofVec2f point;
  
        for (int i=0; i<listL; i++) {
            point = points[i];
            ofDrawCircle(point.x,point.y,3);
        }

    }
    
    // Draw text UI
    ofDrawBitmapStringHighlight("Framerate : "+ofToString(ofGetFrameRate()), 10, 20);
    ofDrawBitmapStringHighlight("Tracker thread framerate : "+ofToString(tracker.getThreadFps()), 10, 40);
    
#ifndef __OPTIMIZE__
    ofSetColor(ofColor::red);
    ofDrawBitmapString("Warning! Run this app in release mode to get proper performance!",10,60);
    ofSetColor(ofColor::white);
#endif
}

//--------------------------------------------------------------
void ofApp::drawLandmarkPoints(){

}
