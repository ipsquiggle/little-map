#include "Landmarks.h"

float placementGridSize = 160.0f;
float avoidRadiusLand = 40.0f;
float avoidRadiusWater = 150.0f;
float shorelineNoiseAvoid = 0.10f;
float iconScale = 0.5f;

Landmarks::Landmarks(CurveTerrain &terrain)
	: terrain(terrain)
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
	printf("Placing landmarks\n");

	landmarks = vector<Landmark>();
	for (int y = 0; y < ofGetHeight(); y += placementGridSize)
	{
		for (int x = 0; x < ofGetWidth(); x += placementGridSize)
		{
			Landmark landmark;
			landmark.iconIdx = std::rand() % icons.size();
			bool found = false;
			for (int attempt = 0; attempt < 10; attempt++)
			{
				ofPoint pt = ofPoint(ofRandom(placementGridSize), ofRandom(placementGridSize))
					+ ofPoint(x, y);

				float onLand = terrain.GetLandValue(pt.x, pt.y);

				// keep a way from shore, cheap way
				if (onLand > -shorelineNoiseAvoid && onLand < shorelineNoiseAvoid)
					continue;

				found = true;
				float avoidRadius = onLand > 0 ? avoidRadiusLand : avoidRadiusWater;
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
					printf("\tPlaced landmark %d,%d after %d tries.\n", x, y, attempt);
					landmark.pos = pt;
					landmark.onLand = onLand;
					landmarks.push_back(landmark);
					break;
				}
			}
		}
	}

	ofEnableAlphaBlending();
	for (int i = 0; i < landmarks.size(); i++)
	{
		Landmark landmark = landmarks[i];
		ofImage icon = icons[landmark.iconIdx];
		if (landmark.onLand > 0)
		{
			ofSetColor(255, 255, 255, 255);
			printf("%d onland %f\n", landmark.iconIdx, landmark.onLand);
		}
		else
		{
			printf("%d onwater %f\n", landmark.iconIdx, landmark.onLand);
			ofSetColor(255, 255, 255, 150);
		}

		icon.draw(landmark.pos, icon.getWidth()*iconScale, icon.getHeight()*iconScale);
	}
	ofDisableAlphaBlending();

	image.end();

	return true;
}

void Landmarks::Draw()
{
	image.draw(0, 0);
}
