#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Tiny Map");

	ofBackground(0, 0, 0);
}

//--------------------------------------------------------------
void ofApp::update(){

}

float noise(float x, float y, int octaves, float alpha, float beta)
{
	float t = 0;
	float max = 0;

	for (int n = 0; n < octaves; n++)
	{
		float factor = 1.0f / std::pow(alpha, n);
		max += factor;
		float betapow = std::pow(beta, n);
		t += factor * ofNoise(betapow * x, betapow * y);
	}

	return t/max;
}

float THRESHOLD_X = 0;
float THRESHOLD_Y = 0;
const float NOISE_SCALE = 0.05f;

bool onLand(float x, float y)
{
	return noise(x*NOISE_SCALE, y*NOISE_SCALE, 4, 0.5f, 0.4f) < 0.45f;
}

ofColor terrain(float x, float y)
{
	bool hits[4];
	hits[0] = onLand(x, y);
	hits[1] = onLand(x + 1, y);
	hits[2] = onLand(x, y + 1);
	hits[3] = onLand(x + 1, y + 1);

	bool land = false;
	bool water = false;
	for (int i = 0; i < 4; i++)
	{
		if (hits[i])
			land = true;
		else
			water = true;
	}

	if (land && water)
		return ofColor(0, 0, 0, 255);
	else if (land)
		return ofColor(150, 255, 200, 255);
	else
		return ofColor(150, 200, 255);
}

const float gridNoiseScale = 0.05f;

float latLonNoise(float x, float y)
{
	return noise(x*gridNoiseScale, y*gridNoiseScale, 3, 0.5f, 0.4f);
}

const float gridSpacing = 100.0f;
const float gridWobble = 15.0f;

ofColor latLon(float x, float y)
{
	float vals[4];
	vals[0] = x + latLonNoise(x, y) * gridWobble;
	vals[1] = x + 1 + latLonNoise(x + 1, y) * gridWobble;
	vals[2] = y + latLonNoise(x, y) * gridWobble;
	vals[3] = y + 1 + latLonNoise(x, y + 1) * gridWobble;

	if (std::fmod(vals[0], gridSpacing) > std::fmod(vals[1], gridSpacing) || std::fmod(vals[2], gridSpacing) > std::fmod(vals[3], gridSpacing))
		return ofColor(0, 0, 0, 255);
	else
		return ofColor(0, 0, 0, 0);
}

void statusMessage(char* message)
{
	printf("%s\n", message);
	ofDrawBitmapString(message, 10, ofGetHeight() - 10);
}

//--------------------------------------------------------------
int i = 0;

enum step {
	start,
	islands,
	lines,
	paper,
	legend,

	done,
};

step currentStep = step::start;

void ofApp::draw()
{
	switch (currentStep)
	{
		case (step::start):
		{
			statusMessage("Start");
			break;
		}

		case(step::islands):
		{
			islands = ofImage();
			islands.allocate(ofGetWidth(), ofGetHeight(), ofImageType::OF_IMAGE_COLOR_ALPHA);

			for (int y = 0; y < ofGetHeight(); y++)
			{
				for (int x = 0; x < ofGetWidth(); x++)
				{
					ofColor color = terrain(x, y);
					islands.setColor(x, y, color);
				}
			}

			islands.update();
			islands.draw(0, 0);
			statusMessage("Islands");
			break;
		}

		case(step::lines):
		{
			lines = ofImage();
			lines.allocate(ofGetWidth(), ofGetHeight(), ofImageType::OF_IMAGE_COLOR_ALPHA);

			for (int y = 0; y < ofGetHeight(); y++)
			{
				for (int x = 0; x < ofGetWidth(); x++)
				{
					ofColor color = latLon(x, y);
					lines.setColor(x, y, color);
				}
			}

			lines.update();

			printf("%f\n", THRESHOLD_X);

			islands.draw(0, 0);
			lines.draw(0, 0);
			statusMessage("Lines");

			break;
		}

		case(step::paper):
		{
			statusMessage("Paper");
			break;
		}

		case(step::legend):
		{
			statusMessage("Legend");
			break;
		}

		case(step::done):
		{
			islands.draw(0, 0);
			lines.draw(0, 0);
			statusMessage("Done");
			break;
		}
	}

	if (currentStep < step::done)
	{
		currentStep = (step)(((int)currentStep)+1);
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

	THRESHOLD_Y = (float)y / ofGetHeight();
	THRESHOLD_X = (float)x / ofGetWidth();
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
