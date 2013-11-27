#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    
    
    // gui
    gui.setup();
    gui.setPosition(20, 140);
    gui.add(nearDis.setup("Near", 20.0, 0.0, 300.0));
    gui.add(farDis.setup("Far", 1500.0, 301.0, 5000.0));
    gui.add(fov.setup("FOV", 60.0, 1.0, 100.0));
    gui.add(clipPoints.setup("Clip Points", true));
    gui.loadFromFile("settings.xml");
    
    
    
    selectedCamera = 1;
    cams.push_back(&cameraA);
    cams.push_back(&cameraB);
    
    
    
    cameraA.setPosition(ofVec3f() + (100 * cameraA.getZAxis()));

    // the easy camera
    cameraB.setPosition(ofVec3f() + (100 * cameraA.getZAxis()));;
    cameraB.setDistance(900);
    
    
    // add some points to the world
    int nPts = 2000;
    int div  = 400;
    for (int i=0; i<nPts; i++) {
        ofVec3f p(ofRandom(-div, div), ofRandom(-div, div), ofRandom(-div, div));
        points.push_back(p);
    }
    
    sphere.set(2, 3);
    sphere.setUseVbo(true);
}

//--------------------------------------------------------------
void testApp::update(){

    cameraA.rotateAround(0.8, ofVec3f(0, 1, 0), ofVec3f());
    cameraA.lookAt(ofVec3f());
    
    cameraA.setNearClip(nearDis);
    cameraA.setFarClip(farDis);
    cameraA.setFov(fov);

}

//--------------------------------------------------------------
void drawBox(ofPoint tr, ofPoint tl, ofPoint br, ofPoint bl) {
    ofLine(tl, tr);
    ofLine(tr, br);
    ofLine(br, bl);
    ofLine(bl, tl);
}


//--------------------------------------------------------------
bool pointInFrustum(ofVec3f &p, Plane pl[6]) {
    for(int i=0; i < 6; i++) {
        if (pl[i].distance(p) < 0) return false;
    }
    return true;
}

//--------------------------------------------------------------
void testApp::draw() {
    
    ofBackgroundGradient(ofColor(100), ofColor(10));
    
    ofEnableDepthTest();
    cams[selectedCamera]->begin();

    // a box for space...
    ofNoFill();
    ofSetColor(255);
    ofDrawBox(0, 0, 0, 10, 10, 10);

    
    
    ofRectangle viewport = ofGetCurrentViewport();
    
    float ratio     = viewport.width / viewport.height;
    float nearH     = 2 * tan(cameraA.getFov() / 2.0) * -cameraA.getNearClip();
	float nearW     = nearH * ratio;
    
    float farH      = 2 * tan(cameraA.getFov() / 2.0) * -cameraA.getFarClip();
	float farW      = farH * ratio;
    
    ofVec3f p  = cameraA.getPosition();
    ofVec3f l  = cameraA.getLookAtDir();
    ofVec3f u  = cameraA.getUpDir();
    
    ofVec3f fc = p + l * cameraA.getFarClip();
	ofVec3f nc = p + l * cameraA.getNearClip();

    ofSetColor(255);
    ofLine(nc, fc);
    
    
    ofVec3f up  = cameraA.getUpDir();
    ofVec3f right = cameraA.getXAxis();
    
    ofVec3f ftl = fc + (up * farH/2) - (right * farW/2);
    ofVec3f ftr = fc + (up * farH/2) + (right * farW/2);
	ofVec3f fbl = fc - (up * farH/2) - (right * farW/2);
	ofVec3f fbr = fc - (up * farH/2) + (right * farW/2);
    
    ofVec3f ntl = nc + (up * nearH/2) - (right * nearW/2);
	ofVec3f ntr = nc + (up * nearH/2) + (right * nearW/2);
	ofVec3f nbl = nc - (up * nearH/2) - (right * nearW/2);
	ofVec3f nbr = nc - (up * nearH/2) + (right * nearW/2);
    
    
    drawBox(ftl, ftr, fbl, fbr);    // draw far plane
    drawBox(ntl, ntr, nbl, nbr);    // draw near plane
    
    ofLine(ftl, ntl);
    ofLine(ftr, ntr);
    ofLine(fbl, nbl);
    ofLine(fbr, nbr);

    
    Plane pl[6];
    pl[0].set(ntr,ntl,ftl);
    pl[1].set(nbl,nbr,fbr);
    pl[2].set(ntl,nbl,fbl);
    pl[3].set(nbr,ntr,fbr);
    pl[4].set(ntl,ntr,nbr);
    pl[5].set(ftr,ftl,fbl);
    
    
    // draw the 3d Points
    for(int i=0; i<points.size(); i++) {
        
        bool bInFrustrum = pointInFrustum(points[i], pl);
        
        if(clipPoints && bInFrustrum) {
            ofSetColor(255, 0, 0);
            ofPushMatrix();
            ofTranslate(points[i]);
            sphere.draw();
            ofPopMatrix();
        }
        else if(!clipPoints) {
            bInFrustrum ? ofSetColor(255, 0, 0) : ofSetColor(255, 100);
            ofPushMatrix();
            ofTranslate(points[i]);
            sphere.draw();
            ofPopMatrix();
        }
        
    }
    
    
    
    ofSetColor(255);
    // draw cameras
    for(int i=0; i<cams.size(); i++) {
        cams[i]->draw();
    }
    
    cams[selectedCamera]->end();
 
    string info = "";
    info += "FPS "+ofToString(ofGetFrameRate(),0) + "\n";
    info += "Aspect "+ofToString(ratio) + "\n";
    info += "FOV "+ofToString(cameraA.getFov()) + "\n";
    info += "NC "+ofToString(cameraA.getNearClip()) + "\n";
    info += "FC "+ofToString(cameraA.getFarClip()) + "\n";
    
    info += "nearW " + ofToString(nearW)  + "\n";
    info += "nearH " + ofToString(nearH)  + "\n";
   
    info += "farW " + ofToString(farW)  + "\n";
    info += "farH " + ofToString(farH)  + "\n";
    
    ofFill();
    ofDisableDepthTest();
    ofDrawBitmapString(info, 20, 20);
    
    // draw the gui
    gui.draw();
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    if(key == '1') selectedCamera = 0;
    if(key == '2') selectedCamera = 1;
    
    if(key == OF_KEY_DOWN)   cameraB.setDistance(cameraB.getDistance()+2);
    if(key == OF_KEY_UP)     cameraB.setDistance(cameraB.getDistance()-2);

    if(key == OF_KEY_RIGHT)  cameraA.move(ofVec3f(1, 0, 0));
    if(key == OF_KEY_LEFT)   cameraA.move(ofVec3f(-1, 0, 0));

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if(gui.getShape().inside(x, y)) {
        cameraB.disableMouseInput();
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    cameraB.enableMouseInput();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
