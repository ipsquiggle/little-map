#pragma once
#include "ofMain.h"
#include "Stage.h"

#include "Landmarks.h"

#include <vector>

class Paths : public Stage
{
public:
	Paths(Landmarks &landmarks);
	~Paths();

	virtual void Setup();
	virtual bool Render();
	virtual void Draw();
	
	struct Path {
		ofPoint start;
		ofPoint end;
	};

private:
	Landmarks &landmarks;

	vector<Path> paths;

	ofFbo image;
};
