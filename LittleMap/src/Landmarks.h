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

private:
	vector<ofFile> files;
};
