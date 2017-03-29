#include "Landmarks.h"

int numLandmarks = 20;
float avoidRadius = 40.0f;
float iconScale = 0.5f;

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
		if (found >= 0)
		{
			files.push_back(f);
		}
	}
}

bool Landmarks::Render()
{
	image = ofFbo();
	image.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

	image.begin();
	ofClear(0, 0, 0, 0);

	icons = vector<ofImage>();
	for (int i = 0; i < files.size(); i++)
	{
		printf("\tFound icon: %s\n", files[i].getFileName().c_str());
		ofImage icon;
		icon.load(files[i]);
		icons.push_back(icon);
	}

	landmarks = vector<Landmark>();
	for (int i = 0; i < numLandmarks; i++)
	{
		Landmark landmark;
		landmark.iconIdx = std::rand() % icons.size();
		bool found = false;
		while (!found)
		{
			ofPoint pt(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
			found = true;
			for (int o = 0; o < landmarks.size(); o++)
			{
				Landmark other = landmarks[o];
				if (other.pos.distance(pt) < avoidRadius)
				{
					found = false;
					break;
				}
			}
			if (found)
			{
				landmark.pos = pt;
				landmarks.push_back(landmark);
			}
		}
	}

	for (int i = 0; i < landmarks.size(); i++)
	{
		Landmark landmark = landmarks[i];
		ofImage icon = icons[landmark.iconIdx];
		icon.draw(landmark.pos, icon.getWidth()*iconScale, icon.getHeight()*iconScale);
	}

	image.end();

	return true;
}

void Landmarks::Draw()
{
	image.draw(0, 0);
}
