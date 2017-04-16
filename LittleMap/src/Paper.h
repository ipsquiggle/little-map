#pragma once
#include "Stage.h"
#include "ofMain.h"

#include "Legend.h"

class Paper : public Stage
{
public:
	Paper(Legend& legendRef);
	~Paper();

	virtual void Setup();
	virtual bool Render();
	virtual void Draw();
	virtual void Reset();

private:
	Legend& legendRef;
	
	float PaperNoise(float x, float y);

	ofFbo image;
};

