#include "ofApp.h"

#include "Terrain.h"
#include "LatLon.h"
#include "Saver.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Tiny Map");

	ofBackground(0, 0, 0);

	stages = new Stage*[(int)step::done];
	stages[(int)step::start] = NULL;
	stages[(int)step::islands] = new Terrain();
	stages[(int)step::lines] = new LatLon();
	stages[(int)step::landmarks] = NULL;
	stages[(int)step::paths] = NULL;
	stages[(int)step::paper] = NULL;
	stages[(int)step::legend] = NULL;
	stages[(int)step::save] = new Saver();

	for (int i = 0; i < (int)step::done; i++)
	{
		if (stages[i] != NULL)
			stages[i]->Setup();
	}
}

//--------------------------------------------------------------
void ofApp::update(){

	for (int i = 0; i < (int)step::done; i++)
	{
		if (stages[i] != NULL)
			stages[i]->Setup();
	}

	if (currentStep < step::done)
	{
		if (stages[(int)currentStep] != NULL)
			stages[(int)currentStep]->Render();
		currentStep = (step)(((int)currentStep)+1);
	}
}

void statusMessage(char* message)
{
	printf("%s\n", message);
	ofSetColor(ofColor::black);
	ofDrawBitmapString(message, 11, ofGetHeight() - 9);
	ofSetColor(ofColor::white);
	ofDrawBitmapString(message, 10, ofGetHeight() - 10);
}

//--------------------------------------------------------------
int i = 0;

void ofApp::draw()
{
	for (int i = 0; i < (int)step::done; i++)
	{
		if (stages[i] != NULL)
			stages[i]->Draw();
	}

	switch (currentStep)
	{
		case (step::start):
			statusMessage("Start");
			break;
		case(step::islands):
			statusMessage("Islands");
			break;
		case(step::lines):
			statusMessage("Lines");
			break;
		case(step::landmarks):
			statusMessage("Landmarks");
			break;
		case(step::paths):
			statusMessage("Paths");
			break;
		case(step::paper):
			statusMessage("Paper");
			break;
		case(step::legend):
			statusMessage("Legend");
			break;
		case(step::save):
			statusMessage("Save");
			break;
		case(step::done):
			statusMessage("Done");
			break;
	}

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

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
