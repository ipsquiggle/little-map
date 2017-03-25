#include "CurveTerrain.h"
#include "Noise.h"

const float noiseScale = 0.05f;
const int cellSize = 20;


CurveTerrain::CurveTerrain()
{
}

CurveTerrain::~CurveTerrain()
{
}

void CurveTerrain::Setup()
{
	SetupTiles();
}

bool CurveTerrain::OnLand(int x, int y)
{
	return Noise(x*noiseScale, y*noiseScale, 4, 0.5f, 0.4f) < 0.45f;
}

ofColor CurveTerrain::TerrainColour(int x, int y)
{
	bool hits[4];
	hits[0] = OnLand(x, y);
	hits[1] = OnLand(x + cellSize, y);
	hits[2] = OnLand(x, y + cellSize);
	hits[3] = OnLand(x + cellSize, y + cellSize);

	bool land = false;
	bool water = false;
	for (int i = 0; i < 4; i++)
	{
		if (hits[i])
			land = true;
		else
			water = true;
	}

	if (land && water)
		return ofColor(0, 0, 0, 255);
	else if (land)
		return ofColor(150, 255, 200, 255);
	else
		return ofColor(150, 200, 255);
}

CurveTerrain::Tile CurveTerrain::TileForPos(int x, int y)
{
	bool hits[4];
	hits[0] = OnLand(x, y);
	hits[1] = OnLand(x + cellSize, y);
	hits[2] = OnLand(x, y + cellSize);
	hits[3] = OnLand(x + cellSize, y + cellSize);

	int index = hits[3] << 3 | hits[2] << 2 | hits[1] << 1 | hits[0];
	return tiles[index];
}

ofPoint CurveTerrain::PointForDir(dir d)
{
	switch (d)
	{
	case top:
		return ofPoint(0.5f, 0);
	case left:
		return ofPoint(0, 0.5f);
	case right:
		return ofPoint(1.0f, 0.5f);
	case bottom:
		return ofPoint(0.5f, 1.0f);
	}
}

void CurveTerrain::DrawLink(int x, int y, dir start, dir end)
{
	ofPoint corner(x, y);
	ofPoint ps = corner + PointForDir(start) * cellSize;
	ofPoint pe = corner + PointForDir(end) * cellSize;
	ofSetColor(0, 0, 0, 255);
	ofDrawLine(ps, pe);
}

void CurveTerrain::Render()
{
	image = ofFbo();
	image.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

	image.begin();
	ofClear(255, 255, 255, 255);

	for (int y = 0; y < ofGetHeight(); y += cellSize)
	{
		for (int x = 0; x < ofGetWidth(); x += cellSize)
		{
			Tile t = TileForPos(x, y);
			ofPoint start, end;
			for (int l = 0; l < 4; l++)
			{
				if (t.links[l] != none)
				{
					DrawLink(x, y, (CurveTerrain::dir)l, t.links[l]);
				}
			}
				//if (i == 2)
				//	image.setColor(tx, ty, ofColor(0, 0, 0, 255));
				//else if (i == 1)
				//	image.setColor(tx, ty, ofColor(150, 200, 255, 255));
				//else
				//	image.setColor(tx, ty, ofColor(150, 255, 200, 255));
		}
	}
	image.end();
}

void CurveTerrain::Draw()
{
	image.draw(0, 0);
}

void CurveTerrain::SetupTiles()
{
	// RULE IS: 1 on the left, 0 on the right

	// 0 0
	// 0 0
	tiles[0] = { {CurveTerrain::dir::none,
		CurveTerrain::dir::none, CurveTerrain::dir::none,
				CurveTerrain::dir::none} };
	// 1 0
	// 0 0
	tiles[1] = { {CurveTerrain::dir::none,
		CurveTerrain::dir::top, CurveTerrain::dir::none,
				CurveTerrain::dir::none} };
	// 0 1
	// 0 0
	tiles[2] = { {CurveTerrain::dir::right,
		CurveTerrain::dir::none, CurveTerrain::dir::none,
				CurveTerrain::dir::none} };
	// 1 1
	// 0 0
	tiles[3] = { {CurveTerrain::dir::none,
		CurveTerrain::dir::right, CurveTerrain::dir::none,
				CurveTerrain::dir::none} };
	// 0 0
	// 1 0
	tiles[4] = { {CurveTerrain::dir::none,
		CurveTerrain::dir::none, CurveTerrain::dir::none,
				CurveTerrain::dir::left} };
	// 1 0
	// 1 0
	tiles[5] = { {CurveTerrain::dir::none,
		CurveTerrain::dir::none, CurveTerrain::dir::none,
				CurveTerrain::dir::top} };
	// 0 1
	// 1 0
	tiles[6] = { {CurveTerrain::dir::right,
		CurveTerrain::dir::none, CurveTerrain::dir::none,
				CurveTerrain::dir::left} };
	// 1 1
	// 1 0
	tiles[7] = { {CurveTerrain::dir::none,
		CurveTerrain::dir::none, CurveTerrain::dir::none,
				CurveTerrain::dir::right} };
	// 0 0
	// 0 1
	tiles[8] = { {CurveTerrain::dir::none,
		CurveTerrain::dir::none, CurveTerrain::dir::bottom,
				CurveTerrain::dir::none} };
	// 1 0
	// 0 1
	tiles[9] = { {CurveTerrain::dir::none,
		CurveTerrain::dir::top, CurveTerrain::dir::bottom,
				CurveTerrain::dir::none} };
	// 0 1
	// 0 1
	tiles[10] = { {CurveTerrain::dir::bottom,
		CurveTerrain::dir::none, CurveTerrain::dir::none,
				CurveTerrain::dir::none} };
	// 1 1
	// 0 1
	tiles[11] = { {CurveTerrain::dir::none,
		CurveTerrain::dir::bottom, CurveTerrain::dir::none,
				CurveTerrain::dir::none} };
	// 0 0
	// 1 1
	tiles[12] = { {CurveTerrain::dir::none,
		CurveTerrain::dir::none, CurveTerrain::dir::left,
				CurveTerrain::dir::none} };
	// 1 0
	// 1 1
	tiles[13] = { {CurveTerrain::dir::none,
		CurveTerrain::dir::none, CurveTerrain::dir::top,
				CurveTerrain::dir::none} };
	// 0 1
	// 1 1
	tiles[14] = { {CurveTerrain::dir::none,
		CurveTerrain::dir::top, CurveTerrain::dir::none,
				CurveTerrain::dir::none} };
	// 1 1
	// 1 1
	tiles[15] = { {CurveTerrain::dir::none,
		CurveTerrain::dir::none, CurveTerrain::dir::none,
				CurveTerrain::dir::none} };
}

