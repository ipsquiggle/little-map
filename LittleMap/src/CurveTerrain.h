#pragma once
#include "Stage.h"
#include "ofMain.h"

class CurveTerrain : public Stage
{
public:
	CurveTerrain();
	~CurveTerrain();

	virtual void Setup();
	virtual void Render();
	virtual void Draw();

	bool OnLand(int x, int y);

	enum dir {
		top,
		left,
		right,
		bottom,
		none,
	};

	struct Tile {
		dir links[4];
	};

private:
	ofColor TerrainColour(int x, int y);
	Tile TileForPos(int x, int y);

	ofPoint PointForDir(dir d);
	void DrawLink(int x, int y, dir start, dir end);

	void SetupTiles();

	ofFbo image;

	Tile tiles[16];
};

