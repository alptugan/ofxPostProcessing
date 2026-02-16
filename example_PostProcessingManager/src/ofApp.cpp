#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(0);
    
    //setup manager
    manager.setup(ofGetWidth(), ofGetHeight(), "VCR_OSD_MONO_1.001.ttf", 8);

	 //Load Settings
	//manager.loadSettings();
	manager.loadSessionState();

    // Setup box positions
    for (unsigned i = 0; i < NUM_BOXES; ++i)
    {
        posns.push_back(glm::vec3(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300)));
        cols.push_back(ofColor::fromHsb(255 * i / (float)NUM_BOXES, 255, 255, 255));
    }
    
    // Setup light
    light.setPosition(1000, 1000, 2000);
    
    // create our own box mesh as there is a bug with
    // normal scaling and ofDrawBox() at the moment
    boxMesh = ofMesh::box(20, 20, 20);
}

//--------------------------------------------------------------
void ofApp::exit() {
	ofLogNotice("ofApp") << "exit()";
	manager.saveSessionState();
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));

    // update manager
    manager.updateValues();
}

//--------------------------------------------------------------
void ofApp::draw(){
    // setup gl state

    
    // begin scene to post process
    manager.begin(cam);
    ofEnableDepthTest();
    light.enable();
    // draw boxes
    for (unsigned i = 0; i < posns.size(); ++i)
    {
        ofSetColor(cols[i]);
        ofPushMatrix();
        ofTranslate(posns[i]);
        boxMesh.draw();
        ofPopMatrix();
    }
    
    ofDrawAxis(100);
    ofDisableDepthTest();
    // end scene and draw
    manager.end();
    

    
    if(!hideGui) {
        manager.drawGui(ofGetWidth() - manager.getGUIWidth(), 0);
    }else{
        // There is a kind of buggy issue on MAC.
        // Drawing the GUI out of the window.
        // Otherwise FPS draws continously
        manager.hideGui();
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'd')
        manager.disableAll();

    else if(key == '1')
        manager.loadSettings("scene1.json");

    else if(key == '2')
        manager.loadSettings("scene2.json");

    else if(key == '3')
        manager.loadSettings("scene3.json");

    else if(key == 'h')
        hideGui = !hideGui;

    else if(key == 'r') {
        manager.disableAll();
        int randId = int(ofRandom(manager.getEffectNum()));
        manager.switchFX(randId);
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    manager.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
