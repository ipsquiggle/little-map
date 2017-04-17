#include "LatLon.h"
#include "Noise.h"

#include "RoughDrawer.h"

const float gridNoiseScale = 0.05f;
const float gridSpacing = 100.0f;
const float gridWobble = 10.0f;
const float gridDetail = 12.0f;

LatLon::LatLon()
{
}

LatLon::~LatLon()
{
}

void LatLon::Setup()
{
	Reset();
}

void LatLon::Reset()
{
	if (image.isAllocated())
		image.clear();
	image.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	image.begin();
	ofClear(0, 0, 0, 0);
	image.end();
}

float LatLon::LatLonNoise(float x, float y)
{
	return Noise(x*gridNoiseScale, y*gridNoiseScale, 3, 0.5f, 0.4f);
}

bool LatLon::Render()
{
	image.begin();
	ofClear(0, 0, 0, 0);

	for (int y = 0; y < ofGetHeight(); y += gridSpacing)
	{
		ofPolyline path = ofPolyline();
		path.curveTo(ofPoint(0, y));
		for (int x = -gridDetail; x < ofGetWidth() + gridDetail; x += gridDetail)
		{
			ofPoint offset(LatLonNoise(x, y), LatLonNoise(x, y + 1000.0f));
			path.curveTo(offset * gridWobble + ofPoint(x, y));
		}

		ofFill();
		ofSetColor(ofColor::black);
		ofEnableSmoothing();
		RoughTracePath(path, 1, 2);
	}

	for (int x = 0; x < ofGetWidth(); x += gridSpacing)
	{
		ofPolyline path = ofPolyline();
		path.curveTo(ofPoint(x, 0));
		for (int y = -gridDetail; y < ofGetHeight() + gridDetail; y += gridDetail)
		{
			ofPoint offset(LatLonNoise(x, y), LatLonNoise(x, y + 1000.0f));
			path.curveTo(offset * gridWobble + ofPoint(x, y));
		}
		path.curveTo(ofPoint(x, ofGetHeight()));

		ofFill();
		ofSetColor(ofColor::black);
		ofEnableSmoothing();
		RoughTracePath(path, 1, 2);
	}

	image.end();
	return true;
}

void LatLon::Draw()
{
	if(image.isAllocated())
		image.draw(0, 0);
}
