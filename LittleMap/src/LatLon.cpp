#include "LatLon.h"
#include "Noise.h"

const float gridNoiseScale = 0.05f;
const float gridSpacing = 100.0f;
const float gridWobble = 15.0f;

LatLon::LatLon()
{
}

LatLon::~LatLon()
{
}

void LatLon::Setup()
{
}

float LatLon::LatLonNoise(float x, float y)
{
	return Noise(x*gridNoiseScale, y*gridNoiseScale, 3, 0.5f, 0.4f);
}

ofColor LatLon::LatLonColour(int x, int y)
{
	float vals[4];
	vals[0] = x + LatLonNoise(x, y) * gridWobble;
	vals[1] = x + 2 + LatLonNoise(x + 2, y) * gridWobble;
	vals[2] = y + LatLonNoise(x, y) * gridWobble;
	vals[3] = y + 2 + LatLonNoise(x, y + 2) * gridWobble;

	if (std::fmod(vals[0], gridSpacing) > std::fmod(vals[1], gridSpacing) || std::fmod(vals[2], gridSpacing) > std::fmod(vals[3], gridSpacing))
		return ofColor(0, 0, 0, 255);
	else
		return ofColor(0, 0, 0, 0);
}


void LatLon::Render()
{
	image = ofImage();
	image.allocate(ofGetWidth(), ofGetHeight(), ofImageType::OF_IMAGE_COLOR_ALPHA);

	for (int y = 0; y < ofGetHeight(); y++)
	{
		for (int x = 0; x < ofGetWidth(); x++)
		{
			ofColor color = LatLonColour(x, y);
			image.setColor(x, y, color);
		}
	}

	image.update();
}

void LatLon::Draw()
{
	image.draw(0, 0);
}
