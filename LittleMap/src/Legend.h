#pragma once
#include "Stage.h"
#include "ofMain.h"

#include "Landmarks.h"
#include "Paths.h"

class Legend : public Stage
{
public:
	Legend(Landmarks& landmarksRef, Paths& pathsRef);
	~Legend();

	virtual void Setup();
	virtual bool Render();
	virtual void Draw();
	virtual void Reset();

	struct Key {
		Landmarks::Landmark landmark;
		int count;
		std::string name;
	};

	ofRectangle GetBounds() { return legendBounds; }

private:
	Landmarks& landmarksRef;
	Paths& pathsRef;

	std::string BreakString(std::string& srd, float maxWidth);
	std::string GenerateName(int count);
	std::string GeneratePathName(Paths::PathStyle style);

	vector<std::string> singleNames;
	vector<std::string> pluralNames;
	vector<std::string> pathNames;
	vector<std::string> adjectives;

	map<int, Key> landmarks;
	ofTrueTypeFont font;

	ofRectangle legendBounds;

	ofFbo image;
};

