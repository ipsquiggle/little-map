#include "ofApp.h"

#include "Noise.h"

#include "CurveTerrain.h"
#include "LatLon.h"
#include "Landmarks.h"
#include "Paths.h"
#include "Saver.h"


char* nextMessage;
void statusMessage(char* message)
{
	printf("%s\n", message);
	nextMessage = message;
}

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetWindowTitle("Tiny Map");

	ofBackground(0, 0, 0);
	ofEnableSmoothing();

	int seed = (int)std::time(nullptr);
	printf("Seed: %d\n", seed);
	SetNoiseSeed(seed);

	stages = new Stage*[(int)step::done];
	stages[(int)step::start] = NULL;
	CurveTerrain *terrain = new CurveTerrain(false, false);
	stages[(int)step::islands] = terrain;
	stages[(int)step::lines] = new LatLon();
	Landmarks *landmarks = new Landmarks(*terrain);
	stages[(int)step::landmarks] = landmarks;
	stages[(int)step::paths] = new Paths(*terrain, *landmarks, false);
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
void ofApp::update()
{
	if (currentStep < step::done)
	{
		bool next = true;
		if (stages[(int)currentStep] != NULL)
			next = stages[(int)currentStep]->Render();

		if (next)
		{
			currentStep = (step)(((int)currentStep) + 1);
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
				//statusMessage("Done");
				break;
			}
		}

	}
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

	if (nextMessage != nullptr)
	{
		ofSetColor(ofColor::black);
		ofDrawBitmapString(nextMessage, 11, ofGetHeight() - 9);
		ofSetColor(ofColor::white);
		ofDrawBitmapString(nextMessage, 10, ofGetHeight() - 10);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	stages[(int)step::islands]->Render();
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
