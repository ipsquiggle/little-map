#include "Saver.h"

Saver::Saver()
{
	saved = false;
}

Saver::~Saver()
{
	if (!saved)
	{
		Render();
	}
}

void Saver::Setup()
{
}

bool Saver::Render()
{
	if (!saved)
	{
		ofImage snapshot;
		snapshot.grabScreen(0, 0, ofGetWidth(), ofGetHeight());

		time_t currentTime = time(0);
		tm tmStruct = *localtime(&currentTime);
		char filename[MAX_PATH];
		strftime(filename, sizeof(filename), "little_map-%Y%m%d-%H%M%S.png", &tmStruct);
		snapshot.save(filename);
		snapshot.save("../latest.png");

		saved = true;
	}

	return true;
}

void Saver::Draw()
{
}
