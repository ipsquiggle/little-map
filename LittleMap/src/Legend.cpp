#include "Legend.h"

#include "Noise.h"

int yOffset = 70;
int ySpacing = 30;
int xNegOffset = 220;
int xMargin = 20;
ofPoint imageOffset(-20, -12);
int fontSize = 20;
float adjectiveChance = 0.2f;


Legend::Legend(Landmarks& landmarksRef)
	: landmarksRef(landmarksRef)
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
	adjectives.push_back("SAFE");

	legendBounds.set(0, 0, 0, 0);

	if (image.isAllocated())
		image.clear();
	image.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
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

	image.begin();
	int current = yOffset;
	legendBounds.set(ofGetWidth() - xNegOffset, current, 0, 0);
	for (auto lit : landmarks)
	{
		lit.second.name = GenerateName(lit.second.count);

		ofPoint pos = ofPoint(ofGetWidth() - xNegOffset, current);

		ofSetColor(ofColor::black);
		ofRectangle iconBounds = landmarksRef.DrawIcon(lit.second.landmark.iconIdx, pos + imageOffset);
		std::string str = BreakString(lit.second.name, xNegOffset - xMargin);
		font.drawString(str, pos.x, pos.y);

		legendBounds.growToInclude(font.getStringBoundingBox(str, pos.x, pos.y));
		legendBounds.growToInclude(iconBounds);

		current += font.stringHeight(str);
		current += ySpacing;
	}
	image.end();
	return true;
}

void Legend::Draw()
{
	image.draw(0, 0);
}
