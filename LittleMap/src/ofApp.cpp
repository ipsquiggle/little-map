#include "ofApp.h"

#include "Start.h"
#include "CurveTerrain.h"
#include "LatLon.h"
#include "Landmarks.h"
#include "Paths.h"
#include "Legend.h"
#include "Paper.h"
#include "Saver.h"


char* nextMessage;
void statusMessage(char* message)
{
	if (message != nullptr)
		printf("%s\n", message);
	nextMessage = message;
}

char* subMessage;
void statusMessage2(char* message)
{
	if (message != nullptr)
		printf("%s\n", message);
	subMessage = message;
}

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetWindowTitle("Tiny Map");

	ofBackground(0, 0, 0);
	ofEnableSmoothing();

	autoAdvance = true;
	doneStep = false;

	stages = new Stage*[(int)step::done];

	Start *start = new Start();
	stages[(int)step::start] = start;

	CurveTerrain *terrain = new CurveTerrain(false, false);
	stages[(int)step::islands] = terrain;

	LatLon *latLon = new LatLon();
	stages[(int)step::lines] = latLon;
	
	Landmarks *landmarks = new Landmarks(*terrain);
	stages[(int)step::landmarks] = landmarks;

	Paths *paths = new Paths(*terrain, *landmarks, 0);
	stages[(int)step::paths] = paths;

	Legend *legend = new Legend(*landmarks);
	stages[(int)step::legend] = legend;

	Paper *paper = new Paper(*legend);
	stages[(int)step::paper] = paper;

	Saver *saver = new Saver();
	stages[(int)step::save] = saver;



	drawOrder = new Stage*[(int)step::done];
	drawOrder[0] = start;
	drawOrder[1] = terrain;
	drawOrder[2] = latLon;
	drawOrder[3] = landmarks;
	drawOrder[4] = paths;
	drawOrder[5] = paper;
	drawOrder[6] = legend;
	drawOrder[7] = saver;


	for (int i = 0; i < (int)step::done; i++)
	{
		if (stages[i] != NULL)
			stages[i]->Setup();
	}
}

//--------------------------------------------------------------
void ofApp::update()
{
	if (!doneStep && currentStep < step::done)
	{
		if (stages[(int)currentStep] != NULL)
		{
			doneStep = stages[(int)currentStep]->Render();
		}
		else
		{
			statusMessage2("Done");
			doneStep = true;
		}

		if (autoAdvance && doneStep)
		{
			Advance();
		}
	}
}

void ofApp::Advance()
{
	doneStep = false;
	statusMessage2(nullptr);

	currentStep = (step)(((int)currentStep) + 1);
	switch (currentStep)
	{
	case (step::start):
		statusMessage("0: Start");
		break;
	case(step::islands):
		statusMessage("1: Islands");
		break;
	case(step::lines):
		statusMessage("2: Lines");
		break;
	case(step::landmarks):
		statusMessage("3: Landmarks");
		break;
	case(step::paths):
		statusMessage("4: Paths");
		break;
	case(step::legend):
		statusMessage("5: Legend");
		break;
	case(step::paper):
		statusMessage("6: Paper");
		break;
	case(step::save):
		statusMessage("7: Save");
		break;
	case(step::done):
		statusMessage("Done");
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
int i = 0;

void ofApp::draw()
{
	for (int i = 0; i < (int)step::done; i++)
	{
		if (drawOrder[i] != NULL)
			drawOrder[i]->Draw();
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
void ofApp::keyPressed(int key)
{
	int targetStep = ((int)currentStep) + 1;
	if (key == ' ')
	{
		if (doneStep)
			Advance();
	}
	else if (key >= '0' && key <= '9')
	{
		if (currentStep < (int)done)
			stages[currentStep]->DebugNum(key);
	}
	else if (key == 'r')
	{
		targetStep = 0;
		autoAdvance = true;
	}
	else if (key == 'a')
	{
		autoAdvance = !autoAdvance;
	}
	else if (key == ')')
	{
		targetStep = 0;
		autoAdvance = false;
	}
	else if (key == '!')
	{
		targetStep = 1;
		autoAdvance = false;
	}
	else if (key == '@')
	{
		targetStep = 2;
		autoAdvance = false;
	}
	else if (key == '#')
	{
		targetStep = 3;
		autoAdvance = false;
	}
	else if (key == '$')
	{
		targetStep = 4;
		autoAdvance = false;
	}
	else if (key == '%')
	{
		targetStep = 5;
		autoAdvance = false;
	}
	else if (key == '^')
	{
		targetStep = 6;
		autoAdvance = false;
	}
	else if (key == '&')
	{
		targetStep = 7;
		autoAdvance = false;
	}
	else if (key == '*')
	{
		targetStep = 8;
		autoAdvance = false;
	}
	else if (key == '(')
	{
		targetStep = 9;
		autoAdvance = false;
	}

	bool reset = false;
	for (int i = currentStep; i >= targetStep; i--)
	{
		if (i < (int)done && stages[i] != nullptr)
			stages[i]->Reset();
		reset = true;
	}
	if (reset)
	{
		currentStep = (step)(targetStep-1);
		Advance();
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
void ofApp::mousePressed(int x, int y, int button)
{
	if (stages[currentStep] != nullptr)
	{
		stages[currentStep]->DebugClick(x, y);
	}
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
