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
	struct Cell {
		Tile tile;
		bool visited;
	};

private:
	void RenderBegin();
	void RenderStep();

	int render_x;
	int render_y;

	ofFbo image;

	ofColor TerrainColour(int x, int y);
	Tile TileForPos(int x, int y);

	bool OnLand(int x, int y);

	ofPoint PointForDir(dir d);
	dir PairDir(dir d);
	ofPoint DirOffset(dir d);
	void DrawLink(int x, int y, dir start, dir end);
	void DrawIsland(int cellx, int celly);

	void SetupTiles();
	Tile tiles[16];

	int cellWidth;
	int cellHeight;
	Cell* cells;
};

