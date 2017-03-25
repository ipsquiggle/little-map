#include "Saver.h"

Saver::Saver()
{
}

Saver::~Saver()
{
}

void Saver::Setup()
{
}

void Saver::Render()
{
	ofImage snapshot;
	snapshot.grabScreen(0, 0, ofGetWidth(), ofGetHeight());

	time_t currentTime = time(0);
	tm tmStruct = *localtime(&currentTime);
	char filename[MAX_PATH];
	strftime(filename, sizeof(filename), "little_map-%Y%m%d-%H%M%S.png", &tmStruct);
	snapshot.save(filename);
}

void Saver::Draw()
{
}
