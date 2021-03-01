#ifndef __TGEN_ENDLESSTERRAIN_H__
#define __TGEN_ENDLESSTERRAIN_H__

//#include <iostream>
#include <thread>
#include <vector>
#include <utility>
#include <unordered_map>
#include <forward_list>
#include "Terrain.h"

typedef std::pair<int, int> ivec2;

class EndlessTerrain
{
public:
	EndlessTerrain();
	EndlessTerrain(glm::vec3 pos, float viewDist);
	void GetViewerPosition(glm::vec3 pos);
	void UpdateVisiblePlanes();
	void RenderVisiblePlanes();
	~EndlessTerrain();
private:
	ivec2 currKey;
	float maxViewDist;
	int viewIndexRange;

	int GetLODIndex(ivec2 &viskey);
};
#endif