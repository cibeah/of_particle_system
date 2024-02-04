#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	numel = 2000;
	ps = ParticlesSystem(numel);
	//ofResetElapsedTimeCounter();
	timer = 0;


}

//--------------------------------------------------------------
void ofApp::update() {
	timer++;
	ps.update(75);
	/*if ( timer % 5 == 0) {
		ps.update(100);
	}*/
	
	//ofLog(OF_LOG_NOTICE, ofToString(ofGetElapsedTimeMillis()));
	/*if ( timer % 30 == 0) {
		ps.add(color2);
	}
	else if (timer % 30 == 15) {
		ps.add(color1);
	}
	*/

}

//--------------------------------------------------------------
void ofApp::draw() {
	ps.display();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
