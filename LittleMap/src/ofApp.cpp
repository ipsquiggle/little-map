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
	stages[(int)step::paths] = new Paths(*terrain, *landmarks, 0);
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
		bool next = false;
		if (stages[(int)currentStep] != NULL)
			next = stages[(int)currentStep]->Render();
		else
			next = true;

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
				statusMessage("Done");
				break;
			default:
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
		ofDrawBitmapString(nextMessage, 11, ofGetHeight() - 19);
		ofSetColor(ofColor::white);
		ofDrawBitmapString(nextMessage, 10, ofGetHeight() - 20);
	}
	if (currentStep < (int)step::done && stages[currentStep] != nullptr)
	{
		char* stageMessage = stages[currentStep]->GetMessage();
		if (stageMessage != nullptr)
		{
			ofSetColor(ofColor::black);
			ofDrawBitmapString(stageMessage, 11, ofGetHeight() - 9);
			ofSetColor(ofColor::white);
			ofDrawBitmapString(stageMessage, 10, ofGetHeight() - 10);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if (key == 'i')
		stages[(int)step::islands]->Render();
	else if (key >= '0' && key <= '9')
		stages[currentStep]->DebugNum(key);
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

	float valCost;
	float distCost;
	float totalCost;
	float spend;
	Paths* paths = (Paths*)stages[(int)step::paths];
	paths->GetCosts(ofPoint(x, y), valCost, distCost, totalCost, spend);
	printf("COSTS: valCost: %f \tdistCost: %f \ttotalCost: %f \tspend: %f\n", valCost, distCost, totalCost, spend);
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

void ofApp::exit()
{
	if (stages[(int)save] != nullptr)
	{
		stages[(int)save]->Render();
	}
}
