#include "Paths.h"

#include <chrono>

int numPaths = 4;
int minNearest = 1;
int maxNearest = 17;
float pathSegDist = 10.0f;
float shoreline = 0.0f;
float dotSpacing = 10.0f;
float noDrawSpacingSq = 4.0f*4.0f;

Paths::Paths(CurveTerrain &terrain, Landmarks &landmarks, int debugNum)
	: landmarks(landmarks)
	, terrain(terrain)
	, debugNum(debugNum)
	, nextMessage(nullptr)
{
}

Paths::~Paths()
{
}

void Paths::Setup()
{
	offsets[0] = ofPoint(-1.0f, 0.0f);
	offsets[1] = ofPoint(-1.0f, 1.0f);
	offsets[2] = ofPoint(0.0f, 1.0f);
	offsets[3] = ofPoint(1.0f, 1.0f);
	offsets[4] = ofPoint(1.0f, 0.0f);
	offsets[5] = ofPoint(1.0f, -1.0f);
	offsets[6] = ofPoint(0.0f, -1.0f);
	offsets[7] = ofPoint(-1.0f, -1.0f);

	Reset();
}

void Paths::Reset()
{
	pathIdx = -1;
	if (image.isAllocated())
		image.clear();
	image.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

	if (debugImage.isAllocated())
		debugImage.clear();
	debugImage.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

	paths.clear();
	drawnPaths.clear();
}

void Paths::TracePath(Path& path)
{
	ofPolyline stroke = ofPolyline();
	//stroke.setStrokeWidth(2);
	//stroke.setFilled(false);
	//stroke.setStrokeColor(ofColor::black);

	stroke.curveTo(path.progress.visited[path.progress.currentIndex].pos);
	stroke.curveTo(path.progress.visited[path.progress.currentIndex].pos);

	while (path.progress.visited[path.progress.currentIndex].parent != -1)
	{
		stroke.curveTo(path.progress.visited[path.progress.visited[path.progress.currentIndex].parent].pos);
		path.progress.currentIndex = path.progress.visited[path.progress.currentIndex].parent;
		path.progress.length++;
	}

	stroke.curveTo(path.progress.visited[path.progress.currentIndex].pos);
	stroke.curveTo(path.progress.visited[path.progress.currentIndex].pos);

	stroke = stroke.getSmoothed(30, 1);

	//stroke.draw(0, 0);

	ofFill();
	ofEnableSmoothing();
	ofSetColor(ofColor::black);
	float len = 0;
	while (stroke.getIndexAtLength(len) < stroke.size()-1) // apparently size - 1....
	{
		ofPoint pt = stroke.getPointAtLength(len);
		bool blocked = false;
		for (auto otherLine : drawnPaths)
		{
			ofPoint otherPt = otherLine.getClosestPoint(pt);
			if (pt.distanceSquared(otherPt) < noDrawSpacingSq)
			{
				blocked = true;
				break;
			}
		}
		
		if (!blocked)
		{
			ofDrawCircle(pt, 3);
		}
		len += dotSpacing;
	}

	drawnPaths.push_back(stroke);

	path.progress.traced = true;
	printf("\t%d length\n", path.progress.length);
}

bool Paths::DoRender()
{
	Path& path = paths[pathIdx];
	if (path.progress.currentIndex == -1 && path.progress.found == false && path.progress.traced == false)
	{
		SetupPath(path);
	}
	else if (path.progress.found == false)
	{
		FindPath(path);
	}
	else if (path.progress.traced == false)
	{
		ofSetLineWidth(3);
		ofSetColor(ofColor::black);
		TracePath(path);
	}
	else
	{
		return true;
	}
	return false;
}

void Paths::DebugRender()
{
	Path& path = paths[pathIdx];

	for (std::map<int, pathBit>::iterator it = path.progress.visited.begin(); it != path.progress.visited.end(); it++)
	{
		if (it->second.parent == -1)
			continue;

		ofSetColor(255, 0, 0, 100);
		ofDrawRectangle(it->second.pos - ofPoint(pathSegDist / 2, pathSegDist / 2), pathSegDist, pathSegDist);
		ofSetColor(255, 255, 0, 255);
		ofSetLineWidth(1);
		ofDrawLine(it->second.pos, path.progress.visited[it->second.parent].pos);
	}

	for (std::list<int>::iterator it = path.progress.open.begin(); it != path.progress.open.end(); it++)
	{
		if (it == path.progress.open.begin())
		{
			ofSetColor(0, 150, 0, 255);
		}
		else
		{
			ofSetColor(0, 150, 150, 255);
		}
		ofDrawCircle(path.progress.visited[*it].pos, pathSegDist / 2);
	}

	ofSetLineWidth(3);
	ofSetColor(255, 0, 255, 255);
	ofDrawLine(path.start, path.end);
	ofSetLineWidth(1);
}

void Paths::RenderCosts()
{
	Path& path = paths[pathIdx];

	for (std::map<int, pathBit>::iterator it = path.progress.visited.begin(); it != path.progress.visited.end(); it++)
	{
		if (it->second.parent == -1)
			continue;


		ofPoint start = path.start;
		ofPoint target = path.end;
		ofPoint next = it->second.pos;

	float valCost;
	float distCost;
	float totalCost;
	float shoreCost;
	Cost(start, next, target, valCost, distCost, totalCost, shoreCost);

	//float startVal = terrain.GetLandValue(start.x, start.y);
	//float nextVal = terrain.GetLandValue(next.x, next.y);
	//float targetVal = terrain.GetLandValue(target.x, target.y);
	//
	//float pathDist = target.distance(start);
	//float nextDist = target.distance(next);
	//float lowPoint = std::min(startVal, targetVal);
	//float highPoint = std::max(startVal, targetVal);
	//float idealHeight = std::min(highPoint, std::max(lowPoint,
	//	lerp(targetVal, startVal, nextDist / pathDist)));

	//float valDiff = std::abs(nextVal - idealHeight);
	//float valCost = valDiff * pathSegDist * 4000.0f;
	//float valCost = std::abs(nextVal) < shoreline
	//	? 4000.0f *pathSegDist / valDiff
	//	: valDiff * pathSegDist * 4000.0f;

	//float distCost = nextDist;
	//if (nextDist > pathDist)
	//{
	//	float outerPart = nextDist - pathDist;
	//	distCost += outerPart * outerPart;
	//}

		if (debugNum == 1)
		{
			float cost = it->second.TotalCost();
			int totalCostColor = cost / 100;
			ofSetColor(totalCostColor % 255, (totalCostColor / 4) % 255, (totalCostColor / 16) % 255, 255);
			ofDrawRectangle(it->second.pos - ofPoint(pathSegDist / 2, pathSegDist / 2), pathSegDist, pathSegDist);
			nextMessage = "Pathbit total cost";
		}

		/*
		if (debugNum == 2)
		{
			int idealHeightColor = (idealHeight + 1.0f) * 3 * 128;
			ofSetColor(idealHeightColor % 255, (idealHeightColor / 4) % 255, (idealHeightColor / 16) % 255, 255);
			ofDrawRectangle(it->second.pos - ofPoint(pathSegDist / 2, pathSegDist / 2), pathSegDist, pathSegDist);
			nextMessage = "Ideal height";
		}
		if (debugNum == 3)
		{
			int nextHeightColor = (nextVal + 1.0f) * 5 * 128;
			ofSetColor(nextHeightColor % 255, (nextHeightColor / 4) % 255, (nextHeightColor / 16) % 255, 255);
			ofDrawCircle(it->second.pos, pathSegDist / 5);
			nextMessage = "Next height";
		}
		*/

		if (debugNum == 4)
		{
			int valCostColor = valCost / 100;
			ofSetColor(valCostColor % 255, (valCostColor / 4) % 255, (valCostColor / 16) % 255, 255);
			ofDrawRectangle(it->second.pos - ofPoint(pathSegDist / 2, pathSegDist / 2), pathSegDist, pathSegDist);
			nextMessage = "Val cost";
		}

		if (debugNum == 5)
		{
			int distCostColor = distCost / 100;
			ofSetColor(distCostColor % 255, (distCostColor / 4) % 255, (distCostColor / 16) % 255, 255);
			ofDrawRectangle(it->second.pos - ofPoint(pathSegDist / 2, pathSegDist / 2), pathSegDist, pathSegDist);
			//ofDrawLine(it->second.pos - ofPoint(pathSegDist / 2, pathSegDist / 2), it->second.pos + ofPoint(pathSegDist / 2, pathSegDist / 2));
			nextMessage = "Dist cost";
		}

		if (debugNum == 6)
		{
			int costColor = shoreCost / 100;
			ofSetColor(costColor % 255, (costColor / 4) % 255, (costColor / 16) % 255, 255);
			ofDrawRectangle(it->second.pos - ofPoint(pathSegDist / 2, pathSegDist / 2), pathSegDist, pathSegDist);
			//ofDrawLine(it->second.pos - ofPoint(pathSegDist / 2, pathSegDist / 2), it->second.pos + ofPoint(pathSegDist / 2, pathSegDist / 2));
			nextMessage = "Shore cost";
		}

		if (debugNum == 7)
		{
			int totalCostColor = totalCost / 100;
			ofSetColor(totalCostColor % 255, (totalCostColor / 4) % 255, (totalCostColor / 16) % 255, 255);
			ofDrawRectangle(it->second.pos - ofPoint(pathSegDist / 2, pathSegDist / 2), pathSegDist, pathSegDist);
			nextMessage = "Total cost";
		}

		if (debugNum == 8)
		{
			float cost = it->second.cost;
			int totalCostColor = cost / 100;
			ofSetColor(totalCostColor % 255, (totalCostColor / 4) % 255, (totalCostColor / 16) % 255, 255);
			ofDrawRectangle(it->second.pos - ofPoint(pathSegDist / 2, pathSegDist / 2), pathSegDist, pathSegDist);
			nextMessage = "Pathbit cost";
		}
	}

	ofSetLineWidth(3);
	ofSetColor(255, 0, 255, 255);
	ofDrawLine(path.start, path.end);
	ofSetLineWidth(1);
}

bool Paths::Render()
{
	if (pathIdx == -1)
	{
		for (int i = 0; i < numPaths; i++)
		{
			Landmarks::Landmark start = landmarks.GetRandomLandmark();
			int r = (std::rand() % (maxNearest - minNearest)) + minNearest;
			Landmarks::Landmark end = landmarks.GetNthClosestLandmark(start, r);
			if (start.pos.x < 0 || start.pos.y < 0 || start.pos.x > ofGetWidth() || start.pos.y > ofGetHeight()
				|| end.pos.x < 0 || end.pos.y < 0 || end.pos.x > ofGetWidth() || end.pos.y > ofGetHeight())
			{
				i--;
				continue;
			}
			Path p = { start.pos, end.pos };
			paths.push_back(p);
		}

		debugImage.begin();
		ofClear(0, 0, 0, 0);
		debugImage.end();

		pathIdx++;

		image.begin();
		ofClear(0, 0, 0, 0);
	}
	else
	{
		image.begin();
	}

	int iterations = 0;
	std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(300);
	bool doneWorking = false;
	while (!doneWorking && std::chrono::steady_clock::now() < endTime)
	{
		doneWorking = DoRender();
		iterations++;
	}
	//printf("Searched %d iterations, %d tries so far...\n", iterations, paths[pathIdx].progress.iteration);

	image.end();

	if (debugNum > 0)
	{
		debugImage.begin();
		//DebugRender();
		RenderCosts();
		debugImage.end();
	}

	if (debugNum == 0 && doneWorking)
	{
		debugImage.begin();
		ofClear(0, 0, 0, 0);
		debugImage.end();

		pathIdx++;
	}

	return pathIdx >= paths.size();
}

void Paths::Draw()
{
	if (debugNum > 0)
	{
		debugImage.draw(0, 0);
	}
	image.draw(0, 0);
}

void Paths::DebugNum(int key)
{
	debugNum = key - '0';
}

void Paths::DebugClick(int x, int y)
{
	float valCost;
	float distCost;
	float totalCost;
	float shoreCost;
	GetCosts(ofPoint(x, y), valCost, distCost, totalCost, shoreCost);
	printf("COSTS: valCost: %f \tdistCost: %f \ttotalCost: %f \tshoreCost: %f\n", valCost, distCost, totalCost, shoreCost);
}

char * Paths::GetMessage()
{
	char* message = nextMessage;
	nextMessage = nullptr;
	return message;
}

float lerp(float a, float b, float t)
{
	return ((b - a)*t) + a;
}

void Paths::GetCosts(ofPoint pos, float& valCost, float& distCost, float& totalCost, float& shoreCost)
{
	Cost(paths[pathIdx].start, pos, paths[pathIdx].end, valCost, distCost, totalCost, shoreCost);
}

float Paths::Cost(ofPoint start, ofPoint next, ofPoint target, float& valCost, float& distCost, float& totalCost, float& shoreCost)
{
	float startVal = terrain.GetLandValue(start.x, start.y);
	float nextVal = terrain.GetLandValue(next.x, next.y);
	float targetVal = terrain.GetLandValue(target.x, target.y);
	
	float pathDist = target.distance(start);
	float nextDist = target.distance(next);
	float lowPoint = std::min(startVal, targetVal);
	float highPoint = std::max(startVal, targetVal);
	float idealHeight = std::min(highPoint, std::max(lowPoint,
		lerp(targetVal, startVal, nextDist / pathDist)));

	float valDiff = std::abs(nextVal - idealHeight);
	/*float*/ valCost = /*std::abs(nextVal) < shoreline
		? 4000.0f *pathSegDist / valDiff
		:*/ valDiff * pathSegDist * 4000.0f;

	if (nextDist <= pathSegDist)
		int i = 0;
	/*float*/ distCost = nextDist;
	if (nextDist > pathDist)
	{
		float outerPart = nextDist - pathDist;
		distCost += outerPart * outerPart;
	}

	float shoreDist = std::abs(1 / (nextVal * 100.0f));
	/*float*/ shoreCost = std::min(shoreDist * 50000.0f, 10000.0f);
	/*float*/ totalCost = distCost + valCost + shoreCost;
	return totalCost;
}


float Paths::Cost(ofPoint start, ofPoint next, ofPoint target)
{
	float valCost;
	float distCost;
	float totalCost;
	float shoreCost;
	return Cost(start, next, target, valCost, distCost, totalCost, shoreCost);
}

void Paths::SetupPath(Path& path)
{
	path.progress.currentPos = path.start;
	path.progress.currentIndex = (int)path.start.x + (int)(path.start.y * ofGetWidth());

	path.progress.visited = std::map<int, pathBit>();
	path.progress.open = std::list<int>();
	path.progress.open.push_back(path.progress.currentIndex);
	pathBit startBit = {
		1000000,
		0,
		path.start,
		-1
	};
	path.progress.visited.insert_or_assign(path.progress.currentIndex, startBit);

	path.progress.iteration = 0;
	path.progress.found = false;

	path.progress.length = 0;
	path.progress.traced = false;
}

void Paths::FindPath(Path& path)
{
	if (path.start.x < 0 || path.start.y < 0 || path.start.x > ofGetWidth() || path.start.y > ofGetHeight()
		|| path.end.x < 0 || path.end.y < 0 || path.end.x > ofGetWidth() || path.end.y > ofGetHeight())
	{
		assert(false);
	}
	path.progress.open.sort(OpenSorter(this));

	int prev = -1;
	for (auto pit : path.progress.open)
	{
		if (prev == -1)
		{
			prev = pit;
			continue;
		}

		pathBit& pba = path.progress.visited[prev];
		pathBit& pbb = path.progress.visited[pit];

		assert(pba.TotalCost() <= pbb.TotalCost());

		pathBit &parenta = path.progress.visited[pba.parent];
		pathBit &parentb = path.progress.visited[pbb.parent];

		assert(pba.spend > parenta.spend);
		assert(pbb.spend > parentb.spend);

		prev = pit;
	}

	if (!path.progress.open.empty())
	{
		path.progress.currentIndex = path.progress.open.front();
		path.progress.currentPos = path.progress.visited[path.progress.currentIndex].pos;
		float currentSpend = path.progress.visited[path.progress.currentIndex].spend;
		path.progress.open.pop_front();
		if (path.progress.visited[path.progress.currentIndex].pos.distance(path.end) < pathSegDist)
		{
			pathBit lastBit = {
				currentSpend + 1,
				currentSpend + 1,
				path.end,
				path.progress.currentIndex
			};
			path.progress.currentIndex = (int)path.end.x + (int)(path.end.y * ofGetWidth());
			path.progress.visited.insert_or_assign(path.progress.currentIndex, lastBit);
			path.progress.found = true;
			printf("%s path with %d iterations\n", path.progress.found ? "Found" : "Didn't find", path.progress.iteration);
			return;
		}

		path.progress.iteration++;

		for (int i = 0; i < 8; i++)
		{
			ofPoint test = path.progress.currentPos + offsets[i] * pathSegDist;
			float cost = Cost(path.start, test, path.end);

			if (test.x < 0 || test.y < 0 || test.x > ofGetWidth() || test.y > ofGetHeight())
			{
				continue;
				//cost *= 4;
			}

			int visitedIndex = (int)test.x + (int)(test.y * ofGetWidth());
			pathBit newBit = {
				cost,
				currentSpend + 1,
				test,
				path.progress.currentIndex
			};
			if (path.progress.visited.find(visitedIndex) != path.progress.visited.end())
			{
				//if (path.progress.visited[visitedIndex].cost > cost)
				//{
				//	path.progress.visited.insert_or_assign(visitedIndex, newBit);
				//}
			}
			else
			{
				path.progress.visited.insert_or_assign(visitedIndex, newBit);
				path.progress.open.push_back(visitedIndex);
			}
		}
	}
	else
	{
		printf("%s path with %d iterations\n", path.progress.found ? "Found" : "Didn't find", path.progress.iteration);
	}
}
