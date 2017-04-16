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

	ofDirectory iconsDir(path);
	iconsDir.allowExt("png");
	iconsDir.listDir();

	for (int i = 0; i < iconsDir.size(); i++)
	{
		ofFile f = iconsDir.getFile(i);
		int found = f.getFileName().find("icon-");
		if (found >= 0)
		{
			files.push_back(f);
		}
	}

	for (int i = 0; i < files.size(); i++)
	{
		printf("\tFound icon: %s\n", files[i].getFileName().c_str());
		ofImage icon;
		icon.load(files[i]);
		icons.push_back(icon);
	}
	
	Reset();
}

void Landmarks::Reset()
{
	if (image.isAllocated())
		image.clear();
	image.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
}

bool Landmarks::Render()
{
	image.begin();
	ofClear(0, 0, 0, 0);

	printf("Placing landmarks\n");

	landmarks.clear();
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

		DrawIcon(landmark.iconIdx, landmark.pos);
	}
	ofDisableAlphaBlending();

	image.end();

	return true;
}

void Landmarks::DrawIcon(int idx, ofPoint pt)
{
	ofImage icon = icons[idx];
	ofPoint offset(icon.getWidth() * iconScale / 2, icon.getHeight() * iconScale / 2);
	icon.draw(pt - offset, icon.getWidth()*iconScale, icon.getHeight()*iconScale);
}

void Landmarks::Draw()
{
	image.draw(0, 0);
}

const vector<Landmarks::Landmark> Landmarks::GetLandmarks()
{
	return landmarks;
}

Landmarks::Landmark Landmarks::GetRandomLandmark()
{
	int r = std::rand() % landmarks.size();
	return landmarks[r];
}

class DistanceSort
{
public:
	Landmarks::Landmark target;
	DistanceSort(Landmarks::Landmark t)
		: target(t)
	{ }

	bool operator() (Landmarks::Landmark a, Landmarks::Landmark b)
	{
		return target.pos.distanceSquared(a.pos) < target.pos.distanceSquared(b.pos);
	}

};

Landmarks::Landmark Landmarks::GetNthClosestLandmark(Landmark target, int n)
{
	vector<Landmark> close = vector<Landmark>(landmarks);
	DistanceSort sorter = DistanceSort(target);
	std::sort(close.begin(), close.end(), sorter);
	printf("About to look for the %dth landmark.\n", n);
	for (int i = 0; i < close.size(); i++)
	{
		printf("  %d : %f\n", i, close[i].pos.distance(target.pos));
	}
	return close[n];
}