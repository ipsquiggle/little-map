#pragma once
#include "ofMain.h"
#include "Stage.h"

#include "CurveTerrain.h"
#include "Landmarks.h"

#include <vector>

class Paths : public Stage
{
public:
	Paths(CurveTerrain &terrain, Landmarks &landmarks, bool debug = false);
	~Paths();

	virtual void Setup();
	virtual bool Render();
	virtual void Draw();

	struct pathBit
	{
		float cost;
		float spend;
		ofPoint pos;
		int parent;
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
	
	struct Path {
		ofPoint start;
		ofPoint end;
		progress progress;
	};

private:
	bool debug;

	struct OpenSorter
	{
		OpenSorter(Paths* owner)
			: owner(owner) {};
		Paths* owner;
		bool operator() (int a, int b)
		{
			pathBit& pba = owner->paths[owner->pathIdx].progress.visited[a];
			pathBit& pbb = owner->paths[owner->pathIdx].progress.visited[b];
			return (pba.cost + pba.spend) < (pbb.cost + pbb.spend);
		}
	};

	void SetupPath(Path& path);
	void FindPath(Path& path);
	void TracePath(Path& path);

	bool DoRender();
	void DebugRender();
	void RenderCosts();
	float Cost(ofPoint start, ofPoint pos, ofPoint next, ofPoint target);

	Landmarks &landmarks;
	CurveTerrain &terrain;

	int pathIdx = -1;
	vector<Path> paths;
	ofPoint offsets[8];

	ofFbo image;
	ofFbo debugImage;
};
