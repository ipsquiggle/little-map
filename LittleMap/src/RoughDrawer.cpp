#include "RoughDrawer.h"
#include "ofMain.h"

void RoughTracePath(ofPolyline& path, float minSize, float maxSize)
{
	float len = 0;
	while (path.getIndexAtLength(len) < path.size()-1) // apparently size - 1....
	{
		ofPoint pt = path.getPointAtLength(len);
		
		ofDrawCircle(pt, ofRandom(minSize, maxSize));

		len += 1;
	}
}