#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Plane.h"

class testApp : public ofBaseApp{

	public:
    
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    ofxPanel            gui;
    ofxFloatSlider      nearDis;
    ofxFloatSlider      farDis;
    ofxFloatSlider      fov;
    ofxToggle           clipPoints;
    
    int                 selectedCamera;
    ofCamera            cameraA;
    ofEasyCam           cameraB;
    vector  <ofCamera*> cams;
    
    // some 3d shapes
    ofSpherePrimitive   sphere;
    vector  <ofVec3f>   points;

};
