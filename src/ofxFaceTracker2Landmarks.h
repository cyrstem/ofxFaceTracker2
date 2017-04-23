#pragma once
#include "ofConstants.h"
#include "ofGraphics.h"
#include "ofxCv.h"

#include "ofxFaceTracker2InputInfo.h"

#include <dlib/image_processing.h>

class ofxFaceTracker2Landmarks {
public:
    
    enum Feature {
        LEFT_EYE_TOP, RIGHT_EYE_TOP,
        
        LEFT_EYEBROW, RIGHT_EYEBROW,
        LEFT_EYE, RIGHT_EYE,
        LEFT_JAW, RIGHT_JAW, JAW,
        OUTER_MOUTH, INNER_MOUTH,
        NOSE_BRIDGE, NOSE_BASE,
        FACE_OUTLINE, ALL_FEATURES
    };
    
    ofxFaceTracker2Landmarks(dlib::full_object_detection shape, ofxFaceTracker2InputInfo & info);
    
    /// Get specific 2D image point from the 68 landmarks coordinates
    /// Coordinate is returned in input image size
    ofVec2f getImagePoint(int i) const;
    
    /// Get a list of 2D points with all 68 face landmarks found.
    /// Coordinates are returned in input image size
    vector<ofVec2f> getImagePoints() const;
    vector<ofVec2f> getImagePointsMod() const;
    
    /// Like getImagePoints, just returns the points as cv:Point2f
    vector<cv::Point2f> getCvImagePoints() const;
    
    /// Get poly line of a feature
    ofPolyline getImageFeature(Feature feature) const;
    
    ofMesh getImageMesh() const;
    
    template <class T> ofMesh getMesh(vector<T> points) const;
    template <class T> ofMesh getMeshFromImagePoint(vector<T> points) const;//added to test a new getMesh method, from old version of tracker

    
private:
    void addTriangleIndices(ofMesh& mesh) const;//added to test a new getMesh method, from old version of tracker

    dlib::full_object_detection shape;
    ofxFaceTracker2InputInfo & info;
    
    static std::vector<int> consecutive(int start, int end);
    static vector<int> getFeatureIndices(Feature feature);
    
    template <class T>
    ofPolyline getFeature(Feature feature, vector<T> points) const;
    
    //Multimidentional Array specifying the mesh points used to generate the mesh
    int faceTriCols = 91;
    int faceTri[3][91] = {
        {20, 21, 0, 15, 0, 16, 17, 25, 17, 26, 18, 24, 18, 25, 19, 23, 20, 22, 20, 23, 21, 21, 22, 27, 27, 28, 28, 1, 15, 1, 14, 28, 28, 2, 14, 2, 14, 2, 13, 29, 29, 3, 13, 30, 30, 30, 30, 3, 12, 4, 11, 5, 10, 6, 10, 6, 9, 7, 9, 8, 8, 7, 8, 4, 12, 31, 35, 31, 35, 31, 34, 32, 33, 33, 33, 48, 49, 50, 50, 51, 61, 52, 53, 54, 55, 56, 56, 64, 57, 58, 48 },   /*  initializers for row indexed by 0 */
        {21, 22, 1, 16, 17, 26, 18, 26, 36, 44, 19, 25, 37, 43, 20, 24, 21, 23, 38, 42, 22, 27, 27, 28, 28, 42, 39, 36, 45, 2, 15, 29, 29, 40, 46, 29, 29, 3, 14, 30, 30, 29, 29, 32, 33, 31, 34, 4, 13, 5, 12, 6, 11, 48, 54, 7, 10, 58, 55, 57, 56, 8, 9, 31, 35, 48, 53, 49, 52, 32, 35, 33, 34, 50, 51, 49, 60, 60, 51, 52, 62, 53, 54, 55, 56, 63, 57, 65, 58, 59, 59},   /*  initializers for row indexed by 1 */
        {23, 23, 36, 45, 36, 45, 37, 44, 37, 45, 38, 43, 38, 44, 38, 43, 39, 42, 39, 43, 27, 39, 42, 42, 39, 47, 40, 41, 46, 41, 46, 40, 47, 41, 47, 40, 47, 29, 29, 31, 35, 31, 35, 33, 34, 32, 35, 31, 35, 48, 54, 48, 54, 59, 55, 59, 55, 59, 56, 58, 57, 58, 56, 48, 54, 49, 54, 50, 53, 50, 52, 50, 52, 51, 52, 60, 50, 61, 61, 61, 52, 62, 62, 63, 63, 64, 64, 57, 65, 65, 65 }   /*  initializers for row indexed by 2 */
    };

};
