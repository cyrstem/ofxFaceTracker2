#include "ofxFaceTracker2Landmarks.h"
#ifdef DEBUG
#define it at<int>
#define db at<double>
#endif

ofxFaceTracker2Landmarks::ofxFaceTracker2Landmarks(dlib::full_object_detection shape, ofxFaceTracker2InputInfo & info) : shape(shape), info(info){
    
    #ifdef DEBUG
    if(!ofFile("model/face.tri").exists()) {
        ofLogError() << "Make sure you've placed the files face2.tracker, face.tri and face.con in the data/model/ folder.";
        ofExit();
    }
    
    string triFile = ofToDataPath("model/face.tri");
    
    tri = LoadTri(triFile.c_str());
#endif
}




ofVec2f ofxFaceTracker2Landmarks::getImagePoint(int i) const {
    ofVec3f p = ofVec3f(shape.part(i).x(),
                        shape.part(i).y(), 0);
    p = p * info.rotationMatrix;
    
    return ofVec2f(p);
}

vector<ofVec2f> ofxFaceTracker2Landmarks::getImagePoints() const {
    int n = shape.num_parts();
    vector<ofVec2f> imagePoints(n);
    for(int i = 0; i < n; i++) {
        imagePoints[i] = getImagePoint(i);
    }
    return imagePoints;
}

//Test method to avoid points 60 and 64
vector<ofVec2f> ofxFaceTracker2Landmarks::getImagePointsMod() const {
    int n = shape.num_parts();
    vector<ofVec2f> imagePoints(n-2);
    int cont = 0;
    for(int i = 0; i < n; i++) {
        if (i != 60 || i != 64) {
            
        } else {
            imagePoints[cont] = getImagePoint(i);
            cont++;
        }
    }
    return imagePoints;
}

vector<cv::Point2f> ofxFaceTracker2Landmarks::getCvImagePoints() const {
    int n = shape.num_parts();
    vector<cv::Point2f> imagePoints(n);
    for(int i = 0; i < n; i++) {
        imagePoints[i] = ofxCv::toCv(getImagePoint(i));
    }
    return imagePoints;
}



ofPolyline ofxFaceTracker2Landmarks::getImageFeature(Feature feature) const {
    return getFeature(feature, getImagePoints());
}

ofMesh ofxFaceTracker2Landmarks::getImageMesh() const{
    //return getMesh(getCvImagePoints()); // <-- the original return

    /*NEED TO WORK OVER getMeshFromImagePoint() and addTriangleIndices()
     tofix the mesh problem, by the time being is using the actual getMesh function 
     */
    return getMeshFromImagePoint(getImagePoints());//getMesh(getImagePoints());
}


vector<int> ofxFaceTracker2Landmarks::getFeatureIndices(Feature feature) {
    switch(feature) {
        case LEFT_EYE_TOP: return consecutive(36, 40);
        case RIGHT_EYE_TOP: return consecutive(42, 46);
        case LEFT_JAW: return consecutive(0, 9);
        case RIGHT_JAW: return consecutive(8, 17);
        case JAW: return consecutive(0, 17);
        case LEFT_EYEBROW: return consecutive(17, 22);
        case RIGHT_EYEBROW: return consecutive(22, 27);
        case LEFT_EYE: return consecutive(36, 42);
        case RIGHT_EYE: return consecutive(42, 48);
        case OUTER_MOUTH: return consecutive(48, 60);
        case INNER_MOUTH: return consecutive(60, 68);
        case NOSE_BRIDGE: return consecutive(27, 31);
        case NOSE_BASE: return consecutive(31, 36);
        case FACE_OUTLINE: {
            static int faceOutline[] = {17,18,19,20,21,22,23,24,25,26, 16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
            return vector<int>(faceOutline, faceOutline + 27);
        }
        case ALL_FEATURES: return consecutive(0, 68);
    }
}


template <class T>
ofPolyline ofxFaceTracker2Landmarks::getFeature(Feature feature, vector<T> points) const {
    ofPolyline polyline;
    vector<int> indices = getFeatureIndices(feature);
    for(int i = 0; i < indices.size(); i++) {
        int cur = indices[i];
        polyline.addVertex(points[cur]);
    }
    switch(feature) {
        case LEFT_EYE:
        case RIGHT_EYE:
        case OUTER_MOUTH:
        case INNER_MOUTH:
        case FACE_OUTLINE:
            polyline.close();
            break;
        default:
            break;
    }
    
    return polyline;
}

vector<int> ofxFaceTracker2Landmarks::consecutive(int start, int end) {
    int n = end - start;
    vector<int> result(n);
    for(int i = 0; i < n; i++) {
        result[i] = start + i;
    }
    return result;
}

//getMesh method as in ofxFaceTracker (previous version of face tracker)
template <class T>
ofMesh ofxFaceTracker2Landmarks::getMeshFromImagePoint(vector<T> points) const {
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    if(true) {
        int n = points.size();
        for(int i = 0; i < n; i++) {
            //ofLog(OF_LOG_NOTICE, "points["+ofToString(i)+"]: " + ofToString(points[i]));
            
            
            mesh.addVertex(points[i]);
            mesh.addTexCoord(getImagePoint(i));
        }
        addTriangleIndices(mesh);
    }
    return mesh;
}

//TODO: Set this method right!!!!!  
//addTriangleIndices method as in ofxFaceTracker (previous version of face tracker)
void ofxFaceTracker2Landmarks::addTriangleIndices(ofMesh& mesh) const {
    /* This comment is the version from ofxFaceTracker
    int in = tri.rows;
    for(int i = 0; i < tri.rows; i++) {
        int i0 = tri.it(i, 0), i1 = tri.it(i, 1), i2 = tri.it(i, 2);
        bool visible = getVisibility(i0) && getVisibility(i1) && getVisibility(i2);
        if(useInvisible || visible) {
            mesh.addIndex(i0);
            mesh.addIndex(i1);
            mesh.addIndex(i2);
        }
    }
     */
    
    //This is the new version using directly a two dimensional array defined in the .h file. The array represents the x,y,z positions of the mesh used as mask
#ifdef DEBUG
    printf("----------------------------------\n");
    printf("----------------------------------\n");
    for(int i = 0; i < tri.rows; i++) {
        printf("%i , ", tri.it(i, 0) );
    }
    printf("\n----------------------------------\n");
    for(int i = 0; i < tri.rows; i++) {
        printf("%i , ", tri.it(i, 1) );
    }
    printf("\n----------------------------------\n");
    for(int i = 0; i < tri.rows; i++) {
        printf("%i , ", tri.it(i, 2) );
    }
    printf("\n----------------------------------\n");
    for(int i = 0; i < tri.rows; i++) { //why this faceTri[0].size() doesnt works?
        int i0 = tri.it(i, 0), i1 = tri.it(i, 1), i2 = tri.it(i, 2);
        printf("%i %i %i \n", i0, i1, i2);
#else
    for(int i = 0 ; i < faceTriCols ;i++){
        int i0 = faceTri[0][i], i1 = faceTri[1][i], i2 = faceTri[2][i];
#endif
        
        
        if(true) { // getVisible?? is not part of faceTracker2, so is ignored, is too bad???
            mesh.addIndex(i0);
            mesh.addIndex(i1);
            mesh.addIndex(i2);
        }
    }
#ifdef DEBUG
    printf("----------------------------------\n");
#endif
}


template <class T>
ofMesh ofxFaceTracker2Landmarks::getMesh(vector<T> points) const {
    cv::Rect rect(0, 0, info.inputWidth, info.inputHeight);
    cv::Subdiv2D subdiv(rect);
    
    for(int i=0;i<points.size();i++){
        if( rect.contains(points[i]) ){
            subdiv.insert(points[i]);
        }
    }
    
    vector<cv::Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    for( size_t i = 0; i < triangleList.size(); i++ )
    {
        cv::Vec6f t = triangleList[i];
        
        cv::Point2f pt1 = cv::Point(cvRound(t[0]), cvRound(t[1]));
        cv::Point2f pt2 = cv::Point(cvRound(t[2]), cvRound(t[3]));
        cv::Point2f pt3 = cv::Point(cvRound(t[4]), cvRound(t[5]));
        
        // Draw rectangles completely inside the image.
        if ( rect.contains(pt1) && rect.contains(pt2) && rect.contains(pt3))
        {
            mesh.addVertex(ofxCv::toOf(pt1));
            mesh.addVertex(ofxCv::toOf(pt2));
            mesh.addVertex(ofxCv::toOf(pt3));
        }
    }
    return mesh;
    
}

//=============================================================================
#ifdef DEBUG
cv::Mat ofxFaceTracker2Landmarks::LoadTri(const char* fname)
{
    int i,n; char str[256]; char c; fstream file(fname,fstream::in);
    if(!file.is_open()){
        printf("ERROR(%s,%d) : Failed opening file %s for reading\n",
               __FILE__,__LINE__,fname); abort();
    }
    while(1){file >> str; if(strncmp(str,"n_tri:",6) == 0)break;}
    file >> n; cv::Mat tri(n,3,CV_32S);
    while(1){file >> c; if(c == '{')break;}
    for(i = 0; i < n; i++)
        file >> tri.at<int>(i,0) >> tri.at<int>(i,1) >> tri.at<int>(i,2);
    file.close(); return tri;
}

#endif
