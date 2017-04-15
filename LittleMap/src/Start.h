#pragma once
#include "Stage.h"
#include "ofMain.h"

class Start : public Stage
{
public:
	Start();
	~Start();

	virtual void Setup();
	virtual void Reset();
};

