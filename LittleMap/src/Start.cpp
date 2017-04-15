#include "Start.h"

#include "Noise.h"

Start::Start()
{
}

Start::~Start()
{
}

void Start::Setup()
{
	Reset();
}

void Start::Reset()
{
	int seed = (int)std::time(nullptr);
	printf("Seed: %d\n", seed);
	SetNoiseSeed(seed);
}