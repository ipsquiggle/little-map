#pragma once
#include "Stage.h"
#include "ofMain.h"

class Saver : public Stage
{
public:
	Saver();
	~Saver();

	virtual void Setup();
	virtual bool Render();
	virtual void Draw();

private:
	bool saved;
};
