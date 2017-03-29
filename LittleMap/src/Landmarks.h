#pragma once
#include "ofMain.h"
#include "Stage.h"

#include <vector>

class Landmarks : public Stage
{
public:
	Landmarks();
	~Landmarks();

	virtual void Setup();
	virtual bool Render();
	virtual void Draw();

	struct Landmark {
		ofPoint pos;
		int iconIdx;
	};

private:
	vector<ofFile> files;
	vector<ofImage> icons;
	vector<Landmark> landmarks;

	ofFbo image;
};
