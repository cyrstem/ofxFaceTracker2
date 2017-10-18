#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// All examples share data files from example-data, so setting data path to this folder
    // This is only relevant for the example apps
    

    // Setup grabber
    grabber.setup(1280,720);
    //copy shape_predictor_68_face_landmarks.dat to bin/data/model
    // Setup tracker
    tracker.setup(ofToDataPath("model/shape_predictor_68_face_landmarks.dat"));
}

//--------------------------------------------------------------
void ofApp::update(){
	grabber.update();

    // Update tracker when there are new frames
    if(grabber.isFrameNew()){
        tracker.update(grabber);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
	// Draw camera image
    grabber.draw(0, 0);

    // Draw tracker landmarks
    tracker.drawDebug();

    // Draw estimated 3d pose
    tracker.drawDebugPose();
    vector<ofxFaceTracker2Instance> instances =  tracker.getInstances();
    for (auto instance : instances){
        ofMesh mesh = instance.getLandmarks().getImageMesh();
        mesh.drawWireframe();
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
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::lostFocus(){

}

//--------------------------------------------------------------
void ofApp::gotFocus(){

}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){

}
