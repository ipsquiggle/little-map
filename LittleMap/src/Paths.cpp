#include "Paths.h"

int numPaths = 10;
int minNearest = 0;
int maxNearest = 7;

Paths::Paths(Landmarks &landmarks)
	: landmarks(landmarks)
{
}

Paths::~Paths()
{
}

void Paths::Setup()
{
}

bool Paths::Render()
{
	paths = vector<Path>();

	for (int i = 0; i < numPaths; i++)
	{
		Landmarks::Landmark start = landmarks.GetRandomLandmark();
		int r = (std::rand() % (maxNearest - minNearest)) + minNearest;
		Landmarks::Landmark end = landmarks.GetNthClosestLandmark(start, r);
		Path p = { start.pos, end.pos };
		paths.push_back(p);
	}

	image = ofFbo();
	image.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

	image.begin();
	ofClear(0, 0, 0, 0);

	for (int i = 0; i < paths.size(); i++)
	{
		ofSetLineWidth(3);
		ofSetColor(ofColor::black);
		ofDrawLine(paths[i].start, paths[i].end);
	}

	image.end();

	return true;
}

void Paths::Draw()
{
	image.draw(0, 0);
}
