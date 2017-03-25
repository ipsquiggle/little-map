#include "Noise.h"

float Noise(float x, float y, int octaves, float alpha, float beta)
{
	float t = 0;
	float max = 0;

	for (int n = 0; n < octaves; n++)
	{
		float factor = 1.0f / std::pow(alpha, n);
		max += factor;
		float betapow = std::pow(beta, n);
		t += factor * ofNoise(betapow * x, betapow * y);
	}

	return t/max;
}
