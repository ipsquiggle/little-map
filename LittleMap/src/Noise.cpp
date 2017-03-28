#include "Noise.h"

float roffsetx = 0;
float roffsety = 0;

float Noise(float x, float y, int octaves, float alpha, float beta)
{
	float t = 0;
	float max = 0;

	for (int n = 0; n < octaves; n++)
	{
		float factor = 1.0f / std::pow(alpha, n);
		max += factor;
		float betapow = std::pow(beta, n);
		t += factor * ofNoise(betapow * x + roffsetx, betapow * y + roffsety);
	}

	return t/max;
}

void SetNoiseSeed(int seed)
{
	ofSeedRandom(seed);
	roffsetx = ofRandom(1024.0f); // some arbitrary number, lets just move around the space a bit
	roffsety = ofRandom(1024.0f); // some arbitrary number, lets just move around the space a bit
}
