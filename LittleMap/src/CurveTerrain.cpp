#include "CurveTerrain.h"
#include "Noise.h"

const int cellSize = 20;
const float noiseScale = 0.05f * cellSize;


CurveTerrain::CurveTerrain(bool debug)
{
	this->debug = debug;
}

CurveTerrain::~CurveTerrain()
{
}

void CurveTerrain::Setup()
{
	SetupTiles();

	cellWidth = ofGetWidth() / cellSize;
	cellHeight = ofGetHeight() / cellSize;
	cells = new Cell[cellWidth * cellHeight];

	render_x = 0;
	render_y = 0;
}

bool CurveTerrain::OnLand(int x, int y)
{
	// note this test is for cellWidth, not cellWidth-1, because we test both the left
	// and right edge of a cell, i.e. the final cell we test's right edge is actually
	// at cellWidth, even though the cell's index is cellWidth-1
	if (x == 0 || y == 0 || x == cellWidth || y == cellHeight)
		return false;

	return Noise(x*noiseScale, y*noiseScale, 4, 0.5f, 0.4f) < 0.45f;
}

ofColor CurveTerrain::TerrainColour(int x, int y)
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

CurveTerrain::Tile CurveTerrain::TileForPos(int x, int y)
{
	int hits[4];
	hits[0] = (int)OnLand(x, y);
	hits[1] = (int)OnLand(x + 1, y);
	hits[2] = (int)OnLand(x, y + 1);
	hits[3] = (int)OnLand(x + 1, y + 1);

	if (debug)
	{
		for (int i = 0; i < 4; i++)
		{
			int ix = x * cellSize + (cellSize / 8) + (i % 2) * cellSize * 6 / 8;
			int iy = y * cellSize + (cellSize / 8) + (i / 2) * cellSize * 6 / 8;
			if (hits[i])
				ofSetColor(0, 150, 0, 255);
			else
				ofSetColor(200, 0, 0, 255);
			ofDrawCircle(ix, iy, 2);
		}
	}

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

CurveTerrain::dir CurveTerrain::PairDir(CurveTerrain::dir d)
{
	switch (d)
	{
	case top: return bottom;
	case bottom: return top;
	case left: return right;
	case right: return left;
	}
}

ofPoint CurveTerrain::DirOffset(CurveTerrain::dir d)
{
	switch (d)
	{
	case top: return ofPoint(0, -1);
	case bottom: return ofPoint(0, 1);
	case left: return ofPoint(-1, 0);
	case right: return ofPoint(1, 0);
	}
}

void CurveTerrain::DrawLink(int x, int y, dir start, dir end)
{
	ofPoint corner(x*cellSize, y*cellSize);
	ofPoint ps = corner + PointForDir(start) * cellSize;
	ofPoint pe = corner + PointForDir(end) * cellSize;
	ofDrawLine(ps, pe);
}

ofPoint CurveTerrain::LinkPos(int x, int y, dir end)
{
	ofPoint corner(x*cellSize, y*cellSize);
	return corner + PointForDir(end) * cellSize;
}

void CurveTerrain::NextCell(int x, int y, CurveTerrain::dir currentDir, int &outx, int &outy, CurveTerrain::dir &nextDir)
{
	Cell &current = cells[y*cellWidth + x];
	ofPoint offset = DirOffset(current.tile.links[currentDir]);
	outx = x + offset.x;
	outy = y + offset.y;
	nextDir = PairDir(current.tile.links[currentDir]);
}

bool CurveTerrain::DrawIsland(int x, int y)
{
	// the first cell in the link is kinda weird.
	Cell *first = &cells[y*cellWidth + x];
	Cell *next = first;
	// pick the first link we can find in the cell.
	dir d = none;
	for (int i = 0; i < 4; i++)
	{
		if (first->tile.links[i] != none)
		{
			d = (dir)i;
		}
	}
	
	if (d == none)
		return false;

	ofPath path = ofPath();
	path.setMode(ofPath::Mode::POLYLINES);
	path.setStrokeWidth(3);
	path.setStrokeColor(ofColor::black);
	path.setFillColor(ofColor(150, 255, 200, 255));

	ofPoint linkPos = LinkPos(x, y, d);
	// ofPath doesn't draw the first or last points, they are just control points for the curve, 
	// so we will draw the entrance and exit points in the start cell.
	// This makes the first Drawn point the exit of the first cell.
	path.curveTo(linkPos);
	do {
		next->visited = true;
		linkPos = LinkPos(x, y, next->tile.links[d]);
		path.curveTo(linkPos);

		NextCell(x, y, d, x, y, d);
		next = &cells[y*cellWidth + x];
	} while (next != first);
	// ... and since the last Drawn point also needs, to be the exit of the first cell,
	// we draw through the first cell AGAIN, and the second cell AGAIN! Nice....
	linkPos = LinkPos(x, y, next->tile.links[d]);
	path.curveTo(linkPos);

	NextCell(x, y, d, x, y, d);
	next = &cells[y*cellWidth + x];

	linkPos = LinkPos(x, y, next->tile.links[d]);
	path.curveTo(linkPos);

	path.draw(0, 0);

	return true;
}

void CurveTerrain::RenderBegin()
{
	image = ofFbo();
	image.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

	image.begin();
	ofClear(255, 255, 255, 255);

	ofSetColor(0, 0, 255, 255);
	ofSetLineWidth(1);
	for (int y = 0; y < cellHeight; y++)
	{
		for (int x = 0; x < cellWidth; x++)
		{
			Tile t = TileForPos(x, y);
			cells[y*cellWidth + x] = { t, false };
			for (int l = 0; l < 4; l++)
			{
				if (t.links[l] != none)
				{
					DrawLink(x, y, (CurveTerrain::dir)l, t.links[l]);
				}
			}
		}
	}
}

void CurveTerrain::RenderStep()
{

}

bool CurveTerrain::DoRender()
{
	if (render_y == cellHeight)
		return true;

	if (render_x == 0 && render_y == 0)
	{
		RenderBegin();
	}
	else
	{
		image.begin();
	}

	Cell c = cells[render_y*cellWidth + render_x];
	if (c.visited == false)
	{
		bool newIsland = DrawIsland(render_x, render_y);
		if (debug)
		{
			if (newIsland)
				ofSetColor(0, 255, 0, 30);
			else
				ofSetColor(255, 0, 0, 30);
		}
	}
	else if (debug)
	{
		ofSetColor(0, 0, 255, 30);
	}

	if (debug)
	{
		ofFill();
		ofDrawRectangle(render_x * cellSize, render_y * cellSize, cellSize, cellSize);
	}

	render_x++;
	if (render_x == cellWidth)
	{
		render_y++;
		render_x = 0;
	}

	image.end();
	return false;
}

bool CurveTerrain::Render()
{
	if (debug)
	{
		return DoRender();
	}
	else
	{
		while (!DoRender()) {}
		return true;
	}
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
	tiles[14] = { {CurveTerrain::dir::left,
		CurveTerrain::dir::none, CurveTerrain::dir::none,
				CurveTerrain::dir::none} };
	// 1 1
	// 1 1
	tiles[15] = { {CurveTerrain::dir::none,
		CurveTerrain::dir::none, CurveTerrain::dir::none,
				CurveTerrain::dir::none} };
}

