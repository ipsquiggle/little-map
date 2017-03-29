#include "Landmarks.h"

Landmarks::Landmarks()
{
}

Landmarks::~Landmarks()
{
}

void Landmarks::Setup()
{
	string path = "..";

	ofDirectory icons(path);
	icons.allowExt("png");
	icons.listDir();

	files = vector<ofFile>();
	for (int i = 0; i < icons.size(); i++)
	{
		ofFile f = icons.getFile(i);
		int found = f.getFileName().find("icon-");
		printf("Find: %d %s\n", found, f.getFileName().c_str());
		if (found >= 0)
		{
			printf("... found!\n");
			files.push_back(f);
		}
	}
}

bool Landmarks::Render()
{
	for (int i = 0; i < files.size(); i++)
	{
		printf("Found icon: %s\n", files[i].getFileName().c_str());
	}
	return true;
}

void Landmarks::Draw()
{
}
