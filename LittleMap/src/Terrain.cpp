#include "Terrain.h"
#include "Noise.h"

const float noiseScale = 0.05f;


Terrain::Terrain()
{
}

Terrain::~Terrain()
{
}

void Terrain::Setup()
{
	//SetupTiles1px();
	SetupTiles2px();
}

bool Terrain::OnLand(int x, int y)
{
	return Noise(x*noiseScale, y*noiseScale, 4, 0.5f, 0.4f) < 0.45f;
}

ofColor Terrain::TerrainColour(int x, int y)
{
	bool hits[4];
	hits[0] = OnLand(x, y);
	hits[1] = OnLand(x + 1, y);
	hits[2] = OnLand(x, y + 1);
	hits[3] = OnLand(x + 1, y + 1);

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

Terrain::Tile Terrain::TileForPos(int x, int y)
{
	bool hits[4];
	hits[0] = OnLand(x, y);
	hits[1] = OnLand(x + 3, y);
	hits[2] = OnLand(x, y + 3);
	hits[3] = OnLand(x + 3, y + 3);

	int index = hits[3] << 3 | hits[2] << 2 | hits[1] << 1 | hits[0];
	return tiles[index];
}


void Terrain::Render()
{
	image = ofImage();
	image.allocate(ofGetWidth(), ofGetHeight(), ofImageType::OF_IMAGE_COLOR_ALPHA);

	for (int y = 0; y < ofGetHeight(); y += 3)
	{
		for (int x = 0; x < ofGetWidth(); x += 3)
		{
			Tile t = TileForPos(x, y);
			for (int n = 0; n < 3 * 3; n++)
			{
				int tx = x + (n % 3);
				int ty = y + (n / 3);
				if (tx >= ofGetWidth() || ty >= ofGetHeight())
					continue;

				int i = t[n];
				if (i == 2)
					image.setColor(tx, ty, ofColor(0, 0, 0, 255));
				else if (i == 1)
					image.setColor(tx, ty, ofColor(150, 200, 255, 255));
				else
					image.setColor(tx, ty, ofColor(150, 255, 200, 255));
			}
		}
	}

	image.update();
}

void Terrain::Draw()
{
	image.draw(0, 0);
}

void Terrain::SetupTiles1px()
{
	// 0 0
	// 0 0
	tiles[0] = { {0, 0, 0,
				  0, 0, 0,
				  0, 0, 0} };
	// 1 0
	// 0 0
	tiles[1] = { {1, 2, 0,
				  2, 0, 0,
				  0, 0, 0} };
	// 0 1
	// 0 0
	tiles[2] = { {0, 2, 1,
				  0, 0, 2,
				  0, 0, 0} };
	// 1 1
	// 0 0
	tiles[3] = { {1, 1, 1,
				  2, 2, 2,
				  0, 0, 0} };
	// 0 0
	// 1 0
	tiles[4] = { {0, 0, 0,
				  2, 0, 0,
				  1, 2, 0} };
	// 1 0
	// 1 0
	tiles[5] = { {1, 2, 0,
				  1, 2, 0,
				  1, 2, 0} };
	// 0 1
	// 1 0
	tiles[6] = { {0, 2, 1,
				  2, 0, 2,
				  1, 2, 0} };
	// 1 1
	// 1 0
	tiles[7] = { {1, 1, 1,
				  1, 1, 2,
				  1, 2, 0} };
	// 0 0
	// 0 1
	tiles[8] = { {0, 0, 0,
				  0, 0, 2,
				  0, 2, 1} };
	// 1 0
	// 0 1
	tiles[9] = { {1, 2, 0,
				  2, 0, 2,
				  0, 2, 1} };
	// 0 1
	// 0 1
	tiles[10]= { {0, 2, 1,
				  0, 2, 1,
				  0, 2, 1} };
	// 1 1
	// 0 1
	tiles[11]= { {1, 1, 1,
				  2, 1, 1,
				  0, 2, 1} };
	// 0 0
	// 1 1
	tiles[12]= { {0, 0, 0,
				  2, 2, 2,
				  1, 1, 1} };
	// 1 0
	// 1 1
	tiles[13]= { {1, 2, 0,
				  1, 1, 2,
				  1, 1, 1} };
	// 0 1
	// 1 1
	tiles[14]= { {0, 2, 1,
				  2, 1, 1,
				  1, 1, 1} };
	// 1 1
	// 1 1
	tiles[15]= { {1, 1, 1,
				  1, 1, 1,
				  1, 1, 1} };
}

void Terrain::SetupTiles2px()
{
	// 0 0
	// 0 0
	tiles[0] = { {0, 0, 0,
				  0, 0, 0,
				  0, 0, 0} };
	// 1 0
	// 0 0
	tiles[1] = { {2, 2, 0,
				  2, 0, 0,
				  0, 0, 0} };
	// 0 1
	// 0 0
	tiles[2] = { {0, 2, 2,
				  0, 0, 2,
				  0, 0, 0} };
	// 1 1
	// 0 0
	tiles[3] = { {2, 2, 2,
				  2, 2, 2,
				  0, 0, 0} };
	// 0 0
	// 1 0
	tiles[4] = { {0, 0, 0,
				  2, 0, 0,
				  2, 2, 0} };
	// 1 0
	// 1 0
	tiles[5] = { {2, 2, 0,
				  2, 2, 0,
				  2, 2, 0} };
	// 0 1
	// 1 0
	tiles[6] = { {0, 2, 2,
				  2, 2, 2,
				  2, 2, 0} };
	// 1 1
	// 1 0
	tiles[7] = { {1, 2, 2,
				  2, 2, 2,
				  2, 2, 0} };
	// 0 0
	// 0 1
	tiles[8] = { {0, 0, 0,
				  0, 0, 2,
				  0, 2, 2} };
	// 1 0
	// 0 1
	tiles[9] = { {2, 2, 0,
				  2, 2, 2,
				  0, 2, 2} };
	// 0 1
	// 0 1
	tiles[10]= { {0, 2, 2,
				  0, 2, 2,
				  0, 2, 2} };
	// 1 1
	// 0 1
	tiles[11]= { {2, 2, 1,
				  2, 2, 2,
				  0, 2, 2} };
	// 0 0
	// 1 1
	tiles[12]= { {0, 0, 0,
				  2, 2, 2,
				  2, 2, 2} };
	// 1 0
	// 1 1
	tiles[13]= { {2, 2, 0,
				  2, 2, 2,
				  1, 2, 2} };
	// 0 1
	// 1 1
	tiles[14]= { {0, 2, 2,
				  2, 2, 2,
				  2, 2, 1} };
	// 1 1
	// 1 1
	tiles[15]= { {1, 1, 1,
				  1, 1, 1,
				  1, 1, 1} };
}

