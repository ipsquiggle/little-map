#include "Legend.h"

#include "Noise.h"

int yOffset = 70;
int ySpacing = 20;
int xNegOffset = 220;
int xMargin = 20;
ofPoint imageOffset(-20, -9);
int fontSize = 18;
float adjectiveChance = 0.2f;
float pathAdjectiveChance = 0.1f;


Legend::Legend(Landmarks& landmarksRef, Paths& pathsRef)
	: landmarksRef(landmarksRef)
	, pathsRef(pathsRef)
{
}

Legend::~Legend()
{
}

void Legend::Setup()
{
	font.loadFont("../ManicMondayBold.otf", fontSize);

	Reset();
}

void Legend::Reset()
{
	landmarks.clear();

	singleNames.clear();
	singleNames.push_back("TREASURE");
	singleNames.push_back("HOMETOWN");
	singleNames.push_back("END OF THE WORLD");
	singleNames.push_back("NO RETURN");
	singleNames.push_back("EVIL CASTLE");
	singleNames.push_back("BEST FRIEND");
	singleNames.push_back("SWORD OF POWER");
	singleNames.push_back("MAGIC CRYSTAL");
	singleNames.push_back("DANGER ZONE");
	singleNames.push_back("ALTAR");
	singleNames.push_back("PORTAL");
	singleNames.push_back("BONUS ZONE");
	singleNames.push_back("MONOLITH");

	pluralNames.clear();
	pluralNames.push_back("DEAD END");
	pluralNames.push_back("VILLAGE");
	pluralNames.push_back("TOWN");
	pluralNames.push_back("TAVERN");
	pluralNames.push_back("CEMETERY");
	pluralNames.push_back("HOPELESSNESS");
	pluralNames.push_back("DESPAIR");
	pluralNames.push_back("TRIBULATIONS");
	pluralNames.push_back("CAVE");
	pluralNames.push_back("FOREST");
	pluralNames.push_back("WRECK");
	pluralNames.push_back("NPC");
	pluralNames.push_back("PLACE");
	pluralNames.push_back("COVEN");
	pluralNames.push_back("PLURALITY");

	pathNames.clear();
	pathNames.push_back("ROUTE");
	pathNames.push_back("PATH");
	pathNames.push_back("PASSAGE");
	pathNames.push_back("TUNNEL");
	pathNames.push_back("VOYAGE");
	pathNames.push_back("ROAD");

	adjectives.clear();
	adjectives.push_back("DANK");
	adjectives.push_back("OLD");
	adjectives.push_back("SMELLY");
	adjectives.push_back("HOPELESS");
	adjectives.push_back("BRIGHT");
	adjectives.push_back("UNCOUTH");
	adjectives.push_back("WRETCHED");
	adjectives.push_back("POTTED");
	adjectives.push_back("MUDDY");
	adjectives.push_back("ABANDONED");
	adjectives.push_back("NEW");
	adjectives.push_back("HAUNTED");
	adjectives.push_back("FABLED");
	adjectives.push_back("LOST");
	adjectives.push_back("SECRET");
	adjectives.push_back("PLEASANT");
	adjectives.push_back("ANCIENT");
	adjectives.push_back("HIDDEN");
	adjectives.push_back("DECREPIT");
	adjectives.push_back("CURSED");

	legendBounds.set(0, 0, 0, 0);

	if (image.isAllocated())
		image.clear();
	image.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	image.begin();
	ofClear(0, 0, 0, 0);
	image.end();
}

std::string Legend::GenerateName(int count)
{
	std::string dest = "";
	if (count == 1)
	{
		int idx = ((int)ofRandom(0, 1000)) % singleNames.size();
		dest += singleNames[idx];
	}
	else
	{
		int idx = ((int)ofRandom(0, 1000)) % pluralNames.size();
		dest += pluralNames[idx];
	}

	while (ofRandom(0, 1) < adjectiveChance)
	{
		int idx = ((int)ofRandom(0, 1000)) % adjectives.size();
		dest = adjectives[idx] + " " + dest;
	}
	return dest;
}

std::string Legend::GeneratePathName(Paths::PathStyle style)
{
	std::string dest = "";
	switch(style)
	{
	case Paths::PathStyle::Above:
	case Paths::PathStyle::Below:
	case Paths::PathStyle::Mixed:
		{
			int idx = ((int)ofRandom(0, 1000)) % pathNames.size();
			dest += pathNames[idx];
		}
	}

	{
		int idx = ((int)ofRandom(0, 1000)) % adjectives.size();
		dest = adjectives[idx] + " " + dest;
	}

	while (ofRandom(0, 1) < pathAdjectiveChance)
	{
		int idx = ((int)ofRandom(0, 1000)) % adjectives.size();
		dest = adjectives[idx] + " " + dest;
	}
	return dest;
}

std::string Legend::BreakString(std::string& src, float maxWidth)
{
	std::string dest = "";
	int currentSpace = -1;
	int nextSpace = 0;
	int testSpace = 0;
	while (nextSpace < src.length())
	{
		testSpace = src.find(' ', nextSpace+1);
		if (font.stringWidth(src.substr(currentSpace + 1, testSpace-(currentSpace+1))) > maxWidth)
		{
			if (dest.length() > 0)
				dest += "\n";
			dest += src.substr(currentSpace + 1, nextSpace-(currentSpace+1));
			currentSpace = nextSpace;
		}
		else
		{
			nextSpace = testSpace;
		}
	}
	if (dest.length() > 0)
		dest += "\n";
	dest += src.substr(currentSpace + 1, src.length());
	return dest;
}

bool Legend::Render()
{
	const vector<Landmarks::Landmark> landmarksIn = landmarksRef.GetLandmarks();
	for (auto lit : landmarksIn)
	{
		if (landmarks.find(lit.iconIdx) == landmarks.end())
		{
			landmarks[lit.iconIdx] = Key{ lit, 1 };
		}
		else
		{
			landmarks[lit.iconIdx].count++;
		}
	}
	landmarks[-1] = Key{ { ofPoint(), -1, true } };
	landmarks[-2] = Key{ { ofPoint(), -2, true } };
	landmarks[-3] = Key{ { ofPoint(), -3, true } };

	image.begin();
	int current = yOffset;
	legendBounds.set(ofGetWidth() - xNegOffset, current, 0, 0);

	vector<int> keys;
	for (auto lit : landmarks)
	{
		keys.push_back(lit.first);
	}

	ofRandomize(keys);

	for(auto key : keys)
	{
		landmarks[key].name = key < 0
			? GeneratePathName(Paths::PathStyle::Above)
			: GenerateName(landmarks[key].count);

		ofPoint pos = ofPoint(ofGetWidth() - xNegOffset, current);

		ofSetColor(ofColor::black);
		ofEnableSmoothing();

		if (key < 0)
		{
			ofPolyline stroke = ofPolyline();
			stroke.lineTo(pos + imageOffset + ofPoint(-12, -12));
			stroke.curveTo(pos + imageOffset + ofPoint(-12, -12));
			stroke.curveTo(pos + imageOffset + ofPoint(ofRandom(-12.0f, 12.0f), ofRandom(-12.0f, 12.0f)));
			stroke.curveTo(pos + imageOffset + ofPoint(12, 12));
			stroke.curveTo(pos + imageOffset + ofPoint(12, 12));
			Paths::PathStyle style = key == -1 ? Paths::PathStyle::Above
									: key == -2 ? Paths::PathStyle::Below
									: Paths::PathStyle::Mixed;
			pathsRef.DrawRoute(stroke, style, false);
		}
		else
		{
			ofRectangle iconBounds = landmarksRef.DrawIcon(landmarks[key].landmark.iconIdx, pos + imageOffset);
			legendBounds.growToInclude(iconBounds);
		}

		ofSetColor(ofColor::black);
		std::string str = BreakString(landmarks[key].name, xNegOffset - xMargin);
		font.drawString(str, pos.x, pos.y);
		legendBounds.growToInclude(font.getStringBoundingBox(str, pos.x, pos.y));

		current += font.stringHeight(str);
		current += ySpacing;
	}
	image.end();
	return true;
}

void Legend::Draw()
{
	if(image.isAllocated())
		image.draw(0, 0);
}
