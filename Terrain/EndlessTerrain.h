#ifndef __TGEN_ENDLESSTERRAIN_H__
#define __TGEN_ENDLESSTERRAIN_H__

#include "Shader.h"

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
	struct LODInfo {
		int LOD;
		float viewerDist;
		LODInfo() {}
		LODInfo(int lod, float viewer) {
			LOD = lod;
			viewerDist = viewer;
		}
	};

	struct ColorData {
		glm::vec3 color;
		float baseHeight;
		float blendStrength;
	};

	EndlessTerrain();
	EndlessTerrain(glm::vec3 pos, float viewDist);
	void GetViewerPosition(glm::vec3 pos);
	void UpdateVisiblePlanes();
	void RenderVisiblePlanes();

	void SetColorDataArray(Shader *shader, ColorData colordata[], int colorCount);
	~EndlessTerrain();
private:
	glm::vec3 viewerPos, viewerPosOld;
	ivec2 currKey;
	float maxViewDist;
	int viewIndexRange;

	int GetLODIndex(ivec2 &viskey);

	static int chunksize;
	static LODInfo LODS[];
	static const float MoveThreshold;

};
#endif