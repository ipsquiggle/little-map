#pragma once
#include "Stage.h"
#include "ofMain.h"

class CurveTerrain : public Stage
{
public:
	CurveTerrain(bool debug);
	~CurveTerrain();

	virtual void Setup();
	virtual bool Render();
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
	bool debug = false;

	void RenderBegin();
	void RenderStep();
	bool DoRender();

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
	ofPoint LinkPos(int x, int y, dir end);
	void NextCell(int x, int y, dir currentDir, int &outx, int &outy, dir &nextDir);
	bool DrawIsland(int cellx, int celly);

	void SetupTiles();
	Tile tiles[16];

	int cellWidth;
	int cellHeight;
	Cell* cells;
};

