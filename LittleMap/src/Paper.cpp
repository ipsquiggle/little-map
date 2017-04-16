#include "Paper.h"

#include "Noise.h"
#include "RoughDrawer.h"

int margin = 25;
const float gridNoiseScale = 0.05f;
const float gridWobble = 8.0f;
const float gridDetail = 12.0f;


Paper::Paper(Legend& legendRef)
	: legendRef(legendRef)
{
}

Paper::~Paper()
{
}

void Paper::Setup()
{
	Reset();
}

void Paper::Reset()
{
	if (image.isAllocated())
		image.clear();
	image.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
}

float Paper::PaperNoise(float x, float y)
{
	return Noise(x*gridNoiseScale, y*gridNoiseScale, 3, 0.5f, 0.4f);
}

bool Paper::Render()
{
	ofRectangle legendBounds = legendRef.GetBounds();
	legendBounds.translate(-margin, -margin);
	legendBounds.setWidth(legendBounds.getWidth() + margin * 2);
	legendBounds.setHeight(legendBounds.getHeight() + margin * 2);

	ofPolyline path;
	path.addVertex(legendBounds.getTopLeft());
	float x, y;
	y = legendBounds.getMinY();
	for (x = legendBounds.getMinX(); x <= legendBounds.getMaxX(); x += gridDetail)
	{
		ofPoint offset(PaperNoise(x, y), PaperNoise(x, y + 1000.0f));
		path.curveTo(offset * gridWobble + ofPoint(x, y));
	}
	path.lineTo(legendBounds.getTopRight());
	x = legendBounds.getMaxX();
	for (y = legendBounds.getMinY(); y <= legendBounds.getMaxY(); y += gridDetail)
	{
		ofPoint offset(PaperNoise(x, y), PaperNoise(x, y + 1000.0f));
		path.curveTo(offset * gridWobble + ofPoint(x, y));
	}
	path.lineTo(legendBounds.getBottomRight());
	y = legendBounds.getMaxY();
	for (x = legendBounds.getMaxX(); x >= legendBounds.getMinX(); x -= gridDetail)
	{
		ofPoint offset(PaperNoise(x, y), PaperNoise(x, y + 1000.0f));
		path.curveTo(offset * gridWobble + ofPoint(x, y));
	}
	path.lineTo(legendBounds.getBottomLeft());
	x = legendBounds.getMinX();
	for (y = legendBounds.getMaxY(); y >= legendBounds.getMinY(); y -= gridDetail)
	{
		ofPoint offset(PaperNoise(x, y), PaperNoise(x, y + 1000.0f));
		path.curveTo(offset * gridWobble + ofPoint(x, y));
	}
	path.lineTo(legendBounds.getTopLeft());

	image.begin();



	ofSetColor(ofColor::beige);
	ofFill();
	ofEnableSmoothing();

	ofTessellator tesselator = ofTessellator();
	ofMesh mesh;
	tesselator.tessellateToMesh(path, ofPolyWindingMode::OF_POLY_WINDING_ODD, mesh);
	mesh.draw();

	ofSetColor(ofColor::black);
	ofFill();
	ofEnableSmoothing();
	RoughTracePath(path, 1.5f, 3.0f);

	image.end();
	return true;
}

void Paper::Draw()
{
	image.draw(0, 0);
}
