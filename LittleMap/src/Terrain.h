#pragma once
#include "Stage.h"
#include "ofMain.h"

class Terrain : public Stage
{
public:
	Terrain();
	~Terrain();

	virtual void Setup();
	virtual void Render();
	virtual void Draw();

	bool OnLand(int x, int y);


	struct Tile {
		int points[9];
		int operator[](int idx)
		{
			return points[idx];
		}
	};

private:
	ofColor TerrainColour(int x, int y);
	Tile TileForPos(int x, int y);

	void SetupTiles1px();
	void SetupTiles2px();

	ofImage image;

	Tile tiles[16];
};

