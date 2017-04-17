#pragma once
#include "ofMain.h"
#include "Stage.h"

#include "CurveTerrain.h"
#include "Landmarks.h"

#include <vector>

class Paths : public Stage
{
public:
	Paths(CurveTerrain &terrain, Landmarks &landmarks, int debugNum = 0);
	~Paths();

	virtual void Setup();
	virtual bool Render();
	virtual void Draw();
	virtual void Reset();
	virtual void DebugNum(int key);
	virtual void DebugClick(int x, int y);
	virtual char* GetMessage();

	struct pathBit
	{
		float cost;
		float spend;
		ofPoint pos;
		int parent;
		float TotalCost()
		{
			return cost + spend;
		}
	};

	struct progress
	{
		int currentIndex = -1;
		ofPoint currentPos;
		std::map<int, pathBit> visited;
		std::list<int> open;

		int iteration;
		bool found;

		int length;
		bool traced;
	};

	enum PathStyle {
		Below,
		Above,
		Mixed,
	};

	struct Path {
		ofPoint start;
		ofPoint end;
		PathStyle style;
		progress progress;
	};

	void GetCosts(ofPoint pos, float& valCost, float& distCost, float& totalCost, float& shoreCost);
	void DrawRoute(ofPolyline stroke, PathStyle style, bool testOverlap);

private:
	int debugNum;

	struct OpenSorter
	{
		OpenSorter(Paths* owner)
			: owner(owner) {};
		Paths* owner;
		bool operator() (int a, int b)
		{
			pathBit& pba = owner->paths[owner->pathIdx].progress.visited[a];
			pathBit& pbb = owner->paths[owner->pathIdx].progress.visited[b];
			return pba.TotalCost() < pbb.TotalCost();
		}
	};

	vector<ofPolyline> drawnPaths;

	void SetupPath(Path& path);
	void FindPath(Path& path);
	void TracePath(Path& path);

	bool DoRender();
	void DebugRender();
	void RenderCosts();
	float Cost(ofPoint start, ofPoint next, ofPoint target, float& valCost, float& distCost, float& totalCost, float& shoreCost);
	float Cost(ofPoint start, ofPoint next, ofPoint target);

	Landmarks &landmarks;
	CurveTerrain &terrain;

	int pathIdx = -1;
	vector<Path> paths;
	ofPoint offsets[8];

	ofFbo image;
	ofFbo debugImage;
	
	char* nextMessage;
};
